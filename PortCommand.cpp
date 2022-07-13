#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include "PortCommand.hpp"
#pragma warning(disable : 4996)

using namespace std;
using std::vector;

char* PortCommand::TurnAnglesCommand(int azimuth_d, int elevation_d) {
	if (azimuth_d < 0 || elevation_d < 0 || azimuth_d > 360 || elevation_d > 180) {
		cout << "Invalid angles" << endl;
		return nullptr;
	}
	char* command = new char[8];
	char* azimuth = BuildAngleFromInt(azimuth_d);
	char* elevation = BuildAngleFromInt(elevation_d);
	command[0] = 'W';

	for (int i = 1; i <= 3; i++) {
		command[i] = azimuth[i - 1];
		command[i + 4] = elevation[i - 1];
	}
	command[4] = ' ';
	return command;
}


char* PortCommand::GetAnglesCommand() {
	char* command = new char[2];
	command[0] = 'C';
	command[1] = '2';
	return command;
}


char* PortCommand::BuildAngleFromInt(int x) {// why double?
	string data_s = to_string(x); // data loss during type conversion
	char* data = new char[3]; // who owns this memory?
	if (x == 0) { // wrong comparison 
		data[0] = '0'; // type conversion
		data[1] = '0'; // type conversion
		data[2] = '0'; // type conversion
		return data;
	}
	if (x < 100 && x != 0) { // wrong comparison
		data_s = '0' + data_s;
	}

	strcpy(data, data_s.c_str()); // unsafe copy
	return data;
}

//vector<char> PortCommand::BuildAngleFromInt(int x) {
//	string data_s = to_string(x); 
//	vector<char> data; 
//	if (x == 0) { 
//		data.push_back('0'); 
//		data.push_back('0'); 
//		data.push_back('0'); 
//		return data;
//	}
//	if (x < 100 && x != 0) { // wrong comparison
//		data_s = '0' + data_s;
//	}
//
//	strcpy(data, data_s.c_str()); // unsafe copy
//	return data;
//}
