#include "stdio.h"
#include "stdint.h"
#include "reg_macro.h"

typedef uint32_t reg32_t;

typedef enum
{
    CCR = 0x01013000,
} rxi316_addr_e;

#define CCR__F0__LSB 4
#define CCR__F0__SIZE 4

#define CCR__F1__LSB 8
#define CCR__F1__SIZE 8

void reg_write(uint32_t addr, uint32_t value)
{
    printf("write addr = 0x%0x, data = 0x%0x\n", addr, value);
}

uint32_t reg_read(u_int32_t addr)
{
    return addr;
}

int main()
{
    reg32_t ccr;

    ccr = 0xf0100c0;
    REG_WRITE(
        CCR, ccr,
        F0, 0x03,
        F1, 0xfd);

    REG_RMW(
        CCR,
        F0, 0x01,
        F1, 0x0c);
}