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

#include "Phobos/Engine/BootModule.h"

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Memory.h>

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/Core.h"
#include "Phobos/Engine/ModuleManager.h"


Phobos::Engine::BootModule::BootModule(const String_t &cfgName, int argc, char *const argv[], ModuleManager &manager):
	Module("BootModule"),
	m_strCfgName(cfgName),
	m_iFixedUpdateCount(0),
	m_fUpdateDone(false),
	m_fPrepareFired(false),
	m_fBootFired(false),
	m_rclManager(manager)
{
	if(argc > 1)
	{
		m_vecArgs.reserve(argc-1);

		for(int i = 1;i < argc; ++i)
			m_vecArgs.push_back(argv[i]);
	}
}

void Phobos::Engine::BootModule::OnFixedUpdate()
{
	++m_iFixedUpdateCount;

	if(m_fUpdateDone && (m_iFixedUpdateCount > 2))
	{
		//First time, tell the system that we are ready to go
		if(!m_fPrepareFired)
		{
			m_rclManager.OnEvent(Core::Events::PREPARE_TO_BOOT);
			m_fPrepareFired = true;

			//restart count
			m_iFixedUpdateCount = 0;
			m_fUpdateDone = 0;
		}
		else if(!m_fBootFired)
		{
			Console &console = Console::GetInstance();

			try
			{										
				console.ExecuteFromFile(m_strCfgName);					
			}
			catch(FileNotFoundException &e)
			{
				LogMakeStream() << "[BootModule::OnFixedUpdate] Warning, boot failed: " << e.what();
			}

			if(!m_vecArgs.empty())
			{						
				std::for_each(m_vecArgs.begin(), m_vecArgs.end(), [&console](const std::string &arg)
					{
						console.Execute(arg);
					}
				);

				console.FlushCommandBuffer();
			}

			//Time to boot and game over for us
			m_rclManager.OnEvent(Core::Events::BOOT);

			m_fBootFired = true;

			m_rclManager.RemoveModule(*this);

			//suicide
			delete this;
		}
	}
}

void Phobos::Engine::BootModule::OnUpdate()
{
	m_fUpdateDone = true;
}

