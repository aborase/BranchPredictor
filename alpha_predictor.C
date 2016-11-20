// A very stupid predictor.  It will always predict not taken.
#include <stdio.h>
#include "alpha_predictor.h"

/* Budget specific globals set during init. */
unsigned int alpha_cpt_size;
unsigned int alpha_cpt_idx_max;
unsigned int alpha_cpt_mid;
unsigned int alpha_cpt_max;
unsigned int *alpha_cpt;
alpha_table alpha;

static void alpha_set_budget_constants(budget_size budget)
{
	switch (budget) {
		case BUDGET_8K:
			alpha_cpt_size 	 = ALPHA_CPT_SIZE_8K;
			alpha_cpt_idx_max = ALPHA_CPT_IDX_MAX_8K;
			alpha_cpt_mid = ALPHA_CPT_MID_8K;
			alpha_cpt_max = ALPHA_CPT_MAX_8K;
			alpha_cpt     = alpha.cpt.c8k;
			break;

		case BUDGET_16K:
			alpha_cpt_size 	 = ALPHA_CPT_SIZE_16K;
			alpha_cpt_idx_max = ALPHA_CPT_IDX_MAX_16K;
			alpha_cpt_mid = ALPHA_CPT_MID_16K;
			alpha_cpt_max = ALPHA_CPT_MAX_16K;
			alpha_cpt     = alpha.cpt.c16k;
			break;

		case BUDGET_32K:
			alpha_cpt_size 	 = ALPHA_CPT_SIZE_32K;
			alpha_cpt_idx_max = ALPHA_CPT_IDX_MAX_32K;
			alpha_cpt_mid = ALPHA_CPT_MID_32K;
			alpha_cpt_max = ALPHA_CPT_MAX_32K;
			alpha_cpt     = alpha.cpt.c32k;
			break;

		case BUDGET_64K:
			alpha_cpt_size 	 = ALPHA_CPT_SIZE_64K;
			alpha_cpt_idx_max = ALPHA_CPT_IDX_MAX_64K;
			alpha_cpt_mid = ALPHA_CPT_MID_64K;
			alpha_cpt_max = ALPHA_CPT_MAX_64K;
			alpha_cpt     = alpha.cpt.c64k;
			break;

		case BUDGET_128K:
			alpha_cpt_size 	 = ALPHA_CPT_SIZE_128K;
			alpha_cpt_idx_max = ALPHA_CPT_IDX_MAX_128K;
			alpha_cpt_mid = ALPHA_CPT_MID_128K;
			alpha_cpt_max = ALPHA_CPT_MAX_128K;
			alpha_cpt     = alpha.cpt.c128k;
			break;

		case BUDGET_1M:
			alpha_cpt_size 	 = ALPHA_CPT_SIZE_1M;
			alpha_cpt_idx_max = ALPHA_CPT_IDX_MAX_1M;
			alpha_cpt_mid = ALPHA_CPT_MID_1M;
			alpha_cpt_max = ALPHA_CPT_MAX_1M;
			alpha_cpt     = alpha.cpt.c1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_alpha_predictor (budget_size budget)
{
	/* Init both global and 2-level local predictor */
	init_tllp_alpha_predictor(budget);
	init_global_predictor(budget);

	alpha_set_budget_constants(budget);	

	alpha.ghr = 0x0;
	for (int i = 0; i < alpha_cpt_size; i++) {
		alpha_cpt[i] = alpha_cpt_max;
	}
}

bool make_alpha_prediction (unsigned int pc)
{
	unsigned int cpt_idx = (alpha.ghr & alpha_cpt_idx_max);
	assert(cpt_idx <= alpha_cpt_idx_max);

	if (alpha_cpt[cpt_idx] > alpha_cpt_mid) {
		return make_global_prediction(&(alpha.ghr));
	} else {
		return make_tllp_prediction(pc);
	}
}

void train_alpha_predictor (unsigned int pc, bool outcome)
{
	unsigned int cpt_idx = (alpha.ghr & alpha_cpt_idx_max);
	assert(cpt_idx <= alpha_cpt_idx_max);

	/* Train both predictors before we train choice table */
	train_tllp_predictor(pc, outcome);
	train_global_predictor(&(alpha.ghr), outcome);

	if (outcome) {
		if (alpha_cpt[cpt_idx] < alpha_cpt_max) {
			alpha_cpt[cpt_idx]++;
		}

		alpha.ghr = ((alpha.ghr << 1) | 0x01);
	} else {
		if (alpha_cpt[cpt_idx] > 0) {
			alpha_cpt[cpt_idx]--;
		}

		alpha.ghr = ((alpha.ghr << 1) | 0x00);
	}
}
