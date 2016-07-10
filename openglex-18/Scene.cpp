#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
	m_tl = new TextureLoader();
}

Scene::Scene(string Dir)
{
	Load(Dir);
	m_tl = new TextureLoader();
}

void Scene::Load(string Dir)
{
	std::fstream Scenefile(Dir, std::ios_base::in); //Attempt to open the file given

	if(!Scenefile.is_open()) //If the file didnt open
	{
		std::cerr << "File " << Dir << " not found."; //Output an error that the file didnt open
		DebugBreak(); //Input a breakpoint.
		throw std::invalid_argument( "File not found" ); //Throw a file not found error
		return;	//ERROR!!!
	}

	string line; //Holds the line being read
	string token = ""; //Holds the part of the string that was last read
	sf::Vector3f TempVect; //Holds the vector to rotate, translate or scale by
	Model TempModel; //Holds the model being altered before bing added to the vector.
	std::vector<string> TextureDirs;
	bool LoadingModel = false; //States whether a model is being loaded
	bool TextureFound = false; //States if the texture has been loaded before or not
	int pos = 0;

	//While there are lines to be read
	while (getline(Scenefile, line))
	{
		istringstream iss(line); //Holds the data that is read

		token = "";

		//While the token is not the "push" token
		while(token != "p") //(p = Push onto vector)
		{
			iss >> token; // read to first whitespace

			if (token == "o") //If its an OBJ directory
			{
				iss >> token; //Read the item
				TempModel.loadModel(token); //Load the model
				LoadingModel = true;
			}
			if (token == "t") //If its a translation transform
			{
				iss >>TempVect.x; //Read the x value
				iss >>TempVect.y; //Read the y value
				iss >>TempVect.z; //Read the z value
				TempModel.setTranslation(TempVect); //Sets the value using the coordinates stored
			}
			else if (token == "s") //If its a scale transform
			{
				iss >>TempVect.x; //Read the x value
				iss >>TempVect.y; //Read the y value
				iss >>TempVect.z; //Read the z value
				TempModel.setScale(TempVect); //Sets the value using the coordinates stored
			}
			else if (token == "r") //If its a rotation transform
			{
				iss >>TempVect.x; //Read the x value
				iss >>TempVect.y; //Read the y value
				iss >>TempVect.z; //Read the z value
				TempModel.setRotation(TempVect); //Sets the value using the coordinates stored
			}
			else if (token == "m") //If its a material
			{
				iss >> token; //Read the item
				TempModel.setMaterial(stoi(token)); //Sets the material
			}
			else if (token == "tex") //If its a texture
			{
				iss >> token; //Read the item
				//For every texture directory
				for(int i = 0; i< TextureDirs.size(); i++)
				{
					//If the directory has already been loaded
					if(token == TextureDirs[i])
					{
						TextureFound = true; //Set the texture boolean to found
						pos = i; //Get the index of where it was found
					}
				}

				//If the texture was found
				if(TextureFound)
				{
					TempModel.setTexture(m_textureID[pos]); //Apply it to the model being loaded
					TextureFound = false; //Set the texture boolean to false
				}
				//If the texture wasnt found
				else
				{
					TextureDirs.push_back(token);
					m_textureID.resize(m_textureID.size() + 1); //Increase the vector holding the textures size
					m_tl->LoadBMP(token, m_textureID[m_textureID.size() - 1], false); //Load the texture into the vector
					TempModel.setTexture(m_textureID[m_textureID.size()-1]); //Set the textrue to the current model
				}
			}
			else if (token == "Dtex") //If its a draw texture boolean
			{
				iss >> token; //Read the item
				if(token == "t")
					TempModel.setDrawTextures(true);
				else
					TempModel.setDrawTextures(false);
			}
			else if (token == "Dn") //If its a draw normal boolean
			{
				iss >> token; //Read the item
				if(token == "t")
					TempModel.setDrawNormals(true);
				else
					TempModel.setDrawNormals(false);
			}
		}

		if(LoadingModel)
		{
			Models.push_back(TempModel); //Pushes the model onto the vector
			LoadingModel = false;
		}
	}
	Scenefile.close(); //Closes the file
}

GLuint Scene::getTexture(int index)
{
	return m_textureID[index];
}