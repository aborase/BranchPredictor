#ifndef TLLP_PREDICTOR_H
#define TLLP_PREDICTOR_H

#include <stdint.h>
#include <string.h>
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
#define TLLP_SIZE_8K 512
#define TLLP_BPT_MID_8K 0x0000003f
#define TLLP_LHT_MAX_8K 0x000001ff
#define TLLP_BPT_MAX_8K 0x0000007f

/* For budget 16K */
#define TLLP_SIZE_16K 1024
#define TLLP_BPT_MID_16K 0x0000001f
#define TLLP_LHT_MAX_16K 0x000003ff
#define TLLP_BPT_MAX_16K 0x0000003f

/* For budget 32K */
#define TLLP_SIZE_32K 2048
#define TLLP_BPT_MID_32K 0x0000000f
#define TLLP_LHT_MAX_32K 0x000007ff
#define TLLP_BPT_MAX_32K 0x0000001f

/* for budget 64K */
#define TLLP_SIZE_64K 4096
#define TLLP_BPT_MID_64K 0x00000007
#define TLLP_LHT_MAX_64K 0x00000fff
#define TLLP_BPT_MAX_64K 0x0000000f

/* For budget 128K */
#define TLLP_SIZE_128K 8192
#define TLLP_BPT_MID_128K 0x00000003
#define TLLP_LHT_MAX_128K 0x00001fff
#define TLLP_BPT_MAX_128K 0x00000007

/* For budget 1M */
#define TLLP_SIZE_1M 32768
#define TLLP_BPT_MID_1M 0x00003fff
#define TLLP_LHT_MAX_1M 0x00007fff
#define TLLP_BPT_MAX_1M 0x00007fff

typedef struct {
	union {
		unsigned int b8k[TLLP_SIZE_8K];
		unsigned int b16k[TLLP_SIZE_16K];
		unsigned int b32k[TLLP_SIZE_32K];
		unsigned int b64k[TLLP_SIZE_64K];
		unsigned int b128k[TLLP_SIZE_128K];
		unsigned int b1m[TLLP_SIZE_1M];
	} lht;
	union {
		unsigned int b8k[TLLP_SIZE_8K];
		unsigned int b16k[TLLP_SIZE_16K];
		unsigned int b32k[TLLP_SIZE_32K];
		unsigned int b64k[TLLP_SIZE_64K];
		unsigned int b128k[TLLP_SIZE_128K];
		unsigned int b1m[TLLP_SIZE_1M];
	} bpt;
} tllp_table;

/*
  Initialize the predictor.
*/
void init_tllp_predictor (budget_size budget);

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_tllp_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_tllp_predictor (unsigned int pc, bool outcome);

#endif
