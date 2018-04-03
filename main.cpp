#include <QCoreApplication>
#include <QCoreApplication>
#include <QTextCodec>

#include<QThread>

#define CFG_ENGPIO_LIB 1
#define CFG_MISCS_LIB 1
#define CFG_DATETIM_LIB 1

#include "libs_emfuture_odm.h"

#include <QDebug>
#include <iostream>

using namespace std;

string getWeekDay(int week){
    string ret ="";
    switch (week) {
    case 1:
        ret = "Mon.";
        break;
    case 2:
        ret = "Tues.";
        break;
    case 3:
        ret = "Wed.";
        break;
    case 4:
        ret = "Thur.";
        break;
    case 5:
        ret = "Fri.";
        break;
    case 6:
        ret = "Sat.";
        break;
    case 7:
        ret = "Sun.";
        break;

    default:
        break;
    }
    return ret;
}


#include "libs_ljd_odm.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

bool isTrue(){
#ifndef DEF_ARMv7A8
    FILE *TFCard = NULL;
    TFCard = fopen("/dev/mmcblk0p1","r");
    if (TFCard == NULL)
    {
        TFCard = fopen("/dev/mmcblk0p0","r");  // // Avoid change of device_file sequence
        if (TFCard == NULL)
        {
            TFCard = fopen("/dev/mmcblk0p2","r");
        }
    }
    //
    if (TFCard != NULL)
    {
        fclose(TFCard) ;
        TFCard =NULL;
        return true;
    }
    else
    {
        return false;
    }
#else

#endif
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(tc);

    Init_Key();
    cout<<"1111"<<endl;
    show_something_in_first_line(1,"  3434433434");
    cout<<"2222"<<endl;
    int fdLCD = Init_Lcd();

    if (fdLCD <=0)
    {
        exit(0);
    }

    return 1;

  /*   show_something_in_first_line(fdLCD, "192.168.1.241");
   show_something_in_second_line(fdLCD, "255.255.255.0");
    Uninit_Lcd(fdLCD);
*/


     //* TTS Play
    /*
     * int ttsFD = -1;
    ttsFD = init_tts();
    if (ttsFD<=0)
    {
        return 0;
    }

    qDebug("TTS Init");
    QString info;
    info = QString::fromLocal8Bit("[x1]sound101 机房报警: 配电室1#火灾报警，当前值20%；当前温度28.6℃，A相电压384.56V，2018-1-23 10:35，播报完毕！");
    //info = QString::fromLocal8Bit("[x1]sound103 芯片音量调节，[v1]当前等级1,[v9]当前等级9, 设置恢复到初始值");
    //info = QString::fromLocal8Bit("[x1]sound101 [n1]7053控制标记测试[n0] [y0]010-88995631 [p3000]当前值125463,[p3000],当前值[n2]125463,完毕");

    char * text = (char * )info.toStdString().c_str();
    Set_Power_Saving_Mode(ttsFD);
    int state2 =1;
    int i =0;
    do {
        i++;
        state2 = chipStatusbyPin(ttsFD);
        qDebug ("State_%i= %i",i, state2);
    }while(i<50);

    TTS_Wake_UP(ttsFD);

    ttsvoice(ttsFD,text);
    qDebug (" -> Play done");
    i =0;
    do {
        i++;
        //QThread::msleep(10);

        state2 = chipStatusbyPin(ttsFD);
        qDebug ("State_%i= %i",i, state2);
    }while(i<10);

    //Set_Power_Saving_Mode(ttsFD);
    qDebug (" -> Set_Power_Saving_Mode");
    i =0;
    do {
        i++;
        state2 = chipStatusbyPin(ttsFD);
        qDebug ("State_%i= %i",i, state2);
    }while(i<50);

    uninit_tts(ttsFD);
    qDebug("TTS close");

    return 1;

    /*   QString info  = QString::fromLocal8Bit("报警信息: 配电室1#火灾报警，当前值20%；当前温度28.6 ℃，A相电压384.56 V，2018-1-23  10:35，播报完毕！");
    if(argc<3)
    {
        printf("eg:./testDial telno,content\n");
        return -1;
    }

    // gsm_ttsDail

    int sim_fd= init_sim_fd();
    if(sim_fd<=0)
    {
        printf("eg:./testDial telno,content\n");
        return -1;
    }

    int timeout = 20;
    int res = -1;

    for (int i=0; i<3; i++)
    {
        res = gsm_ttsdial(sim_fd, argv[1], (char *)info.toStdString().c_str(), timeout);

        QThread::sleep(3);

        int sim_fd = init_sim_fd();
        if(sim_fd<=0)
            exit(0);

        int res = gsm_writesms_chinese(sim_fd, argv[1], (char *)info.toStdString().c_str());

        qDebug()<<res;
    }
*/

    // SerialID
    unsigned char ID[16]={0};
    unsigned char lenSerial =16;
    if (ERROR_OK!= ReadSerialID(ID, &lenSerial))
    {
        cout<<"ReadSerialID Error."<<endl;
        return 0;
    }

    QString strID;
    for(int i =0; i<lenSerial; i++)
    {
        printf(" 0x%02X ", ID[i]);
        strID .append(QString("%1").arg(ID[i],2,16,QLatin1Char('0')).toUpper() );
    }
    cout<<endl<<"*************** 1 *****************"<<endl;
    cout<<endl<<"EW-320-F SerialID: "<<strID.toStdString()<<endl<<endl;

    cout<<"*************** 2 *****************"<<endl<<endl;

    // RTC
    unsigned short RTCTime[7]={0};
    if (ERROR_OK != GetRtcTime(RTCTime))
    {
        cout<<"GetRtcTime Error."<<endl;
        return -1;
    }
    unsigned short rtcYear  = RTCTime[0];
    unsigned short rtcMonth = RTCTime[1];
    unsigned short rtcDay   = RTCTime[2];
    unsigned short rtcHour  = RTCTime[3];
    unsigned short rtcMinute= RTCTime[4];
    unsigned short rtcSecond= RTCTime[5];
    unsigned short rtcWeek  = RTCTime[6];

    cout<<rtcYear<<"/"<<rtcMonth<<"/"<<rtcDay<<" ";
    cout<<rtcHour<<"-"<<rtcMinute<<"-"<<rtcSecond<<" "<<getWeekDay(rtcWeek)<<endl<<endl;


    cout<<"*************** 3 *****************"<<endl<<endl;
    cout<<"Buzzer Running ..."<<endl;
    char ch;
    int gpio=0;
    int len=0,state=0;


    // Buzzer
    IoctlBuzzer(10,500);
    cout<<"Enter to end"<<endl;
    while (ch =getchar() != '\n');
    IoctlBuzzer(0,0);

    // DI
    cout<<"**************** 4 ****************"<<endl;
    cout<<"Please Input IO Channel:"<<endl;
    len = scanf("%d",&gpio);
    if (len != 1)
        return 0;

    if (ERROR_OK != GetEnIOInValue(gpio,&state))
    {
        cout<<"fail"<<endl;
        return -1;
    }
    cout<<"GPIO_state: "<<state<<endl<<endl;

    // DO
    cout<<"**************** 5 ****************"<<endl;
    //
    cout<<"GPIO output (!Pulse)"<<endl;

    if(ERROR_OK!= GetEnIOOutState(gpio,&state)){
        printf("SetOutIOState error\n\r");
        return 0;
    }

    state =-1;
    cout<<"Please Input Channel and state,eg {0,1}"<<endl;
    cout<<"Before DO_"<<gpio<<" = "<<state<<endl;
    len = scanf("%d, %d", &gpio, &state);
    if (len !=2)
        return 0;

    if(SetEnIOOutState(gpio,state) != ERROR_OK){
        printf("SetOutIOState error\n\r");
        return 0;
    }
    cout<<"to Set DO_"<<gpio<<" = "<<state<<endl;

    state =-1;
    if(ERROR_OK!= GetEnIOOutState(gpio,&state)){
        printf("SetOutIOState error\n\r");
        return 0;
    }

    cout<<"After DO_"<<gpio<<" = "<<state<<endl<<endl;

    return a.exec();
}
