//#include<iostream>
//#include<sys/mman.h>
//using namespace std;
//
//extern "C"
//{
//	#include<stdio.h>
//	#include<sys/stat.h>
//	#include<sys/types.h>
//	#include<fcntl.h>
//	#include<unistd.h>
//	#include<linux/input.h>
//}
//class LCD
//{
//public:
//	LCD()
//	{
//		fd = open("/dev/fb0", O_RDWR);//��lcd��
//		if (fd == -1)
//		{
//			cerr << "lcdopen fail!" << endl;
//		}
//		lcd_p = (unsigned int*)mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//	}
//	//��ʾbmpͼƬ
//	void show_bmp(int x,int y,const char* pic)
//	{
//		//bmp��ÿ��������ɶ�������4���ֽ�
//		unsigned char buf[4];
//		int fd1 = open(pic, O_RDONLY);
//		//�ж��Ƿ�Ϊbmp�ļ�
//		lseek(fd1, 0, SEEK_SET);
//		read(fd1, buf, 2);
//		if (buf[0] != 0x42 || buf[1] != 0x4D)
//		{
//			cerr << "this picture is not bmp" << endl;
//		}
//		//��λ�� 4�ֽ�
//		int bmp_w = 0;
//		lseek(fd1, 0x12, SEEK_SET);
//		read(fd1, &bmp_w, 4);
//		//��λ�� 4�ֽ�
//		int bmp_h = 0;
//		lseek(fd1, 0x16, SEEK_SET);
//		read(fd1, &bmp_h, 4);
//		//��ɫ�� 2�ֽ�
//		int bmp_colordepth = 0;
//		lseek(fd1, 0x1C, SEEK_SET);
//		read(fd1, &bmp_colordepth, 2);
//		printf("bmp:%ld %ld %ld", bmp_w, bmp_h, bmp_colordepth);
//		//������������ݣ���ͨ�����㺯������
//		lseek(fd1, 54, SEEK_SET);
//		int i, j;
//		//��������ۣ�
//		// ���ɫ��Ϊ32
//		//��Ϊbmp���ص��¼����Ϊ���̺�������Ҫ����λ����
//		char bmp_R, bmp_B, bmp_A, bmp_G;
//		for (i = bmp_h - 1; i >= 0; i--)
//		{
//			for (j = 0; j < bmp_w; j++)
//			{
//				int color = 0;
//				if (bmp_colordepth == 32)
//				{
//					//���������Զ��ƶ���ĩβλ��
//					//read(fd1, &color, bmp_colordepth / 8);
//					read(fd1, &bmp_B, 1);
//					read(fd1, &bmp_G, 1);
//					read(fd1, &bmp_R, 1);
//					read(fd1, &bmp_A, 1);
//					color = bmp_A << 24 | bmp_R << 16 | bmp_G << 8 | bmp_B;
//					*(lcd_p + x + j + (y + i) * 800) = color;
//				}
//				else if (bmp_colordepth == 24)
//				{
//					read(fd1, &bmp_B, 1);
//					read(fd1, &bmp_G, 1);
//					read(fd1, &bmp_R, 1);
//					bmp_A = 0;
//					color = bmp_A << 24 | bmp_R << 16 | bmp_G << 8 | bmp_B;
//					*(lcd_p + x + j + (y + i) * 800) = color;
//				}
//			}
//			lseek(fd1, (4 - bmp_colordepth / 8 * bmp_w % 4) % 4, SEEK_CUR);
//		}
//		close(fd1);
//	}
//	//�����νӿ�
//	void draw_juxing(int color, int w, int h)
//	{
//		for (int y = 0; y < h; y++)
//		{
//			for (int x = 0; x < w; x++)
//			{
//				*(lcd_p + x + y * 800) = color;
//			}
//		}
//	}
//	~LCD()
//	{
//		close(fd);
//		munmap(lcd_p, 800 * 480 * 4);
//	}
//private:
//	int fd;//�ļ�������
//	unsigned int* lcd_p;
//};
//
//class touch
//{
//public:
//	touch()
//	{
//		fd = open("/dev/input/event0", O_RDWR);
//	}
//	//��Ļ����¼�������������x��y����
//	void get_coordinate(int &x, int &y)
//	{
//		struct input_event et;
//		while (1)
//		{
//			int r = read(fd, &et, sizeof(et));
//			if (r == sizeof(et))
//			{
//				if (et.type == EV_ABS && et.code == ABS_X)//��ȡx������
//				{
//					x = et.value * (1.0 * 800 / 1024);
//				}
//				if (et.type == EV_ABS && et.code == ABS_Y)//��ȡy������
//				{
//					y = et.value * (1.0 * 480 / 600);
//				}
//
//				if (et.type == EV_KEY && et.code == BTN_TOUCH)//��ָ�뿪��Ļ
//				{
//					if (et.value == 0)
//					{
//						break;
//					}
//
//				}
//			}
//		}
//	}
//	~touch()
//	{
//		close(fd);
//	}
//private:
//	int fd;
//};
//
//char picpath[10][50] = { "1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp"};
//int main()
//{
//	touch a;
//	LCD b;
//	int i = 0, x = 0, y = 0;
//	while (1)
//	{
//		a.get_coordinate(x, y);
//		if (x > 400)
//		{
//			i++;
//			if (i >= 6)
//			{
//				i = 0;
//			}
//			b.show_bmp(0,0,picpath[i]);
//		}
//		else if (x < 400)
//		{
//			i--;
//			if (i < 0)
//			{
//				i = 5;
//			}
//			b.show_bmp(0,0,picpath[i]);
//		}
//	}
//}
//
//