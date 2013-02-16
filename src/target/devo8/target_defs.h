#ifndef _DEVO8_TARGET_H_
#define _DEVO8_TARGET_H_

#define VECTOR_TABLE_LOCATION 0x4000
#define SPIFLASH_SECTOR_OFFSET 54
#define SPIFLASH_SECTORS 1024

//#define ENABLE_SCANNER 1
#define HAS_TOUCH 1
#define HAS_VIBRATINGMOTOR 0
#define HAS_LOGICALVIEW 0
#define MIN_BRIGHTNESS 1 
#define DEFAULT_BATTERY_ALARM 4000
#define DEFAULT_BATTERY_CRITICAL 3800
#define MAX_BATTERY_ALARM 12000
#define MIN_BATTERY_ALARM 3300

//Protocols
#define PROTO_HAS_CYRF6936
#define PROTO_HAS_A7105

#define NUM_OUT_CHANNELS 12
#define NUM_VIRT_CHANNELS 10

#define NUM_TRIMS 6
#define MAX_POINTS 13
#define NUM_MIXERS ((NUM_OUT_CHANNELS + NUM_VIRT_CHANNELS) * 4)

#define INP_HAS_CALIBRATION 4

/* Compute voltage from y = 0.0021x + 0.3026 */
#define VOLTAGE_NUMERATOR 210
#define VOLTAGE_OFFSET    303

#endif //_DEVO8_TARGET_H_
