/*
Phobos 3d
May 2010
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

#include "W32/PH_InputManagerW32.h"

#include <PH_Kernel.h>

#include "PH_InputDefs.h"

#include "W32/PH_KeyboardInputDeviceW32.h"
#include "W32/PH_MouseInputDeviceW32.h"

namespace Phobos
{
	static InputManagerPtr_t CreateInstanceLocalImpl(const String_c &name)
	{
		return InputManagerPtr_t(new InputManagerW32_c(name));
	}

	InputManagerPtr_t InputManager_c::CreateInstanceImpl(const String_c &name)
	{
		return CreateInstanceLocalImpl(name);
	}

	InputManagerW32_c::InputManagerW32_c(const String_c &name):
		InputManager_c(name),
		fPooled(false)
	{
		Kernel_c::GetInstance().LogMessage("[InputManagerW32] Created.");
	}

	void InputManagerW32_c::PollDevices(void)
	{
		if(fPooled)
			return;

		fPooled = true;
		this->AttachDevice(KeyboardInputDeviceW32_c::Create(InputManager_c::GetDeviceTypeName(INPUT_DEVICE_KEYBOARD)), 0);
		this->AttachDevice(MouseInputDeviceW32_c::Create(InputManager_c::GetDeviceTypeName(INPUT_DEVICE_MOUSE)), 0);
	}
}
