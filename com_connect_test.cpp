//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//#include <windows.h>
//#pragma warning(disable : 4996)
//
//using namespace std;
//using std::string;
//const int DATA_SIZE = 3;
//
//HANDLE hSerial;
//
//void ReadCOM() {
//    DWORD iSize;
//    char sReceivedChar;
//    while (true)
//    {
//        ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
//        if (sReceivedChar == '\n') {
//            break;
//        }
//        if (iSize > 0) { // если что-то принято, выводим
//            cout << sReceivedChar;
//        }
//        else {
//            break;
//        }
//    }
//    cout << endl;
//}
//
//
//int main() {
//
//	LPCTSTR portName = L"COM3";
//	hSerial = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//    if (hSerial == INVALID_HANDLE_VALUE) {
//        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
//            cout << "serial port does not exist.\n";
//        }
//        cout << "some other error occurred." << endl << " Error: " << GetLastError();
//    }
//    else {
//        cout << "Connected" << endl;
//    }
//    DCB dcbSerialParams = { 0 };
//    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
//    if (!GetCommState(hSerial, &dcbSerialParams)) {
//        cout << "getting state error\n";
//    }
//    dcbSerialParams.BaudRate = CBR_9600;
//    dcbSerialParams.ByteSize = 8;
//    dcbSerialParams.StopBits = ONESTOPBIT;
//    dcbSerialParams.Parity = NOPARITY;
//    if (!SetCommState(hSerial, &dcbSerialParams))
//    {
//        cout << "error setting serial port state\n";
//    }
//
//    //char data[] = "W030 030";  // строка для передачи
//    //string data_s = "W000 000";
//    //const int n = data_s.length();
//    //char data[DATA_SIZE];
//    //strcpy(data, data_s.c_str());
//    char* data = new char[8];
//    data[0] = 'W';
//    data[1] = '0';
//    data[2] = '0';
//    data[3] = '0';
//    data[4] = ' ';
//    data[5] = '0';
//    data[6] = '0';
//    data[7] = '0';
//    
//   // DWORD dwSize = sizeof(data);   // размер этой строки
//    DWORD dwSize = 8;
//    DWORD dwBytesWritten;
//
//    BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);
//
//  // // while (1) {
//    char get_data[] = "C2";  // строка для передачи
//    DWORD dwSize_get = sizeof(get_data);   // размер этой строки
//    DWORD dwBytesGetting;
//
//      iRet = WriteFile(hSerial, get_data, dwSize_get, &dwBytesGetting, NULL);
//        ReadCOM();
//  ////  }
//    if (hSerial != INVALID_HANDLE_VALUE) {
//        CloseHandle(hSerial);
//    }
//    return 0;
//    // don't forget to close the port
//}
//
//
//// close port connection
//// read data c, b ....
//// organaize class structure
//
////
////
////
////
////
////#include <iostream>
////#include <conio.h>
////#include <windows.h>
////
////using namespace std;
////
////void open_port(char* name);
////void check_port(char* name);
////void close_port();
////DWORD __stdcall ReadThread(LPVOID hwnd);
////
////HANDLE hCom = INVALID_HANDLE_VALUE, hThread;
////HWND hwnd;
////DCB dcb;
////LPCWSTR sPortName = L"COM3";
////int main()
////{
////    cout << "<=SERIAL PORT=>\n\n";
////
////
////    char sPortName[] = "COM3";
////    // сперва открываем порт
////    open_port(sPortName);
////
////    // проверяем какой он      
////    check_port(sPortName);
////
////    // в конце работы закрываем порт
////    close_port();
////
////
////    //getch();
////    return 0;
////}
////
////
////void open_port(char* name)
////{
////
////    hCom = CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
////
////    if (hCom == INVALID_HANDLE_VALUE)
////    {
////        cout << " *** Error opening port!\n";
////        //getch();
////        return;
////    }
////    else
////    {
////        cout << " *** Port succesfully opened!\n";
////
////        GetCommState(hCom, &dcb);
////
////        dcb.BaudRate = CBR_57600;
////        dcb.ByteSize = 8;
////        dcb.Parity = NOPARITY;
////        dcb.StopBits = ONESTOPBIT;
////
////        if (SetCommState(hCom, &dcb))
////            cout << " *** Configuring OK.\n\n";
////        else
////            cout << " *** Configuring error.\n\n";
////
////        hThread = CreateThread(0, 0, ReadThread, (LPVOID)hwnd, 0, 0);
////    }
////}
////
////
////void check_port(char* name)
////{
////    COMMCONFIG comm;
////    if (GetDefaultCommConfig(sPortName, &comm, &comm.dwSize))
////    {
////        switch (comm.dwProviderSubType)
////        {
////        case PST_FAX:
////            cout << "FAX device " << endl;
////            break;
////        case PST_LAT:
////            cout << "LAT protocol" << endl;
////            break;
////        case PST_MODEM:
////            cout << "Modem device " << endl;
////            break;
////        case PST_NETWORK_BRIDGE:
////            cout << "Unspecified network bridge " << endl;
////            break;
////        case PST_PARALLELPORT:
////            cout << "Parallel port " << endl;
////            break;
////        case PST_RS232:
////            cout << "RS-232 serial port " << endl;
////            break;
////        case PST_RS422:
////            cout << "RS-422 port " << endl;
////            break;
////        case PST_RS423:
////            cout << "RS-423 port " << endl;
////            break;
////        case PST_RS449:
////            cout << "RS-449 port " << endl;
////            break;
////        case PST_SCANNER:
////            cout << "Scanner device " << endl;
////            break;
////        case PST_TCPIP_TELNET:
////            cout << "TCP/IP TelnetR protocol " << endl;
////            break;
////        case PST_UNSPECIFIED:
////            cout << "Unspecified " << endl;
////            break;
////        case PST_X25:
////            cout << "X.25 standards " << endl;
////            break;
////        default:
////            cout << "?????? Microsft mail please ...." << endl;
////            break;
////        }
////    }
////    else
////        cout << "Error access" << endl;
////}
////
////
////void close_port()
////{
////    if (hCom == INVALID_HANDLE_VALUE)
////        return;
////
////    TerminateThread(hThread, 0);
////    CloseHandle(hCom);
////    CloseHandle(hThread);
////    cout << "\n *** Port succesfully closed!\n";
////
////    hCom = INVALID_HANDLE_VALUE;
////}
////
////
////DWORD __stdcall ReadThread(LPVOID hwnd)
////{
////    DWORD iSize;
////    char sReceivedChar;
////    while (true)
////    {
////        ReadFile(hCom, &sReceivedChar, 1, &iSize, 0);
////        // SendDlgItemMessage((HWND)hwnd, IDC_EDIT2, WM_CHAR, sReceivedChar, 0);
////        cout << sReceivedChar << "#\n";
////    }
////}