#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>
#include "SatTrackInterface.hpp"
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"
#include "Program.hpp"

using std::vector;

using namespace std;

namespace po = boost::program_options;


//парсер командной строки +
//класс связи спутника, антенны, порта +
//std::optional -no need

//запустили программу проверить подключение
//п-ль вводит имя порта (одно значение по умолчанию)
//считать состояни - проверка тек положения - порт верен иначе ошибка
//с логом или без, функция(слежка спутника, нескоьких, прогноз, вывести текущее положение антенны) + команда help
//перепроверьте данные


// нужна проверка, что спутник пересекает 0..... как? не знаю .... рассмаривать разное нправление движения вверх вниз,


bool AzimuthIsInreasing(shared_ptr<Satellite> currentSat) {
	double azFirst = currentSat->GetAzimuthByTime(currentSat->GetAos());
	double azSecond = currentSat->GetAzimuthByTime(currentSat->GetAos().AddMinutes(1.0));
	return azFirst < azSecond;
}

bool CrossSiteLongtitude(shared_ptr<Satellite> currentSat) {
	double aosLong = currentSat->GetLongitudeByTime(currentSat->GetAos());
	double losLong = currentSat->GetLongitudeByTime(currentSat->GetLos());
	double siteLong = currentSat->radiansToDegrees(currentSat->GetSiteInfo().GetLocation().longitude);
	return (siteLong > losLong && siteLong < aosLong) || (siteLong < losLong&& siteLong > losLong);
}

bool CrossZero(shared_ptr<Satellite> currentSat) {
	//на запад
	if (currentSat->GetDirection() == Direction::west) {
		return (currentSat->GetLos() > currentSat->GetAos()) && !AzimuthIsInreasing(currentSat) && CrossSiteLongtitude(currentSat);
	}
	//на восток
	if (currentSat->GetDirection() == Direction::east) {
		return (currentSat->GetAos() > currentSat->GetLos()) && AzimuthIsInreasing(currentSat) && CrossSiteLongtitude(currentSat);
	}
}


int main(int argc, char* argv[]) {

	shared_ptr<Program> program = make_shared<Program>(argc, argv);
	program->Run();

	//SatTrackInterface* track = new SatTrackInterface(1, (string)"NOAA 15");
	//shared_ptr<Satellite> sat = track->GetSatellite();
	//sat->UpdateData();
	//
	//sat->UpdatePassInfo(DateTime::Now());
	//cout << DateTime::Now() << endl << sat->GetAos() << endl;
	//struct Info {
	//	int val;
	//	string name;
	//};

	//list<Info> _list;
	//for (int i = 0; i < 10; i++) {
	//	Info info = { i, "a" };
	//	_list.push_back(info);
	//}
	//int j = 5;
	//list<Info>::const_iterator it = _list.begin();
	//while (j >= it->val) {
	//	_list.pop_front();
	//	it = _list.begin();
	//}
	//cout << it->val;
	return 0;
}