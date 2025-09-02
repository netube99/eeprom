/*
EEPROM读写 2023/02/25
zhongziming <netube@163.com>
https://github.com/netube99/eeprom
*/

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stdint.h"

//选择实际使用的EEPROM规格
//#define USE_24C01
#define USE_24C02
// #define USE_24C04
// #define USE_24C08
// #define USE_24C16
// #define USE_24C32
// #define USE_24C64
// #define USE_24C128
// #define USE_24C256

//选择地址引脚的实际连接情况
//0_下拉，1_上拉
#define A0  0
#define A1  0
#define A2  0

//以下部分为条件编译无需修改
#define EEPROM_SUCCESS          0
#define EEPROM_ERROR            1
#define CHIP_ID                 (0xA0 | (A2 << 3) | (A1 << 2) | (A0 << 1))

#ifdef USE_24C01
    #define MORE_REG_BITS       0           //寄存器高位地址占用ID地址多少bit
    #define REG_BYTES           1           //寄存器地址占用几个字节
    #define PAGE_BYTES          8           //一页有多少个字节
    #define MAX_ADDR            (128-1)     //最高可读写地址
    #define WRITE_TIME          5           //片内部处理数据时间
#endif

#ifdef USE_24C02
    #define MORE_REG_BITS       0
    #define REG_BYTES           1
    #define PAGE_BYTES          8
    #define MAX_ADDR            (256-1)
    #define WRITE_TIME          5
#endif

#ifdef USE_24C04
    #define MORE_REG_BITS       1
    #define REG_BYTES           1
    #define PAGE_BYTES          16
    #define MAX_ADDR            (512-1)
    #define WRITE_TIME          5
#endif

#ifdef USE_24C08
    #define MORE_REG_BITS       2
    #define REG_BYTES           1
    #define PAGE_BYTES          16
    #define MAX_ADDR            (1024-1)
    #define WRITE_TIME          5
#endif

#ifdef USE_24C16
    #define MORE_REG_BITS       3
    #define REG_BYTES           1
    #define PAGE_BYTES          16
    #define MAX_ADDR            (2048-1)
    #define WRITE_TIME          5
#endif

#ifdef USE_24C32
    #define MORE_REG_BITS       0
    #define REG_BYTES           2
    #define PAGE_BYTES          32
    #define MAX_ADDR            (4096-1)
    #define WRITE_TIME          10
#endif

#ifdef USE_24C64
    #define MORE_REG_BITS       0
    #define REG_BYTES           2
    #define PAGE_BYTES          32
    #define MAX_ADDR            (8192-1)
    #define WRITE_TIME          10
#endif

#ifdef USE_24C128
    #define MORE_REG_BITS       0
    #define REG_BYTES           2
    #define PAGE_BYTES          64
    #define MAX_ADDR            (16384-1)
    #define WRITE_TIME          10
#endif

#ifdef USE_24C256
    #define MORE_REG_BITS       0
    #define REG_BYTES           2
    #define PAGE_BYTES          64
    #define MAX_ADDR            (32768-1)
    #define WRITE_TIME          10
#endif

uint8_t Eeprom_Write(uint16_t rom_addr, uint8_t *pdata, uint16_t len);
uint8_t Eeprom_Read(uint16_t rom_addr, uint8_t *pdata, uint16_t len);

#endif