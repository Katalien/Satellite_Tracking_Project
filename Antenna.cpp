#include "Antenna.hpp"
using namespace std;

/// <summary>
/// rotate antenna to track or wait for the satellite with writing in the file
/// </summary>
/// <param name="sat">satellite to track</param>
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
			waitingState = true;
			port->turnOnAngles(antennaParkAzimuth(), 0);
			cout << currentSat->toLocalTime(currentSat->getTime()) << endl;
			cout << currentSat->getName() <<" is out of view";
			Sleep(1000);
			system("cls");
			return;
		}

		handle.writeFile(sat->toLocalTime(sat->getTime()), (int)sat->getAzimuth(), (int)sat->getElevation());

		if (isWaiting() && convertAngle((int)sat->getAzimuth()) > antennaParkAzimuth() && waitingState == true) {
			return;
		}

		waitingState = false;
		curAzimuth = (int)sat->getAzimuth();
		curElevation = (int)sat->getElevation();

		if (needToConvertAngle() ) {
			if (sat->getAzimuth() > 0 && sat->getAzimuth() < 90) {
				curAzimuth = convertAngle((int)sat->getAzimuth());
			}
		}

		port->turnOnAngles(curAzimuth, curElevation);
		updateCurrentAngles();

		cout << sat->getName() << endl;
		cout << sat->toLocalTime(sat->getTime()) << endl;
		cout << "Az: " << sat->getAzimuth() << " El: " << sat->getElevation() << endl;		
		Sleep(1000);
		system("cls");
	}
	handle.closeFile();
}

/// <summary>
/// Update current antenna angles (azimuth and elevation)
/// </summary>
void Antenna::updateCurrentAngles() {
	elevation = port->getElevation();
	azimuth = port->getAzimuth();
}

/// <summary>
/// Calculate the optimal antenna angle for waiting the satellite, moving in the west direction
/// </summary>
/// <param name="aosAz">azimuth at the moment of acquisition of signal</param>
/// <param name="losAz">azimuth at the moment of loss of signal</param>
/// <returns>the optimal azimuth for waiting</returns>
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

/// <summary>
/// Calculate the optimal antenna angle for waiting the satellite, moving in the east direction
/// </summary>
/// <param name="aosAz">azimuth at the moment of the acquisition of signal</param>
/// <param name="losAz">azimuth at the moment of the loss of signal</param>
/// <returns>the optimal azimuth for waiting</returns>
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

/// <summary>
/// Decide if antenna needs to wait for satellite at the moment of acquisition of signal
/// </summary>
/// <returns>returns "true" if it's necessary to wait</returns>
bool Antenna::isWaiting() const {
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

/// <summary>
/// Calculate atenna azimuth for waiting satellite
/// </summary>
/// <returns>Antenna angle for waiting satellite</returns>
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

/// <summary>
/// Check if we need to wait while satellite is moving in the west direction
/// </summary>
/// <param name="aosAz">azimuth at the moment of the acquisition of signal</param>
/// <param name="losAz">azimuth at the moment of the loss of signal</param>
/// <returns></returns>
bool Antenna::delayToWest(int const& aosAz, int const& losAz) const {
	return aosAz > 90 && aosAz - 90 < 360 - losAz;
}

/// <summary>
/// Check if we need to wait while satellite is moving in the east direction
/// </summary>
/// <param name="aosAz">azimuth at the moment of the acquisition of signal</param>
/// <param name="losAz">azimuth at the moment of the loss of signal</param>
/// <returns></returns>
bool Antenna::delayToEast(int const& aosAz, int const& losAz) const {
	return losAz > 90 && 360 - aosAz < losAz - 90;
}

/// <summary>
/// Check if azimuth is increasing
/// </summary>
/// <returns></returns>
bool Antenna::azimuthIsInreasing() const {
	double azFirst = currentSat->getAzimuthByTime(currentSat->getAos());
	double azSecond = currentSat->getAzimuthByTime(currentSat->getAos().AddMinutes(1.0));
	return azFirst < azSecond;
}

/// <summary>
/// Decide if satellite crosses observer's longtitude
/// </summary>
/// <returns> </returns>
bool Antenna::crossSiteLongtitude() {
	double aosLong = currentSat->getLongitudeByTime(currentSat->getAos());
	double losLong = currentSat->getLongitudeByTime(currentSat->getLos());
	double siteLong = currentSat->radiansToDegrees(currentSat->getSiteInfo().GetLocation().longitude);
	return (siteLong > losLong && siteLong < aosLong) || (siteLong < losLong&& siteLong > losLong);
}

/// <summary>
/// Decide if antenna needs to cross zero point while tracking the satellite
/// </summary>
/// <returns></returns>
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

/// <summary>
/// Decide if it's necessary to convert angles from 0-90 to 360-450
/// </summary>
/// <returns></returns>
bool Antenna::needToConvertAngle(){
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

/// <summary>
/// Convert angles by making one rotation around the axis
/// </summary>
/// <param name="angle"></param>
/// <returns>Angle + 360</returns>
int Antenna::convertAngle(int const& angle) const {
	return angle + 360;
}