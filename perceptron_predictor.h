#ifndef PERCEPTRON_PREDICTOR_H
#define PERCEPTRON_PREDICTOR_H

#include <stdint.h>
#include <string.h>
#include <stdint.h>
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
#define PERCEPTRON_SIZE_8K 85 //256
#define PERCEPTRON_PT_IDX_MAX_8K 85 //0x000000ff
#define PERCEPTRON_WEIGHT_CNT_8K 12 //8 
#define PERCEPTRON_THRESHOLD_8K 36 //15
#define PERCEPTRON_WEIGHT_MAX_8K 127 //7
#define PERCEPTRON_WEIGHT_MIN_8K -127 //-7

/* For budget 16K */
#define PERCEPTRON_SIZE_16K 128 //256
#define PERCEPTRON_PT_IDX_MAX_16K 128 //0x000000ff
#define PERCEPTRON_WEIGHT_CNT_16K 16 //12
#define PERCEPTRON_THRESHOLD_16K 44 //25
#define PERCEPTRON_WEIGHT_MAX_16K 127 //15
#define PERCEPTRON_WEIGHT_MIN_16K -127 //-15

/* For budget 32K */
#define PERCEPTRON_SIZE_32K 170 //256
#define PERCEPTRON_PT_IDX_MAX_32K 170 //0x000000ff
#define PERCEPTRON_WEIGHT_CNT_32K 24 //20
#define PERCEPTRON_THRESHOLD_32K 60 //40
#define PERCEPTRON_WEIGHT_MAX_32K 127 //31
#define PERCEPTRON_WEIGHT_MIN_32K -127 //-31

/* for budget 64K */
#define PERCEPTRON_SIZE_64K 292 //512
#define PERCEPTRON_PT_IDX_MAX_64K 292 //0x000001ff
#define PERCEPTRON_WEIGHT_CNT_64K 28 //20
#define PERCEPTRON_THRESHOLD_64K 68 //40
#define PERCEPTRON_WEIGHT_MAX_64K 127 //31
#define PERCEPTRON_WEIGHT_MIN_64K -127 //-31

/* For budget 128K */
#define PERCEPTRON_SIZE_128K 340 //512
#define PERCEPTRON_PT_IDX_MAX_128K 340 //0x000001ff
#define PERCEPTRON_WEIGHT_CNT_128K 48 //36
#define PERCEPTRON_THRESHOLD_128K 115 //75
#define PERCEPTRON_WEIGHT_MAX_128K 127 //63
#define PERCEPTRON_WEIGHT_MIN_128K -127 //-63

/* For budget 1M */
#define PERCEPTRON_SIZE_1M 1024 //2048
#define PERCEPTRON_PT_IDX_MAX_1M 1024 //0x000007ff
#define PERCEPTRON_WEIGHT_CNT_1M 128 //64
#define PERCEPTRON_THRESHOLD_1M 270 //130
#define PERCEPTRON_WEIGHT_MAX_1M 127
#define PERCEPTRON_WEIGHT_MIN_1M -127

typedef struct {
	int weights[PERCEPTRON_WEIGHT_CNT_1M];
} weight_set;

typedef struct {
	unsigned long long int ghr;
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

