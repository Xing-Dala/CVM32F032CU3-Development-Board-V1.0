#include "Includes.h"

#define RST_READY_TIME            0x07                                                             // ��λ�ȶ�ʱ��
#define OSC32_STABLE_TIME         47                                                               // �����ȶ�ʱ��  2ms��47   6ms��180
#define WAIT_BLE_EXIT_LOWPWR_TIME 10                                                               // �ȴ������˳����ʱ��
#define PRE_WAKEUP_TIME           (RST_READY_TIME + OSC32_STABLE_TIME + WAIT_BLE_EXIT_LOWPWR_TIME) // ������ʱ��

#define CPR_SYS_TIME              ((volatile unsigned *)(0x40002400 + 0x1C))
#define CPR_SLP_CTL               ((volatile unsigned *)(0x40002400 + 0x4))
#define CPR_SLPCTL_INT_MASK       ((volatile unsigned *)(0x40002400 + 0x8))
#define CPR_SLP_PD_MASK           ((volatile unsigned *)(0x40002400 + 0xC))
#define CPR_SLP_SRC_MASK          ((volatile unsigned *)(0x40000000 + 0x000))

/**
 * @brief ϵͳ����
 *
 * @param val
 * @return 0-�ɹ�
 */
static u32 system_wakeup(uint16_t val)
{
    Timer_disable(1);
    return 0;
}
#define CPR_AO_BASE 0x40002400

/**
 * @brief ϵͳIDLE��ʼ��
 */
void ble_system_idle_init(void)
{
    __write_hw_reg32(CPR_SYS_TIME, ((RST_READY_TIME << 12) | OSC32_STABLE_TIME));

    __write_hw_reg32(CPR_SLP_CTL, 0x00);
    __write_hw_reg32(CPR_SLPCTL_INT_MASK, 0xFFFFFFBF);
    __write_hw_reg32(CPR_SLP_PD_MASK, 0x101);
    __write_hw_reg32(CPR_SLP_SRC_MASK, 0x60006);

    *((volatile unsigned *)(CPR_AO_BASE + 0x40)) &= 0x0F; // ROM�ϵ�
    *((volatile unsigned *)(CPR_AO_BASE + 0x44)) |= 0x10; // ROM�ϵ���� ��˯��Ҫ��������ע������֤����������λ

    //*((volatile unsigned *)(CPR_AO_BASE + 0x20)) = 0x2D;  // �ڲ�LDO��ѹ����
    Timer_Register_Callback(system_wakeup, 1); // ����ϵͳ

    /*gpio_direction_output(20);*/
}

extern u32 volatile testlp;
extern uint8_t miss_rev_pkt_flag;

/**
 * @brief ϵͳIDLE������
 *
 */
void ble_system_idle(void)
{
    u32 VAL, VAL1;

    VAL  = ble_lowpower_remaining_clock(0);
    VAL1 = *(((volatile unsigned *)(0x40003000 + 0x0C)));

    if ((VAL <= PRE_WAKEUP_TIME)) //   || (miss_rev_pkt_flag ==1))
        return;

    __disable_irq();

    VAL = ble_lowpower_remaining_clock(1);
    if (VAL == 0xFFFFFFFF)
        do {
            VAL = timer_current_count(0);
            if (VAL != 0xFFFFFFFF)
                break;
        } while (1);

    if (VAL <= PRE_WAKEUP_TIME) {
        __enable_irq();
        return;
    }

#if 0
            {
     
                    DMA_buf[0]=VAL&0XFF;
                    DMA_buf[1]=(VAL&0XFF00)>>8;
                    DMA_buf[2]=VAL1&0xff;
                    DMA_buf[3]=0XCC;
                    DMA_Uart_SendChar(1);
            }
#endif
    VAL = VAL - PRE_WAKEUP_TIME;
    Init_Timer(1, VAL);

    ble_enter_lowpower();

    __write_hw_reg32(CPR_SLP_PD_MASK, 0x101); // ���ϴ˾� auto switch core ldo voltage 1.17v to 0.89v ����һֱ��Ч

    // xc_rf_ldo_off();                                            // close rf_ldo
    *((volatile unsigned *)(CPR_AO_BASE + 0x50)) &= 0xFFFFFFFE; // close rf digital �ֶ�����RF�ܿ���
    *((volatile unsigned *)(CPR_AO_BASE + 0x40)) &= 0x1E;       // cpr_ao_vdd_switch_en BT_MODEM�ϵ�
    *((volatile unsigned *)(CPR_AO_BASE + 0x44)) |= 0x01;       // cpr_ao_vdd_iso_en    BT_MODEM�ϵ����
    // *((volatile unsigned *)(CPR_AO_BASE + 0x20)) = 0x2C;
    __NOP();
    __NOP();
    __NOP();
    __WFI();
    __NOP();
    __NOP();
    __NOP();
    *((volatile unsigned *)(CPR_AO_BASE + 0x40)) |= 0x1;  // cpr_ao_vdd_switch_en BT_MODEM�ϵ�
    *((volatile unsigned *)(CPR_AO_BASE + 0x44)) &= 0x1E; // cpr_ao_vdd_iso_en    BT_MODEM�ϵ����
    *((volatile unsigned *)(CPR_AO_BASE + 0x50)) |= 0x1;  // open rf digital �ֶ�����RF�ܿ���
    // xc_rf_ldo_on();                                       // open rf_ldo
    ble_exit_lowpower(VAL);

    __enable_irq();
}
