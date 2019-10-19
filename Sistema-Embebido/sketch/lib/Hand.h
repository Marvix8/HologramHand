#ifndef _Hand_h
#define _Hand_h

#include "Flex.h"

// Posición de la mano
#define STAR		0
#define	SCISSORS	1
#define GOOD		2
#define ROCK		3

// Posición de la mano en el espacio
#define X_PLUS
#define Y_PLUS
#define Z_PLUS
#define Z_LESS

class Hand {
	private:
		Flex bigFinger; 			// Flex del pulgar.
		Flex indexFinger;			// Flex del dedo índice.
		Flex middleFinger;			// Flex del dedo medio
		Acelerometer acelerometer; 	// Acelerómetro
		bool calibrated;			// Booleano que indica si la mano está calibrada o no.
		int handPosition;
		int spacePosition;
		
	public:
		/*
		* Constructor de Hand.
		* Entrada:
		*	@bigFinger: Flex del dedo pulgar de la mano.
		*	@indexFinger: Flex del dedo índice de la mano.
		*	@middleFinger: Flex del dedo del medio de la mano.
		*	@acelerometer: <por ahora valor recibido por acelerómetro (VER CON MIGUEL)>
		*/
		Hand(Flex bigFinger, Flex indexFinger, Flex middleFinger, Acelometer aceloremeter) {
			this->bigFinger = bigFinger;
			this->indexFinger = indexFinger;
			this->middleFinger = middleFinger;
			this->acelerometer = acelerometer;
			this->calibrated = false;
		}
		
	private:
		void calibrateStraightHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger.setStraightResistance(bigFingerSensorValue);
			indexFinger.setStraightResistance(indexFingerSensorValue);
			middleFInger.setStraightResistance(middleFingerSensorValue);
		}
		
		void calibrateBendHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger.setBendResistance(bigFingerSensorValue);
			indexFinger.setBendResistance(indexFingerSensorValue);
			middleFInger.setBendResistance(middleFingerSensorValue);
		}
		
		void process() {
			processPosition();
			processSpacePosition();
		}
		
		void processPosition() {
			if (bigFinger.getFlexPosition() == (int) STRAIGHT_FLEX) {
				if (indexFinger.getFlexPosition() == (int) STRAIGHT_FLEX
					&& middleFinger.getFlexPosition()  == (int) STRAIGHT_FLEX)
					this->handPosition = (int)STAR;
					return;
					
				else if (indexFinger.getFlexPosition() == (int) BEND_FLEX
					&& middleFinger.getFlexPosition()  == (int) BEND_FLEX)
					this->handPosition = (int)GOOD;
					return;
			} else if (bigFinger.getFlexPosition() == (int) BEND_FLEX) {
				if (indexFinger.getFlexPosition() == (int) STRAIGHT_FLEX
					&& middleFinger.getFlexPosition()  == (int) STRAIGHT_FLEX)
					this->handPosition = (int)SCISSORS;
					return;
					
				else if (indexFinger.getFlexPosition() == (int) BEND_FLEX
					&& middleFinger.getFlexPosition()  == (int) BEND_FLEX)
					this->handPosition = (int)ROCK;
					return;
			
			this->handPosition = (int)UNDEFINED;
		}

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
		}
		
}

#endif