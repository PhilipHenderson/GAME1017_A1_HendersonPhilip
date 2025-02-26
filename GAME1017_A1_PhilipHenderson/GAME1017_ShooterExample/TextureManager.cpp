#include "TextureManager.h"
#include "Engine.h"
#include <iostream>

void TextureManager::Init()
{
	//initialize  SDL's image subsystem
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
		std::cout << "Image Init Seccuss!" << std::endl;
	else // is 0
		std::cout << "Image Init Failed! ERROR - " << IMG_GetError() << std::endl;;
}

void TextureManager::Load(const char* path, const std::string key)
{
	SDL_Texture* temp = IMG_LoadTexture(Engine::Instance().GetRenderer(), path);
	if (temp == nullptr)
		std::cout << "Could not load texture: ERROR - " << IMG_GetError() << std::endl;
	else
		s_textures.emplace(key, temp);
}

void TextureManager::Unload(const std::string key)
{
	if (s_textures.find(key) != s_textures.end())
	{
		SDL_DestroyTexture(s_textures[key]); //deallocate
		s_textures.erase(key); //remove from map
	}
	else
		std::cout << "Could not Unload '" << key << "' - key not found!"<< std::endl;
}

SDL_Texture* TextureManager::GetTexture(const std::string key)
{
	return s_textures[key];
}

void TextureManager::Quit()
{
	for (auto const i : s_textures)
		SDL_DestroyTexture(s_textures[i.first]);
	s_textures.clear(); // wrangle dangles
}

TextureManager::TextureManager(){}

std::map<std::string, SDL_Texture* > TextureManager::s_textures;