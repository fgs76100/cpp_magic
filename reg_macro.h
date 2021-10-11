#ifndef __REG_MACRO__
#define __REG_MACRO__
#include "marco_magic.h"

#define BIT_MASK(SIZE) ((1 << (SIZE)) - 1)

#define GET_REG_FIELD_SIZE(reg, field) reg##__##field##__SIZE
#define GET_REG_FIELD_LSB(reg, field) reg##__##field##__LSB

#define __ERROR__(...)                                              \
    do                                                              \
    {                                                               \
        printf("file: %s at line %d, [Error]", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                                        \
        puts("");                                                   \
    } while (0)

#define ASSERT_OVERFLOW(reg, field, value, ...)                                         \
    if (value & ~(BIT_MASK(GET_REG_FIELD_SIZE(reg, field))))                            \
    {                                                                                   \
        __ERROR__("[Overflow] " #reg "." #field ": value = 0x%x, but it's size is %0d", \
                  value, GET_REG_FIELD_SIZE(reg, field));                               \
    }

#define UNSET_REG_FIELD(reg, var, field, ...) \
    var &= ~(BIT_MASK(GET_REG_FIELD_SIZE(reg, field)) << (GET_REG_FIELD_LSB(reg, field)))

#define SET_REG_FIELD(reg, var, field, value, ...)     \
    UNSET_REG_FIELD(reg, var, field);                  \
    var |= (value) << (GET_REG_FIELD_LSB(reg, field)); \
    ASSERT_OVERFLOW(reg, field, value)

#define MAP_SET_REG(m, reg, var, first, second, ...)                                        \
    m(reg, var, first, second);                                                             \
    IF_ELSE(HAS_ARGS(__VA_ARGS__))                                                          \
    (                                                                                       \
        DEFER2(__MAP_SET_REG)()(m, reg, var, __VA_ARGS__))(/* Do nothing, just terminate */ \
    )
#define __MAP_SET_REG() MAP_SET_REG

#define REG_WRITE(reg, var, ...)                                 \
    {                                                            \
        EVAL(MAP_SET_REG(SET_REG_FIELD, reg, var, __VA_ARGS__)); \
        reg_write(reg, var);                                     \
    }

#define REG_RMW(reg, ...)                                            \
    {                                                                \
        uint32_t tmp_reg;                                            \
        tmp_reg = reg_read(reg);                                     \
        EVAL(MAP_SET_REG(SET_REG_FIELD, reg, tmp_reg, __VA_ARGS__)); \
        reg_write(reg, tmp_reg);                                     \
    }

#endif