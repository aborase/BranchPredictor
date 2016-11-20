// A very stupid predictor.  It will always predict not taken.
#include <stdio.h>
#include <assert.h>
#include "tllp_predictor.h"

/* Budget specific globals set during init. */
unsigned int tllp_lht_size;
unsigned int tllp_bpt_size;
unsigned int tllp_bpt_mid;
unsigned int tllp_bpt_max;
unsigned int tllp_bpt_idx_max;
unsigned int tllp_lht_idx_max;
unsigned int *tllp_lht;
unsigned int *tllp_bpt;
tllp_table tllp;

static void tllp_set_budget_constants(budget_size budget, bool is_alpha)
{
	switch (budget) {
		case BUDGET_8K:
			tllp_lht_size		= is_alpha ? TLLP_ALPHA_LHT_SIZE_8K : TLLP_LHT_SIZE_8K;
			tllp_bpt_size		= is_alpha ? TLLP_ALPHA_BPT_SIZE_8K : TLLP_BPT_SIZE_8K;
			tllp_bpt_mid		= is_alpha ? TLLP_ALPHA_BPT_MID_8K : TLLP_BPT_MID_8K;
			tllp_bpt_max		= is_alpha ? TLLP_ALPHA_BPT_MAX_8K : TLLP_BPT_MAX_8K;
			tllp_bpt_idx_max	= is_alpha ? TLLP_ALPHA_BPT_IDX_MAX_8K : TLLP_BPT_IDX_MAX_8K;
			tllp_lht_idx_max	= is_alpha ? TLLP_ALPHA_LHT_IDX_MAX_8K : TLLP_LHT_IDX_MAX_8K;
			tllp_lht			= tllp.lht.b8k;
			tllp_bpt			= tllp.bpt.b8k;
			break;

		case BUDGET_16K:
			tllp_lht_size		= is_alpha ? TLLP_ALPHA_LHT_SIZE_16K : TLLP_LHT_SIZE_16K;
			tllp_bpt_size		= is_alpha ? TLLP_ALPHA_BPT_SIZE_16K : TLLP_BPT_SIZE_16K;
			tllp_bpt_mid		= is_alpha ? TLLP_ALPHA_BPT_MID_16K : TLLP_BPT_MID_16K;
			tllp_bpt_max		= is_alpha ? TLLP_ALPHA_BPT_MAX_16K : TLLP_BPT_MAX_16K;
			tllp_bpt_idx_max	= is_alpha ? TLLP_ALPHA_BPT_IDX_MAX_16K : TLLP_BPT_IDX_MAX_16K;
			tllp_lht_idx_max	= is_alpha ? TLLP_ALPHA_LHT_IDX_MAX_16K : TLLP_LHT_IDX_MAX_16K;
			tllp_lht			= tllp.lht.b16k;
			tllp_bpt			= tllp.bpt.b16k;
			break;

		case BUDGET_32K:
			tllp_lht_size		= is_alpha ? TLLP_ALPHA_LHT_SIZE_32K : TLLP_LHT_SIZE_32K;
			tllp_bpt_size		= is_alpha ? TLLP_ALPHA_BPT_SIZE_32K : TLLP_BPT_SIZE_32K;
			tllp_bpt_mid		= is_alpha ? TLLP_ALPHA_BPT_MID_32K : TLLP_BPT_MID_32K;
			tllp_bpt_max		= is_alpha ? TLLP_ALPHA_BPT_MAX_32K : TLLP_BPT_MAX_32K;
			tllp_bpt_idx_max	= is_alpha ? TLLP_ALPHA_BPT_IDX_MAX_32K : TLLP_BPT_IDX_MAX_32K;
			tllp_lht_idx_max	= is_alpha ? TLLP_ALPHA_LHT_IDX_MAX_32K : TLLP_LHT_IDX_MAX_32K;
			tllp_lht			= tllp.lht.b32k;
			tllp_bpt			= tllp.bpt.b32k;
			break;

		case BUDGET_64K:
			tllp_lht_size		= is_alpha ? TLLP_ALPHA_LHT_SIZE_64K : TLLP_LHT_SIZE_64K;
			tllp_bpt_size		= is_alpha ? TLLP_ALPHA_BPT_SIZE_64K : TLLP_BPT_SIZE_64K;
			tllp_bpt_mid		= is_alpha ? TLLP_ALPHA_BPT_MID_64K : TLLP_BPT_MID_64K;
			tllp_bpt_max		= is_alpha ? TLLP_ALPHA_BPT_MAX_64K : TLLP_BPT_MAX_64K;
			tllp_bpt_idx_max	= is_alpha ? TLLP_ALPHA_BPT_IDX_MAX_64K : TLLP_BPT_IDX_MAX_64K;
			tllp_lht_idx_max	= is_alpha ? TLLP_ALPHA_LHT_IDX_MAX_64K : TLLP_LHT_IDX_MAX_64K;
			tllp_lht			= tllp.lht.b64k;
			tllp_bpt			= tllp.bpt.b64k;
			break;

		case BUDGET_128K:
			tllp_lht_size		= is_alpha ? TLLP_ALPHA_LHT_SIZE_128K : TLLP_LHT_SIZE_128K;
			tllp_bpt_size		= is_alpha ? TLLP_ALPHA_BPT_SIZE_128K : TLLP_BPT_SIZE_128K;
			tllp_bpt_mid		= is_alpha ? TLLP_ALPHA_BPT_MID_128K : TLLP_BPT_MID_128K;
			tllp_bpt_max		= is_alpha ? TLLP_ALPHA_BPT_MAX_128K : TLLP_BPT_MAX_128K;
			tllp_bpt_idx_max	= is_alpha ? TLLP_ALPHA_BPT_IDX_MAX_128K : TLLP_BPT_IDX_MAX_128K;
			tllp_lht_idx_max	= is_alpha ? TLLP_ALPHA_LHT_IDX_MAX_128K : TLLP_LHT_IDX_MAX_128K;
			tllp_lht			= tllp.lht.b128k;
			tllp_bpt			= tllp.bpt.b128k;
			break;

		case BUDGET_1M:
			tllp_lht_size		= is_alpha ? TLLP_ALPHA_LHT_SIZE_1M : TLLP_LHT_SIZE_1M;
			tllp_bpt_size		= is_alpha ? TLLP_ALPHA_BPT_SIZE_1M : TLLP_BPT_SIZE_1M;
			tllp_bpt_mid		= is_alpha ? TLLP_ALPHA_BPT_MID_1M : TLLP_BPT_MID_1M;
			tllp_bpt_max		= is_alpha ? TLLP_ALPHA_BPT_MAX_1M : TLLP_BPT_MAX_1M;
			tllp_bpt_idx_max	= is_alpha ? TLLP_ALPHA_BPT_IDX_MAX_1M : TLLP_BPT_IDX_MAX_1M;
			tllp_lht_idx_max	= is_alpha ? TLLP_ALPHA_LHT_IDX_MAX_1M : TLLP_LHT_IDX_MAX_1M;
			tllp_lht			= tllp.lht.b1m;
			tllp_bpt			= tllp.bpt.b1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_tllp_predictor (budget_size budget)
{
	tllp_set_budget_constants(budget, false);	

	memset(tllp_lht, 0, sizeof(unsigned int) * tllp_lht_size);
	for (int i = 0; i < tllp_bpt_size; i++) {
		tllp_bpt[i] = tllp_bpt_max;
	}
}

void init_tllp_alpha_predictor (budget_size budget)
{
	tllp_set_budget_constants(budget, true);	

	memset(tllp_lht, 0, sizeof(unsigned int) * tllp_lht_size);
	for (int i = 0; i < tllp_bpt_size; i++) {
		tllp_bpt[i] = tllp_bpt_max;
	}
}

bool make_tllp_prediction (unsigned int pc)
{
	unsigned int lsb_addr = ((pc & tllp_lht_idx_max) % tllp_lht_size);

	unsigned int bpt_idx = tllp_lht[lsb_addr];

	if (tllp_bpt[bpt_idx] > tllp_bpt_mid) {
		assert(tllp_bpt[bpt_idx] <= tllp_bpt_max);
		return true;
	} else {
		return false;
	}
}

void train_tllp_predictor (unsigned int pc, bool outcome)
{
	unsigned int lsb_addr = ((pc & tllp_lht_idx_max) % tllp_lht_size);
	unsigned int bpt_idx = tllp_lht[lsb_addr];

	if (outcome) {
		if (tllp_bpt[bpt_idx] < tllp_bpt_max) {
			tllp_bpt[bpt_idx]++;
		}

		bpt_idx = ((bpt_idx << 1) | 0x01);
	} else {
		if (tllp_bpt[bpt_idx] > 0) {
			assert(tllp_bpt[bpt_idx] <= tllp_bpt_max);
			tllp_bpt[bpt_idx]--;
		}

		bpt_idx = ((bpt_idx << 1) | 0x00);
	}

	tllp_lht[lsb_addr] = (bpt_idx & tllp_bpt_idx_max);
}
