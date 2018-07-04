//  Scene.cpp
//  Holds all function code for the core of the program

#include <SDL.h>
#include "glew.h"

#include "Scene.h"

#include <iostream>
#include <string>

bool InitialiseSDL();
bool InitialiseOpenGl();
SDL_Window* InitialiseWindow(SDL_Window*);

void Update(bool, SDL_Window*, SDL_Renderer*);

void Renderer(Scene, SDL_Window*, float);

//main Function
int main(int argc, char *argv[])
{
	//creates the bool to loop through the program
	bool gameLoop = false;

	//creates the SDL variables for displaying the window and connecting SDL to OPenGL
	SDL_Window *gameWindow = 0;
	SDL_Renderer *gameRenderer = 0;
	SDL_GLContext gameContext = 0;

	//initialises SDL, allowing the program to run if initialised correctly
	gameLoop = InitialiseSDL();

	//initialies the window and renderer and connects SDL and OpenGl
	gameWindow = InitialiseWindow(gameWindow);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0);
	gameContext = SDL_GL_CreateContext(gameWindow);

	//initialises OpenGl, allowing the program to run if initialised correctly
	gameLoop = InitialiseOpenGl();

	//Enables depth
	glEnable(GL_DEPTH_TEST);

	//Runs the Update function
	Update(gameLoop, gameWindow, gameRenderer);

	//deletes the SDL and OpenGl context, the window and then quits SDL
	SDL_GL_DeleteContext(gameContext);
	SDL_DestroyWindow(gameWindow);
	SDL_Quit();

	return 0;
}

//Initialisation functions
#pragma region Initialisation

//Function used to initialise SDL
bool InitialiseSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not intialise correctly or cannot initialise" << std::endl;
		return false;
	}

	return true;
}

//Function used to initialise OpenGl
bool InitialiseOpenGl()
{
	GLenum checkGlew;

	glewExperimental = GL_TRUE;

	checkGlew = glewInit();

	//Checks whether OpenGl initialised correctly
	if (checkGlew != GLEW_OK)
	{

		std::cerr << "Glew did not initalise correctly or cannot be initialised" << std::endl;
		return false;
	}

	return true;
}


//Initialises the SDL window
SDL_Window* InitialiseWindow(SDL_Window* window)
{
	int windowXPosition = SDL_WINDOWPOS_CENTERED;
	int windowYPosition = SDL_WINDOWPOS_CENTERED;
	int windowHeight = 1000;
	int windowWidth = 1000;

	window = SDL_CreateWindow("3D Game", windowXPosition, windowYPosition, windowWidth, windowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	return window;
}

#pragma endregion

//Update function used to loop through the program
void Update(bool gameLoop, SDL_Window* gameWindow, SDL_Renderer* gameRenderer)
{
	//creates the array for checking movement
	bool movement[4];

	//for loop used to set all movement variables to false
	for (int i = 0; i < 4; i++)
	{
		movement[i] = false;
	}

	//checks for the number of milliseconds since SDL was initialised
	unsigned int lastFrame = SDL_GetTicks();

	//creates the scene
	Scene gameScene;

	//while loop used to loop through the program
	while (gameLoop)
	{

		//creates the event variable
		SDL_Event gameEvent;

		//while loop to check for events
		while (SDL_PollEvent(&gameEvent))
		{

			switch (gameEvent.type)
			{
				//checks for quitting SDL
			case SDL_QUIT:
			{
				//stops the program loop variable
				gameLoop = false;
				break;
			}
			//checks for a key down
			case SDL_KEYDOWN:
			{
				//sets the relevant movement boolean for a key down
				switch (gameEvent.key.keysym.sym)
				{
					//Up
				case SDLK_w:
				{
					movement[0] = true;
					break;
				}
				//down
				case SDLK_s:
				{
					movement[1] = true;
					break;
				}
				//left
				case SDLK_a:
				{
					movement[2] = true;
					break;
				}
				//right
				case SDLK_d:
				{
					movement[3] = true;
					break;
				}
				}
				break;
			}
			//checks for a key up
			case SDL_KEYUP:
			{
				//Resets the relevant movement boolean for a key up
				switch (gameEvent.key.keysym.sym)
				{
				case SDLK_w:
				{
					movement[0] = false;
					break;
				}
				case SDLK_s:
				{
					movement[1] = false;
					break;
				}
				case SDLK_a:
				{
					movement[2] = false;
					break;
				}
				case SDLK_d:
				{
					movement[3] = false;
					break;
				}
				}
			}
			break;
			}

		}
		//checks for the number of milliseconds since SDL was initialised
		unsigned int currentFrame = SDL_GetTicks();

		//checks for change in time between current frame and pervious frame
		float deltaTime = (float)(currentFrame - lastFrame) / 1000.0f;

		//sets the previous frame to the current frame
		lastFrame = currentFrame;

		//sets the gamescore
		gameScene.SetGameScore(gameScene.GetSceneTimeCounter() * 1000);

		//calls the update function for the scene
		gameScene.Update(deltaTime, movement);

		//calls the render function
		Renderer(gameScene, gameWindow, deltaTime);

		//limits the speed if the program is running too quickly
		if (deltaTime < (1.0f / 50.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTime) * 1000.0f));
		}

		//increments the scene timer
		gameScene.IncrementSceneTimeCounter();

		//checks whether the player's health is less than 0
		if (gameScene.GetPlayerHealth() < 0)
		{
			//ends the program loop
			gameLoop = false;
		}

	}

	//displays the game score
	std::cout << gameScene.GetGameScore() << std::endl;
	std::cin.get();
}

//function used for rendering
void Renderer(Scene gameScene, SDL_Window* gameWindow, float deltaTime)
{
	//colour used to clear to the framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//writes the colour to the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//calls the function to the draw the models
	gameScene.Draw(gameScene.GetModel(0, true));

	//for loop used to loop through the meteorite models, drawing each one
	for (int i = 0; i < 40; i++)
	{
		gameScene.Draw(gameScene.GetModel(i, false));
	}

	//renders to the screen
	SDL_GL_SwapWindow(gameWindow);
}