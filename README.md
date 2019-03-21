# bc95-ads1115
# only analysis .The purpose is to establish the I2C between the ads1115 and bc95
# friend succeeded in making connection between the bc95 and sht30 but failed to get the same connection with ads1115


SHT30.c	adm1115main.c 不同点
1,IIC的选择

代码：
Ql_IIC_Init(0,PINNAME_GPIO4,PINNAME_GPIO5,0);

Ql_IIC_Config(0,TRUE, SHT30_ADDRESS, 0);// simultion IIC interface ,do not care the iic Speed.

结论：
sht30用的是模拟iic,ads1115是STM32自带的硬件IIC接口，所以sht30连接的端口不同，config之时也不必考虑速度

思路：ads1115考虑放弃自带的iic接口选择模拟接口，

2，uart 串口通讯
代码：
#define DEBUG_PORT  UART_PORT0
#define DBG_BUF_LEN   512
static char DBG_BUFFER[DBG_BUF_LEN];
#define APP_DEBUG(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
    Ql_sprintf(DBG_BUFFER,FORMAT,##__VA_ARGS__); \
    if (UART_PORT2 == (DEBUG_PORT)) \
    {\
        Ql_Debug_Trace(DBG_BUFFER);\
    } else {\
        Ql_UART_Write((Enum_SerialPort)(DEBUG_PORT), (u8*)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER)));\
    }\
}
======================================================== 
Ql_UART_Register(m_myUartPort, CallBack_UART_Hdlr, NULL);
Ql_UART_Open(m_myUartPort, 115200, FC_NONE);

结论
串口通讯是为了单片机与单片机之间，单片机与PC之间的通讯。sht30写的这部分是为了将调试信息输出，主要方便debug。
思路：对你的ADS1115用处不大，主要考虑检查下你的ADS1115的APP_DEBUG有没有书写错误

3，延时
代码：
Ql_Sleep(500);
思路：既然sht30没问题，也用500吧，这个随意
