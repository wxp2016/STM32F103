#ifndef MT_HAL_LSM6DSL_H__
#define MT_HAL_LSM6DSL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  
  

#define MAX_BUF_NUM 20
     
#define WAKEUP_INT_THRESHOLD (2)  
#define FIFO_OVERFLOW_INT_THRESHOOLD (2)  
  

//原始数据  
typedef struct
{
    short X;
    short Y;
    short Z;
}SENSOR_RAWDATA;
  
  
//处理后的数据
typedef struct
{
	float X;
	float Y;
	float Z;
}IMU_DATA;

  
//中断类型   
typedef enum 
{
    LSM6DSL_INT_FIFO_OVERFLOW  = 0x0A,//FIFO溢出
  	LSM6DSL_INT_WAKEUP         = 0x0B,//唤醒中断
}LSM6DSL_INT_TYPE_EN;

//中断引脚   
typedef enum 
{
  	LSM6DSL_PIN_INT1 = 0x01,
  	LSM6DSL_PIN_INT2 = 0x02,
}LSM6DSL_INT_PIN_EN;


typedef struct
{
    LSM6DSL_INT_TYPE_EN int_type;//中断类型
    LSM6DSL_INT_PIN_EN int_pin;//中断pin
    uint16_t int_times;//中断次数
}LSM6DSL_INT_T;


extern LSM6DSL_INT_T lsm_int;
extern uint16_t FIFO_Group_Num;  

//FIFO原始数据
extern int AccRaw[MAX_BUF_NUM][3];
extern int GyroRaw[MAX_BUF_NUM][3]; 

//单笔原始数据 
extern SENSOR_RAWDATA Gyro_RawData;
extern SENSOR_RAWDATA Acc_RawData;   

//转换后的数据
extern IMU_DATA GyroFinal;
extern IMU_DATA AccFinal;
  
//Config  
void MT_Hal_LSM6DSL_Init(void);  
void MT_Hal_LSM6DSL_Reset(void); 
void MT_Hal_LSM6DSL_Correct(void);

void MT_Hal_LSM6DSL_Sleep(void);//睡眠配置
void MT_Hal_LSM6DSL_WakeUp(void);//唤醒配置
void MT_Hal_LSM6DSL_PowerOn(void);//上电配置

//Get Raw Data
void MT_Hal_Get_LSM6DSL_RawData(SENSOR_RAWDATA *gyro,SENSOR_RAWDATA *acc);
void MT_Hal_Get_LSM6DSL_AccRawData(SENSOR_RAWDATA *acc);
void MT_Hal_Get_LSM6DSL_GyroRawData(SENSOR_RAWDATA *gyro);

//Data Convert
void MT_Hal_DataConvert(void);
void MT_Hal_DataConvert_FIFO(void);

//Get id
void MT_LSM6DSL_ReadID(uint8_t *id);

//Gyro & Acc
static void MT_Hal_LSM6DSL_Init_Gyro(void);//Init Gyro
static void MT_Hal_LSM6DSL_Init_Acc(void);//Init Acc   

//INT
void LSM6DSL_INT1_Handle(void);
void LSM6DSL_INT2_Handle(void);


//FIFO
static void MT_Hal_LSM6DSL_Init_FIFO(void); 
void MT_Hal_LSM6DSL_Read_FIFO(void);   
static void MT_Hal_LSM6DSL_Read_FIFO_Pattern(uint8_t pattern_idx);
static void MT_Hal_LSM6DSL_GetRaw_FIFOData(void *handle, int *buff);   
static uint16_t MT_Hal_Calculate_FIFO_Pattern(uint16_t *min_odr, uint16_t *max_odr);


#ifdef __cplusplus
}
#endif

#endif // MT_HAL_LSM6DSL_H__