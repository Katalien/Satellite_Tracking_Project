#pragma once
#include <iostream>
#include <windows.h>
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
            //cout << "serial port does not exist.\n";
            throw exception("serial port does not exist.\n");
        }
        //cout << "some other error occurred." << endl << " Error: " << GetLastError();
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
        cout << "error setting serial port state\n";
        //assert(SetCommState(hSerial, &dcbSerialParams));
    }
}

/// <summary>
/// Close the port
/// </summary>
void ComPort::ClosePort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

/// <summary>
/// Send command to the port
/// </summary>
/// <param name="com"> Command in form of array of chars </param>
void ComPort::GiveCommand(char* com) {
    DWORD dwSize = MAX_COM_SIZE;
    DWORD dwBytesWritten;

    BOOL iRet = WriteFile(hSerial, com, dwSize, &dwBytesWritten, NULL);
    if (!iRet) {
        cout << "Error in giving command" << endl;
    }
}

/// <summary>
/// Print data from COM port
/// </summary>
void ComPort::ReadCOM() {
    DWORD iSize;
    char sReceivedChar;
    while (true) {
        ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
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
        ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
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
    int a = ToAngle(data);
    return a;
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
        int a = ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
        return  ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
    }
    if (data[1] != ' ') {
        return ConvertCharToInt(data[1]) * 100 + ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
    }
}

/// <summary>
/// Give command to turn on necessary angles
/// </summary>
/// <param name="azimuth"> Azimuth angle</param>
/// <param name="elevation"> Elevation angle</param>
void ComPort::TurnOnAngles(int azimuth, int elevation) {
    char* command = portCommand.TurnAnglesCommand(azimuth, elevation);
    if (command == nullptr) {
        return;
    }
    GiveCommand(command);
    delete(command);
}

/// <summary>
/// Send command and print angles in form as they are given by port
/// </summary>
void ComPort::PrintCurrentAngles() {
    char* command = portCommand.GetAnglesCommand();
    GiveCommand(command);
    ReadCOM();
}

/// <summary>
/// Send the command and get current azimuth
/// </summary>
/// <returns></returns>
int ComPort::GetAzimuth() {
    char command[] = "C";
    GiveCommand(command);
    return ReadOneAngle();
}


/// <summary>
/// Send the command and get elevation angle
/// </summary>
/// <returns></returns>
int ComPort::GetElevation() {
    char command[] = "B";
    GiveCommand(command);
    return ReadOneAngle();
}