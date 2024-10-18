#include "public/wind_turbine.h"


JE::WindTurbine::WindTurbine(JE::Vec3 position, JE::Vec3 direction, float strength, float spread_angle, float max_distance, bool enabled){

	m_position.x = position.x;
	m_position.y = position.y;
	m_position.z = position.z;

	m_direction.x = direction.x;
	m_direction.y = direction.y;
	m_direction.z = direction.z;

	m_strength = strength;
	m_spreadAngle = spread_angle;
	m_maxDistance = max_distance;
	m_enabled = enabled;
}

JE::WindTurbine::WindTurbine(const WindTurbine& other){
	
	m_position.x = other.m_position.x;
	m_position.y = other.m_position.y;
	m_position.z = other.m_position.z;
	
	m_direction.x = other.m_direction.x;
	m_direction.y = other.m_direction.y;
	m_direction.z = other.m_direction.z;

	m_strength = other.m_strength;
	m_spreadAngle = other.m_spreadAngle;
	m_maxDistance = other.m_maxDistance;
	m_enabled = other.m_enabled;

}

JE::WindTurbine::WindTurbine(WindTurbine&& other){
	m_position.x = other.m_position.x;
	m_position.y = other.m_position.y;
	m_position.z = other.m_position.z;

	m_direction.x = other.m_direction.x;
	m_direction.y = other.m_direction.y;
	m_direction.z = other.m_direction.z;

	m_strength = other.m_strength;
	m_spreadAngle = other.m_spreadAngle;
	m_maxDistance = other.m_maxDistance;
	m_enabled = other.m_enabled;
}

JE::WindTurbine::~WindTurbine(){

}
