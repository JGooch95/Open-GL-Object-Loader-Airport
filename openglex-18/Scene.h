#pragma once

#include "Model.h"
#include "TextureLoader.h"

class Scene
{
	private:
		TextureLoader* m_tl;
		std::vector<GLuint> m_textureID;
	public:
		Scene();
		Scene(string Dir);
		void Load(string Dir);
		std::vector<Model> Models;
		GLuint getTexture(int index);
};