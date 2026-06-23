#ifndef CRUSN_VALIDATOR_H
#define CRUSN_VALIDATOR_H

void mame_validate_word_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);
void mame_validate_arg_impl(const char* caller_file, int caller_line, const char* name, const void* ptr);

#define mame_validate_word(name, ptr) mame_validate_word_impl(__FILE__, __LINE__, (name), (ptr))
#define validate_word(name, ptr) mame_validate_word((name), (ptr))
#define mame_validate_arg(name, ptr) mame_validate_arg_impl(__FILE__, __LINE__, (name), (ptr))

#endif
