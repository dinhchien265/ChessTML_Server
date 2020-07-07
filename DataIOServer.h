#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define DATA_BUFSIZE 8192
#define RECEIVE 0
#define SEND 1

#pragma comment(lib, "Ws2_32.lib")


enum messType { LOGIN = 5, LOGOUT = 8, FIND_OPPONENT, CHALLENGER, WAIT_CHALLENGER, REP_CHALLENGER, SEND_MOVE, ENDGAME, SURRENDER,RANK };
enum code { SUCCESS = 1, INCORRECT_USER_NAME_OR_PASSWORD = 10, ACC_HAS_BLOCKED = 12, LOGGED_IN, NOT_LOGGED_IN, ALLREADY_LOGGED_IN, ACCEPT, REFUSE, WIN, LOSE };


struct Message {
	int messType; //opcode
	int code;	// success or false
	char userName[30];
	char passWord[30];
	char move[5];
	char data[200]; // name of opponent
	int color;
};



// Structure definition
typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF dataBuff;
	CHAR buffer[DATA_BUFSIZE];
	int bufLen;
	int recvBytes;
	int sentBytes;
	int operation;
} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct PerHandleData {
	SOCKET socket;
	bool state;
	LPPER_IO_OPERATION_DATA perIoData;
	char history[200];
	int **board;
	int turn;
	int color;
	int n = 0;
	PerHandleData * opponent; 
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

struct account {
	char userName[30];
	char password[30];
	int isBlocked;
	int score;
	int numberWrongPass = 0;
	int sockNumber = 0;
	bool ranh = false;
	PerHandleData * perHandleData;
};

vector<account> listAcc;

int compare(const void* a, const void* b) {
	account *acc1, *acc2;
	acc1 = (account*)a;
	acc2 = (account*)b;
	return acc1->score - acc2->score;
}

// get account info from file 
void getAccInfo() {
	fstream myFile("account.txt");
	while (!myFile.eof()) {
		account accTemp;
		myFile >> accTemp.userName >> accTemp.password >> accTemp.isBlocked >> accTemp.score;
		if (strlen(accTemp.userName) == 0) break;
		listAcc.push_back(accTemp);
	}
	myFile.close();
	for (int i = 0; i < listAcc.size(); i++) {
		for (int j = 1; j < listAcc.size(); j++) {
			if (listAcc[j - 1].score < listAcc[j].score) {
				account temp = listAcc[j - 1];
				listAcc[j - 1] = listAcc[j];
				listAcc[j] = temp;
			}
		}
	}
}
// update account info 
void updateAccInfo() {
	for (int i = 0; i < listAcc.size(); i++) {
		for (int j = 1; j < listAcc.size(); j++) {
			if (listAcc[j - 1].score < listAcc[j].score) {
				account temp = listAcc[j - 1];
				listAcc[j - 1] = listAcc[j];
				listAcc[j] = temp;
			}
		}
	}
	fstream myFile("account.txt");
	myFile.clear();
	myFile.seekp(0);
	for (int i = 0; i < listAcc.size(); i++) {
		myFile << listAcc[i].userName << " " << listAcc[i].password << " " << listAcc[i].isBlocked << " " << listAcc[i].score << endl;
	}
	myFile.close();
}


void sendMess(LPPER_IO_OPERATION_DATA perIoData, SOCKET s) {
	DWORD transferredBytes = sizeof(Message);
	ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
	perIoData->dataBuff.buf = perIoData->buffer + perIoData->sentBytes;
	perIoData->dataBuff.len = perIoData->recvBytes - perIoData->sentBytes;
	perIoData->operation = SEND;

	if (WSASend(s,
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
void recvMess(LPPER_IO_OPERATION_DATA perIoData, LPPER_HANDLE_DATA perHandleData) {
	DWORD flags;
	DWORD transferredBytes;
	perIoData->recvBytes = 0;
	perIoData->operation = RECEIVE;
	flags = 0;
	ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
	perIoData->dataBuff.len = DATA_BUFSIZE;
	perIoData->dataBuff.buf = perIoData->buffer;
	printf("\nGoi ham wsarecv cho socket: %d", perHandleData->socket);
	if (WSARecv(perHandleData->socket,
		&(perIoData->dataBuff),
		1,
		&transferredBytes,
		&flags,
		&(perIoData->overlapped), NULL) == SOCKET_ERROR) {
		if (WSAGetLastError() != ERROR_IO_PENDING) {
			printf("WSARecv() failed with error %d\n", WSAGetLastError());
		}
	}
}