#ifndef DEFS_H
#define DEFS_H

/* Common definitions required */

/* Budget Sizes */
typedef enum {
	BUDGET_8K = 1,
	BUDGET_16K,
	BUDGET_32K,
	BUDGET_64K,
	BUDGET_128K,
	BUDGET_1M,
	BUDGET_MAX
} budget_size;

/* Prediction Algorithms */
typedef enum {
	TLLP = 1,
	ALPHA21264,
	PERCEPTRON,
	GSHARE,
	PRED_MAX
} pred_type;

typedef struct pred_fptr {
    void (*init_predictor) (budget_size);
	bool (*make_prediction) (unsigned int);
	void (*train_predictor) (unsigned int, bool);
} pred_fptr;

#endif
