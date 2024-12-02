#include "public/clothe_raylib.h"
#include "public/cloth.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace JE {

	ClotheRaylib::ClotheRaylib(unsigned int rows, unsigned int columns, float length_x, float length_y) : Cloth(rows, columns, length_x, length_y){

		m_sphere_matrices.reserve(rows * columns);
		m_sphere_mesh = GenMeshSphere(0.01f, 8, 8);
		m_mat_default = LoadMaterialDefault();
		m_shader.id = 0;
	}

	ClotheRaylib::~ClotheRaylib(){
		if (m_tex.id != 0) {
			UnloadTexture(m_tex);
		}
		if (m_shader.id != 0) {
			UnloadShader(m_shader);
		}
	}

	void ClotheRaylib::InitClothe(Vec3 first_pos, float mass, float friction_factor) {

		Cloth::InitClothe(first_pos, mass, friction_factor);

		const std::vector<Vertex>& vertices = GetVertices();
		

		

		for (auto& v : vertices) {
			position.push_back(v.pos.x);
			position.push_back(v.pos.y);
			position.push_back(v.pos.z);

			normal.push_back(v.normal.x);
			normal.push_back(v.normal.y);
			normal.push_back(v.normal.z);

			uv.push_back(v.uv.x);
			uv.push_back(v.uv.y);
		}

		for (int row = 0; row < m_rows - 1; ++row) {
			for (int col = 0; col < m_columns - 1; ++col) {
				int top_left = row * m_columns + col;             // v0
				int top_right = top_left + 1;                      // v1
				int bottom_left = (row + 1) * m_columns + col;    // v2
				int bottom_right = bottom_left + 1;                // v3

				// Primer triángulo: v0, v1, v2
				indices.push_back(top_left);
				indices.push_back(bottom_left);
				indices.push_back(top_right);

				// Segundo triángulo: v2, v1, v3
				indices.push_back(bottom_left);
				indices.push_back(bottom_right);
				indices.push_back(top_right);
			}
		}

		m_mesh.vertices = &(position[0]);
		m_mesh.normals = &(normal[0]);
		m_mesh.texcoords = &(uv[0]);
		m_mesh.indices = &(indices[0]);

		m_mesh.vertexCount = m_columns * m_rows;
		m_mesh.triangleCount = indices.size() / 3;

		UploadMesh(&m_mesh, true);
		m_model = LoadModelFromMesh(m_mesh);


	}

	void ClotheRaylib::SetColor(Color color){
		m_color = color;
	}

	void ClotheRaylib::DrawWireFrame(){
		float x, y, z;
		float x2, y2, z2;


		float scale = 0.01f;

		int index = 0;
		m_sphere_matrices.clear();

		for (unsigned int row = 0; row < m_rows; row++) {
			for (int i = 0; i < m_num_particles_per_rope; i++) {

				GetPosition(row, i, x, y, z);

				Matrix mat = {
					scale, 0.0f, 0.0f, 0.0f,   // Primera fila
					0.0f, scale, 0.0f, 0.0f,   // Segunda fila
					0.0f, 0.0f, scale, 0.0f,   // Tercera fila
					x, y, z, 1.0f              // Cuarta fila (posición)
				};
				m_sphere_matrices.push_back(mat);

				//DrawSphere(Vector3{ x, y, z }, 0.01f, RED);

				if (i < m_num_particles_per_rope - 1) {
					Vector3 startPos = { x,y,z };
					GetPosition(row, i + 1, x2, y2, z2);
					Vector3 endPos = { x2,y2,z2 };
					DrawLine3D(startPos, endPos, m_color);

				}
				if (row < m_rows - 1) {
					Vector3 startPos = { x,y,z };
					GetPosition(row + 1, i, x2, y2, z2);
					Vector3 endPos = { x2,y2,z2 };
					DrawLine3D(startPos, endPos, m_color);
				}
				index++;
			}
		}

		DrawMeshInstanced(m_sphere_mesh, m_mat_default, &(m_sphere_matrices[0]), m_rows * m_num_particles_per_rope);
	}

	void ClotheRaylib::DrawClothe(){

		/*
		Matrix model = MatrixIdentity(); // O la matriz de transformación de tu modelo
		Matrix view = GetCameraMatrix(m_cam); // Obtén la matriz de vista de la cámara
	
		Matrix projection = MatrixPerspective(m_cam.fovy, (float)GetScreenWidth() / (float)GetScreenHeight(), 0.1f, 1000.0f);
		

		SetShaderValueMatrix(m_shader, GetShaderLocation(m_shader, "model"), model);
		SetShaderValueMatrix(m_shader, GetShaderLocation(m_shader, "view"), view);
		SetShaderValueMatrix(m_shader, GetShaderLocation(m_shader, "projection"), projection);
		*/

		DrawModel(m_model, { 0.0f, 0.0f, 0.0f }, 1.0f, RED);
	
	}

	bool ClotheRaylib::SetTexture(const char* path){

		//assert(m_shader.id != 0);
		
		bool ret = false;
		if (path != nullptr) {
			m_tex = LoadTexture(path);

			SetShaderValueTexture(m_shader, GetShaderLocation(m_shader, "texture0"), m_tex);
			m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_tex;
			if (m_tex.id != 0) ret = true;
		}


		return ret;
	}

	bool ClotheRaylib::SetShader(const char* fragment, const char* vertex){
		m_shader = LoadShader(vertex, fragment);
		
		if(m_shader.id != 0) m_model.materials[0].shader = m_shader;
		
		return m_shader.id != 0;
	}

}