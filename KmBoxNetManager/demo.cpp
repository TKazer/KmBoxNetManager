#include "KmBoxNetManager.h"

int main()
{
	std::cout << "Waiting for connect device..." << std::endl;

	if (KmBoxMgr.InitDevice("192.168.2.188", 43520, "EA735054") != 0)
	{
		std::cout << "KmBoxNet Initialize failed." << std::endl;
		goto END;
	}

	std::cout << "KmBoxNet Initialize succeed." << std::endl;

	KmBoxMgr.KeyBoard.StartMonitor(12345);

	while (true)
	{
		if (KmBoxMgr.KeyBoard.GetKeyState(KEY_HOME))
			std::cout << "Pressed [Home]." << std::endl;

		if (GetAsyncKeyState(VK_END) & 0x8000)
			break;

		if (GetAsyncKeyState(VK_F2) & 0x8000)
		{
			KmBoxMgr.Mouse.Move(50, 50);
			std::cout << "[Call] Move()" << std::endl;
		}
		
		if (GetAsyncKeyState(VK_F3) & 0x8000)
		{
			KmBoxMgr.Mouse.Move_Auto(50, 50, 200);
			std::cout << "[Call] Move_Auto()" << std::endl;
		}

		if (GetAsyncKeyState(VK_F4) & 0x8000)
		{
			KmBoxMgr.Mouse.Left(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			KmBoxMgr.Mouse.Left(false);
			std::cout << "[Call] Left()" << std::endl;
		}

		if (GetAsyncKeyState(VK_F5) & 0x8000)
		{
			KmBoxMgr.Mouse.Right(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			KmBoxMgr.Mouse.Right(false);
			std::cout << "[Call] Right()" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	KmBoxMgr.KeyBoard.EndMonitor();
END:
	system("pause");
	return 0;
}