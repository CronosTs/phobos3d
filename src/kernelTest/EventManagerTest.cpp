/*
Phobos 3d
  version 0.0.1, January 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include <boost/test/unit_test.hpp>

#include <PH_EventManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

using namespace Phobos;

BOOST_AUTO_TEST_CASE(eventManager_basic)
{
	Kernel_c &kernel = Kernel_c::CreateInstance("eventManager_basic.log");
	EventManager_c::CreateInstance(EventManager_c::GetDefaultName());

	NodePtr_t node = kernel.LookupObject(Path_c("/System/EventManager"));
	BOOST_REQUIRE(node);

	EventManager_c::ReleaseInstance();

	BOOST_REQUIRE_THROW(kernel.LookupObject(Path_c("/System/EventManager")), ObjectNotFoundException_c);	

	Kernel_c::ReleaseInstance();
}
