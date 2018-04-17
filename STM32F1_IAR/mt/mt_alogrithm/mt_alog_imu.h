#ifndef __MT_ALOG_IMU_H__
#define __MT_ALOG_IMU_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>



   	

typedef struct
{
    float pitch;
    float roll;
    float yaw;
}IMU_Q;




//姿态
extern IMU_Q Q_Angle;


void IMU_update(float gx, float gy, float gz, float ax, float ay, float az);
void AHRSupdate(float gx,float gy,float gz,float ax,float ay,float az,float mx,float my,float mz);//姿态解算得出欧拉角
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
float invSqrt(float x);



#ifdef __cplusplus
}
#endif	 
	 
#endif	
