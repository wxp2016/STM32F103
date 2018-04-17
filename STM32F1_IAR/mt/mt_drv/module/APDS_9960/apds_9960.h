/**
 * @file    SparkFun_APDS-9960.h
 * @brief   Library for the SparkFun APDS-9960 breakout board
 * @author  Shawn Hymel (SparkFun Electronics)
 *
 * @copyright	This code is public domain but you buy me a beer if you use
 * this and we meet someday (Beerware license).
 *
 * This library interfaces the Avago APDS-9960 to Arduino over I2C. The library
 * relies on the Arduino Wire (I2C) library. to use the library, instantiate an
 * APDS9960 object, call init(), and call the appropriate functions.
 */
 
#ifndef __APDS9960_H__
#define __APDS9960_H__

#include <stdbool.h>
#include "stm32f10x.h"

//�������жϷ�ֵ
#define LIGHT_INT_HIGH  1000 // High light level for interrupt
#define LIGHT_INT_LOW   10   // Low light level for interrupt

//���봫���жϷ�ֵ
#define PROX_INT_HIGH   50 // Proximity level for interrupt
#define PROX_INT_LOW    0  // No far interrupt

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads



/* Debug */
#define DEBUG                   0

/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39	//I2C��ַ

/* Gesture parameters */
#define GESTURE_THRESHOLD_OUT   10	 //�����ֵ
#define GESTURE_SENSITIVITY_1   50   //������1
#define GESTURE_SENSITIVITY_2   20	 //������2   Զ�����ж�  Խ��Խ����

/* Error code for returned values */
#define ERROR                   0xFF 

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C 

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
/*0x80	ʹ�ܼĴ���
7 ����
6 GEN������ʹ��
5 PIEN���ӽ��ж�ʹ��
4 AIEN����������ж�ʹ��
3 WEN���ȴ�ʹ��
2 PEN���ӽ����ʹ��
1 AEN����������ʹ��
0 PON���ϵ�ʹ��
*/
#define APDS9960_ENABLE         0x80	





/*0x81  ADC����ʱ��Ĵ���
7:0	  ATIME 
�ֶ�ֵ			  ����		       ʱ��		      ���ֵ	
0                 256              712ms           65535
182                72              200ms           65535
256-TIME/2.78      ...             ...             ...
219                37              103ms           37889
246                10              27.8ms          10241
255                1               2.78ms           1025
*/
#define APDS9960_ATIME          0x81




/*0x83  �ȴ�ʱ��Ĵ���
7:0  WTIME
�ֶ�ֵ			  �ȴ�ʱ��		   ʱ��(WLONG=0)	   ʱ��(WLONG=1)	  	
0                 256              712ms               8.54s             
256-TIME/2.78      ...             ...                 ...
171                85              236ms               2.84s
255                1               2.78ms              0.03s
*/
#define APDS9960_WTIME          0x83




/*0x84 -- 0x87  �������жϷ�ֵ�Ĵ���
0x84  AILTL     low byte low interrupt threshold
0x85  AILTH	  	high byte low interrupt threshold
0x86  AIHTL		low byte high  interrupt threshold  
0x87  AIHTH		high byte high  interrupt threshold
*/
#define APDS9960_AILTL          0x84
#define APDS9960_AILTH          0x85
#define APDS9960_AIHTL          0x86
#define APDS9960_AIHTH          0x87




/*0x89��0x8B	�ӽ��жϷ�ֵ�Ĵ���
0x89	PILT	���жϷ�ֵ
0x8B	PIHT	���жϷ�ֵ
*/
#define APDS9960_PILT           0x89
#define APDS9960_PIHT           0x8B



/*0x8C	�����Ĵ���
PPERS   7:4   	 �ӽ��жϳ��������������̽ӽ��жϵ�����
		�ֶ�ֵ   �������������ʱ�ӽ��жϲ���
		0        ÿ���ӽ�����
		1		 �κ��ڷ�ֵ��Χ��Ľӽ�ֵ
		2		 2���������ڷ�ֵ��Χ��Ľӽ�ֵ
		...		 ...
APERS   3:0		 �������Ӧ�жϳ��������������̻������Ӧ�жϵ�����
		�ֶ�ֵ	 �������������ʱ�������жϲ���
		0        ÿ���ӽ�����
		1		 �κ��ڷ�ֵ��Χ��Ļ������Ӧֵ
		2		 2���������ڷ�ֵ��Χ��Ļ������Ӧֵ
		3        3...
		4        5...
		5        10...
		6        15...
		7        20...
		8        25...
		9        30...
		10       35...
		11       40...
		...      ...
		15       60
*/
#define APDS9960_PERS           0x8C




/*0x8D  ���üĴ���1  �ϵ�ʱ����Ϊ0x40
7    ����   д0
6    ����   д1
5	 ����   д1
4-2	 ����	д0
1    WLONG	 WaitLong ��ʱ��ȴ�   12x ����ֵ
0	 ����
*/
#define APDS9960_CONFIG1        0x8D




/*0x8E  �ӽ���������Ĵ���    
PPLEN  7:6   �ӽ�����
		0      	4us
		1		8us(default)
		2		16us
		3		32us
PPULSE  5:0   �ӽ��������
		0		1
		1		2
		2		3
		...		...
		63		64
*/
#define APDS9960_PPULSE         0x8E





/*0x8F	���ƼĴ���1
LDRIVE	7:6	  LED����ǿ��
		0		100ma
		1		50ma
		2		25ma
		3		12.5ma
����	5:4		д0
PGAIN	3:2		�ӽ��������
		0		1x
		1		2x
		2		4x
		3		8x
AGAIN	1:0		�������Ӧ����ɫ����
		0		1x
		1		4x
		2		16x
		3		64x
*/
#define APDS9960_CONTROL        0x8F




/*0x90   ���üĴ���2
PSIEN		7	  �ӽ������ж�ʹ��
				  0 �ӽ������ж�ʧ��
				  1	�ӽ������ж�ʹ��
CPSIEN		6	  ����������ܱ����ж�(ALS�����ж�)ʹ��
				  0 ������б����ж�ʧ��
				  1 ������б����ж�ʹ��
LED_BOOST	5:4   ����LDR��ǰ�ӽ�������LED���壬��ǰֵ��LDRIVE���ã�������LDE_BOOST
				  0  100%
				  1	 150%
				  2  200%
				  3  300%
����		3:1   д0
����		0     д1����PORʱĬ�ϱ���Ϊ��
*/			
#define APDS9960_CONFIG2        0x90





/*0x92   ID�Ĵ�����ֻ����
ID			7:0		Ψһ��ʶ������ID
					0xAB = APDS-9960
*/
#define APDS9960_ID             0x92




/*0x93    ״̬�Ĵ�����ֻ�������ϵ�ʱ����Ϊ0x04
CPSAT		7	    ����������ܱ��ͣ����λ�ᱻ��ԭ���������ͨ���ж�����(0xE6 CICLEAR)
					����ʹ��ADCλ(AEN=0),���λ������ж����CPSIEN������
PGSAT		6		��ʾģ�ⱥ���¼���ǰ������ƻ�ӽ����ڣ�һ�������ã����λ��һֱ������
                    ֱ�����ӽ��ж�����Ĺ�������(0xE5 PICLEAR)�������ʹ�ܽӽ�(PEN=0)���λ
					����һ���ж����PSIEN������
PINT		5		�ӽ��жϣ����λ����һ���ж����PIEN��ʹ���б�����
AINT		4		��������жϣ����λ����һ���ж����AIEN��ʹ���б�����
����		3		��������
GINT		2		�����жϣ�GFVLV��ɱ�GFIFOTH���GVALID����λ(GMODE����0ʱ)������,���λ
                    ����λ��FIFO��ȫΪ��ʱ(read)
PVALID		1		��Ч�ӽ�ֵ.������ʾ�ӽ������Դ�PEN�����û�PDATA������,��һ��PDATA���Զ����PVALID
AVALID		0		��Ч�������.������ʾ����������ڵ�AEN�����û���ALS/Color���ݼĴ���������һ������
*/
#define APDS9960_STATUS         0x93




/*0x94--0x9B   RGBC���ݼĴ���
CDATAL	0x94	7:0		clearͨ�����ֽ�
CDATAH	0x95	7:0		clearͨ�����ֽ�
RDATAL	0x96	7:0		redͨ�����ֽ�
RDATAH	0x97	7:0		redͨ�����ֽ�
GDATAL	0x98	7:0		greenͨ�����ֽ�
GDATAH	0x99	7:0		greenͨ�����ֽ�
BDATAL 	0x9A	7:0		blueͨ�����ֽ�
BDATAH	0x9B	7:0		blueͨ�����ֽ�
*/
#define APDS9960_CDATAL         0x94
#define APDS9960_CDATAH         0x95
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B




/*0x9C	�ӽ����ݼĴ���
PDATA	 7:0	�ӽ�����
*/
#define APDS9960_PDATA          0x9C




/*0x9D	�ӽ�ƫ�� UP��RIGHT
POFFSET_UR	7:0		�ֶ�ֵ			ƫ��У������
					01111111			127
					...					...
					00000001			1
					00000000			0
					10000001			-1
					...					...
					11111111			-127
*/
#define APDS9960_POFFSET_UR     0x9D





/*0x9E	�ӽ�ƫ�� DOWN��LEFT
POFFSET_DL	7:0		�ֶ�ֵ			ƫ��У������
					01111111			127
					...					...
					00000001			1
					00000000			0
					10000001			-1
					...					...
					11111111			-127
*/
#define APDS9960_POFFSET_DL     0x9E




/*0x9F		���üĴ���3 
����		7:6		д0
PCMP		5		�ӽ����油��ʹ��.���λ�ṩ���棬���ӽ����������ź������ڵ�������ʱ.���
					ֻ��һ��������(�ڶ����ܶ���)��Ч,��ô����ֻ��һ����ź���ADC������Ч�ģ����
					���ADC���ֵ��127.ʹPCMP���������2X,����ʹ����ADCֵΪ255
					PLMASK_X(U.D.L.R)		PCMP
					0,1,1,1					1
					1,0,1,1					1
					1,1,0,1					1
					1,1,1,0					1
					0,1,0,1					1
					1,0,1,0					1
					All Other				0
SAI			4		�жϺ����˯��.��ʹ�ܺ󵱷����ж�ʱ�Զ�����͹���ģʽ����������״̬��ת����SAI���߿�
					�����Ĳ����ǵ��ж����ű�I2C����ʱ���ָ�
PMASK_U		3		�ӽ��ڵ�UPʹ��	д1ʧ�ܴ˹�������
PMASK_D		2		�ӽ��ڵ�DOWNʹ��	д1ʧ�ܴ˹�������
PMASK_L		1		�ӽ��ڵ�LEFTʹ��	д1ʧ�ܴ˹�������
PMASK_R		0		�ӽ��ڵ�RIGHTʹ�� д1ʧ�ܴ˹�������
*/
#define APDS9960_CONFIG3        0x9F




/*0xA0	���ƽӽ����뷧ֵ�Ĵ��� 
GPENTH		7:0		�ӽ��������뷧ֵ.����Ĵ����趨�ӽ���ֵ�����������ƿ�ʼ������ͬʱ����
					����״̬��(bit4������Ϊ0)
*/
#define APDS9960_GPENTH         0xA0




/*0xA1   �����˳���ֵ�Ĵ���
GEXTH		7:0		�����˳���ֵ.�˼Ĵ������÷�ֵ�������ƽ���,ͬʱ�˳�����״̬��.
					����GTHR_OUTΪ0x00���ֹ�����˳�ֱ��GMODE����Ϊ0
*/
#define APDS9960_GEXTH          0xA1




/*0xA2	�������üĴ���1
GFIFOTH		7:6		����FIFO��ֵ.���ֵ��FIFO  level�Ƚϲ���һ���ж�
					0		��1�����ݼ�����ӵ�FIFO�������ж�
					1		4
					2		8
					3		16
GEXMSK		5:2	    �����˳��ڵ�.�����ĸ�����̽��������ܱ��������������ƽ�������ͬʱ�˳�����״̬��
			0000	All UDLR ̽�����ݱ�������������
			0001	R����������������
			0010	L
			0100	D
			1000	U
			0101	...
			0110	L D
			1111	UDLR
GEXPERS		1:0		�����˳��־���.�����������ƽ���������Ϊ��GEXPERS���ڻ���ڵ�ֵʱ������״̬���˳�
			0		��1�����ƽ���������������״̬���˳�
			1		2
			2		4
			3		7
*/
#define APDS9960_GCONF1         0xA2




/*0xA3   �������üĴ���2
����		7		д0
GGAIN		6:5		����������ƣ��趨һ���������ƽ���������ģʽ��
			0		1x
			1		2x
			2		4x
			3		8x
GLDRIVE		4:3		����LED����ǿ��
			0		100ma
			1		50ma
			2		25ma
			3		12.5ma
GWTIME		2:0		���Ƶȴ�ʱ��	
			0		0ms
			1		2.8ms
			2		5.6ms	
			3		8.4ms
			4		14.0ms
			5		22.4ms
			6		30.8ms
			7		39.2ms
*/
#define APDS9960_GCONF2         0xA3




/*0xA4    ����UPƫ�ƼĴ���
GOFFSET_U	7:0		�ֶ�ֵ				ƫ��У������
					01111111			127
					...					...
					00000001			1
					00000000			0
					10000001			-1
					...					...
					11111111			-127
*/
#define APDS9960_GOFFSET_U      0xA4





/*0xA5    ����DOWNƫ�ƼĴ���
GOFFSET_D	7:0		�ֶ�ֵ				ƫ��У������
					01111111			127
					...					...
					00000001			1
					00000000			0
					10000001			-1
					...					...
					11111111			-127
*/
#define APDS9960_GOFFSET_D      0xA5





/*0xA7    ����LEFTƫ�ƼĴ���
GOFFSET_L	7:0		�ֶ�ֵ				ƫ��У������
					01111111			127
					...					...
					00000001			1
					00000000			0
					10000001			-1
					...					...
					11111111			-127
*/
#define APDS9960_GOFFSET_L      0xA7






/*0xA9    ����RIGHTƫ�ƼĴ���
GOFFSET_R	7:0		�ֶ�ֵ				ƫ��У������
					01111111			127
					...					...
					00000001			1
					00000000			0
					10000001			-1
					...					...
					11111111			-127
*/
#define APDS9960_GOFFSET_R      0xA9




/*0xA6		����������������Ĵ���
GPLEN		7:6		��������.�趨LED_ON����������LDR�����ڼ�
			0		4us
			1		8us(default)
			2		16us
			3		32us
GPULSE		5:0		����������.�ض�����������LDR����
			0		1
			1		2
			2		3
			...		...
			63		64
*/
#define APDS9960_GPULSE         0xA6




/*0xAA	�������üĴ���3
����		7:2		д0
GDIMS		1:0		ѡ���������ƹ��������ڶ�����ʱ�ܹ��ֻ����
			0		���Զ���Ч.UP-DOWN  LEFT-RIGHT FIFO������Ч
			1		UP-DOWN��Ч		LEFT-RIGHT���ù�
			2		LEFT-RIGHT		UP-DOWN
			3		ͬ0
*/
#define APDS9960_GCONF3         0xAA





/*0xAB	�������üĴ���4
����		7:3		д0
GFIFO_CLR	2		��1���GFIFO,GINT,GVALID,GFIFO_OV,GFIFO_LVL
GIEN		1		�����ж�ʹ��	
GMODE		0		����ģʽ	
*/
#define APDS9960_GCONF4         0xAB





/*0xAE	����FIFO Level�Ĵ���
GFLVL		7:0		һ�����ֽ����ݵ�ͬ��һ��GFLVL��
*/
#define APDS9960_GFLVL          0xAE




/*0xAF	����״̬�Ĵ���
����		7:2		��������
GFOV		1		����FIFO���
GVALID		0		����FIFO����
*/
#define APDS9960_GSTATUS        0xAF





/*0xE4--0xE7	����жϼĴ���
IFORCE		0xE4	7:0		ǿ���ж�
PICLEAR		0xE5	7:0		�ӽ��ж����
CICLEAR		0xE6	7:0		��������ж����
AICLEAR		0xE7	7:0		����������ж�
*/
#define APDS9960_IFORCE         0xE4
#define APDS9960_PICLEAR        0xE5
#define APDS9960_CICLEAR        0xE6
#define APDS9960_AICLEAR        0xE7




/*0xFC--0xFF	����FIFO�Ĵ���
GFIFO_U		0xFC	7:0		����FIFO-UP ֵ
GFIFO_D		0xFD	7:0		����FIFO-DOWNֵ
GFIFO_L		0xFE	7��0	����FIFO-LEFTֵ
GFIFO_R		0xFF	7:0		����FIFO-RIGHTֵ
*/
#define APDS9960_GFIFO_U        0xFC
#define APDS9960_GFIFO_D        0xFD
#define APDS9960_GFIFO_L        0xFE
#define APDS9960_GFIFO_R        0xFF



/* Bit fields */
#define APDS9960_PON            0x01//Power on enable
#define APDS9960_AEN            0x02//ALS enable
#define APDS9960_PEN           	0x04//Prox enable
#define APDS9960_WEN            0x08//Wait enable
#define APSD9960_AIEN           0x10//ALS interrupt enable
#define APDS9960_PIEN         	0x20//Peox interrupt enable
#define APDS9960_GEN            0x40//Gesture enable
#define APDS9960_GVALID         0x01//Gesture FIFO data

/* On/Off definitions */
#define OFF                     0
#define ON                      1

/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
#define PROXIMITY               2
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define PROXIMITY_INT           5
#define GESTURE                 6
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* Proximity Gain (PGAIN) values */
#define PGAIN_1X                0
#define PGAIN_2X                1
#define PGAIN_4X                2
#define PGAIN_8X                3

/* ALS Gain (AGAIN) values */
#define AGAIN_1X                0
#define AGAIN_4X                1
#define AGAIN_16X               2
#define AGAIN_64X               3

/* Gesture Gain (GGAIN) values */
#define GGAIN_1X                0
#define GGAIN_2X                1
#define GGAIN_4X                2
#define GGAIN_8X                3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3    

/* Gesture wait time values */
#define GWTIME_0MS              0
#define GWTIME_2_8MS            1
#define GWTIME_5_6MS            2
#define GWTIME_8_4MS            3
#define GWTIME_14_0MS           4
#define GWTIME_22_4MS           5
#define GWTIME_30_8MS           6
#define GWTIME_39_2MS           7

/* Default values */
#define DEFAULT_ATIME           219     // 103ms
#define DEFAULT_WTIME           246     // 27ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_GESTURE_PPULSE  0x89    // 16us, 10 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset      
#define DEFAULT_CONFIG1         0x60    // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_4X
#define DEFAULT_PILT            0       // Low proximity threshold
#define DEFAULT_PIHT            50      // High proximity threshold
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11    // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01    // No saturation interrupts or LED boost  
#define DEFAULT_CONFIG3         0       // Enable all photodiodes, no SAI
#define DEFAULT_GPENTH          40      // Threshold for entering gesture mode
#define DEFAULT_GEXTH           30      // Threshold for exiting gesture mode    
#define DEFAULT_GCONF1          0x40    // 4 gesture events for int., 1 for exit
#define DEFAULT_GGAIN           GGAIN_4X
#define DEFAULT_GLDRIVE         LED_DRIVE_100MA
#define DEFAULT_GWTIME          GWTIME_2_8MS
#define DEFAULT_GOFFSET         0       // No offset scaling for gesture mode
#define DEFAULT_GPULSE          0xC9    // 32us, 10 pulses
#define DEFAULT_GCONF3          0       // All photodiodes active during gesture
#define DEFAULT_GIEN            0       // Disable gesture interrupts

/* Direction definitions */
enum {
  DIR_NONE,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,
  DIR_NEAR,
  DIR_FAR,
  DIR_ALL
};

/* State definitions */
enum {
  NA_STATE,
  NEAR_STATE,
  FAR_STATE,
  ALL_STATE
};

/* Container for gesture data */
typedef struct gesture_data_type {
    uint8_t u_data[32];
    uint8_t d_data[32];
    uint8_t l_data[32];
    uint8_t r_data[32];
    uint8_t index;
    uint8_t total_gestures;
    uint8_t in_threshold;
    uint8_t out_threshold;
} gesture_data_type;


bool APDS_9960_Init(void);
uint8_t getMode(void);
bool setMode(uint8_t mode, uint8_t enable);

/* Turn the APDS-9960 on and off */
bool enablePower(void);
bool disablePower(void);

/* Enable or disable specific sensors */
bool enableLightSensor(bool interrupts);
bool disableLightSensor(void);
bool enableProximitySensor(bool interrupts);
bool disableProximitySensor(void);
bool enableGestureSensor(bool interrupts);
bool disableGestureSensor(void);

/* LED drive strength control */
uint8_t getLEDDrive(void);
bool setLEDDrive(uint8_t drive);
uint8_t getGestureLEDDrive(void);
bool setGestureLEDDrive(uint8_t drive);

/* Gain control */
uint8_t getAmbientLightGain(void);
bool setAmbientLightGain(uint8_t gain);
uint8_t getProximityGain(void);
bool setProximityGain(uint8_t gain);
uint8_t getGestureGain(void);
bool setGestureGain(uint8_t gain);

/* Get and set light interrupt thresholds */
bool getLightIntLowThreshold(uint16_t *threshold);
bool setLightIntLowThreshold(uint16_t threshold);
bool getLightIntHighThreshold(uint16_t *threshold);
bool setLightIntHighThreshold(uint16_t threshold);

/* Get and set proximity interrupt thresholds */
bool getProximityIntLowThreshold(uint8_t *threshold);
bool setProximityIntLowThreshold(uint8_t threshold);
bool getProximityIntHighThreshold(uint8_t *threshold);
bool setProximityIntHighThreshold(uint8_t threshold);

/* Get and set interrupt enables */
uint8_t getAmbientLightIntEnable(void);
bool setAmbientLightIntEnable(uint8_t enable);
uint8_t getProximityIntEnable(void);
bool setProximityIntEnable(uint8_t enable);
uint8_t getGestureIntEnable(void);
bool setGestureIntEnable(uint8_t enable);

/* Clear interrupts */
bool clearAmbientLightInt(void);
bool clearProximityInt(void);

/* Ambient light methods */
bool readAmbientLight(uint16_t *val);
bool readRedLight(uint16_t *val);
bool readGreenLight(uint16_t *val);
bool readBlueLight(uint16_t *val);

/* Proximity methods */
bool readProximity(uint8_t *val);

/* Gesture methods */
bool isGestureAvailable(void);
int readGesture(void);

/* Gesture processing */
void resetGestureParameters(void);
bool processGestureData(void);
bool decodeGesture(void);

/* Proximity Interrupt Threshold */
uint8_t getProxIntLowThresh(void);
bool setProxIntLowThresh(uint8_t threshold);
uint8_t getProxIntHighThresh(void);
bool setProxIntHighThresh(uint8_t threshold);

/* LED Boost Control */
uint8_t getLEDBoost(void);
bool setLEDBoost(uint8_t boost);

/* Proximity photodiode select */
uint8_t getProxGainCompEnable(void);
bool setProxGainCompEnable(uint8_t enable);
uint8_t getProxPhotoMask(void);
bool setProxPhotoMask(uint8_t mask);
/* Gesture threshold control */
uint8_t getGestureEnterThresh(void);
bool setGestureEnterThresh(uint8_t threshold);
uint8_t getGestureExitThresh(void);
bool setGestureExitThresh(uint8_t threshold);

/* Gesture LED, gain, and time control */
uint8_t getGestureWaitTime(void);
bool setGestureWaitTime(uint8_t time);

/* Gesture mode */
uint8_t getGestureMode(void);
bool setGestureMode(uint8_t mode);




// Raw I2C Commands 
bool wireWriteByte(uint8_t val);
bool wireWriteDataByte(uint8_t reg, uint8_t val);
bool wireWriteDataBlock(uint8_t reg, uint8_t *val, unsigned int len);
bool wireReadDataByte(uint8_t reg, uint8_t *val);
int wireReadDataBlock(uint8_t reg, uint8_t *val, unsigned int len);




void HandleGesture(void);
void Get_APDS9960_ID(uint8_t *id);

		


#endif
