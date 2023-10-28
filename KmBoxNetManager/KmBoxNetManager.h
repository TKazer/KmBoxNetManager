#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <math.h>
#include <random>
#include <string>
#include <thread>
#include "KmBoxConfig.h"
#pragma comment(lib,"ws2_32.lib")


class KmBoxMouse
{
public:
	soft_mouse_t MouseData{};
public:
	// �ƶ�
	int Move(int x, int y);
	// �Զ��켣�ƶ�
	int Move_Auto(int x, int y, int Runtime);
	// ���
	int Left(bool Down);
	// �Ҽ�
	int Right(bool Down);
	// �м�
	int Middle(bool Down);
};

class KmBoxNetManager
{
private:
	SOCKADDR_IN AddrServer;
	SOCKET s_Client = 0;
	client_data ReceiveData;
	client_data PostData;
private:
	int NetHandler();
	int SendData(int DataLength);
public:
	~KmBoxNetManager();
	// ��ʼ���豸
	int InitDevice(const std::string& IP, WORD Port, const std::string& Mac);
	// �����豸
	int RebootDevice();
	// �����豸s
	int SetConfig(const std::string& IP, WORD Port);
public:
	friend class KmBoxMouse;
	KmBoxMouse Mouse;
};

inline KmBoxNetManager KmBoxMgr;