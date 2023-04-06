/**
 * @file CV663.h
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief CV663 Register and command
 * @version 1.0
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CV663_H__
#define __CV663_H__

#include "main.h"

#define CV663_RST_Pin  4  // PDOWN
#define CV663_MOSI_Pin 21 // IF0
#define CV663_CLK_Pin  5  // IF1
#define CV663_MISO_Pin 1  // IF2
#define CV663_CS_Pin   6  // IF3
#define CV663_IRQ_Pin  7  // IRQ
#define CV663_SL0_Pin  20 // IF3
#define CV663_SL1_Pin  0  // IRQ

/**
 * @brief Analog SPI pin high level
 *
 */
#define CV663_MOSI_Hight gpio_output_high(CV663_MOSI_Pin)
#define CV663_MISO_Hight gpio_output_high(CV663_MISO_Pin)
#define CV663_CS_Hight   gpio_output_high(CV663_CS_Pin)
#define CV663_CLK_Hight  gpio_output_high(CV663_CLK_Pin)
#define CV663_RST_Hight  gpio_output_high(CV663_RST_Pin)

/**
 * @brief Analog SPI pin low level
 *
 */
#define CV663_MOSI_Low gpio_output_low(CV663_MOSI_Pin)
#define CV663_MISO_Low gpio_output_low(CV663_MISO_Pin)
#define CV663_CS_Low   gpio_output_low(CV663_CS_Pin)
#define CV663_CLK_Low  gpio_output_low(CV663_CLK_Pin)
#define CV663_RST_Low  gpio_output_low(CV663_RST_Pin)

#define CV663_MISO_Read IO_Read(CV663_MISO_Pin)
#define CV663_IRQ_Read  IO_Read(CV663_IRQ_Pin)

/**
 * @brief CV663 Command
 *
 */
#define PCD_IDLE         0x00 // no action, cancels current command execution
#define PCD_LPCD         0x01 // low-power card detection
#define PCD_LOADKEY      0x02 // reads a MIFARE key (size of 6 bytes) from FIFO buffer and puts it into Key buffer MIFARE read/write mode only
#define PCD_ACKREQ       0x04 // performs a query, an Ack and a Req-Rn for ISO/IEC 18000-3 mode 3/ EPC Class-1 HF
#define PCD_RECEIVE      0x05 // activates the receive circuit
#define PCD_TRANSMIT     0x06 // transmits data from the FIFO buffer
#define PCD_TRANSCEIVE   0x07 // transmits data from the FIFO buffer and automatically activates the receiver after transmission finished
#define PCD_WRITEE2      0x08 // gets one byte from FIFO buffer and writes it to the internal EEPROM, valid address range are the addresses of the MIFARE Key area
#define PCD_WRITEE2_PAGE 0x09 // gets up to 64 bytes (one EEPROM page) from the FIFO buffer and writes it to the EEPROM, valid page address range are the pages of the MIFARE Key Area
#define PCD_READE2       0x0A // reads data from the EEPROM and copies it into the FIFO buffer, valid address range are the addresses of the MIFARE Key area
#define PCD_LOADREG      0x0C // reads data from the internal EEPROM and initializes the CLCV663 registers. EEPROM address needs to be within EEPROM sector 2
#define PCD_LOADPROTOCOL 0x0D // reads data from the internal EEPROM and initializes the CLCV663 registers needed for a Protocol change
#define PCD_LOADKEYE2    0x0E // copies a key of the EEPROM into the key buffer
#define PCD_STOREKEYE2   0x0F // stores a MIFARE key (size of 6 bytes) into the EEPROM
#define PCD_READRNR      0x1C // Copies bytes from the Random Number generator into the FIFO until the FiFo is full
#define PCD_SOFTRESET    0x1F // resets the CLCV663

/**
 * @brief CV663 Register
 *
 */

// Command Register.
// Used for starting / stopping commands and for sending the IC into standby mode.
#define REG_COMMAND 0x00

// Host-Control Register.
// Configure Host and SAM interfaces.
#define REG_HOSTCTRL 0x01

// FIFO-Control Register.
// Set FIFO size and retrieve FIFO parameters.\n
// Note: Also contains 1 additional Water-Level bit (MSB) and 2 additional FIFO-Length bits (also MSB).
#define REG_FIFOCONTROL 0x02

// WaterLevel Register.
// FIFO WaterLevel configuration.
#define REG_WATERLEVEL 0x03

// FIFO-Length Register.
// Retrieve the number of bytes within the FIFO.
#define REG_FIFOLENGTH 0x04

// FIFO-Data Register.
// Writing to this register moves a byte into the FIFO
// while incrementing the FIFO length and raising the internal WaterLevel.
#define REG_FIFODATA 0x05

// IRQ Registers
// IRQ0 Register.
//  Read or modify the first 7 IRQ bits.
#define REG_IRQ0 0x06
// IRQ1 Register.
// Read or modify the second 7 IRQ bits.
#define REG_IRQ1 0x07
// IRQ0EN Register.
// Enable or disable the first IRQ bits or invert the IRQ propagation.
#define REG_IRQ0EN 0x08
// IRQ1EN Register.
// Enable or disable the second IRQ bits or enable/disable PushPull mode.
#define REG_IRQ1EN 0x09

// Error Register.
// Contains bits for the occured erros.
#define REG_ERROR 0x0A

// Status Register.
// Contains the Crypto1 state and information about the ComState.
#define REG_STATUS 0x0B

// Rx-Bit-Control Register.
// Set/Get Bit-granularity and collision information.
#define REG_RXBITCTRL 0x0C

/// Rx-Coll Register.
// Contains information about the collision position after a collision.
#define REG_RXCOLL 0x0D

// Timer Registers

// Timer Control Register.
// Provides timer control and status information for all timers.
#define REG_TCONTROL 0x0E

// Timer0 Control Register.
// Configure the timer.
#define REG_T0CONTROL 0x0F

// Timer0 Reload(High) Register.
// Set the most significant byte of the Reload-Value.
#define REG_T0RELOADHI 0x10

// Timer0 Reload(Low) Register.
// Set the least significant byte of the Reload-Value.
#define REG_T0RELOADLO 0x11

// Timer0 Counter(High) Register.
// Get the most significant byte of the Counter-Value.
#define REG_T0COUNTERVALHI 0x12

// Timer0 Counter(Low) Register.
// Get the least significant byte of the Counter-Value.
#define REG_T0COUNTERVALLO 0x13

// Timer1 Control Register.
// Configure the timer.
#define REG_T1CONTROL 0x14

// Timer1 Reload(High) Register.
// Set the most significant byte of the Reload-Value.
#define REG_T1RELOADHI 0x15

// Timer1 Reload(Low) Register.
// Set the least significant byte of the Reload-Value.
#define REG_T1RELOADLO 0x16

// Timer1 Counter(High) Register.
// Get the most significant byte of the Counter-Value.
#define REG_T1COUNTERVALHI 0x17

// Timer1 Counter(Low) Register.
// Get the least significant byte of the Counter-Value.
#define REG_T1COUNTERVALLO 0x18

// Timer2 Control Register.
// Configure the timer.
#define REG_T2CONTROL 0x19

// Timer2 Reload(High) Register.
// Set the most significant byte of the Reload-Value.
#define REG_T2RELOADHI 0x1A

// Timer2 Reload(Low) Register.
// Set the least significant byte of the Reload-Value.
#define REG_T2RELOADLO 0x1B

// Timer2 Counter(High) Register.
// Get the most significant byte of the Counter-Value.
#define REG_T2COUNTERVALHI 0x1C

// Timer2 Counter(Low) Register.
// Get the least significant byte of the Counter-Value.
#define REG_T2COUNTERVALLO 0x1D

// Timer3 Control Register.
// Configure the timer.
#define REG_T3CONTROL 0x1E

// Timer3 Reload(High) Register.
// Set the most significant byte of the Reload-Value.
#define REG_T3RELOADHI 0x1F

// Timer3 Reload(Low) Register.
// Set the least significant byte of the Reload-Value.
#define REG_T3RELOADLO 0x20

// Timer3 Counter(High) Register.
// Get the most significant byte of the Counter-Value.
#define REG_T3COUNTERVALHI 0x21

// Timer3 Counter(Low) Register.
// Get the least significant byte of the Counter-Value.
#define REG_T3COUNTERVALLO 0x22

// Timer4 Control Register.
// Configure the timer.
#define REG_T4CONTROL 0x23

// Timer4 Reload(High) Register.
// Set the most significant byte of the Reload-Value.
#define REG_T4RELOADHI 0x24

// Timer4 Reload(Low) Register.
// Set the least significant byte of the Reload-Value.
#define REG_T4RELOADLO 0x25

// Timer4 Counter(High) Register.
// Get the most significant byte of the Counter-Value.
#define REG_T4COUNTERVALHI 0x26

// Timer4 Counter(Low) Register.
// Get the least significant byte of the Counter-Value.
#define REG_T4COUNTERVALLO 0x27

// Driver Mode Register.
// Enable / Invert the Tx-Driver and set the Clock Mode.
#define REG_DRVMODE 0x28

// Tx Amplifier Register.
// Modify Amplitude and Carrier settings.
#define REG_TXAMP 0x29

// Driver Control Register.
// Select / Invert drivers.
#define REG_DRVCON 0x2A

// TxI Register.
// Contains Overshoot prevention and current control settings.
#define REG_TXI 0x2B

// Tx-CRC Control Register.
// Configure CRC parameters transmission.
#define REG_TXCRCCON 0x2C

// Rx-CRC Control Register.
// Configure CRC parameters for reception.
#define REG_RXCRCCON 0x2D

// Tx-DataNum Register.
// Set TxLastBits and configure KeepBitGrid functionality.
#define REG_TXDATANUM 0x2E

// Tx-Modwidth Register.
// Set the modulation width.
#define REG_TXMODWIDTH 0x2F

// Symbol 0 and 1 Register.
// Configure Burst-lengths of both symbols.
#define REG_TXSYM10BURSTLEN 0x30

// Tx-Wait Control Register.
// Enable / Configure Tx Waiting-Time.
#define REG_TXWAITCTRL 0x31

// TxWaitLo Register.
// Contains the Least-Significant-Bits for the Tx Waiting-Time.
#define REG_TXWAITLO 0x32

// Frame control register.
// Contains active Start/Stop symbol and Parity settings.
#define REG_FRAMECON 0x33

// RxSOFD Register.
// Contains Start-of-Frame and subcarrier detection bits.
#define REG_RXSOFD 0x34

// Rx Control Register.
// Configure Receiver settings such as baudrate and EMD-suppression feature.
#define REG_RXCTRL 0x35

// Rx-Wait Register.
// Configure Receiver Deaf-Time.
#define REG_RXWAIT 0x36

// Rx-Threshold Register.
// Configure Receiver Threshold.
#define REG_RXTHRESHOLD 0x37

// Receiver Register.
// Configure Collision-Level and other features.
#define REG_RCV 0x38

// Rx-Analog Register.
// Configure analog settings and parameters for Receiver circuitry.
#define REG_RXANA 0x39

// Serial Speed Register.
// Configure serial baudrates.
#define REG_SERIALSPEED 0x3B

// LPO_TRIMM Register.
// Trimm Control Input for Low Power Oscillator.
#define REG_LPO_TRIMM 0x3C

// PLL Control Register.
// Configure PLL settings.
#define REG_PLL_CTRL 0x3D

// PLL DivO Register.
// Contains PLL output.
#define REG_PLL_DIV 0x3E

// LPCD QMin Register.
// Configure IMax(2) and QMin values for LPCD.
#define REG_LPCD_QMIN 0x3F

// LPCD QMax Register.
// Configure IMax(1) and QMax values for LPCD.
#define REG_LPCD_QMAX 0x40

// LPCD IMin Register.
// Configure IMax(0) and IMin values for LPCD.
#define REG_LPCD_IMIN 0x41

// LPCD Result(I) Register.
// Contains I-Channel results of LPCD.
#define REG_LPCD_RESULT_I 0x42

// LPCD Result(Q) Register.
// Contains Q-Channel results of LPCD.
#define REG_LPCD_RESULT_Q 0x43

// Version Register.
// Contains IC Version and Subversion.
#define REG_VERSION 0x7F

// CV663 Command Register Contents (0x00)
#define BIT_STANDBY      0x80U ///*< Standby bit; If set, the IC transits to standby mode. ///
#define BIT_MODEM_OFF    0x40U ///*< ModemOff bit; If set, the receiver and the transmitter circuit is powering down. ///
#define CMD_IDLE         0x00U ///*< No action; cancels current command execution. ///
#define CMD_LPCD         0x01U ///*< Low Power Card Detection. ///
#define CMD_LOADKEY      0x02U ///*< Reads a key from the FIFO buffer and puts it into the key buffer. ///
#define CMD_MFAUTHENT    0x03U ///*< Performs the Mifare standard authentication (in Mifare Reader/Writer mode only). ///
#define CMD_ACKREQ       0x04U ///*< Performs a Query, a Ack and a Req-Rn for EPC V2. ///
#define CMD_RECEIVE      0x05U ///*< Activates the receiver circuitry. ///
#define CMD_TRANSMIT     0x06U ///*< Transmits data from the FIFO buffer to Card. ///
#define CMD_TRANSCEIVE   0x07U ///*< Like #CMD_TRANSMIT but automatically activates the receiver after transmission is finished. ///
#define CMD_WRITEE2      0x08U ///*< Gets one byte from FIFO buffer and writes it to the internal EEPROM. ///
#define CMD_WRITEE2PAGE  0x09U ///*< Gets up to 64 Bytes from FIFO buffer and writes it to the EEPROM. ///
#define CMD_READE2       0x0AU ///*< Reads data from EEPROM and puts it into the FIFO buffer. ///
#define CMD_LOADREG      0x0CU ///*< Reads data from EEPROM and initializes the registers. ///
#define CMD_LOADPROTOCOL 0x0DU ///*< Reads data from EEPROM and initializes the registers needed for a protocol change. ///
#define CMD_LOADKEYE2    0x0EU ///*< Copies a Mifare key from the EEPROM into the key buffer. ///
#define CMD_STOREKEYE2   0x0FU ///*< Stores a Mifare key into the EEPROM. ///
#define CMD_SOFTRESET    0x1FU ///*< Resets the IC. ///
#define MASK_COMMAND     0x1FU ///*< Mask for Command-bits. ///

// Host-Control Register Contents (0x01)
#define BIT_REGEN         0x80U
#define BIT_BUSHOST       0x40U
#define BIT_BUSSAM        0x20U
#define MASK_SAMINTERFACE 0x0CU

// FIFO-Control Register Contents (0x02)
#define BIT_FIFOSIZE       0x80U
#define BIT_HIALERT        0x40U
#define BIT_LOALERT        0x20U
#define BIT_FLUSHFIFO      0x10U
#define BIT_WATERLEVEL_HI  0x04U
#define MASK_FIFOLENGTH_HI 0x03U

// IRQ0 Register(s) Contents (0x06/0x08)
#define BIT_SET        0x80U
#define BIT_IRQINV     0x80U
#define BIT_HIALERTIRQ 0x40U
#define BIT_LOALERTIRQ 0x20U
#define BIT_IDLEIRQ    0x10U
#define BIT_TXIRQ      0x08U
#define BIT_RXIRQ      0x04U
#define BIT_ERRIRQ     0x02U
#define BIT_EMDIRQ     0x01U

// IRQ1 Register(s) Contents (0x07/0x09)
/// #define BIT_SET                  0x80U ///
#define BIT_IRQPUSHPULL 0x80U
#define BIT_GLOBALIRQ   0x40U
#define BIT_IRQPINEN    0x40U
#define BIT_LPCDIRQ     0x20U
#define BIT_TIMER4IRQ   0x10U
#define BIT_TIMER3IRQ   0x08U
#define BIT_TIMER2IRQ   0x04U
#define BIT_TIMER1IRQ   0x02U
#define BIT_TIMER0IRQ   0x01U

// Error Register Contents (0x0A)
#define BIT_EE_ERR      0x80U
#define BIT_FIFOWRERR   0x40U
#define BIT_FIFOOVL     0x20U
#define BIT_MINFRAMEERR 0x10U
#define BIT_NODATAERR   0x08U
#define BIT_COLLDET     0x04U
#define BIT_PROTERR     0x02U
#define BIT_INTEGERR    0x01U

// Status Register Contents (0x0B)
#define BIT_CRYPTO1ON  0x20U
#define MASK_COMMSTATE 0x07U

// Rx-Bit-Control Register Contents (0x0C)
#define BIT_VALUESAFTERCOLL 0x80U
#define BIT_NOCOLL          0x08U
#define MASK_RXALIGN        0x70U
#define MASK_RXLASTBITS     0x07U

// Rx-Coll Register Contents (0x0D)
#define BIT_COLLPOSVALID 0x80U
#define MASK_COLLPOS     0x7FU

// Timer-Control Register Contents (0x0E)
#define BIT_T3RUNNING      0x80U
#define BIT_T2RUNNING      0x40U
#define BIT_T1RUNNING      0x20U
#define BIT_T0RUNNING      0x10U
#define BIT_T3STARTSTOPNOW 0x08U
#define BIT_T2STARTSTOPNOW 0x04U
#define BIT_T1STARTSTOPNOW 0x02U
#define BIT_T0STARTSTOPNOW 0x01U

// T[0-3]-Control Register Contents (0x0F/0x14/0x19/0x1E)
#define BIT_TSTOP_RX        0x80U ///*< Stop timer on receive interrupt. ///
#define BIT_TAUTORESTARTED  0x08U ///*< Auto-restart timer after underflow. ///
#define BIT_TSTART_TX       0x10U ///*< Start timer on transmit interrupt. ///
#define BIT_TSTART_LFO      0x20U ///*< Use this timer for LFO trimming. ///
#define BIT_TSTART_LFO_UV   0x30U ///*< Use this timer for LFO trimming (generate UV at a trimming event). ///
#define MASK_TSTART         0x30U ///*< Mask for TSTART bits. ///
#define VALUE_TCLK_1356_MHZ 0x00U ///*< Use 13.56MHz as input clock. ///
#define VALUE_TCLK_212_KHZ  0x01U ///*< Use 212KHz as input clock. ///
#define VALUE_TCLK_T0       0x02U ///*< Use timer0 as input clock. ///
#define VALUE_TCLK_T1       0x03U ///*< Use timer1 as input clock. ///

// T4-Control Register Contents (0x23)
#define BIT_T4RUNNING       0x80U
#define BIT_T4STARTSTOPNOW  0x40U
#define BIT_T4AUTOTRIMM     0x20U
#define BIT_T4AUTOLPCD      0x10U
#define BIT_T4AUTORESTARTED 0x08U
#define BIT_T4AUTOWAKEUP    0x04U
/// #define MASK_TSTART              0x30U///
#define VALUE_TCLK_LFO_64_KHZ 0x00U
#define VALUE_TCLK_LFO_8_KHZ  0x01U
#define VALUE_TCLK_LFO_4_KHZ  0x02U
#define VALUE_TCLK_LFO_2_KHZ  0x03U

// Driver Mode Register Contents (0x28)
#define BIT_TX2INV  0x80U
#define BIT_TX1INV  0x40U
#define BIT_TXEN    0x08U
#define BIT_RFON    0x04U
#define BIT_TPUSHON 0x02U
#define BIT_TPULLON 0x01U

// Tx Amplifier Register Contents (0x29)
#define MASK_CW_AMPLITUDE     0xC0U
#define MASK_RESIDUAL_CARRIER 0x1FU

// Driver Control Register Contents (0x2A)
#define BIT_CWMAX             0x10U
#define BIT_DISOVSHTPREV      0x08U
#define BIT_DRIVERINV         0x04U
#define VALUE_DRIVERSEL_LOW   0x00U
#define VALUE_DRIVERSEL_TXENV 0x01U
#define VALUE_DRIVERSEL_SIGIN 0x02U

// Tx-/Rx-CRC Control Register Contents (0x2C/0x2D)
#define BIT_RXFORCECRCWRITE 0x80U
#define BIT_CRCINVERT       0x02U
#define BIT_CRCEN           0x01U
#define MASK_CRCPRESETVAL   0x70U
#define MASK_CRCTYPE        0x0CU

// Tx-DataNum Register Contents (0x2E)
#define BIT_KEEPBITGRID 0x10U
#define BIT_DATAEN      0x08U
#define MASK_TXLASTBITS 0x07U

// Tx-Wait Control Register Contents (0x31)
#define BIT_TXWAIT_START_RX 0x80U
#define BIT_TXWAIT_DBFREQ   0x40U
#define MASK_TXWAITHI       0x38U
#define MASK_TXSTOPBITLEN   0x07U

// Frame Control Register Contents (0x33)
#define BIT_TXPARITYEN   0x80U
#define BIT_RXPARITYEN   0x40U
#define VALUE_STOP_SYM3  0x0CU
#define VALUE_STOP_SYM2  0x08U
#define VALUE_STOP_SYM1  0x04U
#define VALUE_START_SYM2 0x03U
#define VALUE_START_SYM1 0x02U
#define VALUE_START_SYM0 0x01U
#define MASK_STARTSYM    0x03U
#define MASK_STOPSYM     0x0CU

// Rx Control Register Contents (0x35)
#define BIT_RXALLOWBITS      0x80U
#define BIT_RXMULTIPLE       0x40U
#define BIT_RXEOFTYPE        0x20U
#define BIT_EGT_CHECK        0x10U
#define BIT_EMD_SUPPRESSION  0x08U
#define VALUE_BAUDRATE_26KD  0x02U
#define VALUE_BAUDRATE_52KD  0x03U
#define VALUE_BAUDRATE_106KD 0x04U
#define VALUE_BAUDRATE_212KD 0x05U
#define VALUE_BAUDRATE_424KD 0x06U
#define VALUE_BAUDRATE_847KD 0x07U
#define MASK_RXBAUDRATE      0x07U

// Rx-Wait Register Contents (0x36)
#define BIT_RXWAITDBFREQ 0x80U
#define MASK_RXWAIT      0x7FU

// Rx-Threshold Register Contents (0x37)
#define MASK_MINLEVEL  0xF0U
#define MASK_MINLEVELP 0x0FU

// Rx-Receiver Register Contents (0x38)
#define BIT_RX_SINGLE                               0x80U
#define BIT_RX_SHORT_MIX2ADC                        0x40U
#define BIT_USE_SMALL_EVAL                          0x04U
#define VALUE_SIGINSEL_IDLE                         0x00
#define VALUE_SIGINSEL_RX                           0x10
#define VALUE_SIGINSEL_ISO14443A                    0x20
#define VALUE_SIGINSEL_S3CGEN                       0x30
#define VALUE_COLLLEVEL_SINGAL_AT_LEAST_ONE_EIGHTHS 0x00
#define VALUE_COLLLEVEL_SINGAL_AT_LEAST_ONE_FOURS   0x01
#define VALUE_COLLLEVEL_SINGAL_AT_LEAST_ONE_TWOS    0x02
#define VALUE_COLLLEVEL_SINGAL_OFF                  0x03
#define MASK_RX_SIGPRO_IN_SEL                       0x30U
#define MASK_COLLLEVEL                              0x03U

// Rx-Analog Register Contents (0x39)
#define BIT_RX_OC_ENABLE 0x20U
#define BIT_RX_HP_LOWF   0x10U
#define MASK_VMID_R_SEL  0xC0U
#define MASK_RCV_HPCF    0x0CU
#define MASK_RCV_GAIN    0x03U

// Serial-Speed Register Contents (0x3B)
#define MASK_BR_T0 0xE0U
#define MASK_BR_T1 0x1FU

// LPCD Result(Q) Register Contents (0x43)
#define BIT_LPCDIRQ_CLR 0x40U

// Tx-BitMod Register Contents (0x48)
#define BIT_TXMSBFIRST     0x80U
#define BIT_TXPARITYTYPE   0x20U
#define BIT_TXSTOPBITTYPE  0x08U
#define BIT_TXSTARTBITTYPE 0x02U
#define BIT_TXSTARTBITEN   0x01U

// Rx-BitMod Register Contents (0x58)
#define BIT_RXSTOPONINVPAR 0x20U
#define BIT_RXSTOPONLEN    0x10U
#define BIT_RXMSBFIRST     0x08U
#define BIT_RXSTOPBITEN    0x04U
#define BIT_RXPARITYTYPE   0x02U

// Rx-Mod Register Contents (0x5D)
#define BIT_PREFILTER                      0x20U
#define BIT_RECTFILTER                     0x10U
#define BIT_SYNCHIGH                       0x08U
#define BIT_CORRINV                        0x04U
#define BIT_FSK                            0x02U
#define BIT_BPSK                           0x01U

#define PHHAL_HW_CV663_REG_COMMAND         0x00 // Command Register
#define PHHAL_HW_CV663_REG_HOSTCTRL        0x01 // Configure Host and SAM interfaces.
#define PHHAL_HW_CV663_REG_FIFOCONTROL     0x02 // FIFO-Control Register
#define PHHAL_HW_CV663_REG_WATERLEVEL      0x03 // FIFO WaterLevel configuration
#define PHHAL_HW_CV663_REG_FIFOLENGTH      0x04 // FIFO-Length Register
#define PHHAL_HW_CV663_REG_FIFODATA        0x05 // FIFO-Data Register
#define PHHAL_HW_CV663_REG_IRQ0            0x06 // IRQ Registers
#define PHHAL_HW_CV663_REG_IRQ1            0x07 // IRQ1 Register
#define PHHAL_HW_CV663_REG_IRQ0EN          0x08 // IRQ0EN Register
#define PHHAL_HW_CV663_REG_IRQ1EN          0x09 // IRQ1EN Register.
#define PHHAL_HW_CV663_REG_ERROR           0x0A // Error Register.
#define PHHAL_HW_CV663_REG_STATUS          0x0B // Status Register
#define PHHAL_HW_CV663_REG_RXBITCTRL       0x0C // Rx-Bit-Control Register
#define PHHAL_HW_CV663_REG_RXCOLL          0x0D // Rx-Coll Register
#define PHHAL_HW_CV663_REG_TCONTROL        0x0E // Timer Registers
#define PHHAL_HW_CV663_REG_T0CONTROL       0x0F // Timer0 Control Register
#define PHHAL_HW_CV663_REG_T0RELOADHI      0x10 // Timer0 Reload(High) Register
#define PHHAL_HW_CV663_REG_T0RELOADLO      0x11 // Timer0 Reload(Low) Register
#define PHHAL_HW_CV663_REG_T0COUNTERVALHI  0x12 // Timer0 Counter(High) Register
#define PHHAL_HW_CV663_REG_T0COUNTERVALLO  0x13 // Timer0 Counter(Low) Register
#define PHHAL_HW_CV663_REG_T1CONTROL       0x14 // Timer1 Control Register
#define PHHAL_HW_CV663_REG_T1RELOADHI      0x15 // Timer1 Reload(High) Register
#define PHHAL_HW_CV663_REG_T1RELOADLO      0x16 // Timer1 Reload(Low) Register
#define PHHAL_HW_CV663_REG_T1COUNTERVALHI  0x17 // Timer1 Counter(High) Register
#define PHHAL_HW_CV663_REG_T1COUNTERVALLO  0x18 // Timer1 Counter(Low) Register
#define PHHAL_HW_CV663_REG_T2CONTROL       0x19 // Timer2 Control Register
#define PHHAL_HW_CV663_REG_T2RELOADHI      0x1A // Timer2 Reload(High) Register
#define PHHAL_HW_CV663_REG_T2RELOADLO      0x1B // Timer2 Reload(Low) Register
#define PHHAL_HW_CV663_REG_T2COUNTERVALHI  0x1C // Timer2 Counter(High) Register
#define PHHAL_HW_CV663_REG_T2COUNTERVALLO  0x1D // Timer2 Counter(Low) Register
#define PHHAL_HW_CV663_REG_T3CONTROL       0x1E // Timer3 Control Register
#define PHHAL_HW_CV663_REG_T3RELOADHI      0x1F // Timer3 Reload(High) Register
#define PHHAL_HW_CV663_REG_T3RELOADLO      0x20 // Timer3 Reload(Low) Register
#define PHHAL_HW_CV663_REG_T3COUNTERVALHI  0x21 // Timer3 Counter(High) Register
#define PHHAL_HW_CV663_REG_T3COUNTERVALLO  0x22 // Timer3 Counter(Low) Register
#define PHHAL_HW_CV663_REG_T4CONTROL       0x23 // Timer4 Control Register
#define PHHAL_HW_CV663_REG_T4RELOADHI      0x24 // Timer4 Reload(High) Register
#define PHHAL_HW_CV663_REG_T4RELOADLO      0x25 // Timer4 Reload(Low) Register
#define PHHAL_HW_CV663_REG_T4COUNTERVALHI  0x26 // Timer4 Counter(High) Register.
#define PHHAL_HW_CV663_REG_T4COUNTERVALLO  0x27 // Timer4 Counter(Low) Register
#define PHHAL_HW_CV663_REG_DRVMODE         0x28 // Driver Mode Register
#define PHHAL_HW_CV663_REG_TXAMP           0x29 // Tx Amplifier Register
#define PHHAL_HW_CV663_REG_DRVCON          0x2A // Driver Control Register.
#define PHHAL_HW_CV663_REG_TXI             0x2B // TxI Register
#define PHHAL_HW_CV663_REG_TXCRCCON        0x2C // Tx-CRC Control Register
#define PHHAL_HW_CV663_REG_RXCRCCON        0x2D // Rx-CRC Control Register
#define PHHAL_HW_CV663_REG_TXDATANUM       0x2E // Tx-DataNum Register
#define PHHAL_HW_CV663_REG_TXMODWIDTH      0x2F // Tx-Modwidth Register
#define PHHAL_HW_CV663_REG_TXSYM10BURSTLEN 0x30 // Symbol 0 and 1 Register
#define PHHAL_HW_CV663_REG_TXWAITCTRL      0x31 // Tx-Wait Control Register
#define PHHAL_HW_CV663_REG_TXWAITLO        0x32 // TxWaitLo Register
#define PHHAL_HW_CV663_REG_FRAMECON        0x33 // Frame control register
#define PHHAL_HW_CV663_REG_RXSOFD          0x34 // RxSOFD Register
#define PHHAL_HW_CV663_REG_RXCTRL          0x35 // Rx Control Register
#define PHHAL_HW_CV663_REG_RXWAIT          0x36 // Rx-Wait Register
#define PHHAL_HW_CV663_REG_RXTHRESHOLD     0x37 // Rx-Threshold Register
#define PHHAL_HW_CV663_REG_RCV             0x38 // Receiver Register
#define PHHAL_HW_CV663_REG_RXANA           0x39 // Rx-Analog Register
#define PHHAL_HW_CV663_REG_SERIALSPEED     0x3B // Serial Speed Register.
#define PHHAL_HW_CV663_REG_LPO_TRIMM       0x3C // LPO_TRIMM Register
#define PHHAL_HW_CV663_REG_PLL_CTRL        0x3D // PLL Control Register.
#define PHHAL_HW_CV663_REG_PLL_DIV         0x3E // PLL DivO Register
#define PHHAL_HW_CV663_REG_LPCD_QMIN       0x3F // LPCD QMin Register.
#define PHHAL_HW_CV663_REG_LPCD_QMAX       0x40 // LPCD QMax Register
#define PHHAL_HW_CV663_REG_LPCD_IMIN       0x41 // LPCD IMin Register
#define PHHAL_HW_CV663_REG_LPCD_RESULT_I   0x42 // LPCD Result(I) Register
#define PHHAL_HW_CV663_REG_LPCD_RESULT_Q   0x43 // LPCD Result(Q) Register
#define PHHAL_HW_CV663_REG_VERSION         0x7F // Version Register

// CV663 Command Register Contents (0x00)
#define PHHAL_HW_CV663_BIT_STANDBY      0x80U // 备用位；如果设置，IC将转换为待机模式

#define PHHAL_HW_CV663_CMD_IDLE         0x00U // 无动作；取消当前命令执行。
#define PHHAL_HW_CV663_CMD_LPCD         0x01U // 低功耗卡检测
#define PHHAL_HW_CV663_CMD_LOADKEY      0x02U // 自FIFO缓冲器读取一个 MIFARE 密钥（6个字节大小），并放入密钥缓冲器中
#define PHHAL_HW_CV663_CMD_MFAUTHENT    0x03U // 仅以MIFARE读/写模式执行 MIFARE 标准认证
#define PHHAL_HW_CV663_CMD_ACKREQ       0x04U // 执行查询，用于 ISO/IEC 18000-3 Mode 3/ EPC Class-1 HF 的 Ack 及 Req-Rn
#define PHHAL_HW_CV663_CMD_RECEIVE      0x05U // 启动接收电路
#define PHHAL_HW_CV663_CMD_TRANSMIT     0x06U // 由 FIFO 缓冲器传输数据
#define PHHAL_HW_CV663_CMD_TRANSCEIVE   0x07U // 由 FIFO 缓冲器传输数据，且在传输结束后自动启动接收器
#define PHHAL_HW_CV663_CMD_WRITEE2      0x08U // 自 FIFO 缓冲器取得一个字节，并将其写入内部 EEPROM 中。有效的地址范围限于 MIFARE 密钥区内的地址
#define PHHAL_HW_CV663_CMD_WRITEE2PAGE  0x09U // 自 FIFO 缓冲器中取得最多达 64 个字节（一页 EEPROM 页面）并将其写入 EEPROM 中。有效的地址范围限于MIFARE 密钥区内的地址
#define PHHAL_HW_CV663_CMD_READE2       0x0AU // 自 EEPROM 读取数据并将其复制到FIFO 缓冲器，有效的地址范围限于 MIFARE 密钥区内的地址
#define PHHAL_HW_CV663_CMD_LOADREG      0x0CU // 从 EEPROM 内部读取数据，并初始化CLRC663 寄存器。EEPROM 地址必须在EEPROM 第 2 区内
#define PHHAL_HW_CV663_CMD_LOADPROTOCOL 0x0DU // 从 EEPROM 内部读取数据，并初始化改变协议所需的 CLRC663 寄存器
#define PHHAL_HW_CV663_CMD_LOADKEYE2    0x0EU // 复制EEPROM的一个密钥到密钥缓冲区
#define PHHAL_HW_CV663_CMD_STOREKEYE2   0x0FU // 存储MIFARE密钥（6字节大小）到EEPROM
#define PHHAL_HW_CV663_CMD_SOFTRESET    0x1FU // 复位 CLRC663

#define PHHAL_HW_CV663_MASK_COMMAND     0x1FU // 命令位掩码

/**
 * @brief CV663 command redefinition
 *
 */
#define PCD_CMD_LOADKEY    PHHAL_HW_CV663_CMD_LOADKEY
#define PCD_CMD_MFAUTHENT  PHHAL_HW_CV663_CMD_MFAUTHENT
#define PCD_CMD_RECEIVE    PHHAL_HW_CV663_CMD_RECEIVE
#define PCD_CMD_TRANSMIT   PHHAL_HW_CV663_CMD_TRANSMIT
#define PCD_CMD_TRANSCEIVE PHHAL_HW_CV663_CMD_TRANSCEIVE

/**
 * @brief CV663 Error Code Definition
 *
 */
#define MI_OK                     0x00
#define MI_NOTAGERR               0xFF // 0xFF 无标签卡错误
#define MI_CRCERR                 0xFE
#define MI_EMPTY                  0xFD
#define MI_AUTHERR                0xFC // 认证失败
#define MI_PARITYERR              0xFB
#define MI_CODEERR                0xFA // 代码错误
#define MI_SERNRERR               0xF8 // Sernr error 		尾端错误
#define MI_KEYERR                 0xF7
#define MI_NOTAUTHERR             0xF6 // 没有认证错误
#define MI_BITCOUNTERR            0xF5
#define MI_BYTECOUNTERR           0xF4 // Byte counter error	字节数错误
#define MI_IDLE                   0xF3
#define MI_TRANSERR               0xF2 // 传输错误
#define MI_WRITEERR               0xF1
#define MI_INCRERR                0xF0
#define MI_DECRERR                0xEF
#define MI_READERR                0xEE
#define MI_OVFLERR                0xED
#define MI_POLLING                0xEC
#define MI_FRAMINGERR             0xEB
#define MI_ACCESSERR              0xEA
#define MI_UNKNOWN_COMMAND        0xE9 // 未知命令
#define MI_COLLERR                0xE8 // Collision error	碰撞错误
#define MI_RESETERR               0xE7 // Reset error		复位错误
#define MI_INTERFACEERR           0xE6 // Interface error	接口错误
#define MI_ACCESSTIMEOUT          0xE5
#define MI_NOBITWISEANTICOLL      0xE4 // No bit wise anticoll
#define MI_QUIT                   0xE2
#define MI_RECBUF_OVERFLOW        0xCE
#define MI_SENDBYTENR             0xCD
#define MI_SENDBUF_OVERFLOW       0xCB
#define MI_BAUDRATE_NOT_SUPPORTED 0xCA
#define MI_SAME_BAUDRATE_REQUIRED 0xC9
#define MI_WRONG_PARAMETER_VALUE  0xC4
#define MI_BREAK                  0x9D
#define MI_NY_IMPLEMENTED         0x9C
#define MI_NO_MFRC                0x9B
#define MI_MFRC_NOTAUTH           0x9A
#define MI_WRONG_DES_MODE         0x99
#define MI_HOST_AUTH_FAILED       0x98
#define MI_WRONG_LOAD_MODE        0x96
#define MI_WRONG_DESKEY           0x95
#define MI_MKLOAD_FAILED          0x94
#define MI_FIFOERR                0x93
#define MI_WRONG_ADDR             0x92
#define MI_DESKEYLOAD_FAILED      0x91
#define MI_WRONG_SEL_CNT          0x8E
#define MI_WRONG_TEST_MODE        0x8B
#define MI_TEST_FAILED            0x8A
#define MI_TOC_ERROR              0x89
#define MI_COMM_ABORT             0x88
#define MI_INVALID_BASE           0x87
#define MI_MFRC_RESET             0x86
#define MI_WRONG_VALUE            0x85
#define MI_VALERR                 0x84 // 0x84 钱包错误
#define MI_COM_ERR                0x83
#define MI_FATAL_ERR              0x82
#define MI_CONTINUE_ERR           0x81
#define MI_RESIDUAL_BIT_ERR       0x80

#define PCD_CMD_LOADKEY           PHHAL_HW_CV663_CMD_LOADKEY    // 自FIFO缓冲器读取一个 MIFARE 密钥（6个字节大小），并放入密钥缓冲器中
#define PCD_CMD_MFAUTHENT         PHHAL_HW_CV663_CMD_MFAUTHENT  // 仅以MIFARE读/写模式执行 MIFARE 标准认证
#define PCD_CMD_RECEIVE           PHHAL_HW_CV663_CMD_RECEIVE    // 启动接收电路
#define PCD_CMD_TRANSMIT          PHHAL_HW_CV663_CMD_TRANSMIT   // 由 FIFO 缓冲器传输数据
#define PCD_CMD_TRANSCEIVE        PHHAL_HW_CV663_CMD_TRANSCEIVE // 由 FIFO 缓冲器传输数据，且在传输结束后自动启动接收器

#define MI_OK                     0x00

#define MI_NOTAGERR               0xFF
#define MI_CRCERR                 0xFE
#define MI_EMPTY                  0xFD
#define MI_AUTHERR                0xFC
#define MI_PARITYERR              0xFB
#define MI_CODEERR                0xFA
#define MI_SERNRERR               0xF8 // Sernr error
#define MI_KEYERR                 0xF7
#define MI_NOTAUTHERR             0xF6
#define MI_BITCOUNTERR            0xF5
#define MI_BYTECOUNTERR           0xF4 // Byte counter error
#define MI_IDLE                   0xF3
#define MI_TRANSERR               0xF2
#define MI_WRITEERR               0xF1
#define MI_INCRERR                0xF0
#define MI_DECRERR                0xEF
#define MI_READERR                0xEE
#define MI_OVFLERR                0xED
#define MI_POLLING                0xEC
#define MI_FRAMINGERR             0xEB
#define MI_ACCESSERR              0xEA
#define MI_UNKNOWN_COMMAND        0xE9
#define MI_COLLERR                0xE8 // Collision error
#define MI_RESETERR               0xE7 // Reset error
#define MI_INTERFACEERR           0xE6 // Interface error
#define MI_ACCESSTIMEOUT          0xE5
#define MI_NOBITWISEANTICOLL      0xE4 // No bit wise anticoll
#define MI_QUIT                   0xE2
#define MI_RECBUF_OVERFLOW        0xCE
#define MI_SENDBYTENR             0xCD
#define MI_SENDBUF_OVERFLOW       0xCB
#define MI_BAUDRATE_NOT_SUPPORTED 0xCA
#define MI_SAME_BAUDRATE_REQUIRED 0xC9
#define MI_WRONG_PARAMETER_VALUE  0xC4
#define MI_BREAK                  0x9D
#define MI_NY_IMPLEMENTED         0x9C
#define MI_NO_MFRC                0x9B
#define MI_MFRC_NOTAUTH           0x9A
#define MI_WRONG_DES_MODE         0x99
#define MI_HOST_AUTH_FAILED       0x98
#define MI_WRONG_LOAD_MODE        0x96
#define MI_WRONG_DESKEY           0x95
#define MI_MKLOAD_FAILED          0x94
#define MI_FIFOERR                0x93
#define MI_WRONG_ADDR             0x92
#define MI_DESKEYLOAD_FAILED      0x91
#define MI_WRONG_SEL_CNT          0x8E
#define MI_WRONG_TEST_MODE        0x8B
#define MI_TEST_FAILED            0x8A
#define MI_TOC_ERROR              0x89
#define MI_COMM_ABORT             0x88
#define MI_INVALID_BASE           0x87
#define MI_MFRC_RESET             0x86
#define MI_WRONG_VALUE            0x85
#define MI_VALERR                 0x84
#define MI_COM_ERR                0x83
#define MI_FATAL_ERR              0x82
#define MI_CONTINUE_ERR           0x81
#define MI_RESIDUAL_BIT_ERR       0x80

/**
 * @brief Type Define
 *
 */
typedef struct
{
    unsigned char cType;
    unsigned char cPCB; // Block
    // unsigned char cFSC;
    unsigned char cFSD;
    unsigned char cFWI; // Frame wait intager
    unsigned char cCID; // 0x80:present; 0x00:absent;
    unsigned char cNAD; // 0x40:present; 0x00:absent;
    unsigned char cMaxINF;
} tpd_pcdpara;

typedef struct
{
    unsigned char iLength;
    unsigned char cData[512];
} tpd_pcdtrsv;

// Port Define
#define _WATER_LEVEL_   0x30

#define PCD_TYPE_14443A 0x1A
#define PCD_TYPE_14443B 0x1B

/**
 * @brief Port Define
 *
 */
#define _WATER_LEVEL_   0x30 // WATER LEVEL in RC5xx series card reader chip starts sending and reading FIFO bytes

#define PCD_TYPE_14443A 0x1A
#define PCD_TYPE_14443B 0x1B

void CV663_SPI_Init(void);
void CV663_Init(void);
unsigned char Read_CV663(unsigned char Address);
void Write_CV663(unsigned char Address, unsigned char cValue);
void SetBitMask_CV663(unsigned char cAddr, unsigned char cMask);
void ClearBitMask_CV663(unsigned char cAddr, unsigned char cMask);

unsigned char PcdReset_CV663(void);
unsigned char PcdConfig_CV663(unsigned char cType);
unsigned char PcdTransceive_CV663(unsigned char cCommand);

void PcdSetTmo_CV663(unsigned char tmoLength);
void PcdSetFSD_CV663(unsigned char cFSDI);

void PcdAntenna_CV663(unsigned char cSta);

void CV663_Read_Card(void);

#endif
