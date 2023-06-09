
// profile.h generated from Xinc_ble.gatt for BTstack
// att db format version 1

// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#include <stdint.h>

const uint8_t profile_data[] =
{
    // ATT DB Version
    1,

    // 0x0001 PRIMARY_SERVICE-GAP_SERVICE
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18, 
    // 0x0002 CHARACTERISTIC-GAP_DEVICE_NAME-READ
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a, 
    // 0x0003 VALUE-GAP_DEVICE_NAME-READ-'WeiKeJi'
    // READ_ANYBODY
    0x0f, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x2a, 0x57, 0x65, 0x69, 0x4b, 0x65, 0x4a, 0x69, 
    //OTA Service

    // 0x0004 PRIMARY_SERVICE-0000FF00-0000-1000-8000-00805F9B34FB
    0x18, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x28, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 
    // 0x0005 CHARACTERISTIC-0000FF01-0000-1000-8000-00805F9B34FB-WRITE | DYNAMIC
    0x1b, 0x00, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x08, 0x06, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
    // 0x0006 VALUE-0000FF01-0000-1000-8000-00805F9B34FB-WRITE | DYNAMIC-''
    // WRITE_ANYBODY
    0x16, 0x00, 0x08, 0x03, 0x06, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
    // Test Service

    // 0x0007 PRIMARY_SERVICE-0000FF10-0000-1000-8000-00805F9B34FB
    0x18, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x28, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x10, 0xff, 0x00, 0x00, 
    // Test Characteristic A, notify
    // 0x0008 CHARACTERISTIC-0000FF11-0000-1000-8000-00805F9B34FB-NOTIFY | DYNAMIC
    0x1b, 0x00, 0x02, 0x00, 0x08, 0x00, 0x03, 0x28, 0x10, 0x09, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x11, 0xff, 0x00, 0x00, 
    // 0x0009 VALUE-0000FF11-0000-1000-8000-00805F9B34FB-NOTIFY | DYNAMIC-''
    // 
    0x16, 0x00, 0x00, 0x03, 0x09, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x11, 0xff, 0x00, 0x00, 
    // 0x000a CLIENT_CHARACTERISTIC_CONFIGURATION
    // READ_ANYBODY, WRITE_ANYBODY
    0x0a, 0x00, 0x0e, 0x01, 0x0a, 0x00, 0x02, 0x29, 0x00, 0x00, 
    // Test Characteristic B, write
    // 0x000b CHARACTERISTIC-0000FF12-0000-1000-8000-00805F9B34FB-WRITE | DYNAMIC
    0x1b, 0x00, 0x02, 0x00, 0x0b, 0x00, 0x03, 0x28, 0x08, 0x0c, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0xff, 0x00, 0x00, 
    // 0x000c VALUE-0000FF12-0000-1000-8000-00805F9B34FB-WRITE | DYNAMIC-''
    // WRITE_ANYBODY
    0x16, 0x00, 0x08, 0x03, 0x0c, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0xff, 0x00, 0x00, 
    // Test Characteristic C, read
    // 0x000d CHARACTERISTIC-0000FF13-0000-1000-8000-00805F9B34FB-READ | DYNAMIC
    0x1b, 0x00, 0x02, 0x00, 0x0d, 0x00, 0x03, 0x28, 0x02, 0x0e, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x13, 0xff, 0x00, 0x00, 
    // 0x000e VALUE-0000FF13-0000-1000-8000-00805F9B34FB-READ | DYNAMIC-''
    // READ_ANYBODY
    0x16, 0x00, 0x02, 0x03, 0x0e, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x13, 0xff, 0x00, 0x00, 

    // END
    0x00, 0x00, 
}; // total size 181 bytes 


//
// list service handle ranges
//
#define ATT_SERVICE_GAP_SERVICE_START_HANDLE 0x0001
#define ATT_SERVICE_GAP_SERVICE_END_HANDLE 0x0003
#define ATT_SERVICE_0000FF00_0000_1000_8000_00805F9B34FB_START_HANDLE 0x0004
#define ATT_SERVICE_0000FF00_0000_1000_8000_00805F9B34FB_END_HANDLE 0x0006
#define ATT_SERVICE_0000FF10_0000_1000_8000_00805F9B34FB_START_HANDLE 0x0007
#define ATT_SERVICE_0000FF10_0000_1000_8000_00805F9B34FB_END_HANDLE 0x000e

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_GAP_DEVICE_NAME_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_0000FF01_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE 0x0006
#define ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE 0x0009
#define ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_CLIENT_CONFIGURATION_HANDLE 0x000a
#define ATT_CHARACTERISTIC_0000FF12_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE 0x000c
#define ATT_CHARACTERISTIC_0000FF13_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE 0x000e
