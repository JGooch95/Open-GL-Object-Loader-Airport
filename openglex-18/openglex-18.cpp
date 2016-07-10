// openglex-18.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "TextureLoader.h"
#include "ModelReader.h"
#include "Camera.h"
#include "Model.h"
#include "Scene.h"

#if 0

#define GL_GLEXT_PROTOTYPES 1		// ALLOW USE OF OPEN GL 1.5+ functionality!!!
#define GLEW_STATIC
// this needs to be the first include
#include <SFML\glew.h>		// make 1.5 functions into function pointers - bind at runtime.
#include <SFML\OpenGL.hpp>	// glew BEFORE opengl headers!!!!
#include <SFML\Graphics.hpp>
// put these inside the SFML\include folder
#include <SFML\wglext.h>
#include <SFML\glext.h>
// note weve also added the lib file glew32s.lib

// shader.h included these
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#endif

using namespace std;

// better to use const int rather than define
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;
const float REFRESH_RATE = 0.03f;


// we make the light position a global variable
// (later would belong to a class handling lighting)

// put light behind and above us on right
GLfloat m_lightPosition[] = {50.0f, +50.0f, +100.0f, 1.0f};

void SetMaterialWhite() //Sets the material to white
{
	GLfloat materialWhiteAmbient[] = {0.4f, 0.4f, 0.4f ,1.0f};
	GLfloat materialWhiteDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 0.4f;
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialWhiteShiny() //Sets the material to be white with a shine
{
	GLfloat materialWhiteAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialWhiteShininess = 700.0f;	//For more shininess	

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}


void SetMaterialFullAmbient() //Removes any sort of faded lighting effect but brightens the model
{
	GLfloat materialWhiteAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat materialWhiteSpecular[] = {1.0f ,1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 0.0f;

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}


void SetLightPosition(float x, float y, float z)
{
	m_lightPosition[0] = x;
	m_lightPosition[1] = y;
	m_lightPosition[2] = z;
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
}

void ConfigureLightSources()
{
	GLfloat lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat noLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat lightModelAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};

	// put light behind and above us on left
	SetLightPosition(+800.0f, +450.0f, +600.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
	glLightfv(GL_LIGHT0, GL_AMBIENT, noLight);			// no ambient light from the source

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);	// use global ambient instead

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// with lighting glColor no longer applies
	// create a default material for the models
	SetMaterialWhite();
}

int main()
{
    // Create the main window
	//sf::Window App(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "SFML OpenGL");

	// SFML-2.3.2 depth buffering fails unless we create a more specific window - as below
	int depthBits = 24;
	int stencilBits = 8;
	int antiAliasingLevel = 2;
	int majorVersion = 3;
	int minorVersion = 3;

	sf::ContextSettings context(depthBits, stencilBits, antiAliasingLevel, majorVersion, minorVersion)	;
	sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "SFML Window", 7U, context);

	// check we got the settings we need
	sf::ContextSettings windowSettings = window.getSettings();
	window.setMouseCursorVisible(false);

	HWND handle = window.getSystemHandle();

    // Create a clock for measuring time elapsed
    sf::Clock clock;
	
	// always setup OpenGL After the window is configured
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	// draw single sided faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.f, 5500.f);

	Camera Camera1;
	sf::Vector2i WindowOrigin(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	sf::Mouse::setPosition(WindowOrigin);
	sf::Vector2i MousePosition(sf::Mouse::getPosition(window));
	float xAngle = 0.0f;
	float yAngle = 0.0f;

	const float TileNumX = 50;
	const float TileNumY = 50;
	const float TileWidth = 20;
	const float TileHeight = 20;

	Scene Scene1("Scene1.txt");
	ConfigureLightSources();

    // Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event : adjust viewport
            if (Event.type == sf::Event::Resized)
                  glViewport(0, 0, Event.size.width, Event.size.height);
        }
        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        window.setActive();

		if(clock.getElapsedTime().asSeconds() > REFRESH_RATE)
		{
			Camera1.HandleInput();
			// Clear colour and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Apply some transformations
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
				
			if (GetFocus() == handle)
			{
				WindowOrigin = sf::Vector2i(SCREEN_WIDTH/2, SCREEN_HEIGHT/2); //Holds the centre of the window.
				MousePosition = sf::Mouse::getPosition(window); //Gets the Mouse position to see the distance from the centre of the window.
				sf::Mouse::setPosition(WindowOrigin, window); //Sets the position of the mouse to the centre of the window.
				xAngle = (WindowOrigin-MousePosition).x /1000.0f; //Gets the distance traveled horizontally axis.
				yAngle = (WindowOrigin-MousePosition).y /1000.0f; //Gets the distance traveled vertically axis.
				Camera1.Turn(xAngle, yAngle); //Turns the camera by the amount calculated.
				Camera1.Point(); // Points the camera to the angle the camera is turned to.
			}

			glPushMatrix();
				SetMaterialWhite();
				//Draw the floor
				glEnable(GL_TEXTURE_2D); // Enables textures
				glBindTexture(GL_TEXTURE_2D, Scene1.getTexture(0)); //Sets the texture to be used for the tiles

				//For the width and height of the floor
				for (int y = -TileNumY*TileHeight; y < TileNumY*TileHeight; y+=TileHeight) //Height
				{
					for (int x = -TileNumX*TileWidth; x < TileNumX*TileWidth; x+=TileWidth) //Width
					{
						glBegin(GL_QUADS);
							//Top left corner
							glTexCoord2f(0.0f, 0.0f);  //Sets the position of the coordinate for the texture according to the tile. 
							glVertex3f(float(x+TileWidth), -2.0f, float(y)); //Sets the Position of the coordinate
							glNormal3f(0,1,0); //Sets the normal for the lighting to work on the quad

							//Top right corner
							glTexCoord2f(1.0f, 0.0f);  //Sets the position of the coordinate for the texture according to the tile.
							glVertex3f(float(x), -2.0f, float(y)); //Sets the Position of the coordinate
							glNormal3f(0,1,0); //Sets the normal for the lighting to work on the quad

							//Bottom right corner
							glTexCoord2f(1.0f, 1.0f); //Sets the position of the coordinate for the texture according to the tile.
							glVertex3f(float(x), -2.0f,  float(y + TileHeight)); //Sets the Position of the coordinate
							glNormal3f(0,1,0); //Sets the normal for the lighting to work on the quad

							//Bottom left corner
							glTexCoord2f(0.0f, 1.0f); //Sets the position of the coordinate for the texture according to the tile.
							glVertex3f(float(x+TileWidth), -2.0f, float(y + TileHeight)); //Sets the Position of the coordinate
							glNormal3f(0,1,0); //Sets the normal for the lighting to work on the quad
						glEnd(); 
					}
				}
				glDisable(GL_TEXTURE_2D); // Disables textures
				glBindTexture(GL_TEXTURE_2D, NULL); //Sets the texture to null
			glPopMatrix();

			for(int i = 0; i < Scene1.Models.size(); i++)
			{
				glPushMatrix(); //Starts a new matrix for transformations
					if(Scene1.Models[i].getMaterial() == 1)
						SetMaterialWhite(); //Sets the material to white

					else if(Scene1.Models[i].getMaterial() == 2)
					{
						glEnable(GL_NORMALIZE); //Sets up normals for the shiny objects
						SetMaterialWhiteShiny(); //Sets the material to be white with a shine
					}
					
					else if(Scene1.Models[i].getMaterial() == 3) //Removes any sort of faded lighting effect but brightens the model
						SetMaterialFullAmbient();

					Scene1.Models[i].draw(Scene1.Models[i].getDrawNormals(), Scene1.Models[i].getDrawTextures()); // call the rendering code

					if(glIsEnabled(GL_NORMALIZE))
						glDisable(GL_NORMALIZE); //Stops non shiny objects from being normalised.

				glPopMatrix(); // restore untranslated matrix from matrix stack
			}

			SetLightPosition(+800.0f , +450.0f , +600.0f); //Sets the light position

			clock.restart();
		    // Finally, display rendered frame on screen
			window.display();
		}
    }
    return EXIT_SUCCESS;
}

