#ifndef _Acelerometer_h
#define _Acelerometer_h

class Acelerometer {
  private:
	double axisX;
	double axisY;
	double axisZ;
  
  public:
	// Constructor genérico.
	Acelerometer() {}
	
	setAxisValues(double x, double y, double z) {
		this->axisX = x;
		this->axisY = y;
		this->axisZ = z;
	}
	
	double getAxisX() {
		return this->axisX;
	}
	
	double getAxisY() {
		return this->axisY;
	}
	
	double getAxisZ() {
		return this->axisZ;
	}

};

#endif
