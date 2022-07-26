#pragma once
#include <iostream>
#include <windows.h>
#include<string>
#include "ComPort.hpp"

#define MAX_COM_SIZE 8;

using namespace std;

/// <summary>
/// Connect to the COM port
/// </summary>
void ComPort::GetConnection() {
    hSerial = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            throw exception("serial port does not exist.\n");
        }
        throw exception("some other error occurred.\n");
    }
    else {
        cout << "Connected" << endl;
    }
    SetConnectionParams();
}

/// <summary>
/// Set parametrs for connection (by default)
/// </summary>
void ComPort::SetConnectionParams() {
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        throw exception("error setting serial port state");
        exit(-1);
    }
}

/// <summary>
/// Close the port
/// </summary>
void ComPort::ClosePort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

/// <summary>
/// Send command to the port
/// </summary>
/// <param name="com"> Command in form of array of chars </param>
void ComPort::GiveCommand(string const& com) {
    DWORD dwSize = MAX_COM_SIZE;
    DWORD dwBytesWritten;

    BOOL iRet = WriteFile(hSerial, com.c_str(), dwSize, &dwBytesWritten, NULL);
    if (!iRet) {
        throw exception("error in giving command");
        exit(-1);
    }
}

/// <summary>
/// Print data from COM port
/// </summary>
void ComPort::ReadCOM() {
    DWORD iSize;
    char sReceivedChar;
    while (true) {
        if (!ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0)) { // получаем 1 байт
            cout << "Impossible to read data from port" << endl;
            exit(-1);
        }  
        if (sReceivedChar == '\n') {
            break;
        }
        if (iSize > 0) { // если что-то принято, выводим
            cout << sReceivedChar;
        }
        else {
            break;
        }
    }
    cout << endl;
}

/// <summary>
/// Get one angle from COM port
/// </summary>
/// <returns></returns>
int ComPort::ReadOneAngle() {
    DWORD iSize;
    char sReceivedChar;
    char data[4];
    int i = 0;
    while (true) {
        if (!ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0)) { // получаем 1 байт
            cout << "Impossible to get data from port" << endl;
            exit(-1);
        }  
        if (sReceivedChar == '\r') {
            continue;
        }
        if (sReceivedChar == '\n') {
            break;
        }
        if (iSize > 0 && sReceivedChar != '+') { // если что-то принято, выводим
            data[i++] = sReceivedChar;
        }

    }
    return ToAngle(data);
}

int ConvertCharToInt(char x) {
    return (int)x - 48;
}

/// <summary>
/// Convert data from port to ineger angle
/// </summary>
/// <param name="data"> Data from port in form of char of array</param>
/// <returns>Integer angle</returns>
int ComPort::ToAngle(char* data) {
    if (data[1] == ' ' && data[2] == ' ') {
        return ConvertCharToInt(data[3]);
    }
    if (data[1] == ' ') {
        return  ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
    }
    if (data[1] != ' ') {
        return ConvertCharToInt(data[1]) * 100 + ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
    }
    return -1;
}

string ComPort::MakeTurnCommand(int const& az, int const& el) {
    string azStr = MakeAngle(az);
    string elStr = MakeAngle(el);
    return "W" + azStr + " " + elStr;
}

string ComPort::MakeAngle(int x) {
    string str = to_string(x);
    if (x == 0) {
        return "000";
    }
    if (x < 100 && x != 0) {
        return "0" + str;
    }
    return str;
}

void ComPort::TurnOnAngles(double azimuth, double elevation) {
    string command = MakeTurnCommand((int)azimuth, (int)elevation);
    if (command == "") {
        return;
    }
    GiveCommand(command);
}

/// <summary>
/// Send command and print angles in form as they are given by port
/// </summary>
void ComPort::PrintCurrentAngles() {
    //char* command = portCommand.GetAnglesCommand();
    string command = "C2";
    GiveCommand(command);
    try {
        ReadCOM();
    }
    catch (const std::exception& ex) {
        cout << "error in getting data from port " << ex.what() << endl;
        exit(-1);
    }
}

/// <summary>
/// Send the command and get current azimuth
/// </summary>
/// <returns></returns>
int ComPort::GetAzimuth() {
    string command = "C";
    GiveCommand(command);
    return ReadOneAngle();
}


/// <summary>
/// Send the command and get elevation angle
/// </summary>
/// <returns></returns>
int ComPort::GetElevation() {
    string command = "B";
    GiveCommand(command);
    return ReadOneAngle();
}