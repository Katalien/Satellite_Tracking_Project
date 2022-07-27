#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

class ComPort {
public:
	ComPort() { portName = L"COM3"; }; // default
	ComPort(wstring portName) : portName(portName) {};
	void getConnection();
	void closePort();
	void giveCommand(string const& com);
	void readCOM();
	void turnOnAngles(double azimuth, double elevation);
	void printCurrentAngles();
	int getAzimuth();
	int getElevation();
	~ComPort() { closePort(); };

private:
	HANDLE hSerial{};
	wstring portName;

	void setConnectionParams();  // default
	int readOneAngle();
	int toAngle(char* data);
	string makeTurnCommand(int const& az, int const& el);
	string makeAngle(int x);
};