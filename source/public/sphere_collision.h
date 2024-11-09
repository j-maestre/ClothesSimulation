#pragma once
#include "JEMath.h"

//class CollisionManager;
namespace JE {

	class SphereCollision {

		private:
			SphereCollision(float radius, Vec3 position);
		public:
			SphereCollision(const SphereCollision&);
			SphereCollision(SphereCollision&&);
			~SphereCollision();

			// Must be called everyframe at begining
			void Reset();

			// This function returns true when collision starts or is overlaping
			bool IsColliding();

			// this callback will be called when a collision starts
			void OnCollisionSuscribe(void* callback);

			bool CheckCollision(const SphereCollision& other);

			virtual void RenderSphere();

			SphereCollision operator=(SphereCollision&& other) {
				if (this != &other) {

					m_callback = other.m_callback;
					m_radius = other.m_radius;
					m_position = other.m_position;
					m_id = other.m_id;

					other.m_callback = nullptr;
					other.m_radius = 0.0f;
					other.m_id = 0;

					other.m_position.x = 0.0f;
					other.m_position.y = 0.0f;
					other.m_position.z = 0.0f;
				 }
				return *this;
			}

			bool operator==(const SphereCollision& other) const{
				return m_id == other.m_id;
			}

			bool operator!=(const SphereCollision& other) const{
				return m_id != other.m_id;
			}

			void DebugDrawSphere_Raylib() const;

			const bool GetColliding() const { return m_is_colliding; }

	
			Vec3 m_position;

		private:
			void* m_callback;
			unsigned int m_id;
			bool m_is_colliding = false;

		protected:
			float m_radius;



		friend class CollisionManager;
	};
}