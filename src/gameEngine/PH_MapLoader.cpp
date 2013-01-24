/*
Phobos 3d
September 2010
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


#include "PH_MapLoader.h"

#include <iostream>
#include <vector>

#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>

#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_GameWorld.h"

#define WORLD_SPAWN_ENTITY "worldspawn"

namespace Phobos
{	
	Register::Hive_c *MapLoader_c::pclStaticEntitiesHive_g = NULL;
	Register::Hive_c *MapLoader_c::pclDynamicEntitiesHive_g = NULL;
	Register::Hive_c *MapLoader_c::pclCurrentLevelHive_g = NULL;

	void MapLoader_c::OnBoot()
	{		
		pclCurrentLevelHive_g = &Register::CreateCustomHive("LevelInfo");
		pclStaticEntitiesHive_g = &Register::CreateCustomHive("StaticEntities");
		pclDynamicEntitiesHive_g = &Register::CreateCustomHive("DynamicEntities");		
	}	

	const Register::Hive_c &MapLoader_c::GetStaticEntitiesHive() const
	{
		return *pclStaticEntitiesHive_g;
	}

	const Register::Hive_c &MapLoader_c::GetDynamicEntitiesHive() const
	{
		return *pclDynamicEntitiesHive_g;
	}

	const Register::Hive_c &MapLoader_c::GetCurrentLevelHive() const
	{
		return *pclCurrentLevelHive_g;
	}

	void MapLoader_c::ClearAllHives()
	{
		pclCurrentLevelHive_g->RemoveAllChildren();
		pclStaticEntitiesHive_g->RemoveAllChildren();
		pclDynamicEntitiesHive_g->RemoveAllChildren();
	}

	MapLoader_c::MapLoader_c(const Register::Table_c &settings):
		strWorldSpawnEntityType(settings.GetString("worldSpawnEntityDef"))
	{
		//empty
	}

	std::unique_ptr<Entity_c> MapLoader_c::CreateAndLoadWorldSpawn()
	{	
		Register::Table_c &entityDef = pclCurrentLevelHive_g->GetTable(WORLD_SPAWN_ENTITY);

		std::unique_ptr<Entity_c> ptr(EntityFactory_c::GetInstance().Create(entityDef.GetString(PH_ENTITY_KEY_CLASS_NAME), entityDef.GetName()));
		ptr->Load(entityDef);

		return ptr;
	}

	GameWorldPtr_t MapLoader_c::CreateAndLoadWorld()
	{
		GameWorldPtr_t world = this->CreateGameWorld();

		world->Load(*this, pclCurrentLevelHive_g->GetTable(WORLD_SPAWN_ENTITY));

		return world;
	}

	std::unique_ptr<Register::Table_c> MapLoader_c::CreateWorldSpawnEntityDef()
	{
		std::unique_ptr<Register::Table_c> dict(PH_NEW Register::Table_c(WORLD_SPAWN_ENTITY));

		dict->SetBaseHive("EntityDef");
		dict->SetInherited(strWorldSpawnEntityType);

		return dict;
	}
}
