// A very stupid predictor.  It will always predict not taken.
#include <stdio.h>
#include "global_predictor.h"

/* Budget specific globals set during init. */
unsigned int global_size;
unsigned int global_bpt_idx_max;
unsigned int global_bpt_mid;
unsigned int global_bpt_max;
unsigned int *global_bpt;
global_table global;

static void global_set_budget_constants(budget_size budget)
{
	switch (budget) {
		case BUDGET_8K:
			global_size 	 = GLOBAL_SIZE_8K;
			global_bpt_idx_max = GLOBAL_BPT_IDX_MAX_8K;
			global_bpt_mid = GLOBAL_BPT_MID_8K;
			global_bpt_max = GLOBAL_BPT_MAX_8K;
			global_bpt     = global.bpt.b8k;
			break;

		case BUDGET_16K:
			global_size 	 = GLOBAL_SIZE_16K;
			global_bpt_idx_max = GLOBAL_BPT_IDX_MAX_16K;
			global_bpt_mid = GLOBAL_BPT_MID_16K;
			global_bpt_max = GLOBAL_BPT_MAX_16K;
			global_bpt     = global.bpt.b16k;
			break;

		case BUDGET_32K:
			global_size 	 = GLOBAL_SIZE_32K;
			global_bpt_idx_max = GLOBAL_BPT_IDX_MAX_32K;
			global_bpt_mid = GLOBAL_BPT_MID_32K;
			global_bpt_max = GLOBAL_BPT_MAX_32K;
			global_bpt     = global.bpt.b32k;
			break;

		case BUDGET_64K:
			global_size 	 = GLOBAL_SIZE_64K;
			global_bpt_idx_max = GLOBAL_BPT_IDX_MAX_64K;
			global_bpt_mid = GLOBAL_BPT_MID_64K;
			global_bpt_max = GLOBAL_BPT_MAX_64K;
			global_bpt     = global.bpt.b64k;
			break;

		case BUDGET_128K:
			global_size 	 = GLOBAL_SIZE_128K;
			global_bpt_idx_max = GLOBAL_BPT_IDX_MAX_128K;
			global_bpt_mid = GLOBAL_BPT_MID_128K;
			global_bpt_max = GLOBAL_BPT_MAX_128K;
			global_bpt     = global.bpt.b128k;
			break;

		case BUDGET_1M:
			global_size 	 = GLOBAL_SIZE_1M;
			global_bpt_idx_max = GLOBAL_BPT_IDX_MAX_1M;
			global_bpt_mid = GLOBAL_BPT_MID_1M;
			global_bpt_max = GLOBAL_BPT_MAX_1M;
			global_bpt     = global.bpt.b1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_global_predictor (budget_size budget)
{
	global_set_budget_constants(budget);	

	for (int i = 0; i < global_size; i++) {
		global_bpt[i] = global_bpt_max;
	}
}

bool make_global_prediction (unsigned int *ghr)
{
	unsigned int bpt_idx = (*ghr & global_bpt_idx_max);
	assert(bpt_idx <= global_bpt_idx_max);

	if (global_bpt[bpt_idx] > global_bpt_mid) {
		return true;
	} else {
		return false;
	}
}

void train_global_predictor (unsigned int *ghr, bool outcome)
{
	unsigned int bpt_idx = (*ghr & global_bpt_idx_max);
	assert(bpt_idx <= global_bpt_idx_max);

	if (outcome) {
		if (global_bpt[bpt_idx] < global_bpt_max) {
			global_bpt[bpt_idx]++;
		}

	} else {
		if (global_bpt[bpt_idx] > 0) {
			global_bpt[bpt_idx]--;
		}

	}
}
