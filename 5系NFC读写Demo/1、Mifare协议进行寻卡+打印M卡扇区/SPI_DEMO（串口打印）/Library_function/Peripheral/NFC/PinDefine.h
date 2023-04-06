#ifndef _PIN_DEFINE_H_
#define _PIN_DEFINE_H_

#define ON 1
#define OFF 0

#define LED_RED(a)                                   \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET); \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);   \
    }
#define LED_YEL(a)                                   \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET); \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);   \
    }
#define LED_GRE(a)                                   \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET); \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);   \
    }
#define LED_BLU(a)                                   \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET); \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);   \
    }
#define BEEP(a)                                      \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);   \
    } // �͵�ƽ��Ч

#define TEST_PIN(a)                                  \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET); \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);   \
    }

// ��Ƶ�ӿڶ���
#define RC_RST(a)                                    \
    if (a)                                           \
    {                                                \
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);   \
    }                                                \
    else                                             \
    {                                                \
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET); \
    }
#define RC_NSS(a)                                     \
    if (a)                                            \
    {                                                 \
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);   \
    }                                                 \
    else                                              \
    {                                                 \
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET); \
    }

#define RC_SEL0(a)                                    \
    if (a)                                            \
    {                                                 \
        GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);   \
    }                                                 \
    else                                              \
    {                                                 \
        GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET); \
    }
#define RC_SEL1(a)                                    \
    if (a)                                            \
    {                                                 \
        GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);   \
    }                                                 \
    else                                              \
    {                                                 \
        GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET); \
    }

#endif
