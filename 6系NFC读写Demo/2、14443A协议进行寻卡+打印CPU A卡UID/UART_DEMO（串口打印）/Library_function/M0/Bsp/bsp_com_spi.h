#ifndef __BSP_COM_SPI_H_
#define __BSP_COM_SPI_H_

extern void Init_com_spi(void);

extern void com_spi_write(uint16_t Addr, uint16_t val);
extern void com_spi_read(uint16_t Addr, uint16_t *val);

#endif
