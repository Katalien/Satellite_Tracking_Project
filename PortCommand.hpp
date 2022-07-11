#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#pragma warning(disable : 4996)
using namespace std;

const int ANGLE_SIZE = 3;

class PortCommand {
public:
	char* TurnAnglesCommand(int azimuth_d, int elevation_d);
	char* GetAnglesCommand();

private:
	char* BuildAngleFromInt(int x);
};