/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  3                                      */
/*      FILE         :  3.c                                    */
/*      DESCRIPTION  :  Main Program                           */
/*      CPU SERIES   :  RX200                                  */
/*      CPU TYPE     :  RX23T                                  */
/*                                                             */
/*      This file is generated by e2 studio.                   */
/*                                                             */
/***************************************************************/                              
                                                                           
                                                                           
                                                                           
                                                                          
/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif




/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */



/* Start user code for include. Do not edit comment generated here */
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
#include "platform.h"
#include "ctrl_smp.h"
#include "ctrl_fmu.h"
#include "ctrl_led.h"
#include "ctrl_beep.h"
#include "ctrl_key.h"
#include "ctrl_pid.h"
#include "ctrl_serial_graph.h"
#include "ctrl_debug.h"
#include "ctrl_control.h"
#include "ctrl_flash.h"
#include "ctrl_encoder.h"
#include "ctrl_usart.h"
//#include "ctrl_key.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"


void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	R_SCI1_Create();
	R_SCI5_Create();
	R_CMT2_Start();						// 启动毫秒定时器
	SysCtrl_Msg_Ctrl_To_FMU_Init();     // 初始化控制板发送给飞控的信息
	Msg_Ctrl_To_Smp_Init();             // 初始化控制板发送给采集板的信息

	PID_Parameter_Init();               // 系统PID参数初始化
	//Dbg_PID_Cache_Init();               // PID调试缓存值初始化
	Usart_Rx_Buf_Init ();               // 串口接收缓存区初始化

	Update_Key_PowerOn_Value();         // 获取按键系统上电时的状态
	Update_Encoder_PowerOn_Value();     // 获取编码器在系统上电时的初始状态

	SysDelay_ms(1000);                  // 延时等待系统稳定

	SysCtrl_Init_Find_Start_Point();    // 初始化找起点的数据记录

	//开启串口
	R_SCI1_Start();
	R_SCI5_Start();

	//三个串口各自接收一次初始化的数据，需要关调用一次接收数据的函数R_SCI1_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num);，三个串口名暂未定义
	R_SCI1_Serial_Receive(U1RxBuf,sizeof(Msg_FMUToCtrl_t));          // 串口1接收一次飞控发送的数据包
	R_SCI5_Serial_Receive(U2RxBuf, sizeof(Msg_SmpToCtrl_t));         // 串口2准备接收采集板发送的数据包
	//Usart_Receive_IT(&huart3, U3RxBuf, 1);                          // 串口3准备接收1个字节的调试命令



	Key_PowerOn_Event_Handle();//保存遥控器微调参数,需要保存到flash中
    /* End user code. Do not edit comment generated here */
}

void main(void)
{
	R_MAIN_UserInit();
	while(1)
	{
		Msg_FMU_To_Ctrl_Update();           // 接收、解析飞控发送的信息
		Msg_Smp_To_Ctrl_Update();           // 接收、解析采集板发送的信息
		Update_Key_Value();                 // 更新按键状态值
		Update_Encoder_Value();             // 更新编码器当前值

		Msg_FMU_To_Ctrl_Update();           //两者都需要调用接收函数，暂未定义// 接收、解析飞控发送的信息
		Msg_Smp_To_Ctrl_Update();           // 接收、解析采集板发送的信息

       //Debug_Message_Handle();          串口三接收数据，暂未定义   //处理调试信息

		Unlock_FMU_By_Key_Handle();         // 解锁按件信息处理

		SysCtrl_Update_PID();               // 更新系统PID设定值等
		SysCtrl_Update_Act();               // 更新系统控制状态(动作)
		SysCtrl_Update_Lost_Info();         //更新系统丢失特征点如何继续追踪信息
       //从串口一发送数据 SysCtrl_Update_Msg_Ctrl_To_FMU();   //更新控制板发送给飞控的信息
        //从串口三发送调试信息，串口三暂未定义Dbg_U3_Send_Msg();                  //
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
