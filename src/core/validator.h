#ifndef CRUSN_VALIDATOR_H
#define CRUSN_VALIDATOR_H

#include <stdint.h>

void mame_validate_word_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_validate_arg_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_validate_arg_float_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
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
void mame_validate_reg_at_addr_impl(
    const char* caller_file, int caller_line, uint32_t breakpoint_address, const char* reg_name, const void* ptr);
void mame_validate_reg_at_addr_float_impl(
    const char* caller_file, int caller_line, uint32_t breakpoint_address, const char* reg_name, const void* ptr);

#define mame_validate_word(name, ptr) mame_validate_word_impl(__FILE__, __LINE__, (name), (ptr))
#define MAME_VALIDATE_ARG(name, ptr) mame_validate_arg_impl(__FILE__, __LINE__, (name), (ptr))
#define MAME_VALIDATE_ARG_FLOAT(name, ptr) mame_validate_arg_float_impl(__FILE__, __LINE__, (name), (ptr))
#define MAME_VALIDATE_EXIT() mame_validate_exit_impl(__FILE__, __LINE__)
#define mame_validate_arg_sym(name, ptr) mame_validate_arg_sym_impl(__FILE__, __LINE__, (name), (ptr))
#define MAME_VALIDATE_REGION_AT_ADDR(addr, name, region_addr, ptr, word_count) \
    mame_validate_region_at_addr_impl(__FILE__, __LINE__, (addr), (name), (region_addr), (ptr), (word_count))
#define MAME_VALIDATE_REG_AT_ADDR(addr, reg_name, ptr) \
    mame_validate_reg_at_addr_impl(__FILE__, __LINE__, (addr), (reg_name), (ptr))
#define MAME_VALIDATE_REG_AT_ADDR_FLOAT(addr, reg_name, ptr) \
    mame_validate_reg_at_addr_float_impl(__FILE__, __LINE__, (addr), (reg_name), (ptr))

void mame_validate_print_oks_on(void);
void mame_validate_print_oks_off(void);
void mame_validate_abort_on_error(int abort);
void mame_validate_fail_on_wrong_consumer(int enabled);

#endif
