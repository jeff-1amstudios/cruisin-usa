#ifndef CRUSN_VALIDATOR_H
#define CRUSN_VALIDATOR_H

#include <stdint.h>

void mame_validate_word_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_validate_arg_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
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

#define mame_validate_word(name, ptr) mame_validate_word_impl(__FILE__, __LINE__, (name), (ptr))
#define validate_word(name, ptr) mame_validate_word((name), (ptr))
#define mame_validate_arg(name, ptr) mame_validate_arg_impl(__FILE__, __LINE__, (name), (ptr))
#define mame_validate_region_at_addr(addr, name, region_addr, ptr, word_count) \
    mame_validate_region_at_addr_impl(__FILE__, __LINE__, (addr), (name), (region_addr), (ptr), (word_count))
#define mame_validate_reg_at_addr(addr, reg_name, ptr) \
    mame_validate_reg_at_addr_impl(__FILE__, __LINE__, (addr), (reg_name), (ptr))

void mame_validate_print_oks_on(void);
void mame_validate_print_oks_off(void);

#endif
