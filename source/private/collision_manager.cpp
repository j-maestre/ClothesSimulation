#include "public/collision_manager.h"
#include "assert.h"

namespace JE{

	CollisionManager* CollisionManager::m_instance = nullptr;

	CollisionManager* CollisionManager::GetInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new CollisionManager();
		}
		return m_instance;
	}

	CollisionManager::CollisionManager() {}

	CollisionManager::~CollisionManager(){
		delete m_instance;
	}

	unsigned int CollisionManager::CreateSphereCollision(float radius, Vec3 position){
		assert(radius > 0.0f && "Radius must be greater than 0");
		m_ids++;

		SphereCollision tmp(radius, position);
		tmp.m_id = m_ids;

		m_spheres.push_back(std::move(tmp));
		
		return tmp.m_id;
	}

	const SphereCollision& CollisionManager::GetsphereCollision(unsigned int id) const{
		for (const auto& sphere : m_spheres) {
			if (sphere.m_id == id) return sphere;
		}
	}

	void CollisionManager::Update(float dt){
		// int j = i para empezar y no repetir comprobaciones 
		for (const SphereCollision& sphere_first : m_spheres) {
			sphere_first.DebugDrawSphere_Raylib();	
			for (const SphereCollision& sphere_second : m_spheres) {
				if (sphere_first != sphere_second) {
					
				}

			}
		}
	}
}
