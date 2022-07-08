#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#pragma warning(disable : 4996)
using namespace std;

const int ANGLE_SIZE = 3;

class PortCommand {
public:
	char* TurnAnglesCommand(double azimuth_d, double elevation_d) {
		if (azimuth_d < 0.0 || elevation_d < 0.0 || azimuth_d > 360.0 || elevation_d > 180.0) {
			cout << "Invalid angles" << endl;
			return nullptr;
		}
		char* command = new char[8];
		char* azimuth = BuildAngleFromDouble(azimuth_d);
		char* elevation = BuildAngleFromDouble(elevation_d);
		command[0] ='W';
		
		for (int i = 1; i <= 3; i++) {
			command[i] = azimuth[i - 1];
			command[i + 4] = elevation[i - 1];
		}
		command[4] = ' ';
		for (int i = 0; i < 8; i++) {
			cout << command[i];
		}
		//free(azimuth);
		//free(elevation);
		return command;
	}

	/*!
	 * \brief Short description of the method 
	 * \param <Parameter description>
	 * \return <Describe result>
	 */
	char* GetAnglesCommand() {
		char* command = new char[2];
		command[0] = 'C';
		command[1] = '2';
		return command;
	}

private:
	char* BuildAngleFromDouble(double x) {// why double?
		string data_s = to_string((int)x); // data loss during type conversion
		char* data = new char[3]; // who owns this memory?
		if (x == 0.0) { // wrong comparison 
			data[0] = 0; // type conversion
			data[1] = 0; // type conversion
			data[2] = 0; // type conversion
			return data;
		}
		if (x < 100 && x != 0.0) { // wrong comparison
			data_s = '0' + data_s;
		}	
		
		strcpy(data, data_s.c_str()); // unsafe copy
		return data;
	}
};