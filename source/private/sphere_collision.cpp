#include "public/sphere_collision.h"
#include "raylib.h"

namespace JE {

	SphereCollision::SphereCollision(float radius, Vec3 position) : m_radius(radius), m_position(position){

	}

	SphereCollision::SphereCollision(const SphereCollision& other){
		m_id = other.m_id;
		m_position = other.m_position;
		m_radius = other.m_radius;
		m_callback = other.m_callback;
	}

	SphereCollision::SphereCollision(SphereCollision&& other){
		
		m_id = other.m_id;
		m_position = other.m_position;
		m_radius = other.m_radius;
		m_callback = other.m_callback;
	}

	SphereCollision::~SphereCollision()
	{
	}

	bool SphereCollision::IsColliding()
	{
		return false;
	}

	void SphereCollision::OnCollisionSuscribe(void* callback)
	{
	}

	void SphereCollision::CheckCollision(const SphereCollision& other){
		// Check if this sphere is colliding with other

	}

	void SphereCollision::RenderSphere()
	{
	}

	void SphereCollision::DebugDrawSphere_Raylib() const{
		
		DrawSphereWires(Vector3{ m_position.x, m_position.y, m_position.z }, m_radius * 0.1f, 16, 16, RED);
	}
}
