#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

class ComPort {
public:
	ComPort() { portName = L"COM3"; }; // default
	ComPort(LPCTSTR portName) : portName(portName) {};
	void GetConnection();
	void ClosePort();
	void GiveCommand(string const& com);
	void ReadCOM();
	void TurnOnAngles(double azimuth, double elevation);
	void PrintCurrentAngles();
	int GetAzimuth();
	int GetElevation();
	~ComPort() { ClosePort(); };

private:
	HANDLE hSerial{};
	LPCTSTR portName;

	void SetConnectionParams();  // default
	int ReadOneAngle();
	int ToAngle(char* data);
	string MakeTurnCommand(int const& az, int const& el);
	string MakeAngle(int x);
};