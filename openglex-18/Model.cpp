#include "stdafx.h"
#include "Model.h"

Model::Model()
{
	m_Material = 0;
	m_Translation = sf::Vector3f(0.0f, 0.0f, 0.0f);
	m_Rotation = sf::Vector3f(0.0f, 0.0f, 0.0f);
	m_Scale = sf::Vector3f(1,1,1);
	m_DrawNormals = false;
	m_DrawTextures = false;
}

void Model::loadModel(std::string Model)
{
	m_modelReader = new ModelReader(Model); //Loads the model to the model reader.
}

void Model::setTexture(GLuint newTextureID)
{
	m_textureID = newTextureID; //Sets the texture of the model to the ID given
}

void Model::setRotation(sf::Vector3f NewRotation)
{
	m_Rotation = NewRotation; //Sets the rotation vector given
}
void Model::setScale(sf::Vector3f NewScale)
{
	m_Scale = NewScale; //Sets the scale vector given
}
void Model::setTranslation(sf::Vector3f NewTranslation)
{
	m_Translation = NewTranslation; //Sets the translation vector given
}

void Model::draw(bool drawWithNormals, bool drawWithTexture)
{

	glTranslatef(m_Translation.x, m_Translation.y, m_Translation.z); //Translate by the given amount
	if(m_Rotation.x != 0.0f) //If the x is to be rotated
		glRotatef(m_Rotation.x, 1.0f, 0.0f, 0.0f); //Rotate it by the given amount

	if(m_Rotation.y != 0.0f) //If the y is to be rotated
		glRotatef(m_Rotation.y, 0.0f, 1.0f, 0.0f); //Rotate it by the given amount

	if(m_Rotation.z != 0.0f) //If the z is to be rotated
		glRotatef(m_Rotation.z, 0.0f, 0.0f, 1.0f); //Rotate it by the given amount
	glScalef(m_Scale.x, m_Scale.y, m_Scale.z); //Scale the model by the given amount

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	vector<float>& vertices = m_modelReader->GetVertices();
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

	// pointer to normal array
	if (drawWithNormals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);

		vector<float>& normals = m_modelReader->GetNormals();

		glNormalPointer(GL_FLOAT, 0, &normals[0]);
	}
	
	if (drawWithTexture)
	{
		glEnable(GL_TEXTURE_2D);	// Enables textures
		vector<float>& textureCoordinates = m_modelReader->GetTextureCoordinates(); //Gets the texture UV values from the reader
		glBindTexture(GL_TEXTURE_2D, m_textureID); //Binds the ID with the textrue

		glEnableClientState(GL_TEXTURE_COORD_ARRAY); //glTexCoordPointer will point to an array
		glTexCoordPointer(2, GL_FLOAT, 0, &textureCoordinates[0]);
	}

	// draw the shape...
	glDrawArrays(GL_TRIANGLES, 0, (unsigned int)vertices.size() / 3);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	if (drawWithNormals)
		glDisableClientState(GL_NORMAL_ARRAY); // deactivate
	
	if (drawWithTexture)
	{
		// tidy up
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
}

int Model::getMaterial()
{
	return m_Material;
}

void Model::setMaterial(int newMaterial)
{
	m_Material = newMaterial;
}

void Model::setDrawTextures(bool value)
{
	m_DrawTextures = value;
}

void Model::setDrawNormals(bool value)
{
	m_DrawNormals = value;
}

bool Model::getDrawTextures()
{
	return m_DrawTextures;
}

bool Model::getDrawNormals()
{
	return m_DrawNormals;
}