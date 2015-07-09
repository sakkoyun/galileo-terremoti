// acceleroMMA7361.h - Library for retrieving data from the MMA7361 accelerometer. 
// Copyright 2011-2012 Jef Neefs (neefs@gmail.com) and Jeroen Doggen (jeroendoggen@gmail.com)
// For more information: variable declaration, changelog,... see AcceleroMMA7361.h 
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// acceleroMMA7361.cpp - Library for retrieving data from the MMA7361 accelerometer.
// For more information: variable declaration, changelog,... see AcceleroMMA7361.h

#include "AcceleroMMA7361.h"
#include "Log.h"

double oneG = 1672;
double oneGplusHalf = 1876;

/// constructor
AcceleroMMA7361::AcceleroMMA7361() {}

void AcceleroMMA7361::begin(uint8_t xPin, uint8_t yPin, uint8_t zPin) {
	pinMode(xPin, INPUT);
	pinMode(yPin, INPUT);
	pinMode(zPin, INPUT);
	_xPin = xPin;
	_yPin = yPin;
	_zPin = zPin;
	setOffSets(0, 0, 0);
	setARefVoltage(3.3);
	setAveraging(10);
}

/// setOffSets( int offSetX, int offSetY, int offSetZ): Sets the offset values for the x,y,z axis.
/// The parameters are the offsets expressed in G-force (100 = 1 G)
/// Offsets are added to the raw datafunctions
void AcceleroMMA7361::setOffSets(int xOffSet, int yOffSet, int zOffSet) {
	if (_refVoltage == 3.3) {
		_offSets[0] = map(xOffSet, 0, 3300, 0, 1024);
		_offSets[1] = map(yOffSet, 0, 3300, 0, 1024);
		_offSets[2] = map(zOffSet, 0, 3300, 0, 1024);
	}
	else {
		_offSets[0] = map(xOffSet, 0, 5000, 0, 1024);
		_offSets[1] = map(yOffSet, 0, 5000, 0, 1024);
		_offSets[2] = map(zOffSet, 0, 5000, 0, 1024);
	}
}

/// setARefVoltage(double _refV): Sets the AREF voltage to external, (now only takes 3.3 or 5 as parameter)
/// default is 5 when no AREF is used. When you want to use 3.3 AREF, put a wire between the AREF pin and the
/// 3.3 V VCC pin. This increases accuracy
void AcceleroMMA7361::setARefVoltage(double refV) {
#if ARDUINO < 153
	_refVoltage = refV;
	if (refV == 3.3) {
		analogReference(EXTERNAL);
	}
#endif
}

/// setAveraging(int avg): Sets how many samples have to be averaged in getAccel default is 10.
void AcceleroMMA7361::setAveraging(int avg) {
	_average = avg;
}

// test and read voltage AXIS on idle
void AcceleroMMA7361::test_voltage() {
	Log::i("VOLTAGE TEST ++++++++++++++START+++++++++++++++ ");
	Log::i("VOLTAGE X: %i", analogRead(_xPin));
	Log::i("VOLTAGE X: %i", analogRead(_yPin));
	Log::i("VOLTAGE X: %i", analogRead(_zPin));
	Log::i("VOLTAGE TEST ++++++++++++++END+++++++++++++ ");
}

/// getXRaw(): Returns the raw data from the X-axis analog I/O port of the Arduino as an integer
long AcceleroMMA7361::getXRaw() {
	return analogRead(_xPin) + _offSets[0] + 2;
}

/// getYRaw(): Returns the raw data from the Y-axis analog I/O port of the Arduino as an integer
long AcceleroMMA7361::getYRaw() {
	return analogRead(_yPin) + _offSets[1] + 2;
}

/// getZRaw(): Returns the raw data from the Z-axis analog I/O port of the Arduino as an integer
long AcceleroMMA7361::getZRaw() {
	return analogRead(_zPin) + _offSets[2];
}

/// getXVolt(): Returns the voltage in mV from the X-axis analog I/O port of the Arduino as a integer
long AcceleroMMA7361::getXVolt() {
	return _mapMMA7361V(getXRaw());
}

/// getYVolt(): Returns the voltage in mV from the Y-axis analog I/O port of the Arduino as a integer
long AcceleroMMA7361::getYVolt() {
	return _mapMMA7361V(getYRaw());
}

/// getZVolt(): Returns the voltage in mV from the Z-axis analog I/O port of the Arduino as a integer
long AcceleroMMA7361::getZVolt() {
	return _mapMMA7361V(getZRaw());
}

/// getXAccel(): Returns the acceleration of the X-axis as a int (1 G = 100.00)
long AcceleroMMA7361::getXAccel() {
	long sum = 0;
	for (int i = 0; i < _average; i++) {
		sum = sum + _mapMMA7361G(getXRaw());
	}
	return sum / _average;
}

/// getYAccel(): Returns the acceleration of the Y-axis as a int (1 G = 100.00)
long AcceleroMMA7361::getYAccel() {
	long sum = 0;
	for (int i = 0; i < _average; i++) {
		sum = sum + _mapMMA7361G(getYRaw());
	}
	return sum / _average;
}

/// getZAccel(): Returns the acceleration of the Z-axis as a int (1 G = 100.00)
long AcceleroMMA7361::getZAccel() {
	long sum = 0;
	for (int i = 0; i < _average; i++) {
		sum = sum + _mapMMA7361G(getZRaw());
	}
	return sum / _average;
}

/// getAccelXYZ(int *_XAxis, int *_YAxis, int *_ZAxis) returns all axis at once as pointers
void AcceleroMMA7361::getAccelXYZ(long *_XAxis, long *_YAxis, long *_ZAxis) {
	long sum[3];
	sum[0] = 0;
	sum[1] = 0;
	sum[2] = 0;
	for (int i = 0; i < _average; i++) {
		sum[0] = sum[0] + _mapMMA7361G(getXRaw());
		sum[1] = sum[1] + _mapMMA7361G(getYRaw());
		sum[2] = sum[2] + _mapMMA7361G(getZRaw());
	}
	*_XAxis = sum[0] / _average;
	*_YAxis = sum[1] / _average;
	*_ZAxis = sum[2] / _average;
}

/// mapMMA7361V: calculates and returns the voltage value derived from the raw data. Used in getXVoltage, getYVoltage, getZVoltage
long AcceleroMMA7361::_mapMMA7361V(long value) {
	if (_refVoltage == 3.3) {
		return map(value,0,1024,0,3300);
		// return map(value,0,4096,0,3300);
	}
	else {
		return map(value, 0, 1024, 0, 5000);
	}
}

/// mapMMA7361G: calculates and returns the accelerometer value in degrees derived from the raw data. Used in getXAccel, getYAccel, getZAccel
long AcceleroMMA7361::_mapMMA7361G(long value) {
	if (_sensi == false) {
		if (_refVoltage == 3.3) {
			return map(value, 0, 1024, -825, 800);
		} else {
			return map(value, 0, 1024, -800, 1600);
		}
	} else {
		if (_refVoltage == 3.3) {
			return map(value, 0, 1024, -206, 206);
		} else {
			return map(value, 0, 1024, -260, 419);
		}
	}
}

/// calibrate(): Sets X and Y values via setOffsets to zero. The Z axis will be set to 100 = 1G
/// WARNING WHEN CALIBRATED YOU HAVE TO MAKE SURE THE Z-AXIS IS PERPENDICULAR WITH THE EARTHS SURFACE
void AcceleroMMA7361::calibrate() {
	test_voltage();
	delay(1000);
	setOffSets(0, 0, 0);
	_sensi = false;
	Log::i("Calibrating MMA7361011");
	double var = 5000;
	double sumX = 0;
	double sumY = 0;
	double sumZ = 0;

	for (int i = 0; i < var; i++) {
		sumX = sumX + getXVolt();
		sumY = sumY + getYVolt();
		sumZ = sumZ + getZVolt();
	}

	if (_sensi == false) { // if ref is 3,3V
		setOffSets((int)(oneG - sumX / var), (int)(oneG - sumY / var), (int)(+oneGplusHalf - sumZ / var));
	} else {
		setOffSets((int)(1650 - sumX / var), (int)(1650 - sumY / var), (int)(+2450 - sumZ / var));
	}

	if (abs(getOrientation()) != 3) {
		Log::e("unable to calibrate");
		setOffSets(0, 0, 0);
	} else {
		if (getOrientation() == 3) {
			Log::i("ORIENTATION RIGHT! ");
		} else {
			Log::i("ORIENTATION WRONG!!! ");
		}

		Log::i(" sum: X:%lf Y:%lf Z:%lf", sumX / var, sumY / var, sumZ / var);
		Log::i("Calibration DONE");
	}
}

/// getOrientation returns which axis perpendicular with the earths surface x=1,y=2,z=3 is positive or
/// negative depending on which side of the axis is pointing downwards
int AcceleroMMA7361::getOrientation() {
	int gemiddelde = 10;
	long x = 0;
	long y = 0;
	long z = 0;
	long xAbs = 0;
	long yAbs = 0;
	long zAbs = 0;
	for (int i = 0; i < gemiddelde; i++) {  // We take in this case 10 measurements to average the error a little bit
		x = x + getXAccel();
		y = y + getYAccel();
		z = z + getZAccel();
	}
	x = x / gemiddelde;
	y = y / gemiddelde;
	z = z / gemiddelde;
	xAbs = labs(100 - labs(x));
	yAbs = labs(100 - labs(y));
	zAbs = labs(100 - labs(z));
	if (xAbs < yAbs && xAbs < zAbs) {
		if (x > 0) {
			return 1;
		}
		return -1;
	}
	if (yAbs < xAbs && yAbs < zAbs) {
		if (y > 0) {
			return 2;
		}
		return -2;
	}
	if (zAbs < xAbs && zAbs < yAbs) {
		if (z > 0) {
			return 3;
		}
		return -3;
	}
	return 0;
}

/* manually added because not available anymore in math.h */
double square(double x) { return x * x; }

// getTotalVector returns the magnitude of the total acceleration vector as an integer
double AcceleroMMA7361::getTotalVector() {
	return sqrt(square(_mapMMA7361G(getXRaw())) + square(_mapMMA7361G(getYRaw())) + square(_mapMMA7361G(getZRaw())));
}
