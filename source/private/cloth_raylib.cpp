#include "public/clothe_raylib.h"
#include "public/cloth.h"
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

namespace JE {

	ClotheRaylib::ClotheRaylib(unsigned int rows, unsigned int columns, float length_x, float length_y) : Cloth(rows, columns, length_x, length_y){

		//createShaderProgram("assets/vertex_shader.vs", "assets/fragment_shader.fs");

	}

	ClotheRaylib::~ClotheRaylib(){
		UnloadTexture(m_tex);
	}

	void ClotheRaylib::SetColor(Color color){
		m_color = color;
	}

	void ClotheRaylib::DrawClothe(){

		float x, y, z;
		float x2, y2, z2;

		for (unsigned int row = 0; row < m_rows; row++) {
			for (int i = 0; i < m_num_particles_per_rope; i++) {

				GetPosition(row, i, x, y, z);
				DrawSphere(Vector3{ x, y, z }, 0.01f, RED);

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
			}
		}
	}

	bool ClotheRaylib::SetTexture(const char* path){
		
		bool ret = false;
		if (path != nullptr) {
			m_tex = LoadTexture(path);
			if (m_tex.id != 0) ret = true;
		}


		return ret;
	}

	std::string ClotheRaylib::readShaderCode(const char* filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	GLuint ClotheRaylib::compileShader(const char* filename, GLenum shaderType){
		std::string code = readShaderCode(filename);
		const char* shaderCode = code.c_str();

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		// Comprobar errores de compilación
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "Error al compilar el shader (" << filename << "): " << infoLog << std::endl;
		}

		return shader;
	}

	/*
	GLuint ClotheRaylib::createShaderProgram(const char* vertexPath, const char* fragmentPath) {
		m_vertex_shader = compileShader(vertexPath, GL_VERTEX_SHADER);
		m_fragment_shader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

		m_program = glCreateProgram();
		glAttachShader(m_program, m_vertex_shader);
		glAttachShader(m_program, m_fragment_shader);
		glLinkProgram(m_program);

		// Comprobar errores de enlace
		GLint success;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(m_program, 512, NULL, infoLog);
			std::cerr << "Error al enlazar el shader program: " << infoLog << std::endl;
		}

		// Eliminar shaders ya que están enlazados
		glDeleteShader(m_vertex_shader);
		glDeleteShader(m_fragment_shader);

		return m_program;
	}
	*/

}