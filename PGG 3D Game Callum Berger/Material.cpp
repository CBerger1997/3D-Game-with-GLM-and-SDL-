//  Material.cpp
//  Holds all function code for the Material Class

#include "Material.h"

#include <iostream>
#include <fstream>
#include <SDL.h>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

//initialisation upon creating a Material class object
Material::Material()
{
	//initialises variables used for connection to shaders
	m_modelMatLocation = 0;
	m_viewMatLocation = 0;
	m_projMatLocation = 0;

	m_textureSamplerLocation = 0;

	m_texture = 0;
}

Material::~Material()
{

}

//function used to create the vertex and fragmetn shader
bool Material::ShaderCreation(std::string vertexFileName, std::string fragmentFileName)
{
	//Loading the vertex shader from file
	std::ifstream vertexFile(vertexFileName);

	//variable used to store the text read from the vertex shader file
	char *vertexShaderText = NULL;

	//checks whether the vertex shader file has opened
	if (vertexFile.is_open())
	{
		//variable used to store the amount of characters in the opened file
		int fileLength = 0;

		//stores amount of characters in the file into the fileLength variable
		vertexFile.seekg(0, vertexFile.end);
		fileLength = (int)vertexFile.tellg();
		vertexFile.seekg(0, vertexFile.beg);

		//Create the buffer
		vertexShaderText = new char[fileLength];

		//transfer the data from the file to the buffer
		vertexFile.read(vertexShaderText, fileLength);

		//check that the end of the file has been reached
		if (!vertexFile.eof())
		{
			vertexFile.close();
			std::cout << "Vertex file could not be read from location: " << vertexFileName << std::endl;
			return false;
		}

		//calculate how many characters were read from the file
		fileLength = (int)vertexFile.gcount();

		//terminates
		vertexShaderText[fileLength - 1] = 0;

		//closes the file
		vertexFile.close();

	}
	else
	{
		std::cout << "Vertex file could not be read from location: " << vertexFileName << std::endl;
		return false;
	}

	//Loading the fragment shader from file
	std::ifstream fragmentFile(fragmentFileName);

	//variable used to store the text read from the fragment shader file
	char *fragmentShaderText = NULL;

	//checks whether the fragment shader file has opened
	if (fragmentFile.is_open())
	{
		//variable used to store the amount of characters in the opened file
		int fileLength = 0;

		//stores amount of characters in the file into the fileLength variable
		fragmentFile.seekg(0, fragmentFile.end);
		fileLength = (int)fragmentFile.tellg();
		fragmentFile.seekg(0, fragmentFile.beg);

		//Create the buffer
		fragmentShaderText = new char[fileLength];

		//transfer the data from the file to the buffer
		fragmentFile.read(fragmentShaderText, fileLength);

		//check that the end of the file has been reached
		if (!fragmentFile.eof())
		{
			fragmentFile.close();
			std::cout << "Fragment file could not be read from location: " << fragmentFileName << std::endl;
			return false;
		}

		//calculate how many characters were read from the file
		fileLength = (int)fragmentFile.gcount();

		//sets the end point
		fragmentShaderText[fileLength - 1] = 0;

		//closes the file
		fragmentFile.close();

	}
	else
	{
		std::cout << "Fragment file could not be read from location: " << fragmentFileName << std::endl;
		return false;
	}

	//program used to store the shaders
	m_shaderProgram = glCreateProgram();

	//creates the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//gives OpenGl the source for the shader
	glShaderSource(vertexShader, 1, &vertexShaderText, NULL);

	//deletes the buffer
	delete[] vertexShaderText;

	//compiles the shader
	glCompileShader(vertexShader);

	if (!CheckShaderCreation(vertexShader))
	{
		std::cout << "Failed to compile the Vertex Shader" << std::endl;
		return false;
	}

	//attaches the vertex shader to the program
	glAttachShader(m_shaderProgram, vertexShader);

	//creates the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//gives OpenGl the source for the shader
	glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);

	//deletes the buffer
	delete[] fragmentShaderText;

	//compiles the shader
	glCompileShader(fragmentShader);

	if (!CheckShaderCreation(fragmentShader))
	{
		std::cout << "Failed to compile the Fragment Shader" << std::endl;
		return false;
	}
	//attaches the fragment shader to the program
	glAttachShader(m_shaderProgram, fragmentShader);

	//connects the vertex and fragment shaders together
	glLinkProgram(m_shaderProgram);

	//checks that the link worked correctly producing an error with the log upon a failed link
	GLint Linked;

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &Linked);

	if (!Linked)
	{
		GLsizei	len;

		glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];

		glGetProgramInfoLog(m_shaderProgram, len, &len, log);
		std::cout << "Shaders failed to link: " << log << std::endl;
		delete[] log;

		return false;
	}

	//tells opengl to use the specified program
	glUseProgram(m_shaderProgram);

	//finds the locations of each matrix within the shaders and stores their location in the corresponding variable
	m_modelMatLocation = glGetUniformLocation(m_shaderProgram, "modelMat");
	m_viewMatLocation = glGetUniformLocation(m_shaderProgram, "viewMat");
	m_projMatLocation = glGetUniformLocation(m_shaderProgram, "projMat");

	m_lightPositionLocation = glGetUniformLocation(m_shaderProgram, "lightPosition");

	m_textureSamplerLocation = glGetUniformLocation(m_shaderProgram, "tex");

	glUseProgram(0);

	return true;

}

//Function used to make sure that the shaders were created successfully
bool Material::CheckShaderCreation(GLint shader)
{
	GLint compiled;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{

		GLsizei len;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];

		glGetShaderInfoLog(shader, len, &len, log);

		std::cout << "Shader compilation failed: " << log << std::endl;

		delete[] log;

		return false;

	}

	return true;
}

//Function used to load a texture from a file
GLuint Material::LoadTexture(std::string filename)
{
	//creates and sdl surface variable used to store the BMP file
	SDL_Surface *image = SDL_LoadBMP(filename.c_str());

	if (!image)
	{
		std::cout << "could not load BMP image from directory: " << filename << std::endl;
	}

	//creates a texture for OpenGl
	GLuint texName = 0;

	//creates the texture
	glGenTextures(1, &texName);

	//binds the texture
	glBindTexture(GL_TEXTURE_2D, texName);

	//sets the parameters for the texture so that the texture image is repeated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//sets the texture to work so that the pixel closest to the texture coordinate is selected
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//tells OpenGl to use linear filtering, approximating neighbouring colours for a combined colour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//loads the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);

	//used to generate a mipmap for the texture
	glGenerateMipmap(GL_TEXTURE_2D);

	//frees the surface for memory
	SDL_FreeSurface(image);

	//unbinds the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texName;

}

//Funciton used to set the matrices for the shaders
void Material::SetUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::vec4 lightPosition)
{
	//sets the shader program as the program to be used
	glUseProgram(m_shaderProgram);

	//sets the matrix values relevant to their location within the shader program
	glUniformMatrix4fv(m_modelMatLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(m_viewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(m_projMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));

	glUniform4f(m_lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);

	glUseProgram(0);

}

//Function used to apply the shaderprogram to the texture
void Material::ApplyTexture()
{
	//sets the shader program as the program to be used

	glUseProgram(m_shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(m_textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

}

//Set and Get Functions
#pragma region Set Functions

bool Material::SetTexture(std::string filename)
{
	m_texture = LoadTexture(filename);

	return m_texture > 0;

}

#pragma endregion


