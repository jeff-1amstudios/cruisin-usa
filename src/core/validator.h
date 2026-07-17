#ifndef CRUSN_VALIDATOR_H
#define CRUSN_VALIDATOR_H

#include <stdint.h>

typedef enum MAME_VALIDATE_REG_KIND {
    MAME_VALIDATE_REG_KIND_WORD,
    MAME_VALIDATE_REG_KIND_FLOAT,
} MAME_VALIDATE_REG_KIND;

void mame_validate_arg_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_assert_arg_float_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_validate_arg_sym_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_validate_exit_impl(const char* caller_file, int caller_line);
void mame_validate_region_at_addr_impl(
    const char* caller_file,
    int caller_line,
    uint32_t breakpoint_address,
    const char* name,
    uint32_t region_address,
    const void* ptr,
    uint32_t word_count);
void mame_assert_reg_at_addr_impl(
    const char* caller_file,
    int caller_line,
    uint32_t breakpoint_address,
    const char* reg_name,
    const void* ptr,
    MAME_VALIDATE_REG_KIND reg_kind,
    uint32_t wiggle_room);

#define mame_validate_arg_sym(name, ptr) mame_validate_arg_sym_impl(__FILE__, __LINE__, (name), (ptr))

#define MAME_ASSERT_ARG(name, ptr) mame_validate_arg_impl(__FILE__, __LINE__, (name), (ptr))
#define MAME_ASSERT_ARG_FLOAT(name, ptr) mame_assert_arg_float_impl(__FILE__, __LINE__, (name), (ptr))
#define MAME_VALIDATOR_EXIT() mame_validate_exit_impl(__FILE__, __LINE__)
#define MAME_ASSERT_REGION_AT_ADDR(addr, name, region_addr, ptr, word_count) \
    mame_validate_region_at_addr_impl(__FILE__, __LINE__, (addr), (name), (region_addr), (ptr), (word_count))
#define MAME_ASSERT_REG(addr, reg_name, ptr) \
    mame_assert_reg_at_addr_impl(__FILE__, __LINE__, (addr), (reg_name), (ptr), MAME_VALIDATE_REG_KIND_WORD, 0)
#define MAME_ASSERT_REG_WIGGLE(addr, reg_name, ptr, wiggle_room) \
    mame_assert_reg_at_addr_impl(__FILE__, __LINE__, (addr), (reg_name), (ptr), MAME_VALIDATE_REG_KIND_WORD, (wiggle_room))
#define MAME_ASSERT_REG_FLOAT(addr, reg_name, ptr) \
    mame_assert_reg_at_addr_impl(__FILE__, __LINE__, (addr), (reg_name), (ptr), MAME_VALIDATE_REG_KIND_FLOAT, 0)
#define MAME_ASSERT_MEM(addr, mem_addr, ptr) \
    mame_assert_reg_at_addr_impl(__FILE__, __LINE__, (addr), (mem_addr), (ptr), MAME_VALIDATE_REG_KIND_WORD, 0)
#define MAME_ASSERT_MEM_FLOAT(addr, mem_addr, ptr) \
    mame_assert_reg_at_addr_impl(__FILE__, __LINE__, (addr), (mem_addr), (ptr), MAME_VALIDATE_REG_KIND_FLOAT, 0)

#define MAME_ASSERT_FUNCTION_ENTRY()

void mame_validate_print_oks_on(void);
void mame_validate_print_oks_off(void);
void mame_validate_abort_on_error(int abort);
void mame_validate_fail_on_wrong_consumer(int enabled);

#endif
