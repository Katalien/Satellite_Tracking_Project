#pragma once
#include <iostream>
#include <windows.h>
#include "PortCommand.hpp"

using namespace std;

class ComPort {
public:
	ComPort() { portName = L"COM3"; }; // default
	ComPort(LPCTSTR portName) : portName(portName) {};
	void GetConnection();
	void ClosePort();
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