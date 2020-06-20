#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "DataIOServer.h"
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define RECEIVE 0
#define SEND 1

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

enum messType { LOGIN = 5, LOGOUT = 8, TIM_NGUOI_CHOI, THACH_DAU, TRA_LOI_THACH_DAU, GUI_NUOC_DI };
enum code { SUCCESS = 0, INCORRECT_USER_NAME_OR_PASSWORD = 10, ACC_HAS_BLOCKED = 12, LOGGED_IN, NOT_LOGGED_IN, ALLREADY_LOGGED_IN };


struct Message {
	int messType; //opcode
	int code;	// success or false
	char userName[30];
	char passWord[30];
	char move[4];
	char opponent[60]; // name of opponent
};

void handleLogin(Message *mess, SOCKET s, bool& state) {
	cout << "handling login" << endl;

	mess->code = INCORRECT_USER_NAME_OR_PASSWORD;

	for (int i = 0; i < listAcc.size(); i++) {
		if (state == 1) {
			mess->code = LOGGED_IN;
			break;
		}
		else if ((strcmp(listAcc[i].userName, mess->userName) == 0) && (strcmp(listAcc[i].password, mess->passWord) == 0) && (listAcc[i].isBlocked == 0)) {
			if (listAcc[i].sockNumber == 0) {
				mess->code = SUCCESS;
				state = true;
				listAcc[i].numberWrongPass = 0;
				listAcc[i].sockNumber = s;
				break;
			}
			else {
				mess->code = ALLREADY_LOGGED_IN;
				break;
			}
		}
		else if ((strcmp(listAcc[i].userName, mess->userName) == 0) && (strcmp(listAcc[i].password, mess->passWord) == 0) && (listAcc[i].isBlocked == 1)) {
			mess->code = ACC_HAS_BLOCKED;
			break;
		}
		else if ((strcmp(listAcc[i].userName, mess->userName) == 0) && (strcmp(listAcc[i].password, mess->passWord) != 0) && (listAcc[i].isBlocked == 0)) {
			listAcc[i].numberWrongPass++;
			if (listAcc[i].numberWrongPass == 3) listAcc[i].isBlocked = 1;
			break;
		}
	}

	cout << "password is " << mess->passWord << endl;
	cout << strcmp(mess->passWord, "admin") << endl;
	cout << "send " << mess->code << endl;
}

void handleLogout(Message *mess, SOCKET s, bool& state) {
	cout << "handling longout";
	if (state == true) {
		mess->code = SUCCESS;
		state = false;
		for (int i = 0; i < listAcc.size(); i++) {
			if (listAcc[i].sockNumber == s) listAcc[i].sockNumber = 0;
		}

	}
	else mess->code = NOT_LOGGED_IN;
	cout << "send " << mess->code << endl;
}
void xuTimNguoiChoi(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	DWORD transferredBytes = sizeof(Message);
	Message *mess = (Message*) perIoData->buffer;
	mess->opponent[0] = 0;
	for (account acc : listAcc) {
		if (acc.sockNumber != 0 && acc.ranh == 1) {
			strcat(mess->opponent, acc.userName);
			strcat(mess->opponent, " \0");
		}
	}
	ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
	perIoData->dataBuff.buf = perIoData->buffer + perIoData->sentBytes;
	perIoData->dataBuff.len = perIoData->recvBytes - perIoData->sentBytes;
	perIoData->operation = SEND;

	if (WSASend(perHandleData->socket,
		&(perIoData->dataBuff),
		1,
		&transferredBytes,
		0,
		&(perIoData->overlapped),
		NULL) == SOCKET_ERROR) {
		if (WSAGetLastError() != ERROR_IO_PENDING) {
			printf("WSASend() failed with error %d\n", WSAGetLastError());
		}
	}
}

void xuLyThachDau(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {

}
void xuLyTraLoiThachDau(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {

}
void xuLyNuocDi(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {

}


// handle message from client
//void handleMess(char *mess, SOCKET s,bool& state) {
//	Message *message = (Message*)mess;
//	switch (message->messType)
//	{
//	case LOGIN:
//		handleLogin(message, s,state);
//		break;
//	case LOGOUT:
//		handleLogout(message, s,state);
//		break;
//	default:
//		break;
//	}
//}
void handleMess(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	char *mess = perIoData->buffer;
	SOCKET s = perHandleData->socket;
	bool &state = perHandleData->state;
	Message *message = (Message*)mess;
	switch (message->messType)
	{
	case LOGIN:
		handleLogin(message, s, state);
		break;
	case LOGOUT:
		handleLogout(message, s, state);
		break;
	case TIM_NGUOI_CHOI:
		xuTimNguoiChoi(perIoData, perHandleData);
	case THACH_DAU:
		xuLyThachDau(perIoData, perHandleData);
	case TRA_LOI_THACH_DAU:
		xuLyTraLoiThachDau(perIoData, perHandleData);
	case GUI_NUOC_DI:
		xuLyNuocDi(perIoData, perHandleData);
	default:
		break;
	}
}