/*
EEPROM读写 2023/02/25
zhongziming <netube@163.com>
https://github.com/netube99/eeprom
*/

#include "eeprom.h"

#include "si2c.h"
#include "si2c_port.h"

#include "FreeRTOS.h"
#include "task.h"

void Eeprom_Delay(uint8_t ms)
{
    // HAL_Delay(ms);
    vTaskDelay(pdMS_TO_TICKS(ms));
}

uint8_t Eeprom_Write(uint16_t rom_addr, uint8_t *pdata, uint16_t len)
{
    // 如果目标地址溢出或者可写入空间不足
    if(len == 0)
        return EEPROM_ERROR;
    if((rom_addr > MAX_ADDR) || ((rom_addr + len - 1) > MAX_ADDR))
        return EEPROM_ERROR;
    //循环直至所有数据传输完毕
    while (len)
    {
        uint8_t id = CHIP_ID;
        // 计算从当前地址写到当前页最后地址总共有多少字节（写满一页）
        uint16_t num = PAGE_BYTES - (rom_addr % PAGE_BYTES);
        if(num > len) num = len;
        // 如果用到寄存器地址高位占用，重新计算芯片id（24C04/08/16）
        if(MORE_REG_BITS)
            id = ((id & (0xFF << (MORE_REG_BITS + 1))) | (rom_addr >> 7)) & 0xFE;
        // 根据芯片寄存器地址的字节数选择i2c写入函数
        uint8_t err = 0;
        if(REG_BYTES == 1)
            err = Si2c_Write(&si2c, id, (uint8_t)rom_addr, pdata, num, USE_REG_ADDR, SI2C_REG8);
        else
            err = Si2c_Write(&si2c, id, (uint16_t)rom_addr, pdata, num, USE_REG_ADDR, SI2C_REG16);
        // 检查操作结果
        if(err != SWI2C_TRUE)
            return EEPROM_ERROR;
        // 重新计算下次传输的参数
        pdata += num;
        rom_addr += num;
        len -= num;
        // 延时等待EEPROM芯片内部处理数据
        Eeprom_Delay(WRITE_TIME);
    }
    return EEPROM_SUCCESS;
}

uint8_t Eeprom_Read(uint16_t rom_addr, uint8_t *pdata, uint16_t len)
{
    // 如果目标地址溢出或者可读取空间不足
    if(len == 0)
        return EEPROM_ERROR;
    if ((rom_addr > MAX_ADDR) || ((rom_addr + len - 1) > MAX_ADDR))
        return EEPROM_ERROR;
    uint8_t id = CHIP_ID;
    // 如果用到寄存器地址高位占用，重新计算芯片id（24C04/08/16）
    if(MORE_REG_BITS)
        id = ((id & (0xFF << (MORE_REG_BITS + 1))) | (rom_addr >> 7)) & 0xFE;
    // 根据芯片寄存器地址的字节数选择i2c读取函数
    uint8_t err = 0;
    if(REG_BYTES == 1)
        err = Si2c_Read(&si2c, id, (uint8_t)rom_addr, pdata, len, USE_REG_ADDR, SI2C_REG8);
    else
        err = Si2c_Read(&si2c, id, (uint16_t)rom_addr, pdata, len, USE_REG_ADDR, SI2C_REG16);
    // 检查操作结果
    if(err != SWI2C_TRUE)
            return EEPROM_ERROR;
    return EEPROM_SUCCESS;
}