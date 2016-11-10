#ifndef GSHARE_PREDICTOR_H
#define GSHARE_PREDICTOR_H

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "defs.h"

/*
  Define all your tables and their sizes here.
  All tables must be statically-sized.
  Please don't call malloc () or any of its relatives within your
  program.  The predictor will have a budget, namely (32K + 256) bits
  (not bytes).  That encompasses all storage (all tables, plus GHR, if
  necessary -- but not PC) used for the predictor.  That may not be
  the amount of storage your predictor uses, however -- for example,
  you may implement 2-bit predictors with a table of ints, in which
  case the simulator will use more memory -- that's okay, we're only
  concerned about the memory used by the simulated branch predictor.
*/

/* For budget 8K */
#define GSHARE_SIZE_8K 2048
#define GSHARE_GHR_LSB_8K 0x0000003f
#define GSHARE_ADDR_SZ_8K 5
#define GSHARE_BPT_IDX_MAX_8K 0x000007ff
#define GSHARE_BPT_MID_8K 0x00000007
#define GSHARE_BPT_MAX_8K 0x0000000f

/* For budget 16K */
#define GSHARE_SIZE_16K 4096
#define GSHARE_GHR_LSB_16K 0x0000003f
#define GSHARE_ADDR_SZ_16K 6
#define GSHARE_BPT_IDX_MAX_16K 0x00000fff
#define GSHARE_BPT_MID_16K 0x00000007
#define GSHARE_BPT_MAX_16K 0x0000000f

/* For budget 32K */
#define GSHARE_SIZE_32K 8192
#define GSHARE_GHR_LSB_32K 0x0000007f
#define GSHARE_ADDR_SZ_32K 6
#define GSHARE_BPT_IDX_MAX_32K 0x00001fff
#define GSHARE_BPT_MID_32K 0x00000007
#define GSHARE_BPT_MAX_32K 0x0000000f

/* for budget 64K */
#define GSHARE_SIZE_64K 16384
#define GSHARE_GHR_LSB_64K 0x0000007f
#define GSHARE_ADDR_SZ_64K 7
#define GSHARE_BPT_IDX_MAX_64K 0x00003fff
#define GSHARE_BPT_MID_64K 0x00000007
#define GSHARE_BPT_MAX_64K 0x0000000f

/* For budget 128K */
#define GSHARE_SIZE_128K 32768
#define GSHARE_GHR_LSB_128K 0x000000ff
#define GSHARE_ADDR_SZ_128K 7
#define GSHARE_BPT_IDX_MAX_128K 0x00007fff
#define GSHARE_BPT_MID_128K 0x00000007
#define GSHARE_BPT_MAX_128K 0x0000000f

/* For budget 1M */
#define GSHARE_SIZE_1M 262144
#define GSHARE_GHR_LSB_1M 0x000001ff
#define GSHARE_ADDR_SZ_1M 9
#define GSHARE_BPT_IDX_MAX_1M 0x0003ffff
#define GSHARE_BPT_MID_1M 0x00000007
#define GSHARE_BPT_MAX_1M 0x0000000f

typedef struct {
/*	union {
		unsigned int g8k;
		unsigned int g16k;
		unsigned int g32k;
		unsigned int g64k;
		unsigned int g128k;
		unsigned int g1m;
	} ghr;
*/
	unsigned int ghr;
	union {
		unsigned int b8k[GSHARE_SIZE_8K];
		unsigned int b16k[GSHARE_SIZE_16K];
		unsigned int b32k[GSHARE_SIZE_32K];
		unsigned int b64k[GSHARE_SIZE_64K];
		unsigned int b128k[GSHARE_SIZE_128K];
		unsigned int b1m[GSHARE_SIZE_1M];
	} bpt;
} gshare_table;

/*
  Initialize the predictor.
*/
void init_gshare_predictor (budget_size budget);

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_gshare_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_gshare_predictor (unsigned int pc, bool outcome);

#endif
