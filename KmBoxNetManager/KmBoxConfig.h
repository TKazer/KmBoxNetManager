#pragma once
#define 	cmd_connect			0xaf3c2828 // 连接盒子
#define     cmd_mouse_move		0xaede7345 // 鼠标移动
#define		cmd_mouse_left		0x9823AE8D // 鼠标左键控制
#define		cmd_mouse_middle	0x97a3AE8D // 鼠标中键控制
#define		cmd_mouse_right		0x238d8212 // 鼠标右键控制
#define		cmd_mouse_wheel		0xffeead38 // 鼠标滚轮控制
#define     cmd_mouse_automove	0xaede7346 // 鼠标自动模拟人工移动控制
#define     cmd_keyboard_all    0x123c2c2f // 键盘所有参数控制
#define		cmd_reboot			0xaa8855aa // 盒子重启
#define     cmd_bazerMove       0xa238455a // 鼠标贝塞尔移动
#define     cmd_monitor         0x27388020 // 监控盒子上的物理键鼠数据
#define     cmd_debug           0x27382021 // 开启调试信息
#define     cmd_mask_mouse      0x23234343 // 屏蔽物理键鼠
#define     cmd_unmask_all      0x23344343 // 解除屏蔽物理键鼠
#define     cmd_setconfig       0x1d3d3323 // 设置IP配置信息
#define     cmd_setvidpid       0xffed3232 // 设置device端的vidpid
#define     cmd_showpic         0x12334883 // 显示图片

typedef struct
{
	unsigned int  mac;			//盒子的mac地址（必须）
	unsigned int  rand;			//随机值
	unsigned int  indexpts;		//时间戳
	unsigned int  cmd;			//指令码
}cmd_head_t;

typedef struct
{
	unsigned char buff[1024];
}cmd_data_t;
typedef struct
{
	unsigned short buff[512];
}cmd_u16_t;

// 鼠标数据
typedef struct
{
	int button;
	int x;
	int y;
	int wheel;
	int point[10];
}soft_mouse_t;

// 键盘数据
typedef struct
{
	char ctrl;
	char resvel;
	char button[10];
}soft_keyboard_t;

// 客户端数据
typedef struct
{
	cmd_head_t head;
	union {
		cmd_data_t      u8buff;		  //buff
		cmd_u16_t       u16buff;	  //U16
		soft_mouse_t    cmd_mouse;    //鼠标发送指令
		soft_keyboard_t cmd_keyboard; //键盘发送指令
	};
}client_data;

enum
{
	err_creat_socket = -9000,	//创建socket失败
	err_net_version,			//socket版本错误
	err_net_tx,					//socket发送错误
	err_net_rx_timeout,			//socket接收超时
	err_net_cmd,				//命令错误
	err_net_pts,				//时间戳错误
	success = 0,				//正常执行
	usb_dev_tx_timeout,			//USB devic发送失败
};
