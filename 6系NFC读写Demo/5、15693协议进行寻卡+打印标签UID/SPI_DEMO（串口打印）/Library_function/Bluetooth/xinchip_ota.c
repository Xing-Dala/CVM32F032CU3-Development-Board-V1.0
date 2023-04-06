/**
 * @file xinchip_ota.c
 * @author 张逸逵
 * @brief 蓝牙OTA升级库函数
 * @version 1.0
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "main.h"

#define ZONE_A_SECORT 0  // A区第一个扇区号(A区flash地址空间0--128*1024-1)
#define ZONE_B_SECORT 32 // B区第一个扇区号(B区flash地址空间128*1024--256*1024-1)
// #define ZONE_B_SECORT  0    //B区第一个扇区号

typedef struct ota_updata {
    uint16_t index;         // 接收缓冲区索引
    uint8_t buf[4096];      // 接收缓冲区
    uint8_t secort_index;   // 扇区索引
    uint32_t checksum0;     // 保存APP发过来的bin文件和校验码
    uint32_t checksum1;     // 烧写完成后读出烧写的内容求出校验码 用来和 checksum0比对
    uint32_t time_t;        // 计时器
    uint8_t rev_compl_flag; // 接收完成标志
} ota_updata_t;

ota_updata_t ota_pack = {
    .index          = 0, // 接收缓冲区索引
    .secort_index   = 0, // 扇区索引
    .checksum0      = 0, // 保存APP发过来的bin文件和校验码
    .checksum1      = 0, // 烧写完成后读出烧写的内容求出校验码 用来和 checksum0比对
    .time_t         = 0, // 计时器
    .rev_compl_flag = 0  // 接收完成标志
};

/**
 * @brief 读取BootLoader引导配置 以便OTA升级 时使用
 * 
 * @param buffer 接收到的升级包
 * @param buffer_size 升级包长度
 */
void ota_rev_packet(const uint8_t *buffer, uint16_t buffer_size)
{

    ota_pack.time_t = 1;
    if (buffer_size == 4) { // APP发来的和校验码--并且此数据包是最后一包
        ota_pack.checksum0      = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);
        ota_pack.rev_compl_flag = 1;
    } else { // APP 发过来的OTA升级包 ---APP 每次发过来64个字节
        for (uint32_t i = 0; i < buffer_size; i++) {
            ota_pack.buf[ota_pack.index++] = buffer[i];
        }
        if (ota_pack.index == 4096) { // 接收了4096个字节(flash一个扇区是4096个字节)
            ota_pack.index = 0;
            spi_flash_Release_powerdown();
            spi_flash_write((ZONE_B_SECORT + ota_pack.secort_index) * 4096, ota_pack.buf, 4096); // 写入flash一个扇区
            spi_flash_Enter_powerdown();
            ota_pack.secort_index++;
        }
    }
}

/**
 * @brief 处理ota升级包
 * 
 */
void ota_fun(void)
{
    if (ota_pack.time_t) {
        if (ota_pack.time_t == 2) {
            // Uart_DMA_Send(0,"*",1);
            static unsigned char cout = 0;
            cout++;
            if (cout > 64) // 接收到64*64=4096个字节
            {
                cout = 0;
                Uart_DMA_Send(0, "#", 1);
            }
        }
        ota_pack.time_t++;
        if (ota_pack.time_t >= 300000) // 主循环循环一次大约是31us 31us*300000=9.3s  如过time_t 计时大于9.3s说明数据传输中断
        {
            ota_pack.time_t         = 0;
            ota_pack.index          = 0;
            ota_pack.secort_index   = 0;
            ota_pack.rev_compl_flag = 0;
            ota_pack.checksum0      = 0;
            ota_pack.checksum1      = 0;
            Uart_DMA_Send(0, "Trans Interruption ...\n", sizeof("Trans Interruption ..\n"));
        }
        if (ota_pack.rev_compl_flag) // 接收完成
        {
            Uart_DMA_Send(0, "\nplease wait ...\n", sizeof("\nplease wait ...\n"));
            ota_pack.time_t = 0;
            spi_flash_Release_powerdown();
            if (ota_pack.index != 4096) // 将最后一包写入flash
                spi_flash_write((ZONE_B_SECORT + ota_pack.secort_index) * 4096, ota_pack.buf, ota_pack.index);
            // 将写入到flash的内容读出来进行和校验 用来和APP发过来的和校验码比对

            for (int kk = 0; kk <= ota_pack.secort_index; kk++) {
                if (kk == (ota_pack.secort_index)) {
                    spi_flash_Read((kk + ZONE_B_SECORT) * 4096, ota_pack.buf, ota_pack.index);
                    for (int j = 0; j < ota_pack.index; j++) {
                        ota_pack.checksum1 += ota_pack.buf[j];
                    }
                } else {
                    spi_flash_Read((kk + ZONE_B_SECORT) * 4096, ota_pack.buf, 4096);
                    for (int j = 0; j < 4096; j++) {
                        ota_pack.checksum1 += ota_pack.buf[j];
                    }
                }
            }
            if (ota_pack.checksum1 == ota_pack.checksum0) // 校验成功
            {
#if 1 // 在此处将烧录器烧录进A区的flash第136字节处的mac转存到B去flash第32*4096+136地址处
                {
                    uint8_t temp_mac_buf0[6] = {0};
                    uint8_t temp_mac_buf1[6] = {0};
                    spi_flash_Read(136, temp_mac_buf0, 6);
                    temp_mac_buf1[0] = temp_mac_buf0[1];
                    temp_mac_buf1[2] = temp_mac_buf0[3];
                    temp_mac_buf1[4] = temp_mac_buf0[5];
                    temp_mac_buf1[1] = temp_mac_buf0[0];
                    temp_mac_buf1[3] = temp_mac_buf0[2];
                    temp_mac_buf1[5] = temp_mac_buf0[4];
                    spi_flash_write((32 * 4096) + 136, temp_mac_buf1, 6);
                }
#endif
                // 将B区的内容覆盖到A区-这一步不再进行校验，因为这一步如果出错，那芯片就将变成砖头，校验对其来说已经没有意义
                for (int kk = 0; kk <= ota_pack.secort_index; kk++) {
                    if (kk == (ota_pack.secort_index)) {
                        spi_flash_Read((kk + ZONE_B_SECORT) * 4096, ota_pack.buf, ota_pack.index);
                        for (int n = 0; n < 2048; n++) {
                            uint8_t temp            = ota_pack.buf[2 * n];
                            ota_pack.buf[2 * n]     = ota_pack.buf[2 * n + 1];
                            ota_pack.buf[2 * n + 1] = temp;
                        }
                        spi_flash_write((kk + ZONE_A_SECORT) * 4096, ota_pack.buf, ota_pack.index);
                    } else {
                        spi_flash_Read((kk + ZONE_B_SECORT) * 4096, ota_pack.buf, 4096);
                        for (int n = 0; n < 2048; n++) {
                            uint8_t temp            = ota_pack.buf[2 * n];
                            ota_pack.buf[2 * n]     = ota_pack.buf[2 * n + 1];
                            ota_pack.buf[2 * n + 1] = temp;
                        }
                        spi_flash_write((kk + ZONE_A_SECORT) * 4096, ota_pack.buf, 4096);
                    }
                }

                Uart_DMA_Send(0, "Update complete!\n", sizeof("Update complete!\n"));
                spi_flash_Enter_powerdown();
                xc_wdog_init(0x0); // 开启看门狗等待系统咬狗复位
                while (1)
                    ;

            } else // 校验失败
            {
                Uart_DMA_Send(0, "\nUpdate Fail!\n", sizeof("Update Fail!\n"));
                ota_pack.time_t         = 0;
                ota_pack.index          = 0;
                ota_pack.secort_index   = 0;
                ota_pack.rev_compl_flag = 0;
                ota_pack.checksum0      = 0;
                ota_pack.checksum1      = 0;
                spi_flash_Enter_powerdown();
                // xc_wdog_init(0x1);//开启看门狗等待系统咬狗复位
                // while(1);
            }
        }
    }
}
