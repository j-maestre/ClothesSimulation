#pragma once
#include <vector>
#include "JEMath.h"
#include "sphere_collision.h"

namespace JE {

	class CollisionManager {

		private:
			CollisionManager();

		public:
			static CollisionManager* GetInstance();

			CollisionManager(const CollisionManager&) = delete;
			CollisionManager(CollisionManager&&) = delete;
			~CollisionManager();


			unsigned int CreateSphereCollision(float radius, Vec3 position);
			const SphereCollision& GetsphereCollision(unsigned int id) const;

			void Update(float dt);

		private:
			static CollisionManager* m_instance;
			std::vector<SphereCollision> m_spheres;
			unsigned int m_ids = 0;
	};
}