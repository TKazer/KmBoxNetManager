#pragma once
#define 	cmd_connect			0xaf3c2828 // ���Ӻ���
#define     cmd_mouse_move		0xaede7345 // ����ƶ�
#define		cmd_mouse_left		0x9823AE8D // ����������
#define		cmd_mouse_middle	0x97a3AE8D // ����м�����
#define		cmd_mouse_right		0x238d8212 // ����Ҽ�����
#define		cmd_mouse_wheel		0xffeead38 // �����ֿ���
#define     cmd_mouse_automove	0xaede7346 // ����Զ�ģ���˹��ƶ�����
#define     cmd_keyboard_all    0x123c2c2f // �������в�������
#define		cmd_reboot			0xaa8855aa // ��������
#define     cmd_bazerMove       0xa238455a // ��건�����ƶ�
#define     cmd_monitor         0x27388020 // ��غ����ϵ������������
#define     cmd_debug           0x27382021 // ����������Ϣ
#define     cmd_mask_mouse      0x23234343 // �����������
#define     cmd_unmask_all      0x23344343 // ��������������
#define     cmd_setconfig       0x1d3d3323 // ����IP������Ϣ
#define     cmd_setvidpid       0xffed3232 // ����device�˵�vidpid
#define     cmd_showpic         0x12334883 // ��ʾͼƬ

typedef struct
{
	unsigned int  mac;			//���ӵ�mac��ַ�����룩
	unsigned int  rand;			//���ֵ
	unsigned int  indexpts;		//ʱ���
	unsigned int  cmd;			//ָ����
}cmd_head_t;

typedef struct
{
	unsigned char buff[1024];
}cmd_data_t;
typedef struct
{
	unsigned short buff[512];
}cmd_u16_t;

// �������
typedef struct
{
	int button;
	int x;
	int y;
	int wheel;
	int point[10];
}soft_mouse_t;

// ��������
typedef struct
{
	char ctrl;
	char resvel;
	char button[10];
}soft_keyboard_t;

// �ͻ�������
typedef struct
{
	cmd_head_t head;
	union {
		cmd_data_t      u8buff;		  //buff
		cmd_u16_t       u16buff;	  //U16
		soft_mouse_t    cmd_mouse;    //��귢��ָ��
		soft_keyboard_t cmd_keyboard; //���̷���ָ��
	};
}client_data;

enum
{
	err_creat_socket = -9000,	//����socketʧ��
	err_net_version,			//socket�汾����
	err_net_tx,					//socket���ʹ���
	err_net_rx_timeout,			//socket���ճ�ʱ
	err_net_cmd,				//�������
	err_net_pts,				//ʱ�������
	success = 0,				//����ִ��
	usb_dev_tx_timeout,			//USB devic����ʧ��
};
