/*
Phobos 3d
January 2010
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
#include <boost/test/unit_test.hpp>

#include <string>

#include <boost/filesystem.hpp>

#include <Phobos/Log.h>

#include "SimpleLogListener.h"

using namespace boost::filesystem;
using namespace std;

using namespace Phobos;

static const string LOG_FILE_NAME("logTest.log");

BOOST_AUTO_TEST_CASE(log_basic)
{	
	if(exists(LOG_FILE_NAME))
		BOOST_REQUIRE(remove(LOG_FILE_NAME));

	BOOST_REQUIRE(!exists(LOG_FILE_NAME));	

	size_t size;
	
	{
		Log log(LOG_FILE_NAME, 0);

		BOOST_REQUIRE(exists(LOG_FILE_NAME));	
		size = static_cast<size_t>(file_size(LOG_FILE_NAME));

		log.Message("Test");

		size_t current = static_cast<size_t>(file_size(LOG_FILE_NAME));
		BOOST_REQUIRE(current > size);
		size = current;
	}

	{
		Log log(LOG_FILE_NAME, 0);
		
		BOOST_REQUIRE(file_size(LOG_FILE_NAME) < size);
	}
}

BOOST_AUTO_TEST_CASE(log_listener)
{
	Log log(LOG_FILE_NAME, 0);

	BOOST_CHECK(log.GetListenersCount() == 0);	

	{
		SimpleLogListener a;

		log.AddListener(a);
		BOOST_CHECK(a.oFlag == false);
		BOOST_CHECK(log.GetListenersCount() == 1);

		log.Message("test");
		BOOST_CHECK(a.oFlag == true);
	}

	BOOST_CHECK(log.GetListenersCount() == 0);

	SimpleLogListener a, b;

	log.AddListener(a);
	log.Message("bla");
	BOOST_CHECK(a.oFlag == true);

	a.oFlag = false;
	log.AddListener(b);
	log.Message("bla");
	BOOST_CHECK(a.oFlag == true);
	BOOST_CHECK(b.oFlag == true);
}
