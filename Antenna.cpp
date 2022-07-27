#include "Antenna.hpp"
using namespace std;

void Antenna::trackSatellite(shared_ptr<Satellite> sat) {
	Handle handle;
	int curAzimuth = 0, curElevation = 0;
	currentSat = sat;
	currentSat->updateData();
	currentSat->updatePassInfo(DateTime::Now());
	handle.openNewFile(sat->getName(), sat->toLocalTime(sat->getAos()));
	while (1) {
		currentSat->updateData();
		if (!currentSat->isVisible()) {
			port->turnOnAngles(antennaParkAzimuth(), 0);		
			return;
		}

		handle.writeFile(sat->toLocalTime(sat->getTime()), sat->getAzimuth(), sat->getElevation());

		if (isWaiting() && convertAngle((int)sat->getAzimuth()) > antennaParkAzimuth()) {
			cout << "Need to wait " << endl;
			return;
		}

		curAzimuth = (int)sat->getAzimuth();
		curElevation = (int)sat->getElevation();

		if (needToConvertAngle() ) {
			if (sat->getAzimuth() > 0 && sat->getAzimuth() < 90) {
				curAzimuth = convertAngle((int)sat->getAzimuth());
			}
		}

		port->turnOnAngles(curAzimuth, curElevation);
		updateCurrentAngles();
		
		// for check
		cout << sat->toLocalTime(sat->getTime()) << endl;
		cout << sat->getName() << endl;
		cout << "Sat: Az: " << sat->getAzimuth() << " El: " << sat->getElevation() << endl;
		cout << "Ant: Az: " << curAzimuth << " El: " << curElevation << endl;
		Sleep(1000);
	}
	handle.closeFile();
}

void Antenna::updateCurrentAngles() {
	elevation = port->getElevation();
	azimuth = port->getAzimuth();
}

int Antenna::parkAzimuthToWest(int const& aosAz, int const& losAz) {
	if (!crossZero()) {
		// track satelite all the way
		return aosAz;
	}
	if (aosAz < 90) {
		// no delay, need to convert angles
		return 360 + aosAz;   
	}
	if (aosAz - 90 < 360 - losAz) {
		//delay and convert angles
		return 450; 
	}
	if (aosAz - 90 >= 360 - losAz) {
		// no delay
		return aosAz;  
	}
	return 0;
}

// на восток
int Antenna::parkAzimuthToEast(int const& aosAz, int const& losAz) {
	if (!crossZero()) {
		// track satelite all the way
		return aosAz;
	}
	if (losAz < 90) {
		// no delay, but convert angles
		return aosAz;  
	}
	if (360 - aosAz >= losAz - 90) {
		return aosAz;
	}
	if (360 - aosAz < losAz - 90) {
		// delay
		return 0; 
	}
	return 0;
}

bool Antenna::isWaiting() {
	int aosAz = (int)currentSat->getAzimuthByTime(currentSat->getAos());
	int losAz = (int)currentSat->getAzimuthByTime(currentSat->getLos());
	if (currentSat->getDirection() == Direction::west) {
		return delayToWest(aosAz, losAz);
	}
	if (currentSat->getDirection() == Direction::east) {
		return delayToEast(aosAz, losAz);
	}
	else {
		throw exception("Something went wrong. Unknown direction");
		exit(-1);
	}
}

int Antenna::antennaParkAzimuth() {
	int aosAz = (int)currentSat->getAzimuthByTime(currentSat->getAos());
	int losAz = (int)currentSat->getAzimuthByTime(currentSat->getLos());
	if (currentSat->getDirection() == Direction::west) { return parkAzimuthToWest(aosAz, losAz); }
	if (currentSat->getDirection() == Direction::east) { return parkAzimuthToEast(aosAz, losAz); }
	else {
		throw exception("Something went wrong. Unknown direction");
		exit(-1);
	}
}

bool Antenna::delayToWest(int const& aosAz, int const& losAz) {
	return aosAz > 90 && aosAz - 90 < 360 - losAz;
}

bool Antenna::delayToEast(int const& aosAz, int const& losAz) {
	return losAz > 90 && 360 - aosAz < losAz - 90;
}

bool Antenna::azimuthIsInreasing() {
	double azFirst = currentSat->getAzimuthByTime(currentSat->getAos());
	double azSecond = currentSat->getAzimuthByTime(currentSat->getAos().AddMinutes(1.0));
	return azFirst < azSecond;
}

bool Antenna::crossSiteLongtitude() {
	double aosLong = currentSat->getLongitudeByTime(currentSat->getAos());
	double losLong = currentSat->getLongitudeByTime(currentSat->getLos());
	double siteLong = currentSat->radiansToDegrees(currentSat->getSiteInfo().GetLocation().longitude);
	return (siteLong > losLong && siteLong < aosLong) || (siteLong < losLong&& siteLong > losLong);
}

bool Antenna::crossZero() {
	//на запад
	if (currentSat->getDirection() == Direction::west) {
		return (currentSat->getLos() > currentSat->getAos()) && !azimuthIsInreasing() && crossSiteLongtitude();
	}
	//на восток
	if (currentSat->getDirection() == Direction::east) {
		return (currentSat->getAos() > currentSat->getLos()) && azimuthIsInreasing() && crossSiteLongtitude();
	}
	return false;
}

bool Antenna::needToConvertAngle() {
	int aosAz = (int)currentSat->getAzimuthByTime(currentSat->getAos());
	int losAz = (int)currentSat->getAzimuthByTime(currentSat->getLos());

	if (currentSat->getDirection() == Direction::west) {
		return crossZero() && aosAz < 90;
	}
	if (currentSat->getDirection() == Direction::east) {
		return crossZero() && losAz < 90;
	}
	return false;
}

int Antenna::convertAngle(int const& angle) {
	return angle + 360;
}