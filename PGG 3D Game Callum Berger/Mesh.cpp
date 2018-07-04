//  Mesh.cpp
//  Holds all function code for the Mesh Class

#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//Initialisation upon creation of a mesh class object
Mesh::Mesh()
{

	m_VAO = 0;

	glGenVertexArrays(1, &m_VAO);

	m_numVertices = 0;

}

Mesh::~Mesh()
{

	glDeleteVertexArrays(1, &m_VAO);

}

//Function used to load an object from a file
void Mesh::LoadOBJ(std::string filename)
{
	//retrieves the file from the specified file location
	std::ifstream inputFile(filename);

	//checks whether the file is open
	if (inputFile.is_open())
	{
		//creates vectors for the intial data and the fixed data
		std::vector<glm::vec2> UVData;
		std::vector<glm::vec3> PositionData;
		std::vector<glm::vec3> NormalData;

		std::vector<glm::vec2> fixedUVData;
		std::vector<glm::vec3> fixedPositionData;
		std::vector<glm::vec3> fixedNormalData;

		//floats used for creation of the collision box of the object
		float highestx = 0;
		float curx;
		float highesty = 0;
		float cury;
		float highestz = 0;
		float curz;

		float lowestx = 0;
		float lowesty = 0;
		float lowestz = 0;

		//string variable containing the current line text
		std::string currentLine;

		//while loop used to loop through thr file
		while (std::getline(inputFile, currentLine))
		{
			//variable used to access individual parts of the string
			std::stringstream currentLineStream(currentLine);

			//if statements to loop through the string, finding the specific parts of the object 
			//e.g. the vertices, texture coordinates, vector normals and faces
			//and sets the data to it's corresponding intial vector variable
			if (!currentLine.substr(0, 2).compare(0, 2, "vt"))
			{

				std::string ignoredData;

				float x;
				float y;

				currentLineStream >> ignoredData >> x >> y;

				UVData.push_back(glm::vec2(x, y));

			}
			else if (!currentLine.substr(0, 2).compare(0, 2, "vn"))
			{

				std::string ignoredData;

				float x;
				float y;
				float z;

				currentLineStream >> ignoredData >> x >> y >> z;

				NormalData.push_back(glm::vec3(x, y, z));

			}
			else if (!currentLine.substr(0, 2).compare(0, 1, "v"))
			{

				std::string ignoredData;

				float x;
				float y;
				float z;

				currentLineStream >> ignoredData >> x >> y >> z;

				//if statements finding the biggest and smallest x, y and z variables and setting the box for collision

				curx = x;

				if (curx > highestx)
				{
					highestx = curx;
				}
				else if (curx < lowestx)
				{
					lowestx = curx;
				}

				cury = y;

				if (cury > highesty)
				{
					highesty = cury;
				}
				else if (cury < lowesty)
				{
					lowesty = cury;
				}

				curz = z;

				if (curz > highestz)
				{
					highestz = curz;
				}
				else if (curz < lowestz)
				{
					lowestz = curz;
				}

				m_xWidth = highestx - lowestx;

				m_zWidth = highestz - lowestz;

				m_height = highesty - lowesty;

				PositionData.push_back(glm::vec3(x, y, z));

			}
			else if (!currentLine.substr(0, 2).compare(0, 1, "f"))
			{
				//string variables used to define what is useful vertex data and what is useless data
				std::string ignoredData;
				std::string verticeData[4];

				currentLineStream >> ignoredData >> verticeData[0] >> verticeData[1] >> verticeData[2] >> verticeData[3];

				//checks for triangulated faces
				if (verticeData[3].empty())
				{
					for (unsigned int i = 0; i < 3; i++)
					{
						//splits up the face data into position data, uv data and normal data
						std::stringstream currentSection(verticeData[i]);

						//position data variables
						unsigned int posID = 0;
						unsigned int uvID = 0;
						unsigned int normID = 0;

						if (verticeData[i].find('/') == std::string::npos)
						{
							currentSection >> posID;
						}
						else if (verticeData[i].find('/') == std::string::npos)
						{
							char ignoredData;
							currentSection >> posID >> ignoredData >> ignoredData >> normID;
						}
						else
						{
							char ignoredData;
							currentSection >> posID >> ignoredData >> uvID >> ignoredData >> normID;
						}

						//sets the intial data to the fixed data
						if (posID > 0)
						{
							fixedPositionData.push_back(PositionData[posID - 1]);
						}
						if (uvID > 0)
						{
							fixedUVData.push_back(UVData[uvID - 1]);
						}
						if (normID > 0)
						{
							fixedNormalData.push_back(NormalData[normID - 1]);
						}

					}

				}
				//produces an error if the faces of the object haven't been triangulated
				else
				{
					std::cout << "Quad has been detected, the model needs to be triangulated" << std::endl;
					inputFile.close();
				}

			}

		}
		//closes the file
		inputFile.close();

		//sets the amount of vertices
		m_numVertices = fixedPositionData.size();

		//checks whether the amount of vertices is greater than 0
		if (m_numVertices > 0)
		{
			//binds the VAO 
			glBindVertexArray(m_VAO);

			//creates a buffer variable
			GLuint posBuffer = 0;

			//creates the buffer
			glGenBuffers(1, &posBuffer);

			//binds the buffer
			glBindBuffer(GL_ARRAY_BUFFER, posBuffer);

			//stores the position data passed through within the buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, &fixedPositionData[0], GL_STATIC_DRAW);

			//tells opengl how to link the data to the shader
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(0);

			if (fixedNormalData.size() > 0)
			{

				//creates a buffer variable
				GLuint normBuffer = 0;

				//creates the buffer
				glGenBuffers(1, &normBuffer);

				//binds the buffer
				glBindBuffer(GL_ARRAY_BUFFER, normBuffer);

				//stores the normal data passed through within the buffer
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, &fixedNormalData[0], GL_STATIC_DRAW);

				//tells opengl how to link the data to the shader
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glEnableVertexAttribArray(1);

			}

			if (fixedUVData.size() > 0)
			{

				//creates a buffer variable
				GLuint texBuffer = 0;

				//creates the buffer
				glGenBuffers(1, &texBuffer);

				//binds the buffer
				glBindBuffer(GL_ARRAY_BUFFER, texBuffer);

				//stores the uv data passed through within the buffer
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 2, &fixedUVData[0], GL_STATIC_DRAW);

				//tells opengl how to link the data to the shader
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

				glEnableVertexAttribArray(2);

			}

		}



	}
	//produces an error if the file could not be opened
	else
	{

		std::cout << "The file was not found in directory: " << filename << std::endl;

	}

}

//Function used ot draw the mesh
void Mesh::Draw()
{

	//binds the vao
	glBindVertexArray(m_VAO);

	//draws the mesh
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	//unbinds the VAO
	glBindVertexArray(0);

}

//Get Functions

#pragma region Get Functions

float Mesh::GetXWidth()
{
	return m_xWidth;
}

float Mesh::GetZWidth()
{
	return m_zWidth;
}

float Mesh::GetHeight()
{
	return m_height;
}

#pragma endregion