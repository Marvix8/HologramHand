#ifndef _Gesture_h
#define _Gesture_h

#include "Hand.h"

// Acciones a realizar en aplicación Android
#define PLAY_PAUSE          0
#define NEXT                1
#define PREVIOUS            2
#define SPEED_X1            3
#define SPEED_X2            4
#define SPEED_X05           5

enum State {
	StarPZ,
	StarNZ,
	StarPY,
	StarPX,
	RockPY,
	RockPZ,
	GoodNZ,
	GoodPZ,
	GoodPX,
	ScissorPY,
	ScissorNZ,
	Default
};
 
enum Input {
	InStarPZ,
	InStarNZ,
	InStarPY,
	InStarPX,
	InRockPY,
	InRockPZ,
	InGoodNZ,
	InGoodPZ,
	InGoodPX,
	InScissorPY,
	InScissorNZ,
	InDefault
};

State currentState = Default;
Input currentInput = InDefault;

class Gesture {
	
	public:
		Hand *hand;
		int action;
		bool hasChanged;
		
		// Constructor de la clase Gesture
		Gesture(Hand& hand): hand(&hand) {
			this->action = -1;
			this->hasChanged = false;
		}
	
		void updateStateMachine() {
			switch (currentState) {
				case StarPZ: stateStarPZ(); break;
				case StarNZ: stateStarNZ(); break;
				case StarPY: stateStarPY(); break;
				case StarPX: stateStarPX(); break;
				case RockPY: stateRockPY(); break;
				case RockPZ: stateRockPZ(); break;
				case GoodNZ: stateGoodNZ(); break;
				case GoodPZ: stateGoodPZ(); break;
				case GoodPX: stateGoodPX(); break;
				case ScissorPY: stateScissorPY(); break;
				case ScissorNZ: stateScissorNZ(); break;
				case Default: stateDefault(); break;
				}
			}

		void readInput() {
			currentInput = Input::InDefault;
			
			switch (hand->getMovement()) {
				case (int)SPZ: currentInput = Input::InStarPZ; break;
				case (int)SNZ: currentInput = Input::InStarNZ; break;
				case (int)SPY: currentInput = Input::InStarPY; break;
				case (int)SPX: currentInput = Input::InStarPX; break;
				case (int)RPY: currentInput = Input::InRockPY; break;
				case (int)RPZ: currentInput = Input::InRockPZ; break;
				case (int)GNZ: currentInput = Input::InGoodNZ; break;
				case (int)GPZ: currentInput = Input::InGoodPZ; break;
				case (int)GPX: currentInput = Input::InGoodPX; break;
				case (int)SCPY: currentInput = Input::InScissorPY; break;
				case (int)SCNZ: currentInput = Input::InScissorNZ; break;
				case (int)UNDEFINED: currentInput = Input::InDefault; break;
				default: break;
			}
		}
		
		int getAction() {
			return this->action;
		}
		
		int getGesture() {
			return this->action;
		}
		
		bool getHasChanged() {
			return this->hasChanged;
		}
	
		void setHasChanged(bool hasChanged) {
			this->hasChanged = hasChanged;
		}
		
	private:
		void changeState(int newState) {
			currentState = newState;

			switch (currentState) {
				case State::RockPY: outputRockPY();   break;
				case State::GoodNZ: outputGoodNZ();   break;
				case State::GoodPZ: outputGoodPZ();   break;
				case State::GoodPX: outputGoodPX();   break;
				case State::ScissorPY: outputScissorPY();   break;
				case State::ScissorNZ: outputScissorNZ();   break;				
				default: break;
			}
		}
		
		/*
		*	STATES
		*/
		void stateDefault() {
			if (currentInput == Input::InStarPZ)
				changeState(State::StarPZ);
			if (currentInput == Input::InStarNZ)
				changeState(State::StarNZ);	
			if (currentInput == Input::InStarPY)
				changeState(State::StarPY);	
			if (currentInput == Input::InStarPX)
				changeState(State::StarPX);	
			if (currentInput == Input::InRockPY)
				changeState(State::RockPY);	
			if (currentInput == Input::InRockPZ)
				changeState(State::RockPZ);	
			if (currentInput == Input::InGoodNZ)
				changeState(State::GoodNZ);	
			if (currentInput == Input::InGoodPZ)
				changeState(State::GoodPZ);	
			if (currentInput == Input::InGoodPX)
				changeState(State::GoodPX);	
			if (currentInput == Input::InScissorPY)
				changeState(State::ScissorPY);	
			if (currentInput == Input::InScissorNZ)
				changeState(State::ScissorNZ);	
		}

		void stateStarPZ() {
			if (currentInput == Input::InRockPY)
				changeState(State::RockPY);
			if (currentInput == Input::InGoodNZ)
				changeState(State::GoodNZ);
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateStarNZ() {
			if (currentInput == Input::InGoodPZ)
				changeState(State::GoodPZ);
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateStarPY() {
			if (currentInput == Input::InGoodPX)
				changeState(State::GoodPX);
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateStarPX() {
			if (currentInput == Input::InScissorPY)
				changeState(State::ScissorPY);
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateRockPY() {
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateRockPZ() {
			if (currentInput == Input::InScissorNZ)
				changeState(State::ScissorNZ);
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateGoodNZ() {
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateGoodPZ() {
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateGoodPX() {
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateScissorPY() {
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}
		
		void stateScissorNZ() {
			if (currentInput == Input::InDefault)
				changeState(State::Default);
		}

		/*
		*	OUTPUTS
		*/
		void outputRockPY() {
			this->hasChanged = true;
			this->action = (int)PLAY_PAUSE;
		}

		void outputGoodNZ() {
			this->hasChanged = true;
			this->action = (int)NEXT;
		}
		
		void outputGoodPZ() {
			this->hasChanged = true;
			this->action = (int)PREVIOUS;
		}
		
		void outputGoodPX() {
			this->hasChanged = true;
			this->action = (int)SPEED_X1;
		}
		
		void outputScissorPY() {
			this->hasChanged = true;
			this->action = (int)SPEED_X2;
		}
		
		void outputScissorNZ() {
			this->hasChanged = true;
			this->action = (int)SPEED_X05;
		}

};

#endif