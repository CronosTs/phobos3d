/*
Phobos 3d
August 2012
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

#ifndef PH_GAME_PHYSICS_COLLISION_TAG_H
#define PH_GAME_PHYSICS_COLLISION_TAG_H

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			class CollisionTag
			{
				private:
					UInt32_t m_u32Group;
					UInt32_t m_u32Filter;

				public:					
					inline CollisionTag(UInt32_t group, UInt32_t filter):
						m_u32Group(group),
						m_u32Filter(filter)
					{
						//empty
					}

					inline CollisionTag(const CollisionTag &rhs):
						m_u32Group(rhs.m_u32Group),
						m_u32Filter(rhs.m_u32Filter)
					{
						//empty
					}

					CollisionTag &operator=(const CollisionTag &rhs)
					{
						m_u32Group = rhs.m_u32Group;
						m_u32Filter = rhs.m_u32Filter;

						return *this;
					}

					inline UInt32_t GetGroup() const
					{
						return m_u32Group;
					}

					inline UInt32_t GetFilter() const
					{
						return m_u32Filter;
					}
			};
		}
	}
}

#endif
