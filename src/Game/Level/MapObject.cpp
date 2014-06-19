#include "Phobos/Game/Level/MapObject.h"

#include "Phobos/Game/Level/MapObjectComponent.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"

#include <Phobos/Error.h>
#include <Phobos/OgreEngine/Math/Transform.h>
#include <Phobos/OgreEngine/Render.h>

#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace Phobos
{
	namespace Game
	{
		MapObject::Data::~Data()
		{
			auto &render = Phobos::OgreEngine::Render::GetInstance();

			if (m_pclSceneNode)
			{
				render.DestroySceneNode(m_pclSceneNode);
				m_pclSceneNode = NULL;
			}			
		}
	
#if 0
		void MapObject::Data::SetParentNode(Data &node)
		{
			PH_ASSERT(node.m_pclSceneNode);
			PH_ASSERT(m_pclSceneNode);

			m_pclSceneNode->getParent()->removeChild(m_pclSceneNode);
			node.m_pclSceneNode->addChild(m_pclSceneNode);
		}
#endif

		//Why using those _ functions: http://89.151.96.106/forums/viewtopic.php?f=22&t=62386
		//http://www.ogre3d.org/forums/viewtopic.php?p=221113
		//http://www.ogre3d.org/tikiwiki/-SceneNode
		const Ogre::Vector3 &MapObject::Data::GetWorldPosition() const
		{
			return m_pclSceneNode->_getDerivedPosition();
		}

		const Ogre::Vector3 &MapObject::Data::GetWorldScale() const
		{
			return m_pclSceneNode->_getDerivedScale();
		}

		const Ogre::Quaternion &MapObject::Data::GetWorldOrientation() const
		{
			return m_pclSceneNode->_getDerivedOrientation();
		}

		void MapObject::Data::SetPosition(const Ogre::Vector3 &position)
		{
			m_pclSceneNode->setPosition(position);
		}

		void MapObject::Data::SetOrientation(const Ogre::Quaternion &orientation)
		{
			m_pclSceneNode->setOrientation(orientation);
		}
		

		MapObject::~MapObject()
		{
			//empty
		}

#if 0
		void MapObject::SyncSceneToPhysics()
		{
			PH_ASSERT(m_clData.m_ePhysicsType != PhysicsTypes::NONE);
			
			const auto transform = m_clData.m_clRigidBody.GetTransform();

			m_clData.m_pclSceneNode->setOrientation(transform.GetRotation());
			m_clData.m_pclSceneNode->setPosition(transform.GetOrigin());			
		}

		void MapObject::SyncPhysicsToScene()
		{
			PH_ASSERT(m_clData.m_ePhysicsType != PhysicsTypes::NONE);

			Engine::Math::Transform transform(this->GetWorldPosition(), this->GetWorldOrientation());

			m_clData.m_clRigidBody.Warp(transform);
		}
#endif

		void MapObject::AddComponent(const String_t &name)
		{
			auto component = MapObjectComponentFactory::GetInstance().Create(name, *this, m_rclTable);
			m_vecComponents.push_back(std::move(component));
		}

		void MapObject::AddComponent(MapObjectComponent::UniquePtr_t &&component)
		{
			m_vecComponents.push_back(std::move(component));
		}

		void MapObject::AttachOgreObject(Ogre::MovableObject &object)
		{
			m_clData.m_pclSceneNode->attachObject(&object);
		}

		void MapObject::SetTransform(const Engine::Math::Transform &transform)
		{
			m_clData.m_pclSceneNode->setPosition(transform.GetOrigin());
			m_clData.m_pclSceneNode->setOrientation(transform.GetRotation());
		}

#if 0
		void ModelRendererComponent::AttachObjectToBone(
			const Char_t *boneName,
			Ogre::MovableObject &movable,
			const Ogre::Quaternion &offsetOrientation,
			const Ogre::Vector3 &offsetPosition
			)
		{
			m_pclMeshEntity->attachObjectToBone(boneName, &movable, offsetOrientation, offsetPosition);
		}

		void ModelRendererComponent::AttachObjectToBone(
			const String_t &boneName,
			Ogre::MovableObject &movable,
			const Ogre::Quaternion &offsetOrientation,
			const Ogre::Vector3 &offsetPosition
			)
		{
			m_pclMeshEntity->attachObjectToBone(boneName, &movable, offsetOrientation, offsetPosition);
		}

		Ogre::Bone &ModelRendererComponent::GetBone(const char *boneName)
		{
			return *m_pclMeshEntity->getSkeleton()->getBone(boneName);
		}

		Ogre::Bone &ModelRendererComponent::GetBone(const String_t &boneName)
		{
			return *m_pclMeshEntity->getSkeleton()->getBone(boneName);
		}
#endif

		MapObject::ComponentEnumerator::ComponentEnumerator(const ComponentEnumerator &rhs):
			m_pszType(rhs.m_pszType),
			m_rclMapObject(rhs.m_rclMapObject),
			m_itCurrent(rhs.m_itCurrent),
			m_itPosition(rhs.m_itPosition),
			m_itEnd(rhs.m_itEnd)			
		{
			//empty
		}
					
		MapObject::ComponentEnumerator::ComponentEnumerator(MapObject &object, const char *type):
			m_pszType(type),
			m_rclMapObject(object),
			m_itPosition(object.m_vecComponents.begin()),
			m_itEnd(object.m_vecComponents.end()),
			m_itCurrent(object.m_vecComponents.end())
		{
			PH_ASSERT_VALID(type);
		}
		
		bool MapObject::ComponentEnumerator::Next()
		{									
			for (; m_itPosition != m_itEnd; ++m_itPosition)
			{
				if (strcmp(m_itPosition->get()->GetType(), m_pszType))
					continue;

				m_itCurrent = m_itPosition;

				++m_itPosition;
				
				return true;
			}			
						
			m_itCurrent = m_itPosition;

			return false;			
		}

		MapObjectComponent *MapObject::ComponentEnumerator::GetCurrent()
		{
			return (m_itCurrent == m_itEnd) ? nullptr : m_itCurrent->get();
		}
	}
}
