#include "Antenna.hpp"
using namespace std;

void Antenna::TrackSatellite(shared_ptr<Satellite> sat) {
	Handle handle;
	int curAzimuth = 0, curElevation = 0;
	currentSat = sat;
	currentSat->UpdateData();
	currentSat->UpdatePassInfo(DateTime::Now());
	handle.OpenNewFile(sat->GetName(), sat->ToLocalTime(sat->GetAos()));
	while (1) {
		currentSat->UpdateData();
		if (!currentSat->IsVisible()) {
			port->TurnOnAngles(AntennaParkAzimuth(), 0);		
			return;
		}

		handle.WriteFile(sat->ToLocalTime(sat->GetTime()), sat->GetAzimuth(), sat->GetElevation());

		if (IsWaiting() && ConvertAngle((int)sat->GetAzimuth()) > AntennaParkAzimuth()) {
			cout << "Need to wait " << endl;
			return;
		}

		curAzimuth = (int)sat->GetAzimuth();
		curElevation = (int)sat->GetElevation();

		if (NeedToConvertAngle() ) {
			if (sat->GetAzimuth() > 0 && sat->GetAzimuth() < 90) {
				curAzimuth = ConvertAngle((int)sat->GetAzimuth());
			}
		}

		port->TurnOnAngles(curAzimuth, curElevation);
		UpdateCurrentAngles();
		
		// for check
		cout << sat->ToLocalTime(sat->GetTime()) << endl;
		cout << sat->GetName() << endl;
		cout << "Sat: Az: " << sat->GetAzimuth() << " El: " << sat->GetElevation() << endl;
		cout << "Ant: Az: " << curAzimuth << " El: " << curElevation << endl;
		Sleep(1000);
	}
	handle.CloseFile();
}

void Antenna::UpdateCurrentAngles() {
	elevation = port->GetElevation();
	azimuth = port->GetAzimuth();
}

int Antenna::ParkAzimuthToWest(int const& aosAz, int const& losAz) {
	if (!CrossZero()) {
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
int Antenna::ParkAzimuthToEast(int const& aosAz, int const& losAz) {
	if (!CrossZero()) {
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

bool Antenna::IsWaiting() {
	int aosAz = (int)currentSat->GetAzimuthByTime(currentSat->GetAos());
	int losAz = (int)currentSat->GetAzimuthByTime(currentSat->GetLos());
	if (currentSat->GetDirection() == Direction::west) {
		return DelayToWest(aosAz, losAz);
	}
	if (currentSat->GetDirection() == Direction::east) {
		return DelayToEast(aosAz, losAz);
	}
	else {
		throw exception("Something went wrong. Unknown direction");
		exit(-1);
	}
}

int Antenna::AntennaParkAzimuth() {
	int aosAz = (int)currentSat->GetAzimuthByTime(currentSat->GetAos());
	int losAz = (int)currentSat->GetAzimuthByTime(currentSat->GetLos());
	if (currentSat->GetDirection() == Direction::west) { return ParkAzimuthToWest(aosAz, losAz); }
	if (currentSat->GetDirection() == Direction::east) { return ParkAzimuthToEast(aosAz, losAz); }
	else {
		throw exception("Something went wrong. Unknown direction");
		exit(-1);
	}
}

bool Antenna::DelayToWest(int const& aosAz, int const& losAz) {
	return aosAz > 90 && aosAz - 90 < 360 - losAz;
}

bool Antenna::DelayToEast(int const& aosAz, int const& losAz) {
	return losAz > 90 && 360 - aosAz < losAz - 90;
}

bool Antenna::AzimuthIsInreasing() {
	double azFirst = currentSat->GetAzimuthByTime(currentSat->GetAos());
	double azSecond = currentSat->GetAzimuthByTime(currentSat->GetAos().AddMinutes(1.0));
	return azFirst < azSecond;
}

bool Antenna::CrossSiteLongtitude() {
	double aosLong = currentSat->GetLongitudeByTime(currentSat->GetAos());
	double losLong = currentSat->GetLongitudeByTime(currentSat->GetLos());
	double siteLong = currentSat->radiansToDegrees(currentSat->GetSiteInfo().GetLocation().longitude);
	return (siteLong > losLong && siteLong < aosLong) || (siteLong < losLong&& siteLong > losLong);
}

bool Antenna::CrossZero() {
	//на запад
	if (currentSat->GetDirection() == Direction::west) {
		return (currentSat->GetLos() > currentSat->GetAos()) && !AzimuthIsInreasing() && CrossSiteLongtitude();
	}
	//на восток
	if (currentSat->GetDirection() == Direction::east) {
		return (currentSat->GetAos() > currentSat->GetLos()) && AzimuthIsInreasing() && CrossSiteLongtitude();
	}
	return false;
}

bool Antenna::NeedToConvertAngle() {
	int aosAz = (int)currentSat->GetAzimuthByTime(currentSat->GetAos());
	int losAz = (int)currentSat->GetAzimuthByTime(currentSat->GetLos());

	if (currentSat->GetDirection() == Direction::west) {
		return CrossZero() && aosAz < 90;
	}
	if (currentSat->GetDirection() == Direction::east) {
		return CrossZero() && losAz < 90;
	}
	return false;
}

int Antenna::ConvertAngle(int const& angle) {
	return angle + 360;
}