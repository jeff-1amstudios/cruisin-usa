#ifndef CMOS_H
#define CMOS_H

int ADJUSTMENT_READ(int adj_index);
void VALIDATE_CMOS(void);
void AUDIT_WRITE(void);
void INIT_HSTD_TABLES(void);
void INIT_LASTHS_TABLE(void);

#endif
