#pragma once
#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "States.h"
#include "Engine.h"
#include <vector>


class StateManager
{
public: // Public Properties
	static std::vector<State*> s_states;

public: // public Menthols
	static void Update();
	static void Render();
	static void PushState(State* pState); //Used when goin to pause state
	static void PopState(); //used when leaving pause state
	static void ChangeState(State* pState);
	static void Quit(); // used for cleanup
	static std::vector<State*>& GetStates(); // returns a reference to out state vector

private: // priavet methods
	StateManager() {}

};

typedef StateManager STMA; //creating a shorter alias
#endif _STATEMANAGER_H_