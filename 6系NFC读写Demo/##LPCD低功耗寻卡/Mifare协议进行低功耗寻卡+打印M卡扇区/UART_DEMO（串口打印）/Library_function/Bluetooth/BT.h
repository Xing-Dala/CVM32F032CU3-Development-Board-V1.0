#ifndef __BT_H__
#define __BT_H__

extern uint8_t BT_INtest[266];
extern uint8_t BT_OUTtest[431];

extern uint8_t CPU_ID_Buff[16];
extern uint8_t bd_addr[6];

extern uint8_t updata_parameter;
extern uint16_t connection_handle;

void BT_Init(void);

#endif