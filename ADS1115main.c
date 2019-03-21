void ADS1115(u8 adcMux)
{
    //default configuraton.
    APP_DEBUG("<------ADS1115   %d------->\r",adcMux);  
    s32 ret;
    u8 config_addr = GND; //0x0100 1000 , ADDR PIN = GND

   
    ret = Ql_IIC_Init(1,PINNAME_RI,PINNAME_DCD,1);
    if(ret < 0){
        APP_DEBUG("\r\n<--Failed!! Ql_IIC_Init channel 1 fail ret=%d-->\r\n",ret);
    }
    else{
        APP_DEBUG("\r\n<--pins(%d & %d) Ql_IIC_Init channel 1 ret=%d-->\r\n",PINNAME_RI,PINNAME_DCD,ret);
    }
    Ql_Sleep(1000);

    ret = Ql_IIC_Config(1,TRUE, config_addr, I2C_FREQUENCY_400K);
    Ql_Sleep(1000);
    APP_DEBUG("Ql_IIC_Config = %d\r",ret);


    u8 adcPga  = FS4096;
    u8 adcRate = SPS128; 


    u8 configHi   = (u8)((adcMux << 4) +
                    (adcPga << 1) +
                    (u8)Continuous);

    u8 configLo   = (u8)((adcRate << 5) +
                    ((u8)(Traditional) << 4) +
                    ((u8) Low << 3) +
                    ((u8) NonLatching << 2) +
                    (u8) Disable);

 //   I2C_ADS1115_SendBuf[0] = ADC_CONFIG_REG_ADDR;
 //   I2C_ADS1115_SendBuf[1] = configHi;
 //   I2C_ADS1115_SendBuf[2] = configLo;

    I2C_ADS1115_SendBuf[0] = configHi;
    I2C_ADS1115_SendBuf[1] = configLo;    


    ret = Ql_IIC_Write(1, config_addr , (u8*)I2C_ADS1115_SendBuf, 3);
    Ql_Sleep(1000);
    APP_DEBUG("ret=[%d], Ql_IIC_Write = %d,0x%x,0x%x,0x%x\r",ret, ADC_CONFIG_REG_ADDR,I2C_ADS1115_SendBuf[0],I2C_ADS1115_SendBuf[1],I2C_ADS1115_SendBuf[2]);  
    ret = Ql_IIC_Read(1, ADC_CONVERSION_REG_ADDR, I2C_ADS1115_RecvBuf, 3);
    APP_DEBUG("ret=[%d], Ql_IIC_Read = 0x%x,0x%x,0x%x,0x%x\r",ret, config_addr,I2C_ADS1115_RecvBuf[0],I2C_ADS1115_RecvBuf[1],I2C_ADS1115_RecvBuf[2]); 

    Ql_Sleep(1000);
    I2C_ADS1115_RecvBuf[0] = 0x00;

    ret = Ql_IIC_Write_Read(1, config_addr , I2C_ADS1115_RecvBuf, 1, I2C_ADS1115_RecvBuf, 3);
 //   ret = Ql_IIC_Read(1, ADC_CONVERSION_REG_ADDR, I2C_ADS1115_RecvBuf, 3);
    Ql_Sleep(1000);
    APP_DEBUG("ret=[%d], Ql_IIC_Read_PWR = 0x%x,0x%x,0x%x,0x%x\r",ret, config_addr,I2C_ADS1115_RecvBuf[0],I2C_ADS1115_RecvBuf[1],I2C_ADS1115_RecvBuf[2]); 
    u16 raw = (I2C_ADS1115_RecvBuf[0] << 8) + I2C_ADS1115_RecvBuf[1];
    APP_DEBUG("RAW_value=%d\r",raw);

    APP_DEBUG("adcMux=%d\r",adcMux);
    APP_DEBUG("adcPga=%d\r",adcPga);

    APP_DEBUG("voltage=%f\r",RawToVoltage(adcMux, adcPga, raw));

    /*
                ret = Ql_IIC_Write(0,I2C_SlaveAddr,(u8*)I2C_SendBuf,1);//write register addr
                ret = Ql_IIC_Read(0, I2C_SlaveAddr, I2C_RecvBuf, 1);     //read data
    */
}
