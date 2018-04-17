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

//FIFO�е�ԭʼ����
int AccRaw[MAX_BUF_NUM][3];
int GyroRaw[MAX_BUF_NUM][3];

//����ԭʼ���� 
SENSOR_RAWDATA Gyro_RawData;
SENSOR_RAWDATA Acc_RawData;   
//ƫ����
SENSOR_RAWDATA Accoffset;
SENSOR_RAWDATA Gyrooffset;  

//ת���������
IMU_DATA GyroFinal;
IMU_DATA AccFinal;


uint16_t FIFO_Group_Num = 0;//��ȡFIFO�е����ݵ����� ÿ����� Gx Gy Gz Ax Ay Az

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

//�жϷ������
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
                //HalState_PushMsg(EVENT_SENSOR_WAKEUP);//����
                lsm_int1.int_times = 0;
                lsm_int1.int_type = LSM6DSL_INT_FIFO_OVERFLOW;
                printf("LSM6DSL �����ж�\n");
            }
            break;
        case LSM6DSL_INT_FIFO_OVERFLOW:
            lsm_int1.int_times++;
            if(lsm_int1.int_times >= FIFO_OVERFLOW_INT_THRESHOOLD)
            {
                //FIFO�������
                lsm_int1.int_times = 0;
                printf("FIFO �������\n");
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
* Function   : MT_LSM6DSL��ʼ��
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
* Function   : MT_LSM6DSL��������
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
    
 //   MT_Hal_Task_Imu_Test_Init();//���Ѻ�ʼ��ʱ��ȡ����
    
}




/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_PowerOn(void)
* Function   : MT_LSM6DSL�ϵ�����
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
    
    //�ϵ��ͽ���˯��ģʽ
    MT_Hal_LSM6DSL_Sleep();
    
//    nRFExti_AllEnable();
}


/******************************************************************************
* Name       : void MT_Hal_LSM6DSL_Sleep(void)
* Function   : MT_LSM6DSL˯������
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
  
    LSM6DSL_ACC_GYRO_W_FIFO_MODE(0, LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS);//����FIFOΪBypassģʽ
    
 
    LSM6DSL_ACC_GYRO_W_ODR_G(0,LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN);//����GYRO����ģʽ
     
    LSM6DSL_ACC_GYRO_W_SLEEP_DUR(0,0x00);//No duration  �޳���ʱ��
    LSM6DSL_ACC_GYRO_W_WK_THS(0,0x0F);//���û�����ֵ WK_THS*XL/2^6=2*2g/64=62.5mg
    LSM6DSL_ACC_GYRO_W_SLOPE_FDS(0,LSM6DSL_ACC_GYRO_SLOPE_FDS_ENABLED);//ʹ��б���˲���
    LSM6DSL_ACC_GYRO_W_LIR(0,LSM6DSL_ACC_GYRO_LIR_DISABLED);//�ж������ֹ
    LSM6DSL_ACC_GYRO_W_INT_ACT_LEVEL(0,LSM6DSL_ACC_GYRO_INT_ACT_LEVEL_ACTIVE_LO);//����INTΪ 1-->0
    LSM6DSL_ACC_GYRO_W_BASIC_INT(0,LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED);//ʹ�ܻ����ж�
     
   
    LSM6DSL_ACC_GYRO_W_FS_XL(0,LSM6DSL_ACC_GYRO_FS_XL_2g);//����FS_XL=��2g 
    LSM6DSL_ACC_GYRO_W_ODR_XL(0,LSM6DSL_ACC_GYRO_ODR_XL_13Hz);//����XL_ODR=13HZ 
    LSM6DSL_ACC_GYRO_W_WUEvOnInt1(0,LSM6DSL_ACC_GYRO_INT1_WU_ENABLED);//��WakeUp�ж�ӳ�䵽INT1 
    
    printf("LSM6DSL  Sleep\n");
        
}


void MT_LSM6DSL_ReadID(uint8_t *id)
{
    LSM6DSL_ACC_GYRO_R_WHO_AM_I(0,id);
}


/******************************************************************************
* Name       : void MT_LSM6DSL_Reset(void)
* Function   : MT_LSM6DSL��λ
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: �ָ�Ĭ�����ò�ͨ����ȡID�ж������Ƿ���� ���ڴ�ӡID ����������
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
    
    
    LSM6DSL_ACC_GYRO_W_SW_RESET(0,LSM6DSL_ACC_GYRO_SW_RESET_RESET_DEVICE);//��λ--�ָ�Ĭ������
    do 
    {
        LSM6DSL_ACC_GYRO_R_SW_RESET(0, &rst);
    } while (rst == LSM6DSL_ACC_GYRO_SW_RESET_RESET_DEVICE); 
}


/******************************************************************************
* Name       : void MT_Hal_Get_LSM6DSL_RawData(short *Gx,short *Gy,short *Gz,short *Ax,short *Ay,short *Az)
* Function   : ��ȡLSM6DSL Gyro��Accԭʼ����
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: ��FIFOģʽ�¶�ȡ
******************************************************************************/
void MT_Hal_Get_LSM6DSL_RawData(SENSOR_RAWDATA *gyro,SENSOR_RAWDATA *acc)
{
    LSM6DSL_STATUS_REG_t lsm6dsl_status;
    Type3Axis16bit_U raw_data_acc;//���ٶ�ԭʼ����
    Type3Axis16bit_U raw_data_gyro;//������ԭʼ����
  
    LSM6DSL_ACC_GYRO_ReadReg(0,LSM6DSL_ACC_GYRO_STATUS_REG,(uint8_t *)&lsm6dsl_status,1);//��״̬�Ĵ���
    if(lsm6dsl_status.gda)//GYRO
    {
        LSM6DSL_ACC_GYRO_GetRawGyroData(0,raw_data_gyro.u8bit);//��ȡԭʼ���� 
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
* Function   : ��ȡLSM6DSL Accԭʼ����
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: ��FIFOģʽ�¶�ȡ
******************************************************************************/
void MT_Hal_Get_LSM6DSL_AccRawData(SENSOR_RAWDATA *acc)
{
    LSM6DSL_STATUS_REG_t lsm6dsl_status;
    Type3Axis16bit_U raw_data_acc;//���ٶ�ԭʼ����
    LSM6DSL_ACC_GYRO_ReadReg(0,LSM6DSL_ACC_GYRO_STATUS_REG,(uint8_t *)&lsm6dsl_status,1);//��״̬�Ĵ���
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
* Function   : ��ȡLSM6DSL Gyroԭʼ����
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: ��FIFOģʽ�¶�ȡ
******************************************************************************/
void MT_Hal_Get_LSM6DSL_GyroRawData(SENSOR_RAWDATA *gyro)
{
    LSM6DSL_STATUS_REG_t lsm6dsl_status;
    Type3Axis16bit_U raw_data_gyro;//������ԭʼ����
  
    LSM6DSL_ACC_GYRO_ReadReg(0,LSM6DSL_ACC_GYRO_STATUS_REG,(uint8_t *)&lsm6dsl_status,1);//��״̬�Ĵ���
    if(lsm6dsl_status.gda)//GYRO
    {
        LSM6DSL_ACC_GYRO_GetRawGyroData(0,raw_data_gyro.u8bit);//��ȡԭʼ���� 
        gyro->X = raw_data_gyro.i16bit[0];
        gyro->Y = raw_data_gyro.i16bit[1];
        gyro->Z = raw_data_gyro.i16bit[2];        
    }
}





//Data Convert
void MT_Hal_DataConvert(void)
{
    MT_Hal_Get_LSM6DSL_RawData(&Gyro_RawData,&Acc_RawData);
    //16.4 = 2^16/4000 lsb ��/s     1/16.4=0.061     0.0174 = 3.14/180
	//���������ݴ�ADCת��Ϊ����ÿ��(������Ҫ��ȥƫ��ֵ)
	GyroFinal.X=(Gyro_RawData.X - Gyrooffset.X)*0.061*0.0174;
	GyroFinal.Y=(Gyro_RawData.Y - Gyrooffset.Y)*0.061*0.0174;
	GyroFinal.Z=(Gyro_RawData.Z - Gyrooffset.Z)*0.061*0.0174;		//����ֵ��ȥ��׼ֵ���Ե�λ�����������ǽ��ٶ�

	//+-8g,2^16/16=4096lsb/g--0.244mg/lsb
	//�˴�0.0098�ǣ�(9.8m/s^2)/1000,����mg��m/s^2
	AccFinal.X=(float)((Acc_RawData.X-Accoffset.X)*0.244)*0.0098;		
	AccFinal.Y=(float)((Acc_RawData.Y-Accoffset.Y)*0.244)*0.0098;		
	AccFinal.Z=(float)((Acc_RawData.Z-Accoffset.Z)*0.244)*0.0098;  
}



void MT_Hal_DataConvert_FIFO(void)
{
    //16.4 = 2^16/4000 lsb ��/s     1/16.4=0.061     0.0174 = 3.14/180
	//���������ݴ�ADCת��Ϊ����ÿ��(������Ҫ��ȥƫ��ֵ)
	GyroFinal.X=(Gyro_RawData.X - Gyrooffset.X)*0.061*0.0174;
	GyroFinal.Y=(Gyro_RawData.Y - Gyrooffset.Y)*0.061*0.0174;
	GyroFinal.Z=(Gyro_RawData.Z - Gyrooffset.Z)*0.061*0.0174;		//����ֵ��ȥ��׼ֵ���Ե�λ�����������ǽ��ٶ�

	//+-8g,2^16/16=4096lsb/g--0.244mg/lsb
	//�˴�0.0098�ǣ�(9.8m/s^2)/1000,����mg��m/s^2
	AccFinal.X=(float)((Acc_RawData.X-Accoffset.X)*0.244)*0.0098;		
	AccFinal.Y=(float)((Acc_RawData.Y-Accoffset.Y)*0.244)*0.0098;		
	AccFinal.Z=(float)((Acc_RawData.Z-Accoffset.Z)*0.244)*0.0098;   
}






/******************************************************************************
* Name       : static void MT_LSM6DSL_Init_Gyro(void)
* Function   : ��ʼ��������
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: ����GYRO������ʺ�������(����ODRͬʱʹ����������)
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
* Function   : ��ʼ�����ٶȼ�
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: ����ACC������ʺ�������(����ODRͬʱʹ����ACC) ʹ��BDU
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
* Function   : ��ʼ��FIFO
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
    //LSM6DSL_ACC_GYRO_W_FIFO_Watermark(0, FIFO_THRESHOLD);//����FIFO��ֵ  2*3*512=

    /* Force FIFO to stop on programmed threshold. */
    //LSM6DSL_ACC_GYRO_W_STOP_ON_FTH(0, LSM6DSL_ACC_GYRO_STOP_ON_FTH_ENABLED);//FIFO��ȱ���ֵLEVEL����

    LSM6DSL_ACC_GYRO_W_INT_ACT_LEVEL(0,LSM6DSL_ACC_GYRO_INT_ACT_LEVEL_ACTIVE_LO);//�����ж��ź����Ϊ 1-->0
    
    //LSM6DSL_ACC_GYRO_W_OVERRUN_on_INT1(0,LSM6DSL_ACC_GYRO_INT1_OVR_ENABLED);//����ж�  �Ḳ�����ϵ�һ������
    
    /* Route FIFO FULL event on INT1 */
    LSM6DSL_ACC_GYRO_W_FULL_FLAG_on_INT1(0, LSM6DSL_ACC_GYRO_INT1_FULL_FLAG_ENABLED);//�¸�ODR�����FIFO FULL�жϵ�INT1
    
    //LSM6DSL_ACC_GYRO_W_FIFO_TSHLD_on_INT1(0,LSM6DSL_ACC_GYRO_INT1_FTH_ENABLED);//������ֵ
    
    /*
    * Set FIFO ODR to high frequency. The device will use the maximum ODR among
    * sensors.
    */
    LSM6DSL_ACC_GYRO_W_ODR_FIFO(0, LSM6DSL_ACC_GYRO_ODR_FIFO_50Hz);//����FIFO���ODR

    /* Set FIFO in continuos mode */
    LSM6DSL_ACC_GYRO_W_FIFO_MODE(0, LSM6DSL_ACC_GYRO_FIFO_MODE_FIFO);//����FIFOΪ����ģʽ

    /* Set FIFO Decimation according to pattern composition */
    LSM6DSL_ACC_GYRO_W_DEC_FIFO_XL_val(0, test_6dsl_xl.t_decimation);//FIFO_XL_DEC
    LSM6DSL_ACC_GYRO_W_DEC_FIFO_G_val(0, test_6dsl_gyro.t_decimation);//FIFO_GYRO_DEC
     
}






/******************************************************************************
* Name       : static void MT_Hal_LSM6DSL_GetRaw_FIFOData(void *handle, int *buff)
* Function   : ��FIFO�ж�ȡһ��ԭʼADC����
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
    //�����ڴ˴��ͽ�ԭʼ����ת��
    buff[0] = raw_data_tmp.i16bit[0];
    buff[1] = raw_data_tmp.i16bit[1];
    buff[2] = raw_data_tmp.i16bit[2];
}



/******************************************************************************
* Name       : static void MT_Hal_LSM6DSL_Read_FIFO_Pattern(u8 pattern_idx)
* Function   : ��ȡFIFO Pattern 0-5 
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: ͨ��patternȷ����ʱ��ȡ��������Gyro����Acc
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
* Function   : ��ȡFIFO(����Ϊֹ) 
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
    LSM6DSL_ACC_GYRO_R_FIFONumOfEntries(0, &num);//��ȡFIFO��δ������
    group_num = num/6;// ����ȡ���ݵ����� ÿ����Gx Gy Gz Ax Ay Az���
    FIFO_Group_Num = group_num;
    //LOG(LEVEL_INFO,"\r read start unread num = %d\r",num);
    //LOG(LEVEL_INFO,"\r group_num = %d\r",group_num);
    
    while(group_num-- > 0)//��FIFO ֱ��FIFO������Ϊֹ
    {
        MT_Hal_LSM6DSL_Read_FIFO_Pattern(i++);
    }
    //LSM6DSL_ACC_GYRO_R_FIFONumOfEntries(0, &num);//��ȡδ������
    //LOG(LEVEL_INFO,"\r read end unread num = %d\r",num);
}

  



/******************************************************************************
* Name       : static uint16_t MT_Hal_Calculate_FIFO_Pattern(uint16_t *min_odr, uint16_t *max_odr)
* Function   : ͨ��ODR����FIFO Pattern 
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       :  
* Description: ��Լ��ٶȼƺ������ǵ�ODR�����һ�µ����
******************************************************************************/
static uint16_t MT_Hal_Calculate_FIFO_Pattern(uint16_t *min_odr, uint16_t *max_odr)
{
    uint16_t fifo_samples_tot_num = 0;

    /* calculate min_odr and max_odr for current configuration */
    *max_odr = 0;
    *min_odr = 0xffff;

    if (test_6dsl_gyro.t_enable) 
    {
        LSM6DSL_ACC_GYRO_translate_ODR_G(test_6dsl_gyro.t_odr, &test_6dsl_gyro.t_odr_hz_val);//��ȡODR��ֵ52
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
	uint8_t numAcc = 200;//ȡ200���ۼ���
	
	int Angleaccx=0;  //���ٶȼ�У���м����
	int Angleaccy=0;
	int Angleaccz=0;							

	for(i=0;i<numAcc;i++)
	{		
		MT_Hal_Get_LSM6DSL_AccRawData(&Acc_RawData);//�Ȼ�ȡ����;
		Angleaccx+=Acc_RawData.X;
		Angleaccy+=Acc_RawData.Y;
		Angleaccz+=Acc_RawData.Z;
		MT_Delay_ms(2);
	}	
	Accoffset.X= Angleaccx/numAcc;					   
	Accoffset.Y= Angleaccy/numAcc;
	Accoffset.Z= Angleaccy/numAcc;				   //�õ����ٶȼƻ�׼
}


void Gyro_Correct(void)
{
    unsigned char i=0;
	unsigned char numGyro=200;

	int Gyrox=0;
	int Gyroy=0;
	int Gyroz=0;							  //������У���м����

	for(i=0;i<numGyro;i++)
	{
		MT_Hal_Get_LSM6DSL_GyroRawData(&Gyro_RawData);//�Ȼ�ȡ����;
		Gyrox+=Gyro_RawData.X;
		Gyroy+=Gyro_RawData.Y;
		Gyroz+=Gyro_RawData.Z;
		MT_Delay_ms(2);
	}
	
	Gyrooffset.X= Gyrox/numGyro;					   
	Gyrooffset.Y= Gyroy/numGyro;
	Gyrooffset.Z= Gyroz/numGyro;
}


//���ٶȼ�������У��
void MT_Hal_LSM6DSL_Correct(void)
{
    Acc_Correct();
    Gyro_Correct();
    
}
