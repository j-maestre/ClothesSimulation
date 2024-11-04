#include "cloth.h"
#include <raylib.h>
#include <string>
#include <GL/glew.h>

namespace JE {

	class ClotheRaylib : public Cloth {
		public:
			ClotheRaylib(unsigned int rows, unsigned int columns, float length_x, float lenght_y);
			ClotheRaylib(const ClotheRaylib&);
			ClotheRaylib(ClotheRaylib&&);
			~ClotheRaylib();

			void SetColor(Color color);
			virtual void DrawClothe();

			bool SetTexture(const char* path);

		private:
			Color m_color = GREEN;

			Texture2D m_tex;
			std::string m_vertex_shader;
			std::string m_fragment_shader;

			std::string readShaderCode(const char* filename);
			bool compileShader(const char* filename, GLenum shaderType);
	};



}