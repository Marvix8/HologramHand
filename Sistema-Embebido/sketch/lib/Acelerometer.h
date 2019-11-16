#ifndef _Acelerometer_h
#define _Acelerometer_h

class Acelerometer {
  private:
	axisX;
	axisY;
	axisZ;
  
  public:
	// Constructor genérico.
	Acelerometer() {}
	
	setAxisValues(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	double getAxisX() {
		return this->x;
	}
	
	double getAxisY {
		return this->y;
	}
	
	double getAxisZ {
		return this->z;
	}

};

#endif
