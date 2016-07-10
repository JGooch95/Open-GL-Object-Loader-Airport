#pragma once
#include "ModelReader.h"

class Model
{
	private:
		sf::Vector3f m_Translation;
		sf::Vector3f m_Rotation;
		sf::Vector3f m_Scale;

		GLuint m_textureID; // array of OpenGL texture objects 

		ModelReader* m_modelReader;
		int m_Material;
		bool m_DrawTextures;
		bool m_DrawNormals;

	public:
		Model();
		void loadModel(std::string Model);
		void setTexture(GLuint newTextureID);
		void setRotation(sf::Vector3f NewRotation);
		void setScale(sf::Vector3f NewScale);
		void setTranslation(sf::Vector3f NewTranslation);
		int getMaterial();
		void setMaterial(int newMaterial);
		void draw(bool drawWithNormals, bool drawWithTexture);
		void setDrawNormals(bool value);
		void setDrawTextures(bool value);
		bool getDrawNormals();
		bool getDrawTextures();
};