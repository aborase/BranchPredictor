#ifndef ALPHA_PREDICTOR_H
#define ALPHA_PREDICTOR_H

#include "defs.h"
#include "global_predictor.h"
#include "tllp_predictor.h"

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
#define ALPHA_CPT_SIZE_8K 512
#define ALPHA_CPT_IDX_MAX_8K 0x000001ff
#define ALPHA_CPT_MID_8K 0x00000003
#define ALPHA_CPT_MAX_8K 0x00000007

/* For budget 16K */
#define ALPHA_CPT_SIZE_16K 1024
#define ALPHA_CPT_IDX_MAX_16K 0x000003ff
#define ALPHA_CPT_MID_16K 0x00000003
#define ALPHA_CPT_MAX_16K 0x00000007

/* For budget 32K */
#define ALPHA_CPT_SIZE_32K 2048
#define ALPHA_CPT_IDX_MAX_32K 0x000007ff
#define ALPHA_CPT_MID_32K 0x00000003
#define ALPHA_CPT_MAX_32K 0x00000007

/* for budget 64K */
#define ALPHA_CPT_SIZE_64K 4096
#define ALPHA_CPT_IDX_MAX_64K 0x00000fff
#define ALPHA_CPT_MID_64K 0x00000003
#define ALPHA_CPT_MAX_64K 0x00000007

/* For budget 128K */
#define ALPHA_CPT_SIZE_128K 8192
#define ALPHA_CPT_IDX_MAX_128K 0x00001fff
#define ALPHA_CPT_MID_128K 0x00000003
#define ALPHA_CPT_MAX_128K 0x00000007

/* For budget 1M */
#define ALPHA_CPT_SIZE_1M 65536
#define ALPHA_CPT_IDX_MAX_1M 0x0000ffff
#define ALPHA_CPT_MID_1M 0x00000003
#define ALPHA_CPT_MAX_1M 0x00000007

typedef struct {
	unsigned int ghr;
	union {
		unsigned int c8k[ALPHA_CPT_SIZE_8K];
		unsigned int c16k[ALPHA_CPT_SIZE_16K];
		unsigned int c32k[ALPHA_CPT_SIZE_32K];
		unsigned int c64k[ALPHA_CPT_SIZE_64K];
		unsigned int c128k[ALPHA_CPT_SIZE_128K];
		unsigned int c1m[ALPHA_CPT_SIZE_1M];
	} cpt;
} alpha_table;

/*
  Initialize the predictor.
*/
void init_alpha_predictor (budget_size budget);

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_alpha_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_alpha_predictor (unsigned int pc, bool outcome);

#endif
