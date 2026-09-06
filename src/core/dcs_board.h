#ifndef CRUSN_DCS_BOARD_H
#define CRUSN_DCS_BOARD_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { DCS_BOARD_SAMPLE_RATE = 31250 };

typedef struct dcs_board dcs_board;

dcs_board* dcs_board_create(const char* zip_path, char* error, size_t error_size);
void dcs_board_destroy(dcs_board* board);
void dcs_board_send_command(dcs_board* board, uint16_t command);
int16_t dcs_board_next_sample(dcs_board* board);

#ifdef __cplusplus
}
#endif

#endif
