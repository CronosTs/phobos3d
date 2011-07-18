/*
Phobos 3d
April 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <sstream>

#include <boost/thread.hpp>

#include <OgreTimer.h>

#include <PH_Console.h>
#include <PH_ContextVar.h>
#include <PH_ContextUtils.h>
#include <PH_Core.h>
#include <PH_DictionaryManager.h>
#include <PH_EventManagerModule.h>
#include <PH_Kernel.h>
#include <PH_ModelRendererManager.h>
#include <PH_MoverManager.h>
#include <PH_PluginManager.h>
#include <PH_ProcVector.h>
#include <PH_Render.h>
#include <PH_WorldManager.h>

#define UPDATE_TIME (1.0f / 60.0f)
#define MIN_TIME (10.0f / 1000.0f)

namespace Phobos
{
	class EngineMain_c
	{
		public:
			EngineMain_c();
			~EngineMain_c();

			void MainLoop(void);

		private:
			inline Float_t ConvertMSecToSeconds(UInt_t msec);
			inline Float_t GetUpdateTime(void);
			inline Float_t GetMinFrameTime(void);

			void CmdQuit(const StringVector_t &, Context_c &);

		private:
			ContextVar_c	varFixedTime;
			ContextVar_c	varEngineFPS;
			ContextVar_c	varMinFrameTime;
			ContextCmd_c	cmdQuit;
			Ogre::Timer		clTimer;
			bool			fStop;

			ProcVector_c	clSingletons;
	};

	EngineMain_c::EngineMain_c():
		varFixedTime("dvFixedTime", "0"),
		varEngineFPS("dvEngineFPS", "60"),
		varMinFrameTime("dvMinFrameTime", "0.01"),
		cmdQuit("quit"),
		fStop(false)
	{
		Kernel_c::CreateInstance("phobos.log");
		CorePtr_t core = Core_c::CreateInstance();
		clSingletons.AddProc(Core_c::ReleaseInstance);

		DictionaryManagerPtr_t dictionaryManager = DictionaryManager_c::CreateInstance();
		clSingletons.AddProc(DictionaryManager_c::ReleaseInstance);

		EventManagerModulePtr_t eventManager = EventManagerModule_c::CreateInstance();
		clSingletons.AddProc(EventManagerModule_c::ReleaseInstance);
		core->AddModule(eventManager);

		ConsolePtr_t console = Console_c::CreateInstance();
		clSingletons.AddProc(Console_c::ReleaseInstance);
		core->AddModule(console);		

		WorldManagerPtr_t worldManager = WorldManager_c::CreateInstance();
		clSingletons.AddProc(WorldManager_c::ReleaseInstance);
		core->AddModule(worldManager);

		PluginManagerPtr_t pluginManager = PluginManager_c::CreateInstance();
		clSingletons.AddProc(PluginManager_c::ReleaseInstance);
		core->AddModule(pluginManager, NORMAL_PRIORITY-1);		

		MoverManagerPtr_t moverManager = MoverManager_c::CreateInstance();
		clSingletons.AddProc(MoverManager_c::ReleaseInstance);
		core->AddModule(moverManager);

		ModelRendererManagerPtr_t modelRendererManager = ModelRendererManager_c::CreateInstance();
		clSingletons.AddProc(ModelRendererManager_c::ReleaseInstance);
		core->AddModule(modelRendererManager, LOWEST_PRIORITY+1);

		RenderPtr_t render = Render_c::CreateInstance();
		clSingletons.AddProc(Render_c::ReleaseInstance);
		core->AddModule(render, LOWEST_PRIORITY);

		core->RegisterCommands(*console);
		dictionaryManager->RegisterCommands(*console);

		cmdQuit.SetProc(PH_CONTEXT_CMD_BIND(&EngineMain_c::CmdQuit, this));
		console->AddContextCmd(cmdQuit);

		console->AddContextVar(varFixedTime);
		console->AddContextVar(varEngineFPS);
		console->AddContextVar(varMinFrameTime);

		core->LaunchBootModule("autoexec.cfg");
	}

	EngineMain_c::~EngineMain_c()
	{
		Core_c::GetInstance()->Shutdown();

		clSingletons.CallAll();

		Kernel_c::ReleaseInstance();
	}

	void EngineMain_c::CmdQuit(const StringVector_t &, Context_c &)
	{
		fStop = true;
	}

	inline Float_t EngineMain_c::ConvertMSecToSeconds(UInt_t msec)
	{
		return((Float_t) msec / 1000.0f);
	}

	inline Float_t EngineMain_c::GetUpdateTime(void)
	{
		const Float_t updateTime = varEngineFPS.GetFloat();

		if(updateTime > 0)
			return(1.0f / updateTime);
		else
		{
			Kernel_c::GetInstance().LogStream() << "[MainLoop] Warning: Invalid update time: " << updateTime << ", must be > 0";
			varEngineFPS.SetValue("60");

			return(1.0f / UPDATE_TIME);
		}
	}

	inline Float_t EngineMain_c::GetMinFrameTime(void)
	{
		Float_t minFrameTime = varMinFrameTime.GetFloat();

		if(minFrameTime > 0.05)
		{
			Kernel_c::GetInstance().LogStream() << "[MainLoop] Warning: Invalid minFrameTime: " << minFrameTime << ", must be < 0.05";
			varMinFrameTime.SetValue("0.01");

			minFrameTime = MIN_TIME;
		}

		return minFrameTime;
	}

	/**

		The engine main loop

	*/
	void EngineMain_c::MainLoop(void)
	{
		Float_t			executionTime = 0;
		unsigned int	lastTicks;
		Float_t			updateTime = GetUpdateTime();

		CorePtr_t core = Core_c::GetInstance();

		lastTicks = clTimer.getMilliseconds();
		do
		{
			core->SetFrameRate(updateTime);

			UInt_t ticks = clTimer.getMilliseconds();

			Float_t lastFrameTime = ConvertMSecToSeconds(ticks - lastTicks);

			if(varFixedTime.GetBoolean())
				lastFrameTime = updateTime;

			if(lastFrameTime < GetMinFrameTime())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				continue;
			}

			lastTicks = ticks;

			//how long the simulation can run?
			executionTime += lastFrameTime;
			Float_t totalFrameTime = lastFrameTime;

			#ifdef IM_DEBUG
				//this happens on debug mode while stopped on break points
				if(executionTime > 20)
					executionTime = updateTime;
			#endif

			//update the game on fixed time steps
			while(executionTime >= updateTime)
			{
				//fixed Update
				core->FixedUpdate(updateTime);

				//totalFrameTime += time;

				//printf("%f\n", executionTime);
				executionTime -= updateTime;
			}
			Float_t delta = (Float_t) executionTime / (Float_t) updateTime;
			//printf("%f\n", delta);
			//Now update other modules as fast as we can
			core->Update(totalFrameTime, delta);

			//update it after frame
			updateTime = GetUpdateTime();

		} while(!fStop);
	}
}

int main(int, char **)
{
	Phobos::EngineMain_c engine;

#ifndef PH_DEBUG
	try
#endif
	{
		engine.MainLoop();
	}
#ifndef PH_DEBUG
	catch(std::exception &e)
	{
		std::stringstream stream;
		stream << "main: Unhandled excetion: ";
		stream << e.what();
		Phobos::Kernel_c::GetInstance().LogMessage(stream.str());

		exit(EXIT_FAILURE);
	}
#endif

	return 0;
}
