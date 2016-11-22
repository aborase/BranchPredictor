#ifndef PERCEPTRON_PREDICTOR_H
#define PERCEPTRON_PREDICTOR_H

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
#define PERCEPTRON_SIZE_8K 256
#define PERCEPTRON_PT_IDX_MAX_8K 0x000000ff
#define PERCEPTRON_WEIGHT_CNT_8K 4
#define PERCEPTRON_THRESHOLD_8K 128

/* For budget 16K */
#define PERCEPTRON_SIZE_16K 256
#define PERCEPTRON_PT_IDX_MAX_16K 0x000000ff
#define PERCEPTRON_WEIGHT_CNT_16K 8
#define PERCEPTRON_THRESHOLD_16K 128

/* For budget 32K */
#define PERCEPTRON_SIZE_32K 512
#define PERCEPTRON_PT_IDX_MAX_32K 0x000001ff
#define PERCEPTRON_WEIGHT_CNT_32K 8
#define PERCEPTRON_THRESHOLD_32K 128

/* for budget 64K */
#define PERCEPTRON_SIZE_64K 512
#define PERCEPTRON_PT_IDX_MAX_64K 0x000001ff
#define PERCEPTRON_WEIGHT_CNT_64K 16
#define PERCEPTRON_THRESHOLD_64K 128

/* For budget 128K */
#define PERCEPTRON_SIZE_128K 1024
#define PERCEPTRON_PT_IDX_MAX_128K 0x000003ff
#define PERCEPTRON_WEIGHT_CNT_128K 16
#define PERCEPTRON_THRESHOLD_128K 128

/* For budget 1M */
#define PERCEPTRON_SIZE_1M 4096
#define PERCEPTRON_PT_IDX_MAX_1M 0x0000fff
#define PERCEPTRON_WEIGHT_CNT_1M 32
#define PERCEPTRON_THRESHOLD_1M 128

typedef struct {
	int weights[PERCEPTRON_WEIGHT_CNT_1M];
} weight_set;

typedef struct {
	unsigned int ghr;
	union {
		 weight_set p8k[PERCEPTRON_SIZE_8K];
		 weight_set p16k[PERCEPTRON_SIZE_16K];
		 weight_set p32k[PERCEPTRON_SIZE_32K];
		 weight_set p64k[PERCEPTRON_SIZE_64K];
		 weight_set p128k[PERCEPTRON_SIZE_128K];
		 weight_set p1m[PERCEPTRON_SIZE_1M];
	} pt;
} perceptron_table;


/*
  Initialize the predictor.
*/
void init_perceptron_predictor (budget_size budget);

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_perceptron_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_perceptron_predictor (unsigned int pc, bool outcome);

#endif

