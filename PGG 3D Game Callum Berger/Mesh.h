// \Class used to create an object mesh
// \author Callum Berger

#ifndef _Mesh_
#define _Mesh_

#include <glm.hpp>
#include <SDL.h>
#include "glew.h"
#include <string>

//Mesh Class decleration
class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadOBJ(std::string);

	void Draw();

	float GetXWidth();
	float GetZWidth();
	float GetHeight();

private:
	GLuint m_VAO;

	unsigned int m_numVertices;

	float m_xWidth;
	float m_zWidth;
	float m_height;
};

#endif //defines _Mesh_