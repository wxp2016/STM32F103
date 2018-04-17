#include "mt_hal_lsm6dsl.h"


#include "mt_drv_lsm6dsl.h"
#include "mt_drv_systick.h"



/* Macros for min/max.  */
#define MT_MIN(a,b) (((a)<(b))?(a):(b))
#define MT_MAX(a,b) (((a)>(b))?(a):(b))

#define FIFO_ENABLED     1

#define FIFO_THRESHOLD	3 * 2 * 512         //3072
#define FIFO_SIZE	FIFO_THRESHOLD

#define MAX_PATTERN_NUM FIFO_THRESHOLD/6   //512

//FIFO中的原始数据
int AccRaw[MAX_BUF_NUM][3];
int GyroRaw[MAX_BUF_NUM][3];

//单笔原始数据 
SENSOR_RAWDATA Gyro_RawData;
SENSOR_RAWDATA Acc_RawData;   
//偏移量
SENSOR_RAWDATA Accoffset;
SENSOR_RAWDATA Gyrooffset;  

//转换后的数据
IMU_DATA GyroFinal;
IMU_DATA AccFinal;


uint16_t FIFO_Group_Num = 0;//读取FIFO中的数据的组数 每组包括 Gx Gy Gz Ax Ay Az

/* 6dsl Accelerometer test parameters */
static struct
{
  u8_t				           t_enable;
  LSM6DSL_ACC_GYRO_ODR_XL_t	   t_odr;
  u16_t				           t_odr_hz_val;
  LSM6DSL_ACC_GYRO_FS_XL_t	   t_fs;
  u8_t                         t_decimation;
  u8_t                         t_samples_num_in_pattern;
} test_6dsl_xl = {
                               0,
                               LSM6DSL_ACC_GYRO_ODR_XL_52Hz,
                               0,
                               LSM6DSL_ACC_GYRO_FS_XL_8g,
                               0,
                               0,
                 };


/* 6dsl Gyroscope test parameters */
static struct 
{
  u8_t				        t_enable;
  LSM6DSL_ACC_GYRO_ODR_G_t	t_odr;
  u16_t				        t_odr_hz_val;
  LSM6DSL_ACC_GYRO_FS_G_t	t_fs;
  u8_t                      t_decimation;
  u8_t                      t_samples_num_in_pattern;
} test_6dsl_gyro = 
                    {
                            0,
                            LSM6DSL_ACC_GYRO_ODR_G_52Hz,
                            0,
                            LSM6DSL_ACC_GYRO_FS_G_2000dps,
                            0,
                            0,
                    };

//中断分类管理
LSM6DSL_INT_T lsm_int1;


void LSM6DSL_INT1_Handle()
{
    
    printf("INT1_Normal\n");
    
    switch(lsm_int1.int_type)
    {
        case LSM6DSL_INT_WAKEUP:
            lsm_int1.int_times++;
            if(lsm_int1.int_times >= WAKEUP_INT_THRESHOLD)
            {
                //HalState_PushMsg(EVENT_SENSOR_WAKEUP);//唤醒
                lsm_int1.int_times = 0;
                lsm_int1.int_type = LSM6DSL_INT_FIFO_OVERFLOW;
                printf("LSM6DSL 唤醒中断\n");
            }
            break;
        case LSM6DSL_INT_FIFO_OVERFLOW:
            lsm_int1.int_times++;
            if(lsm_int1.int_times >= FIFO_OVERFLOW_INT_THRESHOOLD)
            {
                //FIFO溢出处理
                lsm_int1.int_times = 0;
                printf("FIFO 溢出处理\n");
            }
            break;
    }  
}


void LSM6DSL_INT2_Handle()
{
    printf("INT2\n");
}




/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_Init(void)
* Function   : MT_LSM6DSL初始化
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Hal_LSM6DSL_Init(void)
{
 
    MT_Hal_LSM6DSL_Reset();
     
    MT_Hal_LSM6DSL_Init_Gyro();
    
    MT_Hal_LSM6DSL_Init_Acc();
    
    MT_Hal_LSM6DSL_Correct();
    
#if (FIFO_ENABLED==1)   
    MT_Hal_LSM6DSL_Init_FIFO();
#endif
    
}



/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_WakeUp(void)
* Function   : MT_LSM6DSL唤醒配置
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Hal_LSM6DSL_WakeUp(void)
{
 //   nRFExti_AllDisable();
  
    MT_Hal_LSM6DSL_Init_Gyro();
    
    MT_Hal_LSM6DSL_Init_Acc();
    
    MT_Hal_LSM6DSL_Correct();
    
#if (FIFO_ENABLED==1)   
    MT_Hal_LSM6DSL_Init_FIFO();
#endif
    
  //  nRFExti_AllEnable();
    
 //   MT_Hal_Task_Imu_Test_Init();//唤醒后开始定时读取数据
    
}




/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_PowerOn(void)
* Function   : MT_LSM6DSL上电配置
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Hal_LSM6DSL_PowerOn(void)
{
 //   nRFExti_AllDisable();
  
    MT_Hal_LSM6DSL_Reset();
     
    MT_Hal_LSM6DSL_Init_Gyro();
    
    MT_Hal_LSM6DSL_Init_Acc();
    
    MT_Hal_LSM6DSL_Correct();
    
#if (FIFO_ENABLED==1)   
    MT_Hal_LSM6DSL_Init_FIFO();
#endif
    
    //上电后就进入睡眠模式
    MT_Hal_LSM6DSL_Sleep();
    
//    nRFExti_AllEnable();
}


/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_Sleep(void)
* Function   : MT_LSM6DSL睡眠配置
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Hal_LSM6DSL_Sleep(void)
{   
    lsm_int1.int_times = 0;
    lsm_int1.int_type = LSM6DSL_INT_WAKEUP;
  
    LSM6DSL_ACC_GYRO_W_FIFO_MODE(0, LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS);//设置FIFO为Bypass模式
    
 
    LSM6DSL_ACC_GYRO_W_ODR_G(0,LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN);//设置GYRO掉电模式
     
    LSM6DSL_ACC_GYRO_W_SLEEP_DUR(0,0x00);//No duration  无持续时间
    LSM6DSL_ACC_GYRO_W_WK_THS(0,0x0F);//设置唤醒阈值 WK_THS*XL/2^6=2*2g/64=62.5mg
    LSM6DSL_ACC_GYRO_W_SLOPE_FDS(0,LSM6DSL_ACC_GYRO_SLOPE_FDS_ENABLED);//使能斜率滤波器
    LSM6DSL_ACC_GYRO_W_LIR(0,LSM6DSL_ACC_GYRO_LIR_DISABLED);//中断锁存禁止
    LSM6DSL_ACC_GYRO_W_INT_ACT_LEVEL(0,LSM6DSL_ACC_GYRO_INT_ACT_LEVEL_ACTIVE_LO);//设置INT为 1-->0
    LSM6DSL_ACC_GYRO_W_BASIC_INT(0,LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED);//使能唤醒中断
     
   
    LSM6DSL_ACC_GYRO_W_FS_XL(0,LSM6DSL_ACC_GYRO_FS_XL_2g);//设置FS_XL=±2g 
    LSM6DSL_ACC_GYRO_W_ODR_XL(0,LSM6DSL_ACC_GYRO_ODR_XL_13Hz);//设置XL_ODR=13HZ 
    LSM6DSL_ACC_GYRO_W_WUEvOnInt1(0,LSM6DSL_ACC_GYRO_INT1_WU_ENABLED);//将WakeUp中断映射到INT1 
    
    printf("LSM6DSL  Sleep\n");
        
}


void MT_LSM6DSL_ReadID(uint8_t *id)
{
    LSM6DSL_ACC_GYRO_R_WHO_AM_I(0,id);
}


/******************************************************************************
* Name       : void MT_LSM6DSL_Reset(void)
* Function   : MT_LSM6DSL复位
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 恢复默认配置并通过读取ID判断器件是否存在 存在打印ID 不存在阻塞
******************************************************************************/
void MT_Hal_LSM6DSL_Reset(void)
{
    
    LSM6DSL_ACC_GYRO_SW_RESET_t rst;
    uint8_t who_am_i;    
    
    LSM6DSL_ACC_GYRO_R_WHO_AM_I(0,&who_am_i);
    if(who_am_i != LSM6DSL_ACC_GYRO_WHO_AM_I)
    {
        while(1)
        {
          
            printf("detect lsm6dsl fail Device ID = %02x\n",who_am_i);
            MT_Delay_ms(500);
        }
    }
    printf("detect lsm6dsl success Device ID = %02x\n",who_am_i);
    
    
    LSM6DSL_ACC_GYRO_W_SW_RESET(0,LSM6DSL_ACC_GYRO_SW_RESET_RESET_DEVICE);//复位--恢复默认配置
    do 
    {
        LSM6DSL_ACC_GYRO_R_SW_RESET(0, &rst);
    } while (rst == LSM6DSL_ACC_GYRO_SW_RESET_RESET_DEVICE); 
}


/******************************************************************************
* Name       : void MT_Hal_Get_LSM6DSL_RawData(short *Gx,short *Gy,short *Gz,short *Ax,short *Ay,short *Az)
* Function   : 读取LSM6DSL Gyro和Acc原始数据
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 非FIFO模式下读取
******************************************************************************/
void MT_Hal_Get_LSM6DSL_RawData(SENSOR_RAWDATA *gyro,SENSOR_RAWDATA *acc)
{
    LSM6DSL_STATUS_REG_t lsm6dsl_status;
    Type3Axis16bit_U raw_data_acc;//加速度原始数据
    Type3Axis16bit_U raw_data_gyro;//陀螺仪原始数据
  
    LSM6DSL_ACC_GYRO_ReadReg(0,LSM6DSL_ACC_GYRO_STATUS_REG,(uint8_t *)&lsm6dsl_status,1);//读状态寄存器
    if(lsm6dsl_status.gda)//GYRO
    {
        LSM6DSL_ACC_GYRO_GetRawGyroData(0,raw_data_gyro.u8bit);//读取原始数据 
        gyro->X = raw_data_gyro.i16bit[0];
        gyro->Y = raw_data_gyro.i16bit[1];
        gyro->Z = raw_data_gyro.i16bit[2];        
    }
  
    if(lsm6dsl_status.xlda)//ACC
    {
        LSM6DSL_ACC_GYRO_GetRawAccData(0,raw_data_acc.u8bit);
        acc->X = raw_data_acc.i16bit[0];
        acc->Y = raw_data_acc.i16bit[1];
        acc->Z = raw_data_acc.i16bit[2];
    }
    /*if(lsm6dsl_status.tda)//temp
    {
        lsm6dsl_temperature_raw(&lsm6dsl_ctx, &temperature_deg_c);
        temp = (float)(temperature_deg_c.i16bit);
        temp = (temp * 10.0)/2560 + 25;
    }    */
}




/******************************************************************************
* Name       : void MT_Hal_Get_LSM6DSL_AccRawData(SENSOR_RAWDATA *acc)
* Function   : 读取LSM6DSL Acc原始数据
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 非FIFO模式下读取
******************************************************************************/
void MT_Hal_Get_LSM6DSL_AccRawData(SENSOR_RAWDATA *acc)
{
    LSM6DSL_STATUS_REG_t lsm6dsl_status;
    Type3Axis16bit_U raw_data_acc;//加速度原始数据
    LSM6DSL_ACC_GYRO_ReadReg(0,LSM6DSL_ACC_GYRO_STATUS_REG,(uint8_t *)&lsm6dsl_status,1);//读状态寄存器
    if(lsm6dsl_status.xlda)//ACC
    {
        LSM6DSL_ACC_GYRO_GetRawAccData(0,raw_data_acc.u8bit);
        acc->X = raw_data_acc.i16bit[0];
        acc->Y = raw_data_acc.i16bit[1];
        acc->Z = raw_data_acc.i16bit[2];
    }
}



/******************************************************************************
* Name       : void MT_Hal_Get_LSM6DSL_GyroRawData(SENSOR_RAWDATA *gyro)
* Function   : 读取LSM6DSL Gyro原始数据
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 非FIFO模式下读取
******************************************************************************/
void MT_Hal_Get_LSM6DSL_GyroRawData(SENSOR_RAWDATA *gyro)
{
    LSM6DSL_STATUS_REG_t lsm6dsl_status;
    Type3Axis16bit_U raw_data_gyro;//陀螺仪原始数据
  
    LSM6DSL_ACC_GYRO_ReadReg(0,LSM6DSL_ACC_GYRO_STATUS_REG,(uint8_t *)&lsm6dsl_status,1);//读状态寄存器
    if(lsm6dsl_status.gda)//GYRO
    {
        LSM6DSL_ACC_GYRO_GetRawGyroData(0,raw_data_gyro.u8bit);//读取原始数据 
        gyro->X = raw_data_gyro.i16bit[0];
        gyro->Y = raw_data_gyro.i16bit[1];
        gyro->Z = raw_data_gyro.i16bit[2];        
    }
}





//Data Convert
void MT_Hal_DataConvert(void)
{
    MT_Hal_Get_LSM6DSL_RawData(&Gyro_RawData,&Acc_RawData);
    //16.4 = 2^16/4000 lsb °/s     1/16.4=0.061     0.0174 = 3.14/180
	//陀螺仪数据从ADC转化为弧度每秒(这里需要减去偏移值)
	GyroFinal.X=(Gyro_RawData.X - Gyrooffset.X)*0.061*0.0174;
	GyroFinal.Y=(Gyro_RawData.Y - Gyrooffset.Y)*0.061*0.0174;
	GyroFinal.Z=(Gyro_RawData.Z - Gyrooffset.Z)*0.061*0.0174;		//读出值减去基准值乘以单位，计算陀螺仪角速度

	//+-8g,2^16/16=4096lsb/g--0.244mg/lsb
	//此处0.0098是：(9.8m/s^2)/1000,乘以mg得m/s^2
	AccFinal.X=(float)((Acc_RawData.X-Accoffset.X)*0.244)*0.0098;		
	AccFinal.Y=(float)((Acc_RawData.Y-Accoffset.Y)*0.244)*0.0098;		
	AccFinal.Z=(float)((Acc_RawData.Z-Accoffset.Z)*0.244)*0.0098;  
}



void MT_Hal_DataConvert_FIFO(void)
{
    //16.4 = 2^16/4000 lsb °/s     1/16.4=0.061     0.0174 = 3.14/180
	//陀螺仪数据从ADC转化为弧度每秒(这里需要减去偏移值)
	GyroFinal.X=(Gyro_RawData.X - Gyrooffset.X)*0.061*0.0174;
	GyroFinal.Y=(Gyro_RawData.Y - Gyrooffset.Y)*0.061*0.0174;
	GyroFinal.Z=(Gyro_RawData.Z - Gyrooffset.Z)*0.061*0.0174;		//读出值减去基准值乘以单位，计算陀螺仪角速度

	//+-8g,2^16/16=4096lsb/g--0.244mg/lsb
	//此处0.0098是：(9.8m/s^2)/1000,乘以mg得m/s^2
	AccFinal.X=(float)((Acc_RawData.X-Accoffset.X)*0.244)*0.0098;		
	AccFinal.Y=(float)((Acc_RawData.Y-Accoffset.Y)*0.244)*0.0098;		
	AccFinal.Z=(float)((Acc_RawData.Z-Accoffset.Z)*0.244)*0.0098;   
}






/******************************************************************************
* Name       : static void MT_LSM6DSL_Init_Gyro(void)
* Function   : 初始化陀螺仪
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 设置GYRO输出速率和满量程(设置ODR同时使能了陀螺仪)
******************************************************************************/
static void MT_Hal_LSM6DSL_Init_Gyro(void)
{
    status_t response;
    
    //Set Gyro odr
    response = LSM6DSL_ACC_GYRO_W_ODR_G(0, test_6dsl_gyro.t_odr);
    if(response==MEMS_ERROR) while(1); //manage here comunication error
    
    //Gyro full scale    
    response = LSM6DSL_ACC_GYRO_W_FS_G(0, test_6dsl_gyro.t_fs);
    if(response==MEMS_ERROR) while(1); //manage here comunication error
    
    //Enable Gyro
    test_6dsl_gyro.t_enable = 1;
}  




/******************************************************************************
* Name       : static void MT_LSM6DSL_Init_Acc(void)
* Function   : 初始化加速度计
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 设置ACC输出速率和满量程(设置ODR同时使能了ACC) 使能BDU
******************************************************************************/
static void MT_Hal_LSM6DSL_Init_Acc(void)
{
    status_t response;    

    /* Set ACC ODR  */
    response = LSM6DSL_ACC_GYRO_W_ODR_XL(0, test_6dsl_xl.t_odr);
    if(response==MEMS_ERROR) while(1); //manage here comunication error
  
    /* Set ACC full scale */
    response = LSM6DSL_ACC_GYRO_W_FS_XL(0, test_6dsl_xl.t_fs);
    if(response==MEMS_ERROR) while(1); //manage here comunication error

    /* BDU Enable */
    response = LSM6DSL_ACC_GYRO_W_BDU(0, LSM6DSL_ACC_GYRO_BDU_BLOCK_UPDATE);
    if(response==MEMS_ERROR) while(1); //manage here comunication error

    /* Enable Acc */
    test_6dsl_xl.t_enable = 1;
}   




/******************************************************************************
* Name       : static void MT_LSM6DSL_Init_FIFO(void)
* Function   : 初始化FIFO
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 
******************************************************************************/ 
static void MT_Hal_LSM6DSL_Init_FIFO(void)
{
    uint16_t min_odr,max_odr;
    
    /* Calculate FIFO pattern composition */
    MT_Hal_Calculate_FIFO_Pattern(&min_odr, &max_odr);
     
    /* Set FIFO threshold to a multiple of pattern length */
    //LSM6DSL_ACC_GYRO_W_FIFO_Watermark(0, FIFO_THRESHOLD);//设置FIFO阈值  2*3*512=

    /* Force FIFO to stop on programmed threshold. */
    //LSM6DSL_ACC_GYRO_W_STOP_ON_FTH(0, LSM6DSL_ACC_GYRO_STOP_ON_FTH_ENABLED);//FIFO深度被阈值LEVEL限制

    LSM6DSL_ACC_GYRO_W_INT_ACT_LEVEL(0,LSM6DSL_ACC_GYRO_INT_ACT_LEVEL_ACTIVE_LO);//设置中断信号输出为 1-->0
    
    //LSM6DSL_ACC_GYRO_W_OVERRUN_on_INT1(0,LSM6DSL_ACC_GYRO_INT1_OVR_ENABLED);//溢出中断  会覆盖最老的一笔数据
    
    /* Route FIFO FULL event on INT1 */
    LSM6DSL_ACC_GYRO_W_FULL_FLAG_on_INT1(0, LSM6DSL_ACC_GYRO_INT1_FULL_FLAG_ENABLED);//下个ODR将溢出FIFO FULL中断到INT1
    
    //LSM6DSL_ACC_GYRO_W_FIFO_TSHLD_on_INT1(0,LSM6DSL_ACC_GYRO_INT1_FTH_ENABLED);//超过阈值
    
    /*
    * Set FIFO ODR to high frequency. The device will use the maximum ODR among
    * sensors.
    */
    LSM6DSL_ACC_GYRO_W_ODR_FIFO(0, LSM6DSL_ACC_GYRO_ODR_FIFO_50Hz);//设置FIFO输出ODR

    /* Set FIFO in continuos mode */
    LSM6DSL_ACC_GYRO_W_FIFO_MODE(0, LSM6DSL_ACC_GYRO_FIFO_MODE_FIFO);//设置FIFO为连续模式

    /* Set FIFO Decimation according to pattern composition */
    LSM6DSL_ACC_GYRO_W_DEC_FIFO_XL_val(0, test_6dsl_xl.t_decimation);//FIFO_XL_DEC
    LSM6DSL_ACC_GYRO_W_DEC_FIFO_G_val(0, test_6dsl_gyro.t_decimation);//FIFO_GYRO_DEC
     
}






/******************************************************************************
* Name       : static void MT_Hal_LSM6DSL_GetRaw_FIFOData(void *handle, int *buff)
* Function   : 从FIFO中读取一笔原始ADC数据
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 
******************************************************************************/ 
static void MT_Hal_LSM6DSL_GetRaw_FIFOData(void *handle, int *buff)
{
    Type3Axis16bit_U raw_data_tmp;
    u8_t i;
    for(i = 0; i < 3; i++)
    {
        LSM6DSL_ACC_GYRO_Get_GetFIFOData(handle, raw_data_tmp.u8bit + 2*i);
    } 
    //可以在此处就将原始数据转换
    buff[0] = raw_data_tmp.i16bit[0];
    buff[1] = raw_data_tmp.i16bit[1];
    buff[2] = raw_data_tmp.i16bit[2];
}



/******************************************************************************
* Name       : static void MT_Hal_LSM6DSL_Read_FIFO_Pattern(u8 pattern_idx)
* Function   : 读取FIFO Pattern 0-5 
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 通过pattern确定此时读取的数据是Gyro还是Acc
******************************************************************************/ 
static void MT_Hal_LSM6DSL_Read_FIFO_Pattern(uint8_t pattern_idx)
{
    uint8_t gy_i = 0;
    uint8_t xl_i = 0;
    uint8_t gy_num = test_6dsl_gyro.t_samples_num_in_pattern;
    uint8_t gy_idx = pattern_idx * gy_num;
    uint8_t xl_num = test_6dsl_xl.t_samples_num_in_pattern;
    uint8_t xl_idx = pattern_idx * xl_num;
    
    while(gy_num > 0 || xl_num > 0) 
    {
        //read gyro
        if (test_6dsl_gyro.t_enable && gy_num > 0) 
        {
            MT_Hal_LSM6DSL_GetRaw_FIFOData(0,GyroRaw[gy_idx + gy_i++]);
            gy_num--;
        }

        //read acc
        if (test_6dsl_xl.t_enable && xl_num > 0) 
        {
            MT_Hal_LSM6DSL_GetRaw_FIFOData(0,AccRaw[xl_idx + xl_i++]);
            xl_num--;
        }
    } 
}




/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_Read_FIFO(void)
* Function   : 读取FIFO(读完为止) 
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 
******************************************************************************/
void MT_Hal_LSM6DSL_Read_FIFO(void)
{
    uint16_t num;
    uint16_t i = 0;
    uint16_t group_num;
    LSM6DSL_ACC_GYRO_R_FIFONumOfEntries(0, &num);//获取FIFO中未读个数
    group_num = num/6;// 待读取数据的组数 每组由Gx Gy Gz Ax Ay Az组成
    FIFO_Group_Num = group_num;
    //LOG(LEVEL_INFO,"\r read start unread num = %d\r",num);
    //LOG(LEVEL_INFO,"\r group_num = %d\r",group_num);
    
    while(group_num-- > 0)//读FIFO 直到FIFO读到空为止
    {
        MT_Hal_LSM6DSL_Read_FIFO_Pattern(i++);
    }
    //LSM6DSL_ACC_GYRO_R_FIFONumOfEntries(0, &num);//获取未读个数
    //LOG(LEVEL_INFO,"\r read end unread num = %d\r",num);
}

  



/******************************************************************************
* Name       : static uint16_t MT_Hal_Calculate_FIFO_Pattern(uint16_t *min_odr, uint16_t *max_odr)
* Function   : 通过ODR计算FIFO Pattern 
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: 针对加速度计和陀螺仪的ODR输出不一致的情况
******************************************************************************/
static uint16_t MT_Hal_Calculate_FIFO_Pattern(uint16_t *min_odr, uint16_t *max_odr)
{
    uint16_t fifo_samples_tot_num = 0;

    /* calculate min_odr and max_odr for current configuration */
    *max_odr = 0;
    *min_odr = 0xffff;

    if (test_6dsl_gyro.t_enable) 
    {
        LSM6DSL_ACC_GYRO_translate_ODR_G(test_6dsl_gyro.t_odr, &test_6dsl_gyro.t_odr_hz_val);//获取ODR数值52
        *max_odr = MT_MAX(*max_odr, test_6dsl_gyro.t_odr_hz_val);//52
        *min_odr = MT_MIN(*min_odr, test_6dsl_gyro.t_odr_hz_val);//52
    }
    
    if (test_6dsl_xl.t_enable) 
    {
        LSM6DSL_ACC_GYRO_translate_ODR_XL(test_6dsl_xl.t_odr, &test_6dsl_xl.t_odr_hz_val);//52
        *max_odr = MT_MAX(*max_odr, test_6dsl_xl.t_odr_hz_val);//52
        *min_odr = MT_MIN(*min_odr, test_6dsl_xl.t_odr_hz_val);//52
       
    }

    if (test_6dsl_gyro.t_enable) 
    {
        test_6dsl_gyro.t_samples_num_in_pattern = test_6dsl_gyro.t_odr_hz_val / *min_odr;//1
        test_6dsl_gyro.t_decimation =  *max_odr / test_6dsl_gyro.t_odr_hz_val;//1
        fifo_samples_tot_num += test_6dsl_gyro.t_samples_num_in_pattern;//1

    }

    if (test_6dsl_xl.t_enable) 
    {
        test_6dsl_xl.t_samples_num_in_pattern = test_6dsl_xl.t_odr_hz_val / *min_odr;//1
        test_6dsl_xl.t_decimation =  *max_odr / test_6dsl_xl.t_odr_hz_val;//1
        fifo_samples_tot_num += test_6dsl_xl.t_samples_num_in_pattern;//2
    }
    return(6 * fifo_samples_tot_num);//12
}







void Acc_Correct(void)
{
    uint8_t i = 0;
	uint8_t numAcc = 200;//取200次累计量
	
	int Angleaccx=0;  //加速度计校正中间变量
	int Angleaccy=0;
	int Angleaccz=0;							

	for(i=0;i<numAcc;i++)
	{		
		MT_Hal_Get_LSM6DSL_AccRawData(&Acc_RawData);//先获取数据;
		Angleaccx+=Acc_RawData.X;
		Angleaccy+=Acc_RawData.Y;
		Angleaccz+=Acc_RawData.Z;
		MT_Delay_ms(2);
	}	
	Accoffset.X= Angleaccx/numAcc;					   
	Accoffset.Y= Angleaccy/numAcc;
	Accoffset.Z= Angleaccy/numAcc;				   //得到加速度计基准
}


void Gyro_Correct(void)
{
    unsigned char i=0;
	unsigned char numGyro=200;

	int Gyrox=0;
	int Gyroy=0;
	int Gyroz=0;							  //陀螺仪校正中间变量

	for(i=0;i<numGyro;i++)
	{
		MT_Hal_Get_LSM6DSL_GyroRawData(&Gyro_RawData);//先获取数据;
		Gyrox+=Gyro_RawData.X;
		Gyroy+=Gyro_RawData.Y;
		Gyroz+=Gyro_RawData.Z;
		MT_Delay_ms(2);
	}
	
	Gyrooffset.X= Gyrox/numGyro;					   
	Gyrooffset.Y= Gyroy/numGyro;
	Gyrooffset.Z= Gyroz/numGyro;
}


//加速度计陀螺仪校正
void MT_Hal_LSM6DSL_Correct(void)
{
    Acc_Correct();
    Gyro_Correct();
    
}
