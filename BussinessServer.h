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



void handleLogin(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	
	Message *mess =(Message*) perIoData->buffer;
	SOCKET s = perHandleData->socket;
	bool &state = perHandleData->state;
	Message *message = (Message*)mess;

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
	sendMess(perIoData, perHandleData);
}

void handleLogout(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {

	Message *mess = (Message*)perIoData->buffer;
	SOCKET s = perHandleData->socket;
	bool &state = perHandleData->state;
	Message *message = (Message*)mess;

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
	sendMess(perIoData, perHandleData);
}
void xuLyTimNguoiChoi(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	Message *mess = (Message*) perIoData->buffer;
	mess->opponent[0] = 0;
	mess->code = SUCCESS;
	for (account acc : listAcc) {
		if (acc.sockNumber != 0 && acc.ranh == true) {
			strcat(mess->opponent, acc.userName);
			strcat(mess->opponent, " \0");
		}
	}
	sendMess(perIoData, perHandleData);
}

void xuLyThachDau(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	
	Message *mess = (Message*)perIoData->buffer;
	char name[30];

	//lay ten cua thang thach dau
	for (int i = 0; i < listAcc.size(); i++) {
		if (listAcc[i].sockNumber==perHandleData->socket) {
			strcpy(name, listAcc[i].userName);
		}
	}

	for (int i = 0; i < listAcc.size(); i++) {
		if (strcmp(listAcc[i].userName,mess->opponent)==0) {
			perHandleData->socket = listAcc[i].sockNumber;
			strcpy(mess->opponent, name);
			sendMess(perIoData, perHandleData);
			break;
		}
	}
}
void xuLyChoThachDau(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	for (int i = 0; i < listAcc.size(); i++) {
		if (listAcc[i].sockNumber == perHandleData->socket) {
			listAcc[i].ranh = true;
		}
	}
}

void xuLyTraLoiThachDau(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	Message *mess = (Message*)perIoData->buffer;
	if (mess->code == ACCEPT) {
		mess->code = SUCCESS;
		mess->color = 1;
		sendMess(perIoData, perHandleData);

		for (int i = 0; i < listAcc.size(); i++) {
			if (strcmp(listAcc[i].userName, mess->opponent) == 0) {
				perHandleData->socket = listAcc[i].sockNumber;
				mess->color = -1;
				sendMess(perIoData, perHandleData);
				break;
			}
		}

	}
}
void xuLyNuocDi(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	Message *mess = (Message*)perIoData->buffer;
	cout << "\nnhan duoc nuoc di: " << mess->move << endl;
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
		handleLogin(perIoData, perHandleData);
		break;
	case LOGOUT:
		handleLogout(perIoData, perHandleData);
		break;
	case TIM_NGUOI_CHOI:
		xuLyTimNguoiChoi(perIoData, perHandleData);
		break;
	case CHO_THACH_DAU:
		xuLyChoThachDau(perIoData, perHandleData);
		break;
	case THACH_DAU:
		xuLyThachDau(perIoData, perHandleData);
		break;
	case TRA_LOI_THACH_DAU:
		xuLyTraLoiThachDau(perIoData, perHandleData);
		break;
	case GUI_NUOC_DI:
		xuLyNuocDi(perIoData, perHandleData);
		break;
	default:
		break;
	}
}