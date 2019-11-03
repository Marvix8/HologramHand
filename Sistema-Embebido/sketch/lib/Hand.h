#ifndef _Hand_h
#define _Hand_h

#include "Flex.h"

// Posición de la mano
#define STAR		        0
#define	SCISSORS        	1
#define GOOD		        2
#define ROCK		        3

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
		//Acelerometer *acelerometer; 	// Acelerómetro
		bool calibrated;			// Booleano que indica si la mano está calibrada o no.
		int handPosition;           // Posición de la mano según sensores flex.
		int spacePosition;          // Posición de la mano en el espacio según sensor acelerómetro.
		int movement;

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
		Hand(Flex& bigFinger, Flex& indexFinger, Flex& middleFinger): bigFinger(&bigFinger), indexFinger(&indexFinger), middleFinger(&middleFinger) {
			//, Acelometer aceloremeter) {
			//this->acelerometer = acelerometer;
			this->calibrated = false;
			this->movement = -1;
		}
		
		/*
		* Método utilizado para procesar la información de la mano.
		*/
		void process(double bigFingerSensor, double indexFingerSensor, double middleFingerSensor) {
			updateFingers(bigFingerSensor, indexFingerSensor, middleFingerSensor);
			processPosition();
			//processSpacePosition();
			spacePosition = (int)X_PLUS;
			switch(spacePosition) {
				case (int)X_PLUS:
					if (handPosition == (int)GOOD ) {
						this->movement = (int)GPZ;
					} else if (handPosition == (int)STAR ) {
						this->movement = (int)SPZ;
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
						default: break;
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
						default: break;
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
						default: break;
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
		
		void setCalibrated() {
			this->calibrated = true;
		}

	    /*
	    * Método utilizado para calibrar la mano estirada.
	    * Entrada:
	    *   @bigFingerSensorValue: valor del sensor del dedo pulgar.
	    *   @indexFingerSensorValue: valor del sensor del dedo índice.
	    *   @middleFingerSensorValue: valor del sensor del dedo medio.
	    */
		void calibrateStraightHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger->setStraightResistance(bigFingerSensorValue - bigFinger->getDivisorResistance());
			indexFinger->setStraightResistance(indexFingerSensorValue - indexFinger->getDivisorResistance());
			middleFinger->setStraightResistance(middleFingerSensorValue - middleFinger->getDivisorResistance());
		}

        /*
	    * Método utilizado para calibrar la mano flexionada.
	    * Entrada:
	    *   @bigFingerSensorValue: valor del sensor del dedo pulgar.
	    *   @indexFingerSensorValue: valor del sensor del dedo índice.
	    *   @middleFingerSensorValue: valor del sensor del dedo medio.
	    */
		void calibrateBendHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger->setBendResistance(bigFingerSensorValue - bigFinger->getDivisorResistance());
			indexFinger->setBendResistance(indexFingerSensorValue - indexFinger->getDivisorResistance());
			middleFinger->setBendResistance(middleFingerSensorValue - middleFinger->getDivisorResistance());
		}



	private:
		void updateFingers(double bigFingerSensor, double indexFingerSensor, double middleFingerSensor) {
			bigFinger->processInformation(bigFingerSensor);
			indexFinger->processInformation(indexFingerSensor);
			middleFinger->processInformation(middleFingerSensor);
		}
	
		/*
		* Método utilizado para procesar la posición de la mano, mapea la posición de los
		* dedos una posición dada la información brindada por los sensores flex.
		*/
		void processPosition() {
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
		/*
		* Método utilizado para procesar la posición de la mano en el espacio,
		* mapea la posición en el espacio de la mano según la información recibida
		* del acelerómetro.
		*/
		/*
		void processSpacePosition() {
			if (this->acelerometer.getXValue() >= 0.9) {
				spacePosition = (int)X_PLUS;
				return;
			} else if (this-->acelerometer.getYValue() >= 0.9) {
				spacePosition = (int)Y_PLUS;
				return;
			} else if (this-->acelerometer.getZValue() >= 0.9) {
				spacePosition = (int)Z_PLUS;
				return;
			} else if (this-->acelerometer.getYValue() <= -0.9) {
				spacePosition = (int)Z_LESS;
				return;
			}

			spacePosition = (int)UNDEFINED;
		}*/
};

#endif
