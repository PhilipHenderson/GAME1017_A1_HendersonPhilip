#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>
#include "SDL.h"

class Button
{
public:
	Button(SDL_Rect s, SDL_Rect d, const char* key);
	void Update();
	void Render();

protected:
	std::string m_key;
	enum BtnState { STATE_UP, STATE_DOWN } m_states;
	virtual void Execute() = 0;

};

class PlayButton : public Button
{
private:
	void Execute();
public:
	PlayButton(SDL_Rect s, SDL_FRect d, const char* k);

};

#endif _BUTTON_H_