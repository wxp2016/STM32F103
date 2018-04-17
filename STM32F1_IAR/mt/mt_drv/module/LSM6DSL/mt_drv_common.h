#ifndef MT_DRV_COMMON_H__
#define MT_DRV_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> 
#include <stdbool.h>

  

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

//these could change accordingly with the architecture

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef unsigned int u32_t;
typedef int i32_t;
typedef short int i16_t;
typedef signed char i8_t;


typedef enum 
{
    MEMS_SUCCESS    = 0x01,
    MEMS_ERROR      = 0x00
}status_t;

typedef enum 
{
    MEMS_ENABLE     = 0x01,
    MEMS_DISABLE    = 0x00
}State_t;
  
  
typedef struct
{
    i16_t AXIS_X;
    i16_t AXIS_Y;
    i16_t AXIS_Z;
}AxesRaw_t;
  


#ifdef __cplusplus
}
#endif

#endif // MT_DRV_COMMON_H__