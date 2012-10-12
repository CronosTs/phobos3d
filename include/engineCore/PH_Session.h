/*
Phobos 3d
October 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_SESSION_H
#define PH_SESSION_H

#include <PH_InputDeviceListener.h>
#include <PH_InputManager.h>
#include <PH_InputMapper.h>
#include <PH_Singleton.h>

#include "PH_EngineCoreAPI.h"

#include "PH_CoreModule.h"

namespace Phobos
{	
	PH_DECLARE_SINGLETON_PTR(Session);

	class IPlayerCommandProducer_c;
	class IClient_c;
	class MouseInputDevice_c;

	namespace Gui
	{
		class Form_c;
	}

	class PH_ENGINE_CORE_API Session_c: public CoreModule_c,
		private InputManagerListener_c, 
		private InputDeviceListener_c
	{
		PH_DECLARE_SINGLETON_METHODS(Session);

		public:
			~Session_c();

			void SetPlayerCommandProducer(IPlayerCommandProducer_c *commandProducer);
			void SetClient(IClient_c *client);
			void SetForm(Gui::Form_c *form);
			void CloseConsole();

		protected:
			virtual void OnFixedUpdate();

		private:
			Session_c();			

			void InputManagerEvent(const InputManagerEvent_s &event);
			void InputEvent(const InputEvent_s &event);
			
			void ClipMouseCursor();
			void UnclipMouseCursor();

			void DisableGameInput();
			void EnableGameInput();

		private:
			struct ConfigInfo_s
			{
				MouseInputDevice_c *pclMouse;				
			};		

			struct ConfigInfo_s GetConfig();

		private:			
			InputMapperPtr_t			ipInputMapper;
			bool						fIgnoreConsoleKey;

			IPlayerCommandProducer_c	*pclPlayerCommandProducer;
			IClient_c					*pclClient;
			Gui::Form_c					*pclForm;

	};
}

#endif