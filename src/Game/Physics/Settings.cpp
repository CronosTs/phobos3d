/*
Phobos 3d
May 2012
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

#include "Phobos/Game/Physics/Settings.h"

#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>

#include "Phobos/Game/Physics/Keys.h"

namespace
{
	static Phobos::Register::Table *pclCollisionGroups_gl;
	static Phobos::Register::Table *pclStaticWorldCollision_gl;
	static Phobos::Register::Hive *pclStaticMeshCollisionShapeDefHive_gl;
}

void Phobos::Game::Physics::Settings::OnBoot()
{		
	pclCollisionGroups_gl = &Register::GetTable(PH_GAME_PHYSICS_DEF, PH_GAME_TABLE_COLLISION_GROUP);
	pclStaticWorldCollision_gl = &Register::GetTable(PH_GAME_PHYSICS_DEF, PH_GAME_TABLE_STATIC_WORLD_COLLISION);
	pclStaticMeshCollisionShapeDefHive_gl = Register::TryGetHive("StaticMeshCollisionShapeDef");					
}

Phobos::UInt32_t Phobos::Game::Physics::Settings::DecodeCollisionMask(const String_t &config)
{
	UInt32_t flags = 0;

	String_t stringFlag;
	size_t pos = 0;
	while(StringSplitBy(stringFlag, config, '|', pos, &pos))
	{
		StringTrim(stringFlag, STRING_TRIM_BOTH);

		flags |= pclCollisionGroups_gl->GetInt(stringFlag);
	}

	return flags;
}

Phobos::Game::Physics::CollisionTag Phobos::Game::Physics::Settings::LoadCollisionTag(const Register::Table &dict)
{
	UInt32_t group = Settings::DecodeCollisionMask(dict.GetString(PH_GAME_PHYSICS_KEY_COLLSION_GROUP));
	UInt32_t filter = DecodeCollisionMask(dict.GetString(PH_GAME_PHYSICS_KEY_COLLSION_FILTER));

	return Physics::CollisionTag(group, filter);
}

Phobos::Game::Physics::CollisionTag Phobos::Game::Physics::Settings::CreateStaticWorldCollisionTag()
{
	return Settings::LoadCollisionTag(*pclStaticWorldCollision_gl);
}

const Phobos::Register::Table *Phobos::Game::Physics::Settings::TryGetStaticMeshCollisionShapeDef(const String_t &name)
{
	return pclStaticMeshCollisionShapeDefHive_gl ? pclStaticMeshCollisionShapeDefHive_gl->TryGetTable(name) : NULL;
}
