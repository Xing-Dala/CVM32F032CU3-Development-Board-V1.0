#include "platform.h"

void sm_peripheral_setup(void)
{
}

void _HWradio_Go_To_Idle_State_Patch(void)
{
}

uint8_t __attribute__((aligned(4))) Wbuff[FLASH_PAGE_SIZE + 4];
uint8_t __attribute__((aligned(4))) Rbuff[FLASH_PAGE_SIZE + 4];

int main(void)
{

	uint16_t i;

	Init_spi_master(0, SPIM_CLK_16MHZ);

	spi_flash_Release_powerdown();

	/****����flash���ⳤ������λ�õĶ�д****/
	uint8_t testWrite[62] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
							 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
							 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
							 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
							 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	uint8_t testRead[62] = "";

	printf("\n############################\n");

	spi_flash_write(127 * 1024, testWrite, 62); //����д4096���ֽ�
	spi_flash_Read(127 * 1024, testRead, 62);

	for (i = 0; i < 62; i++)
	{
		printf(" %c ", testRead[i]);
	}

	/****��������ID���� ��ָ����Զ�ȡһ��оƬ�Ѿ��̻���ֻ����128λID****/
	printf("\n-----***----\n");
	uint8_t ID_buf[16] = {0};
	spi_flash_Read_128bitsID(ID_buf);

	for (i = 0; i < 16; i++)
	{
		printf("ID %#x ", ID_buf[i]);
	}

	printf("\n-----***----\n");

	spi_flash_Enter_powerdown();

	while (1)
		;
}
