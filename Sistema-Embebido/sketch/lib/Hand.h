#ifndef _Hand_h
#define _Hand_h

#include "Flex.h"
#include "Acelerometer.h"

// Posición de la mano
#define STAR		0
#define	SCISSORS	1
#define GOOD		2
#define ROCK		3

// Posición de la mano en el espacio
#define X_PLUS	0
#define Y_PLUS	1
#define Z_PLUS	2
#define Z_LESS	3

// Posición final
#define	SPZ	 	 0
#define	SNZ	 	 1
#define	SPY	 	 2
#define	SPX	 	 3
#define	RPY	 	 4
#define	RPZ	 	 5
#define	GNZ	 	 6
#define	GPZ	 	 7
#define	GPX	 	 8
#define	SCPY	 9
#define	SCNZ	10

// Undefined genérico para posición, espacio y final
#define UNDEFINED	-1

class Hand {
	private:
		Flex *bigFinger; 			// Flex del pulgar.
		Flex *indexFinger;			// Flex del dedo índice.
		Flex *middleFinger;			// Flex del dedo medio
		Acelerometer *acelerometer; 	// Acelerómetro
		bool calibrated;			// Booleano que indica si la mano está calibrada o no.
		int handPosition;
		int spacePosition;
		int movement;
		
		
		int aux;
		

	public:
		// Constructor genérico Hand.
		Hand() {};
		/*
		* Constructor de Hand.
		* Entrada:
		*	@bigFinger: Flex del dedo pulgar de la mano.
		*	@indexFinger: Flex del dedo índice de la mano.
		*	@middleFinger: Flex del dedo del medio de la mano.
		*	@acelerometer: Objeto acelerómetro con los valores de sus axis.
		*/
		Hand(Flex& bigFinger, Flex& indexFinger, Flex& middleFinger, Acelerometer& acelerometer): 
		bigFinger(&bigFinger), indexFinger(&indexFinger), middleFinger(&middleFinger), acelerometer(&acelerometer) {
			this->calibrated = false;
			this->movement = -1;
			this->aux = -1;
		}

		void processPosition(double bigFingerSensor, double indexFingerSensor, double middleFingerSensor) {
			updateFingers(bigFingerSensor, indexFingerSensor, middleFingerSensor);
			if (bigFinger->getFlexPosition() == (int) STRAIGHT_FLEX) {
				if (indexFinger->getFlexPosition() == (int) STRAIGHT_FLEX
					&& middleFinger->getFlexPosition()  == (int) STRAIGHT_FLEX){
					this->handPosition = (int)STAR;
					return;
				}
				else if (indexFinger->getFlexPosition() == (int) BEND_FLEX
					&& middleFinger->getFlexPosition()  == (int) BEND_FLEX) {
					this->handPosition = (int)GOOD;
					return;
				}
			} else if (bigFinger->getFlexPosition() == (int) BEND_FLEX) {
				if (indexFinger->getFlexPosition() == (int) STRAIGHT_FLEX
					&& middleFinger->getFlexPosition()  == (int) STRAIGHT_FLEX) {
					this->handPosition = (int)SCISSORS;
					return;
				}
					
				else if (indexFinger->getFlexPosition() == (int) BEND_FLEX
					&& middleFinger->getFlexPosition()  == (int) BEND_FLEX) {
					this->handPosition = (int)ROCK;
					return;
				}
			}
			
			this->handPosition = (int)UNDEFINED;
		}

		void process() {
			processSpacePosition();
			switch(spacePosition) {
				case (int)X_PLUS:
					if (handPosition == (int)GOOD ) {
						this->movement = (int)GPX;
					} else if (handPosition == (int)STAR ) {
						this->movement = (int)SPX;
					} else {
						this->movement = (int)UNDEFINED;
					}
					break;

				case (int)Y_PLUS:
					switch(handPosition) {
						case (int)STAR: 
							this->movement = (int)SPY;
							break;
						case (int)SCISSORS: 
							this->movement = (int)SCPY;
							break;
						case (int)ROCK: 
							this->movement = (int)RPY;
							break;
						case (int)UNDEFINED:
							this->movement = (int)UNDEFINED;
							break;
						default: 
							this->movement = (int)UNDEFINED;
							break;
					}
					break;

				case (int)Z_PLUS:
					switch(handPosition) {
						case (int)STAR: 
							this->movement = (int)SPZ;
							break;
						case (int)ROCK: 
							this->movement = (int)RPZ;
							break;
						case (int)GOOD: 
							this->movement = (int)GPZ;
							break;
						case (int)UNDEFINED:
							this->movement = (int)UNDEFINED;
							break;
						default: 
							this->movement = (int)UNDEFINED;
							break;
					}
					break;

				case (int)Z_LESS:
					switch(handPosition) {
						case (int)STAR: 
							this->movement = (int)SNZ;
							break;
						case (int)GOOD: 
							this->movement = (int)GNZ;
							break;
						case (int)SCISSORS: 
							this->movement = (int)SCNZ;
							break;
						case (int)UNDEFINED:
							this->movement = (int)UNDEFINED;
							break;
						default: 
							this->movement = (int)UNDEFINED;
							break;
					}
					break;
				
				case (int)UNDEFINED:
					this->movement = (int)UNDEFINED;
					break;
				default: break;
			}       
		}      

		int getMovement() {
			return this->movement;
		}
		
		int getHandPosition() {
			return this->handPosition;
		}
		
		bool getCalibrated() {
			return this->calibrated;
		}
		
		void setCalibrated(bool flag) {
			this->calibrated = flag;
		}
		
		double getAux(){
			return this->aux;
		}
		
		void calibrateStraightHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger->setStraightResistance(bigFingerSensorValue);
			bigFinger->setDifference(bigFinger->getStraightResistance() - bigFinger->getBendResistance());
			indexFinger->setStraightResistance(indexFingerSensorValue);
			indexFinger->setDifference(indexFinger->getStraightResistance() - indexFinger->getBendResistance());
			middleFinger->setStraightResistance(middleFingerSensorValue);
			middleFinger->setDifference(middleFinger->getStraightResistance() - middleFinger->getBendResistance());
		}
		
		void calibrateBendHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger->setBendResistance(bigFingerSensorValue);
			indexFinger->setBendResistance(indexFingerSensorValue);
			middleFinger->setBendResistance(middleFingerSensorValue);
		}
		
	private:
		void updateFingers(double bigFingerSensor, double indexFingerSensor, double middleFingerSensor) {
			bigFinger->processInformation(bigFingerSensor);
			indexFinger->processInformation(indexFingerSensor);
			middleFinger->processInformation(middleFingerSensor);
		}
	
		void processSpacePosition() {
			if (this->acelerometer->getAxisX() >= 0.8 &&
				this->acelerometer->getAxisX() <= 1.3) {
				spacePosition = (int)X_PLUS;
				return;				
			} else if (this->acelerometer->getAxisY() >= 0.8 &&
				this->acelerometer->getAxisY() <= 1.3) {
				spacePosition = (int)Y_PLUS;
				return;				
			} else if (this->acelerometer->getAxisZ() >= 0.8 &&
				this->acelerometer->getAxisZ() <= 1.3) {
				spacePosition = (int)Z_PLUS;
				return;				
			} else if (this->acelerometer->getAxisZ() <= -0.8 &&
				this->acelerometer->getAxisZ() >= -1.3) {
				spacePosition = (int)Z_LESS;
				return;				
			}
			
			this->spacePosition = (int)UNDEFINED;
		}
	
};

#endif