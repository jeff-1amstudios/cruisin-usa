#ifndef HUD_H
#define HUD_H

void HUD_SET_SECTION_HEADER(const unsigned int *header_words);
void SECTION_PALETTE_ALLOC(void);
void alloc_section(void);
void HARDalloc_section(void);
void dealloc_section(void);
void HUD(void);

#endif
