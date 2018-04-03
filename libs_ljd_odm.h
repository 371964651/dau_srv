#ifndef LIBEM_MHKG_L_
#define LIBEM_MHKG_L_
/*************************************************  
* File name   	: Common.h  
* Description 	: 头文件及全局信息定义
* Author      	: lemon
* Version     	: V1.0  
* Date        	:   
* Compiler    	: powerpc-openwrt-linux-gnuspe-gcc  
* History     	: 2018-01-10  

*************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>   
#include <sys/time.h>

#include <math.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <malloc.h>
#include <sys/file.h>
#include <ctype.h>
#include <sys/socket.h>
#include <dirent.h>
#include <termios.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <linux/rtc.h>
#include <sys/wait.h>
#include <memory.h>
#include <linux/spi/spidev.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/select.h>
#include <net/if.h> 
#include <net/if_arp.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdbool.h>
#include <iconv.h>
#include <math.h>
#define ERROR_WRITE	         -10	//写数据进入tts出错

#define ERROR_SIM_NO_CARRIER	-9		//对方未接电话

#define ERROR_SIM_RECONGNOTION	-8		//SIM卡未识别
#define ERROR_SIM_SIGNAL		-7		//信号弱/没有插入天线 
#define ERROR_SIM_MODEM 	    -6		//通信模块故障
#define ERROR_NOSUPPORT	        -5		//当前不支持该功能
#define ERROR_SYS		        -4		//系统API返回错误（系统调用返回错误）
#define ERROR_INPARA	        -3		//参数错误（如参数不合法、缓冲区过小等）
#define ERROR_TIMEOUT	        -2		//操作超时
#define ERROR_FAIL		        -1		//其他错误，原因不明
#define ERROR_OK		         0		//操作成功
//#define DEBUG          			 1


//#define DEBUG                      1
#ifdef DEBUG
#define TRACE(str, args...)	printf(str, ## args)
#else
#define TRACE(str, ...) //定义该宏为空语句，即不输出调试信息
#endif

#define DEFCFGFORCOM      "115200,8,1,N,0"
#define	DEFDEV_LCD		  "/dev/ttyO1"
#define SERIAL_SIM        "/dev/ttyUSB2"
#define SERIAL_TTS        "/dev/ttyO3"
#define SERIAL_TTS_LANDLINE        "/dev/ttyO4"


#define DIODEV	"/dev/DIO0"
#define 	 MAX_LEN_OF_SHORT_MESSAGE 140

/******   ioctl cmd   *********/
#define	DIGITAL_IOCTL_BASE			'D'
#define	SET_IOOUT_VAL				_IOWR(DIGITAL_IOCTL_BASE, 1, int)
#define	GET_IOOUT_VAL				_IOWR(DIGITAL_IOCTL_BASE, 2, int)
#define	GET_IOOUT_CFG				_IOWR(DIGITAL_IOCTL_BASE, 3, int)
#define	GET_IOIN_VAL				_IOWR(DIGITAL_IOCTL_BASE, 4, int)
#define	SET_IOIN_NORMODE			_IOWR(DIGITAL_IOCTL_BASE, 5, int)
#define	SET_IOIN_PULSEMODE			_IOWR(DIGITAL_IOCTL_BASE, 6, int)
#define	GET_IOEVENT_NO				_IOWR(DIGITAL_IOCTL_BASE, 7, int)
#define	GET_IOIN_EVENTCFG			_IOWR(DIGITAL_IOCTL_BASE, 8, int)
#define	GET_IOEVENT_GROUP			_IOWR(DIGITAL_IOCTL_BASE, 9, int)
#define	SET_IOEVENT_CLEAN			_IOWR(DIGITAL_IOCTL_BASE, 10, int)
#define COM_BAUDDEF		(115200)
#define	COM_DABITSDEF	(8)
#define COM_STBITSDEF	(1)
#define COM_PARITYDEF	('N')
#define COM_RTSCTSDEF	(0)

typedef struct _outio_des{	
	int gpio;
	int iostat;
} outio_des, *outio_des_t;

#ifdef __cplusplus
extern "C" {
#endif

//*------------------------------------------------------------------------------------------------
//* 函数名称:  OpenCom
//* 功能描述:  打开并初始化串口
//* 入口参数:
//*           -- dev:      - 串口设备名，如/dev/ttyS1
//*           -- configs:  - 初始化通讯参数字符串:波特率,数据位，停止位，校验位，
//*                          如:   "115200,8,1,N"，若为NULL，则使用默认参数("115200,8,1,N")
//* 返回值    :  成功返回文件描述符；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------
extern int OpenCom(char *dev,char *configs);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  ReadCom
//* 功能描述:  读串口数据
//* 入口参数:
//*           -- fd:         - 指定串口设备的句柄(即OpenCom()的返回值)
//*           -- buffer:     - 读取的数据存放缓冲区
//*           -- len:        - 期望读取的数据长度(指针)
//*           -- timesoutus: - 最大的阻塞读取等待间隔，单位: useconds(微秒)
//*                            若timesoutus<=0，阻塞读模式，即直到读够指定数据个数后函数返回.
//* 返回值    :  成功返回 实际读取数据的长度；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  1、分配采样值存放缓冲区时，一定要足够存放指定的数据个数，否则可能造成缓冲区溢出；
//*           2、当置为阻塞读模式时，指定的读数据个数需要恰当，否则若没有相应个数数据，则进程会阻塞；
//*           3、成功读取数据大小，
//*------------------------------------------------------------------------------------------------
extern int com_read(int fd, char *buffer, unsigned int len, int timesoutus);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  WriteCom
//* 功能描述:  向串口写数据
//* 入口参数:
//*           -- fd:         - 指定串口设备的句柄(即OpenCom()的返回值)
//*           -- buffer:     - 写入数据存放缓冲区
//*           -- len:        - 写入的数据长度(指针)
//*           -- timesoutus: - 最大的阻塞读取等待间隔，单位: useconds(微秒)
//*                            若timesoutus<=0，阻塞读模式.
//* 返回值    :  成功返回实际写的数据大小；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  1、当置为阻塞写模式时，指定的数据个数需要恰当，否则若设备当前不可写的话，可能会造成进程阻塞;
//*          		2、实际写的数据大小
//*------------------------------------------------------------------------------------------------
extern int com_write(int fd, char *buffer, unsigned int len, int timesoutus);

//*------------------------------------------------------------------------------------------------
//* 函数名称:  CloseCom
//* 功能描述:  关闭串口设备
//* 入口参数:
//*           -- fd:  - 指定串口设备的句柄(即OpenCom()的返回值)
//*
//* 返回值    :  
//* 备注         :  无
//*------------------------------------------------------------------------------------------------
extern void CloseCom(int fd);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  init_dio
//* 功能描述:  打开并初始化di,do
//* 入口参数:
//*          
//* 返回值    :  成功返回文件描述符；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int init_dio();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  do_readall
//* 功能描述:  读取所有为电平输出类型IO的当前输出状态
//* 入口参数:
//*			  -- fd  :     设备描述符
//*           -- ch_num:   DO端口数目
//*           -- value  :  值得缓存区
//*
//* 返回值    :  成功返回当期io口的值；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int do_readall(int fd,int *value, int ch_num);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Uninit_dio
//* 功能描述:  卸载dio
//* 入口参数:
//*           -- fd:dio的设备描述符
//*           --
//*                         
//* 返回值    :  ；
//* 备注         :  无
//*------------------------------------------------------------------------------------------------
extern void Uninit_dio(int fd);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Init_Lcd
//* 功能描述:  初始化屏
//* 入口参数:
//*           -- 
//*           --
//*                         
//* 返回值    :  成功返回lcd_Fd:lcd的设备描述符；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int Init_Lcd();

//*------------------------------------------------------------------------------------------------
//* 函数名称:  Clear_Lcd
//* 功能描述:  清屏
//* 入口参数:
//*           -- lcd_Fd:lcd的设备描述符
//*           --
//*                         
//* 返回值    :  成功返回ERROR_OK(==0)；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int Clear_Lcd(int lcd_fd);

//*------------------------------------------------------------------------------------------------
//* 函数名称:  show_something_in_first_line
//* 功能描述:  在第一行显示
//* 入口参数:
//*           -- lcd_Fd:lcd的设备描述符
//*           --str:显示的内容
//*                         
//* 返回值    :  成功返回ERROR_OK(==0)；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int  show_something_in_first_line(int lcd_fd, char *str);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  show_something_in_second_line
//* 功能描述:  在第二行显示
//* 入口参数:
//*           -- lcd_Fd:lcd的设备描述符
//*           --str:显示的内容
//*                         
//* 返回值    :  成功返回ERROR_OK(==0)；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int show_something_in_second_line(int lcd_fd,char *str);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Uninit_Lcd
//* 功能描述:  关闭Lcd描述符
//* 入口参数:
//*           -- lcd_Fd:lcd的设备描述符
//*           --
//*                         
//* 返回值    :  
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern void Uninit_Lcd(int lcd_fd);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Init_Key
//* 功能描述:  初始化按键
//* 入口参数:
//*           -- 
//*           --
//*                         
//* 返回值    :  
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern void Init_Key();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Cheak_Forward_Key
//* 功能描述:  检测前进键是否按下
//* 入口参数:
//*           -- 
//*           --
//*                         
//* 返回值    :  1:按下，0:没按下
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int  Cheak_Forward_Key();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Cheak_Back_Key
//* 功能描述:  检测后退键是否按下
//* 入口参数:
//*           -- 
//*           --
//*                         
//* 返回值    :  1:按下，0:没按下
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int Cheak_Back_Key();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Uninit_Key
//* 功能描述:  
//* 入口参数:
//*           --
//*           --
//*                         
//* 返回值    :  
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern void Uninit_Key();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  init_sim_fd
//* 功能描述:  初始化sim
//* 入口参数:
//*           --
//*           --
//*                         
//* 返回值    :  成功返回文件描述符；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int init_sim_fd();

//*------------------------------------------------------------------------------------------------
//* 函数名称:  gsm_ttsdial
//* 功能描述:  检测后退键是否按下
//* 入口参数:
//*			  --sim_fd:文件描述符
//*           -- telno:电话号码
//*           --content:内容
//*           --  timeout:超时时间          
//* 返回值    : 成功返回ERROR_OK(==0)；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int gsm_ttsdial(int sim_fd,char *telno,char *content,int timeout);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  gsm_writesms_chinese
//* 功能描述:  发送中文短信
//* 入口参数:
//*			  --sim_fd:文件描述符
//*           -- telno:电话号码
//*           -- centerno:短信中心号码，可以查到
//*           --content:内容
//*           --  timeout:超时时间          
//* 返回值    : 成功返回ERROR_OK(==0)；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int gsm_writesms_chinese(int sim_fd,char * telno,char * content);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  gsm_writesms_chinese
//* 功能描述:  发送英文短信
//* 入口参数:
//*			  --sim_fd:文件描述符
//*           -- telno:电话号码
//*
//*           --content:内容
//*           --  timeout:超时时间          
//* 返回值    : 成功返回ERROR_OK(==0)；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------


extern int gsm_writesms_english(int sim_fd,char * telno,char * content);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  close_sim_fd
//* 功能描述:  关闭SIM卡的描述符
//* 入口参数:
//*           --
//*           --
//*                         
//* 返回值    :  
//* 备注         :  无
//*------------------------------------------------------------------------------------------------


extern void close_sim_fd(int sim_fd);
extern char *gsm_readsms(int sim_fd, int timeout);

//*------------------------------------------------------------------------------------------------
//* 函数名称:  init_dio
//* 功能描述:  打开并初始化di,do
//* 入口参数:
//*          
//* 返回值    :  成功返回文件描述符；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :  无
//*------------------------------------------------------------------------------------------------

extern int init_dio();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  do_read
//* 功能描述:  读取为电平输出类型IO的当前输出状态
//* 入口参数:
//*			  -- fd  :     设备描述符
//*           -- channel:     - IO端口号
//*           -- 
//*
//* 返回值    :  成功返回当期io口的值；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :   无
//*------------------------------------------------------------------------------------------------
extern int do_read(int fd,int channel);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  do_readall
//* 功能描述:  读取所有为电平输出类型IO的当前输出状态
//* 入口参数:
//*			  -- fd  :     设备描述符
//*           -- ch_num:   DO端口数目
//*           -- value  :  值得缓存区
//*
//* 返回值    :  成功返回当期io口的值；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int do_readall(int fd,int *value, int ch_num);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  do_write
//* 功能描述:  按通道写DO值
//* 入口参数:
//*			  -- fd  :     设备描述符
//*           -- channel:     - IO端口号
//*           -- int  val   --  值
//*
//* 返回值    :  成功返回当期io口的值；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :   无
//*------------------------------------------------------------------------------------------------
extern int do_write(int fd,int channel,int  val);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  di_read
//* 功能描述:  按通道读取DI值
//* 入口参数:
//*			  -- fd  :     设备描述符
//*           -- channel:     - IO端口号
//*           -- 
//*
//* 返回值    :  成功返回当期io口的值；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :   无
//*------------------------------------------------------------------------------------------------
extern int di_read(int fd,int channel);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  do_readall
//* 功能描述:  读取所有DI通道值
//* 入口参数:
//*			  -- fd  :     设备描述符
//*           -- ch_num:   DO端口数目
//*           -- value  :  值得缓存区
//*
//* 返回值    :  成功返回当期io口的值；失败返回(ERROR_INPARA/ERROR_TIMEOUT/ERROR_SYS/ERROR_NOSUPPORT/ERROR_FAIL)
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int di_readall(int fd,int *value, int ch_num);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  init_tts
//* 功能描述:初始化tts        默认为省电模式
//* 入口参数:
//* 返回值    :  tts文件描述符
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int init_tts();
//*------------------------------------------------------------------------------------------------
//* 函数名称:  chipStatusbyPin
//* 功能描述:获取tts的状态
//* 入口参数:tts_fd tts文件描述符
//* 返回值    : 成功： 0 空闲 1 繁忙 
//* 失败     ：ERROR_TIMEOUT 读超时
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int chipStatusbyPin(int tts_fd);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  Set_Power_Saving_Mode
//* 功能描述:设置tts为省电模式
//* 入口参数:tts_fd tts文件描述符
//* 返回值    :  成功：ERROR_OK
//*           失败：ERROR_WRITE
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int Set_Power_Saving_Mode(int tts_fd);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  TTS_Wake_UP
//* 功能描述:唤醒tts
//* 入口参数:tts_fd tts文件描述符
//* 返回值    :  成功：ERROR_OK
//*           失败：ERROR_WRITE
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int TTS_Wake_UP(int tts_fd);
//*------------------------------------------------------------------------------------------------
//* 函数名称:  ttsvoice
//* 功能描述:tts语音播报
//* 入口参数:tts_fd tts文件描述符
//* 返回值    :  成功：实际合成的字节数
//*           失败：ERROR_INPARA
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern int ttsvoice(int tts_fd,char *content);

//*------------------------------------------------------------------------------------------------
//* 函数名称:  uninit_tts
//* 功能描述:卸载tts 
//* 入口参数:tts_fd tts文件描述符
//* 返回值    :  
//* 备注         :   无
//*------------------------------------------------------------------------------------------------

extern void  uninit_tts(int tts_fd);

//*------------------------------------------------------------------------------------------------
//* 函数名称:  init_landline
//* 功能描述:初始化固话 
//* 入口参数:
//* 返回值    :  固话文件文件描述符
//* 备注         :   无
//*------------------------------------------------------------------------------------------------
extern int init_landline();

//*------------------------------------------------------------------------------------------------
//* 函数名称:  uninit_landline
//* 功能描述:卸载固话描述符 
//* 入口参数:landline_fd 固话描述富
//* 返回值    :  
//* 备注         :   无
//*------------------------------------------------------------------------------------------------
extern void  uninit_landline(int landline_fd);

#ifdef __cplusplus
}
#endif

#endif

