/**
 * @file bsp_spi_master.h
 * @author 华视微科技
 * @brief SPI主函数（FLASH）
 * @version 0.1
 * @date 2022-09-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __BSP_SPI_MASTER_H_
#define __BSP_SPI_MASTER_H_

#define FLASH_PAGE_SIZE   256   // 256页
#define FLASH_BLOCK_SIZE  65536 // 64k
#define FLASH_SECTOR_SIZE 4096  // 4K

// 分频
#define SPIM_CLK_1MHZ  32
#define SPIM_CLK_2MHZ  16
#define SPIM_CLK_4MHZ  8
#define SPIM_CLK_8MHZ  4
#define SPIM_CLK_16MHZ 2

extern void Init_spi_master(uint32_t ch, uint32_t freq);
extern void spi_flash_Read_Page(uint32_t PageAddR, uint8_t *buff);
extern uint8_t spi_flash_wait_till_ready(void);
extern void spi_flash_chip_erase(void);
extern uint32_t spi_flash_block_num(uint32_t size);
extern void spi_flash_block_erase(uint32_t blknum);
extern void spi_flash_write_enable(void);
extern void spi_flash_write_page(uint32_t PageAddR, uint8_t *buff);
extern void spi_flash_sector_erase(uint32_t no);
extern void spi_flash_Release_powerdown(void);
extern void spi_flash_Enter_powerdown(void);
extern uint16_t spi1_write_read_uint16(uint16_t val);
extern void spi1_write_read_stream(uint16_t *input, uint16_t *output, uint16_t length);

extern void spi_flash_Read(uint32_t ReadAddR, uint8_t *buff, uint16_t ReadLength);
extern void spi_flash_write(uint32_t WriteAddR, uint8_t *buff, uint16_t WriteLength);

extern void spi_flash_Read_128bitsID(uint8_t *buff);

/**
 * @brief
 *
 */
/*--------------------------------------------------------------
for Example:
-----------
    for(i=0; i<256; i++) {
        data_buff[i] = i;
    }
    spi_flash_write_enable();
    spi_flash_block_erase(0);
    while(spi_flash_wait_till_ready());

    spi_flash_write_enable();
    spi_flash_write_page(3, &data_buff[0]);
    while(spi_flash_wait_till_ready());

    for(i=0; i<256; i++)	{
        data_buff[i] = 0;
    }
    spi_flash_Read_Page(3, &data_buff[0]);
--------------------------------------------------------------*/

#endif
