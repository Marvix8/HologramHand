#ifndef _Hand_h
#define _Hand_h

#include "Flex.h"

// Posición de la mano
#define STAR		        0
#define	SCISSORS        	1
#define GOOD		        2
#define ROCK		        3
#define UNDEFINED_POSITION  -1

// Posición de la mano en el espacio
#define X_PLUS                      0
#define Y_PLUS                      1
#define Z_PLUS                      2
#define Z_LESS                      3
#define UNDEFINED_SPACE_POSITION    -1

class Hand {
	private:
		Flex bigFinger; 			// Flex del pulgar.
		Flex indexFinger;			// Flex del dedo índice.
		Flex middleFinger;			// Flex del dedo medio
		Acelerometer acelerometer; 	// Acelerómetro
		bool calibrated;			// Booleano que indica si la mano está calibrada o no.
		int handPosition;           // Posición de la mano según sensores flex.
		int spacePosition;          // Posición de la mano en el espacio según sensor acelerómetro.

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

	    /*
	    * Método utilizado para calibrar la mano estirada.
	    * Entrada:
	    *   @bigFingerSensorValue: valor del sensor del dedo pulgar.
	    *   @indexFingerSensorValue: valor del sensor del dedo índice.
	    *   @middleFingerSensorValue: valor del sensor del dedo medio.
	    */
		void calibrateStraightHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger.setStraightResistance(bigFingerSensorValue - bigFinger.getDivisorResistance());
			indexFinger.setStraightResistance(indexFingerSensorValue - indexFinger.getDivisorResistance());
			middleFInger.setStraightResistance(middleFingerSensorValue - middleFInger.getDivisorResistance());
		}

        /*
	    * Método utilizado para calibrar la mano flexionada.
	    * Entrada:
	    *   @bigFingerSensorValue: valor del sensor del dedo pulgar.
	    *   @indexFingerSensorValue: valor del sensor del dedo índice.
	    *   @middleFingerSensorValue: valor del sensor del dedo medio.
	    */
		void calibrateBendHand(double bigFingerSensorValue, double indexFingerSensorValue, double middleFingerSensorValue) {
			bigFinger.setBendResistance(bigFingerSensorValue - bigFinger.getDivisorResistance());
			indexFinger.setBendResistance(indexFingerSensorValue - indexFinger.getDivisorResistance());
			middleFInger.setBendResistance(middleFingerSensorValue - middleFInger.getDivisorResistance());
		}

		/*
		* Método utilizado para procesar la información de la mano.
		*/
		void process() {
			processPosition();
			processSpacePosition();
		}

	private:
		/*
		* Método utilizado para procesar la posición de la mano, mapea la posición de los
		* dedos una posición dada la información brindada por los sensores flex.
		*/
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

			this->handPosition = (int)UNDEFINED_POSITION;
		}

		/*
		* Método utilizado para procesar la posición de la mano en el espacio,
		* mapea la posición en el espacio de la mano según la información recibida
		* del acelerómetro.
		*/
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

			spacePosition = (int)UNDEFINED_SPACE_POSITION;
		}

}

#endif
