#ifndef PH_PHOBOS_GAME_LEVEL_MAP_OBJECT_COMPONENT_FACTORY_H
#define PH_PHOBOS_GAME_LEVEL_MAP_OBJECT_COMPONENT_FACTORY_H

#include <Phobos/GenericFactory.h>
#include <Phobos/DisableCopy.h>
#include <Phobos/Register/TableFwd.h>

#include "Phobos/Game/Level/MapObjectComponent.h"


namespace Phobos
{
	namespace Game
	{
		class MapObjectComponentFactory : public GenericFactoryEx<MapObjectComponent, MapObjectComponent::UniquePtr_t, MapObject &, const Phobos::Register::Table &>
		{
			PH_DISABLE_COPY(MapObjectComponentFactory);

			public:
				static MapObjectComponentFactory &GetInstance();

			private:
				inline MapObjectComponentFactory() {};
		};
	}
}

#define PH_MAP_COMPONENT_CREATOR(NAME, PROC)											\
	static Phobos::Game::MapObjectComponentFactory::ObjectCreator_t TYPE_##CreatorObject_gl(NAME, PROC, MapObjectComponentFactory::GetInstance())	

#define PH_MAP_COMPONENT_FULL_CREATOR(NAME, TYPE, POOL)  		\
	MapObjectComponent::UniquePtr_t TYPE::Create(MapObject &owner, const Phobos::Register::Table &table)	\
{												\
	return MapObjectComponent::UniquePtr_t(POOL.construct(std::ref(owner), std::ref(table)));					\
}\
PH_MAP_COMPONENT_CREATOR(NAME, &TYPE::Create);


#endif