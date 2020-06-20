#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <fstream>
#include <vector>
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define DATA_BUFSIZE 8192

typedef struct PerHandleData {
	SOCKET socket;
	bool state;
	int **board;
	PerHandleData * opponent; // thong tin cua doi thu
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

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

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

struct account {
	char userName[30];
	char password[30];
	int isBlocked;
	int numberWrongPass=0;
	int sockNumber =0;
	bool ranh = true;
	PerHandleData * perHandleData;
};
 
vector<account> listAcc;
// get account info from file 
void getAccInfo() {
	fstream myFile("account.txt");
	while (!myFile.eof()) {
		account accTemp;
		myFile >> accTemp.userName >> accTemp.password >> accTemp.isBlocked;
		if (strlen(accTemp.userName) == 0) break;
		listAcc.push_back(accTemp);
	}
	myFile.close();
}
// update account info 
void updateAccInfo() {
	fstream myFile("account.txt");
	myFile.clear();
	myFile.seekp(0);
	for (int i = 0; i < listAcc.size(); i++) {
		myFile << listAcc[i].userName << " " << listAcc[i].password << " " << listAcc[i].isBlocked << endl;
	}
	listAcc.clear();
	myFile.close();
}
// wrapped of send 
int sendMessage(SOCKET s, char*buff, int len) {

	int nLeft, idx, ret;
	nLeft = len;
	idx = 0;

	while (nLeft > 0) {
		ret = send(s, &buff[idx], nLeft, 0);
		if (ret == SOCKET_ERROR) {

		}
		nLeft -= ret;
		idx += ret;
	}
	return len;
}
// wrapped of recv
int recvMessage(SOCKET s, char*buff, int len) {
	int ret, nLeft = len, idx;
	idx = 0;

	while (nLeft > 0)
	{
		ret = recv(s, &buff[idx], nLeft, 0);
		if (ret == SOCKET_ERROR) {
			return  SOCKET_ERROR;
		}
		nLeft -= ret;
		idx += ret;
	}
	return len;
}