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

#include "PH_String.h"

#include <boost/algorithm/string/trim.hpp>

namespace Phobos
{
	using namespace std;

	size_t StringReplaceAll(String_c &str, char search, char replace)
	{
		size_t count = 0;
		for(string::iterator it = str.begin(), end = str.end(); it != end; ++it)
		{
			if(*it == search)
			{
				++count;
				*it = replace;
			}
		}

		return count;
	}

	void StringReplaceAll(String_c &out, const String_c &src, char search, char replace)
	{
		out.clear();
		for(string::const_iterator it = src.begin(), end = src.end(); it != end; ++it)
		{
			out += *it == search ? replace : *it;
		}
	}

	void StringTrim(String_c &str, int mode)
	{
		using namespace boost;

		if(mode & STRING_TRIM_LEFT)
		{
			trim_left(str);
		}
		if(mode & STRING_TRIM_RIGHT)
		{
			trim_right(str);
		}
	}

	bool StringIsBlank(const String_c &src)
	{
		const std::locale& loc=std::locale();
		for(string::const_iterator it = src.begin(), end = src.end(); it != end; ++it)
		{
			if(!std::isspace(*it, loc))
				return false;
		}

		return true;
	}

	bool StringSplitBy(String_c &out, const String_c &src, char ch, size_t pos, size_t *outPos)
	{
		const size_t sz = src.length();
		size_t startPos = pos;

		size_t len = 0;

		out.clear();
		for(;pos < sz; ++pos)
		{
			if(src[pos] == ch)
			{
				len = pos - startPos;
				if(len == 0)
				{
					startPos = pos + 1;
					continue;
				}

				++pos;
				goto COPY_SUB;
			}
		}

		//When the loop ends normally, we need to compute len
		len = pos - startPos;

		//end of the string?
		if((pos >= sz) && (len == 0))
			return(false);

		//no split char found, check if we can return he full string
		if(startPos == 0)
		{
			out = src;
		}
		else
		{
	COPY_SUB:
			//out.SetSub(this, startPos, len);
			out.assign(src, startPos, len);
		}

		if(outPos)
			*outPos = pos;

		return(true);
	}

	inline int StringToInt(const String_c &str)
	{
		return atoi(str.c_str());
	}

	Float_t StringToFloat(const String_c &str)
	{
		//check this if types changes
		BOOST_STATIC_ASSERT(sizeof(Float_t) == sizeof(float));

		return static_cast<float>(atof(str.c_str()));
	}

	bool StringToBoolean(const String_c &a)
	{
		if(a.compare("true") == 0)
			return true;
		else if(a.compare("false") == 0)
			return false;
		else
		{
			return StringToInt(a) ? true : false;
		}
	}
}
