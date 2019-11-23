#ifndef _Flex_h
#define _Flex_h

#define BEND_FLEX         0
#define STRAIGHT_FLEX     1
#define VOLTAJE_RECEIVED	4.98

class Flex {
	private:
		double straightResistance; 	// Resistencia del flex estirado.
		double bendResistance;		// Resistencia del flex flexionado.
		double divisorResistance;	// Resistencia del divisor de tensión.
		int flexPosition;			// Posición en la que se encuentra el dedo.
		double difference;
		double factor;
	
	public:
		// Constructor genérico.
		Flex() {};

		/*
		* Constructor del Flex.
		* Entrada:
		* 	@divisorResistance: resistencia del divisor de tensión.
		*/
		Flex(double divisorResistance, double factor) {	
			this->straightResistance = -1;
			this->bendResistance = -1;
			this->divisorResistance = divisorResistance;
			int flexPosition = -1;
			this->difference = -1;
			this->factor = factor;
		}
		
		/*
		* Método utilizado para orquestar el procesamiento de la información recibida.
		* Entrada:
		* 	@sensorValue: valor leído del sensor flex.
		*	@voltajeReceived: voltaje que recibe el sensor flex.
		*/
		void processInformation(double sensorValue) {
			flexPositionCalculator(sensorValue);
		}
		
		/*
		* Setters y getters de los atributos del sensor flex.
		*/
		
		void setStraightResistance(double straightResistance) {
			this->straightResistance = straightResistance;
		}
		
		void setBendResistance(double bendResistance) {
			this->bendResistance = bendResistance;
		}
		
		void setDifference(double difference) {
			this->difference = difference;
		}
		
		void setDivisorResistance(double divisorResistance) {
			this->divisorResistance = divisorResistance;
		}
					
		double getStraightResistance() {
			return this->straightResistance;
		}
		
		double getBendResistance() {
			return this->bendResistance;
		}
		
		double getDivisorResistance() {
			return this->divisorResistance;
		}
		
		int getFlexPosition() {
			return this->flexPosition;
		}
		
	private:
		void flexPositionCalculator (double flexValue){
			int aux = this->straightResistance - flexValue;
		
			if (aux < 0) {
				this->flexPosition = (int)STRAIGHT_FLEX;
				return;
			}
			
			if (aux >= this->difference * this->factor) {
				this->flexPosition = (int)BEND_FLEX;
				return;
			}
			else {
				this->flexPosition = (int)STRAIGHT_FLEX;
				return;
			}
		}
		
};

#endif