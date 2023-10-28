#include "KmBoxNetManager.h"

unsigned int StrToHex(const char* pbSrc, int nLen)
{
	char h1, h2;
	unsigned char s1, s2;
	int i;
	unsigned int pbDest[16] = { 0 };
	for (i = 0; i < nLen; i++) {
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];
		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;
		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;
		pbDest[i] = s1 * 16 + s2;
	}
	return pbDest[0] << 24 | pbDest[1] << 16 | pbDest[2] << 8 | pbDest[3];
}

KmBoxNetManager::~KmBoxNetManager()
{
	WSACleanup();
	if (this->s_Client != NULL)
	{
		closesocket(this->s_Client);
		this->s_Client = NULL;
	}
}

int KmBoxNetManager::InitDevice(const std::string& IP, WORD Port, const std::string& Mac)
{
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;	
	int Status = 0;

	Status = WSAStartup(wVersionRequested, &wsaData);
	if (Status != success)
		return err_creat_socket;
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		this->s_Client = -1;
		return err_net_version;
	}
	srand(time(NULL));

	this->s_Client = socket(AF_INET, SOCK_DGRAM, 0);

	this->AddrServer.sin_addr.S_un.S_addr = inet_addr(IP.c_str());
	this->AddrServer.sin_family = AF_INET;
	this->AddrServer.sin_port = htons(Port);

	this->PostData.head.mac = std::stoll(Mac, nullptr, 16);
	this->PostData.head.rand = rand();
	this->PostData.head.indexpts = 0;
	this->PostData.head.cmd = cmd_connect;

	Status = sendto(this->s_Client, reinterpret_cast<const char*>(&this->PostData), sizeof(cmd_head_t), 0,
		reinterpret_cast<sockaddr*>(&this->AddrServer), sizeof(this->AddrServer));
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	int FromLen = sizeof(this->AddrServer);

	Status = recvfrom(this->s_Client, reinterpret_cast<char*>(&this->ReceiveData), 1024, 0,
		reinterpret_cast<sockaddr*>(&this->AddrServer), &FromLen);
	if (Status < 0)
		return err_net_rx_timeout;

	return NetHandler();
}

int KmBoxNetManager::SendData(int DataLength)
{
	int Status;
	sendto(this->s_Client, reinterpret_cast<const char*>(&this->PostData), DataLength, 0,
		reinterpret_cast<sockaddr*>(&this->AddrServer), sizeof(this->AddrServer));

	SOCKADDR_IN s_NewClient;
	int FromLen = sizeof(s_NewClient);

	Status = recvfrom(this->s_Client, reinterpret_cast<char*>(&this->ReceiveData), 1024, 0,
		reinterpret_cast<sockaddr*>(&s_NewClient), &FromLen);

	if (Status < 0)
		return err_net_rx_timeout;

	return NetHandler();
}

int KmBoxNetManager::RebootDevice()
{
	if (this->s_Client <= 0)
		return err_creat_socket;
	
	int Status = 0;

	this->PostData.head.indexpts++;
	this->PostData.head.cmd = cmd_reboot;
	this->PostData.head.rand = rand();

	int Length = sizeof(cmd_head_t);

	this->SendData(Length);
	
	WSACleanup();
	this->s_Client = -1;

	if (Status < 0)
		return err_net_rx_timeout;
	
	return NetHandler();
}

int KmBoxNetManager::SetConfig(const std::string& IP, WORD Port)
{
	if (this->s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	this->PostData.head.indexpts++;
	this->PostData.head.cmd = cmd_setconfig;
	this->PostData.head.rand = inet_addr(IP.c_str());
	this->PostData.u8buff.buff[0] = Port >> 8;
	this->PostData.u8buff.buff[1] = Port >> 0;

	int Length = sizeof(cmd_head_t) + 2;

	return this->SendData(Length);
}

int KmBoxNetManager::NetHandler()
{
	if (ReceiveData.head.cmd != PostData.head.cmd)
		return  err_net_cmd;
	if (ReceiveData.head.indexpts != PostData.head.indexpts)
		return  err_net_pts;
	return 0;
}

int KmBoxMouse::Move(int x, int y)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_move;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.x = x;
	this->MouseData.y = y;

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	this->MouseData.x = 0;
	this->MouseData.y = 0;

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Move_Auto(int x, int y, int Runtime)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_automove;
	KmBoxMgr.PostData.head.rand = Runtime;

	this->MouseData.x = x;
	this->MouseData.y = y;

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	this->MouseData.x = 0;
	this->MouseData.y = 0;

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Left(bool Down)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_left;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.button = (Down ? (this->MouseData.button | 0x01) : (this->MouseData.button & (~0x01)));

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Right(bool Down)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_right;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.button = (Down ? (this->MouseData.button | 0x02) : (this->MouseData.button & (~0x02)));

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Middle(bool Down)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_middle;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.button = (Down ? (this->MouseData.button | 0x04) : (this->MouseData.button & (~0x04)));

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	return KmBoxMgr.SendData(Length);
}