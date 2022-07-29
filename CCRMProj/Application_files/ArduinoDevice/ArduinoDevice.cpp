#include "stdafx.h"
//#include <iostream>
//#include <string>
//#include <stdlib.h>
//
//#include "SerialPort.h"
//
//
//using namespace std;
//
//
//class Arduino {
//
// public:
//
//	   char *port = "\\\\.\\COM3";
//	   char output[MAX_DATA_LENGTH];
//	   char incomingData[MAX_DATA_LENGTH];
//
//	    Arduino() {
//		  
//			 SerialPort arduino(port);
//			 if (arduino.isConnected()) {
//				   cout << "connected";
//			 }
//			 else {
//				   cout << "Error in port name" << endl << endl;
//			 }
//
//			 while (arduino.isConnected()) {
//				   cout << "Enter your command: " << endl;
//				   string data;
//				   cin >> data;
//
//				   char *charArray = new char[data.size() + 1];
//				   copy(data.begin(), data.end(), charArray);
//				   charArray[data.size()] = '\n';
//
//				   arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);
//				   arduino.readSerialPort(output, MAX_DATA_LENGTH);
//
//				   cout << ">> " << output << endl;
//
//				   delete[] charArray;
//			 }
//	
//	   };
//
//
//};