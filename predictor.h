#ifndef PREDICTOR_H
#define PREDICTOR_H

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

/* For budget 64K */
#define TLLP_SIZE_64K 4096
#define TLLP_BPT_MID_64K 0x00000007
#define TLLP_LHT_MAX_64K 0x00000fff
#define TLLP_BPT_MAX_64K 0x0000000f

typedef struct {
	union {
		unsigned int b8k[TLLP_SIZE_8K];
		unsigned int b16k[TLLP_SIZE_16K];
		unsigned int b32k[TLLP_SIZE_32K];
		unsigned int b64k[TLLP_SIZE_64K];
	} lht;
	union {
		unsigned int b8k[TLLP_SIZE_8K];
		unsigned int b16k[TLLP_SIZE_16K];
		unsigned int b32k[TLLP_SIZE_32K];
		unsigned int b64k[TLLP_SIZE_64K];	
	} bpt;
} tllp_table;

/*
  Initialize the predictor.
*/
void init_predictor (budget_size budget);

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_predictor (unsigned int pc, bool outcome);

#endif
