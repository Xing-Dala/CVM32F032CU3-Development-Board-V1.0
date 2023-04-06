/**
 * @file bsp_key.c
 * @author ����΢�Ƽ�
 * @brief ������ȡ����
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "bsp_gpio.h"

/**
 * @brief ������ʼ��
 *
 * @param num ������
 */
void Key_Init(unsigned char num)
{
    switch (num)
    {
    case 1:
        IO_Init(0, 0, 2, 0, 1);
        break;
    case 2:
        IO_Init(1, 0, 2, 0, 1);
        break;
    default:
        break;
    }
}

/**
 * @brief ����״̬��ȡ
 *
 * @param num �������
 * @return ����״̬��1���£�0δ���¡�
 */
unsigned char Key_Read(unsigned char num)
{
    u8 val = 0;
    switch (num)
    {
    case 1:
    {
        if (!IO_Read(0))
        {
            for (unsigned int i = 0; i < 10000; i++)
                ;
            if (!IO_Read(0))
            {
                val = 1;
            }
        }
    }
    break;

    case 2:
    {
        if (!IO_Read(1))
        {
            for (unsigned int i = 0; i < 10000; i++)
                ;
            if (!IO_Read(1))
            {
                val = 1;
            }
        }
    }
    break;

    default:
        break;
    }
    return val;
}