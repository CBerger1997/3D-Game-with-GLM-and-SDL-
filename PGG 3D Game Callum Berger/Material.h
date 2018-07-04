// \Class used to create the material for an object
// \author Callum Berger

#ifndef _Material_
#define _Material_

#include "glew.h"
#include <glm.hpp>

#include <string>

//Material Class decleration
class Material
{
public:
	Material();
	~Material();

	bool ShaderCreation(std::string, std::string);

	bool CheckShaderCreation(GLint);

	void SetUniforms(glm::mat4, glm::mat4, glm::mat4, glm::vec4);

	GLuint LoadTexture(std::string);
	bool SetTexture(std::string);

	void ApplyTexture();

private:

	int m_shaderProgram;

	int m_modelMatLocation;
	int m_viewMatLocation;
	int m_projMatLocation;
	int m_lightPositionLocation;

	int m_textureSamplerLocation;

	GLuint m_texture;

};

#endif //Defines _Material_