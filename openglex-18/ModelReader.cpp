#include "stdafx.h"
#include "ModelReader.h"


ModelReader::ModelReader(string filename)
{
	ReadModelObjData(filename);

	//expand the data suitable for lDrawArrays()
	CreateExpandedVertices();
	CreateExpandedNormals();
	CreateExpandedTextureCoordinates();
}

ModelReader::~ModelReader(void)
{
}

void ModelReader::ReadModelObjData(string filename)
{
	std::fstream modelfile(filename, std::ios_base::in);

	if(!modelfile.is_open())
	{
		std::cerr << "File " << filename << " not found.";
		DebugBreak();
		throw std::invalid_argument( "File not found" );
		return;	//ERROR!!!
	}

	string line;
	string token = "";
	while (getline(modelfile, line))
	{
		istringstream iss(line);

		// process the line
		token = "";

		iss >> token; // read to first whitespace
	
		if (token == "g")
			iss >> m_modelName; // read the model name

		else if (token == "v")
			ProcessVertexLine(iss);

		else if (token == "vn")
			ProcessVertexNormalLine(iss);

		else if (token == "vt")
			ProcessVertexTextureLine(iss);

		else if (token == "f")
			ProcessFaceLine(iss);
	}
	modelfile.close();

}

void ModelReader::ProcessVertexLine(istringstream& iss)
{
	float value;

	while(iss >> value)
		m_vertices.push_back(value);

}

void ModelReader::ProcessVertexNormalLine(istringstream& iss)
{
	float value;

	while(iss >> value)
		m_vertexNormals.push_back(value);
}

void ModelReader::ProcessVertexTextureLine(istringstream& iss)
{
	float value;

	while(iss >> value)
		m_vertexTextureCoordinates.push_back(value);
}


void ModelReader::ProcessFaceLine(istringstream& iss)
{
	unsigned int value; //Holds the value to be stored
	static const int forwardSlash = 0x2F; // "/" character

	//For every item
	for(int i = 0; i <3;i++)
	{
		iss >> value; //Reads the face index
		int lookAhead = iss.peek();  	// peek character
		m_faceVertexIndices.push_back(value-1); //Adds the face index to the list

		if (lookAhead == forwardSlash)    //If the next character is a "/"
		{
			iss.ignore(1); //Ignore it
			lookAhead = iss.peek(); // peek character

			if (lookAhead == forwardSlash) //If the next character is a "/"
			{
				iss.ignore(1); //Ignore it

				iss >> value; //Reads the face normal index
				m_faceNormalIndices.push_back(value-1); //Adds the face normal index to the list
			}

			else //If the next character is not a "/"
			{
				iss >> value; //Reads the face texture index
				m_faceTextureIndices.push_back(value-1); //Adds the face texture index to the list

				iss.ignore(1); //Ignore the slash

				iss >> value; //Reads the face normal index
				m_faceNormalIndices.push_back(value-1); //Adds the face normal index to the list
			}
		}
	}
}


void ModelReader::CreateExpandedVertices()
{
	for (std::vector<unsigned int>::iterator it = m_faceVertexIndices.begin() ; it != m_faceVertexIndices.end(); ++it)
	{
		int vertexNumber = (*it);

		// 3 floats per triangular face
		for(int i = 0; i < 3; i++)
			m_vertexTriplets.push_back(m_vertices[(vertexNumber * 3) + i]);
	}
}
void ModelReader::CreateExpandedNormals()
{
	// we create a list of normal triplets for each face (with duplicates)
	// this will then be the same size as the vertexTriplets vector

	for (std::vector<unsigned int>::iterator it = m_faceNormalIndices.begin() ; it != m_faceNormalIndices.end(); ++it)
	{
		//here we have a 1 base index
		// get the face number as 0 indexed
		int vertexNormalNumber = (*it);

		// 3 floats per triangular face
		for(int i = 0; i < 3; i++)
			m_vertexNormalTriplets.push_back(m_vertexNormals[(vertexNormalNumber * 3) + i]);
	}
}
void ModelReader::CreateExpandedTextureCoordinates()
{
	// create actual vertices here (with duplicates)
	// this is in the form that glDrawArrays can work with
	//
	// assume triangles so far
	// assert((faceVertexIndices.size() % 3) == 0);

	for (std::vector<unsigned int>::iterator it = m_faceTextureIndices.begin() ; it != m_faceTextureIndices.end(); ++it)
	{
		int vertexNormalNumber = (*it);

		for(int i = 0; i < 2; i++)
			m_vertexTexturePairs.push_back(m_vertexTextureCoordinates[(vertexNormalNumber * 2) + i]);
	}
}


// Get methods gove access to the vector data

vector<float>& ModelReader::GetVertices()
{
	return m_vertexTriplets;
}
vector<float>& ModelReader::GetNormals()
{
	return m_vertexNormalTriplets;
}
vector<float>& ModelReader::GetTextureCoordinates()
{
	return m_vertexTexturePairs;
}