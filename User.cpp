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




int main(int argc, char* argv[]) {

	shared_ptr<Program> program = make_shared<Program>(argc, argv);
	program->Run();

	
	return 0;
}