/**
 * @file Status.h
 * @author Zhang Yikui(xingdala@qq.com)
 * @brief Common status return value
 * @version 1.0
 * @date 2022-10-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _STATUS_H__
#define _STATUS_H__

/**
 * @brief Judge
 * 
 */
#define TRUE 1
#define FALSE 0

#define YES 1
#define NO 0

/**
 * @brief level
 * 
 */
#define HIGH 1
#define LOW 0

/**
 * @brief Switch
 * 
 */
#define ON 1
#define OFF 0

/**
 * @brief Error return value
 *
 */
#define ERR_SUCCESS 0x00 // Error free, perfect operation

#define ERR_FAILURE 0xE0 // Fault error
#define ERR_LENGTH 0xE1  // Length error
#define ERR_PARA 0xE2    // Parameter error
#define ERR_TIMEOUT 0xE3 // Overtime error
#define ERR_EXECUTE 0xE4 // ִEXECUTE error
#define ERR_CRC 0xE5     // Redundancy error
#define ERR_CMD 0xE6     // Command error
#define ERR_VERIFY 0xE7  // Verification error
#define ERR_ADDRESS 0xE8 // Address error
#define ERR_SN_MIS 0xE9  // Wrong sequence
#define ERR_COMM 0xEF    // Communication error

/**
 * @brief Warning return value
 * 
 */
#define ALM_ALARM 0xA0  //Alert warning

#define ALM_NOSAVE 0xA1 //No save warning
#define ALM_REPEAT 0xA3 //Repeat warning

//------------------------------------------------------------------------------
// Modbus Exception Code
//------------------------------------------------------------------------------
#define EXCEP_FUNCTION 0x01 // Function code not supported
#define EXCEP_ADDRESS 0x02  // Starting Address == OK  AND Starting Address + Quantity of Inputs == OK
#define EXCEP_QUANTITY 0x03 // 0x0001 < Quantity of Registers < 0x007D
#define EXCEP_DISCRETE 0x04 // ReadDiscreteOutputs == OK

#ifndef NULL
#define NULL ((void *)(0x0000))
#endif

#endif
