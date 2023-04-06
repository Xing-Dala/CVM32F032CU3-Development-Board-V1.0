/**
 * @file Status.h
 * @author your name (you@domain.com)
 * @brief 常量定义，设备状态指示
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef STATUS_H
#define STATUS_H

//#define TRUE			1
//#define FALSE			0

#define YES 1
#define NO 0

#define HIGH 1
#define LOW 0

#define ON 1
#define OFF 0

// 错误状态
#define ERR_SUCCESS 0x00 // 成功

#define ERR_FAILURE 0xE0 // 失败
#define ERR_LENGTH 0xE1  // 长度错误	ERR_LENGTH
#define ERR_PARA 0xE2    // 参数错误	ERR_PARAMETER
#define ERR_TIMEOUT 0xE3 // 超时错误
#define ERR_EXECUTE 0xE4 // 执行错误	EXECUTE
#define ERR_CRC 0xE5     // CRC错误
#define ERR_CMD 0xE6     // 命令错误
#define ERR_VERIFY 0xE7  // 校验错误
#define ERR_ADDRESS 0xE8 // 地址错误
#define ERR_SN_MIS 0xE9  // 序号不匹配
#define ERR_COMM 0xEF    // 通信错误	ERR_COMMUNICATION

// 警告状态
#define ALM_ALARM 0xA0  // 警告
#define ALM_NOSAVE 0xA1 // 没有保存
#define ALM_REPEAT 0xA3 // 重复执行		ALM = Alarm

// Modbus Exception Code
#define EXCEP_FUNCTION 0x01 // Function code not supported
#define EXCEP_ADDRESS 0x02  // Starting Address == OK  AND Starting Address + Quantity of Inputs == OK
#define EXCEP_QUANTITY 0x03 // 0x0001 < Quantity of Registers < 0x007D
#define EXCEP_DISCRETE 0x04 // ReadDiscreteOutputs == OK

#ifndef NULL
#define NULL ((void *)(0x0000))
#endif

#endif
