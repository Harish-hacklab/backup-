/** @file dev_id.h
    This file is also included in .gitignore as it varies for devices 
    Please note that it must be manually created and unique values must be assigned to the variable */
byte dev_ID[4] = {0x00, 0x00, 0x13, 0x09}; ///< Stores device ID in 4 bytes

/** @brief MACRO for Touch Sensor
 *  for operator helmet its 1 and for normal helmet its 0 */
//#define TOUCH_ENABLE 

bool vib_motor = 1;//to ON vibration motor make it 1 and OFF vibration motor make it 0

bool buzz = 0;//to ON buzzer make it 1 and OFF buzzer make it 0
