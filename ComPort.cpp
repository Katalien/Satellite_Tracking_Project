#pragma once
#include <iostream>
#include <windows.h>
#include <cassert>
#include "ComPort.hpp"

#define MAX_COM_SIZE 8;

using namespace std;

void ComPort::GetConnection() {
    hSerial = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            cout << "serial port does not exist.\n";
           // assert(GetLastError() != ERROR_FILE_NOT_FOUND);
        }
        cout << "some other error occurred." << endl << " Error: " << GetLastError();
        //assert(hSerial != INVALID_HANDLE_VALUE);
    }
    else {
        cout << "Connected" << endl;
    }
    SetConnectionParams();
}

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

void ComPort::ClosePort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

void ComPort::GiveCommand(char* com) {
    //DWORD dwSize = sizeof(com);  
    DWORD dwSize = MAX_COM_SIZE;
    DWORD dwBytesWritten;

    BOOL iRet = WriteFile(hSerial, com, dwSize, &dwBytesWritten, NULL);
    if (!iRet) {
        cout << "Error in giving command" << endl;
    }
}

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

int ComPort::ReadOneAngle() {
    DWORD iSize;
    char sReceivedChar;
    char data[4];
    int i = 0;
    while (true) {
        ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
        if (sReceivedChar == '\r') {
            //cout << "r ";
            continue;
        }
        if (sReceivedChar == '\n' ) {
            //cout << "n ";
            break;
        }
        if (iSize > 0 && sReceivedChar != '+' ) { // если что-то принято, выводим
           // cout << sReceivedChar;
            data[i++] = sReceivedChar;
        }
        else {
           // break;
        }
    }
    int a = ToAngle(data);
    return a;
}

int ConvertCharToInt(char x) {
    return (int)x - 48;
}

int ComPort::ToAngle(char* data) {
    if (data[1] == ' ') {
        int a = ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
        return  ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
    }
    if (data[1] != ' ') {
        return ConvertCharToInt(data[1]) * 100 + ConvertCharToInt(data[2]) * 10 + ConvertCharToInt(data[3]);
    }
}



void ComPort::TurnOnAngles(int azimuth, int elevation) {
    char* command = portCommand.TurnAnglesCommand(azimuth, elevation);
    if (command == nullptr) {
        return;
    }
    GiveCommand(command);
    free(command);
}

void ComPort::PrintCurrentAngles() {
    char* command = portCommand.GetAnglesCommand();
    //cout << endl << command[0]  << command[1] << endl;
    GiveCommand(command);
    ReadCOM();
}

int ComPort::GetAzimuth() {
    char command[] = "C";
    GiveCommand(command);
    return ReadOneAngle();
}

int ComPort::GetElevation() {
    char command[] = "B";
    GiveCommand(command);
    //ReadCOM();
    return ReadOneAngle();
}