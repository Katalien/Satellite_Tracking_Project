#pragma once
#include <iostream>
#include <windows.h>
#include "PortCommand.hpp"

using namespace std;

class ComPort {
public:
	ComPort(LPCTSTR portName) : portName(portName) {};
	void GetConnection(); //return -1 for error and check (where)?  assert?
	void ClosePort();
	void SetAngles(double azimuth, double elevation);
	void GiveCommand(char* com);
	void ReadCOM();
	void TurnOnAngles(int azimuth, int elevation);
	void PrintCurrentAngles();
	int GetAzimuth();
	int GetElevation();

private:
	HANDLE hSerial;
	LPCTSTR portName;
	PortCommand portCommand;

	void SetConnectionParams();  // default
	int ReadOneAngle();
	int ToAngle(char* data);
};