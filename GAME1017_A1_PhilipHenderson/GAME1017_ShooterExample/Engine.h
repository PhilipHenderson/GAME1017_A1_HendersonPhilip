#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "Sprites.h"

class Engine
{
// I am avoiding in-class initialization.
private: // Private properties.
	const Uint8* m_iKeystates; // Keyboard state container.
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.

private: // Private methods.
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();

	void Update();
	void Render();
	void Clean();

public: // Public methods.
	Engine();
	~Engine();
	int Run();
	static Engine& Instance();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	bool KeyDown(SDL_Scancode c);
	void CheckCollision();
	// Public properties.
	bool m_bRunning, // Loop control flag.
		m_bENull, // These three flags check if we need to clear the respective vectors of nullptrs.
		m_bPBNull,
		m_bEBNull,
		m_bCanShoot;

	SDL_Texture* m_pBGText; // For the bg.
	Sprite bgArray[2];

	SDL_Texture* m_pSprText; // For the sprites.
	Player* m_player;

	int m_iESpawn, // The enemy spawn frame timer properties.
		m_iESpawnMax;

	vector<Enemy*> m_vEnemies;
	vector<Bullet*> m_vPBullets;
	vector<Bullet*> m_vEBullets;
	SDL_Point m_pivot;

	Mix_Music* m_pMusic;
	vector<Mix_Chunk*> m_vSounds;


};

#endif _ENGINE_H_ /* defined (_ENGINE_H_) */