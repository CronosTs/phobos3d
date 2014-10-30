#ifndef PH_GAME_MAP_OBJECT_COMPONENT_H
#define PH_GAME_MAP_OBJECT_COMPONENT_H

#include <Phobos/DisableCopy.h>

#include <Phobos/Error.h>
#include <Phobos/Register/TableFwd.h>
#include <Phobos/Types.h>

#include <functional>
#include <memory>

namespace Phobos
{
	namespace Game
	{
		class MapObject;
		class MapObjectComponentAccess;

		class MapObjectComponent
		{
			PH_DISABLE_COPY(MapObjectComponent);

			public:
				typedef std::unique_ptr<MapObjectComponent> UniquePtr_t;

				class LoadFinishedAccess
				{
					friend class MapObject;

					private:
						static void LoadFinished(MapObjectComponent &comp, const Register::Table &table)
						{
							comp.LoadFinished(table);
						}
				};

			public:
				const char *GetType() const
				{
					return m_pszType;
				}

				static void TickReminders();

				virtual ~MapObjectComponent();

			protected:
				MapObjectComponent(const char *type, MapObject &owner) :
					m_rclOwner(owner),
					m_pszType(type)
				{
					PH_ASSERT_VALID(type);
				}				
				
				MapObjectComponentAccess AccessMapObject();

				typedef std::function<void(MapObjectComponent*)> ReminderProcType_t;
				void AddReminder(ReminderProcType_t proc, Float_t delay);

				virtual void OnLoadFinished(const Register::Table &table) {};

				template <typename T>
				inline T &GetOwnerComponent()
				{
					return m_rclOwner.GetComponent<T>();
				}

			private:
				void LoadFinished(const Register::Table &table);

			private:
				MapObject	&m_rclOwner;
				const char	*m_pszType;
		};		
	}
}

#endif
