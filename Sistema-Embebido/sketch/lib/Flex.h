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
		double flexVoltaje;				// Voltaje que recibe el flex.
		int flexPosition;			// Posición en la que se encuentra el dedo.
		double actualValue;
		double angleV;
	
	public:
		// Constructor genérico.
		Flex() {};

		/*
		* Constructor del Flex.
		* Entrada:
		* 	@divisorResistance: resistencia del divisor de tensión.
		*/
		Flex(double divisorResistance) {	
			this->straightResistance = -1;
			this->bendResistance = -1;
			this->divisorResistance = divisorResistance;
			this->flexVoltaje = -1;
			int flexPosition = -1;
			this->actualValue = -1;
			this->angleV = -1;
		}
		
		/*
		* Método utilizado para orquestar el procesamiento de la información recibida.
		* Entrada:
		* 	@sensorValue: valor leído del sensor flex.
		*	@voltajeReceived: voltaje que recibe el sensor flex.
		*/
		double processInformation(double sensorValue) {
			this->actualValue = sensorValue;
			flexPositionCalculator (angleCalculator(sensorValue));
			return sensorValue;
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
		
		double getAngle() {
			return this->angleV;
		}
		
		double getActualValue() {
			return this->actualValue;
		}
		
	private:
		double angleCalculator(double sensorValue) {
			double flexResistance;
			
			this->flexVoltaje = (sensorValue - this->divisorResistance) * (double)VOLTAJE_RECEIVED / 1023.0;
			flexResistance = this->divisorResistance * ((double)VOLTAJE_RECEIVED / this->flexVoltaje - 1.0);

			// Uso la resistencia calculada para estimar el angulo de inclinación del sensor
			double angle = map(flexResistance, this->straightResistance, this->bendResistance, 0, 90.0);
			
			
			this->angleV = angle;
			
			return angle;
		}
		
		void flexPositionCalculator (double flexAngle){
			if (flexAngle <= 45.0){
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