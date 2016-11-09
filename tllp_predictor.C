// A very stupid predictor.  It will always predict not taken.
#include <stdio.h>
#include "tllp_predictor.h"

/* Budget specific globals set during init. */
unsigned int tllp_size;
unsigned int tllp_bpt_mid;
unsigned int tllp_lht_max;
unsigned int  tllp_bpt_max;
unsigned int *tllp_lht;
unsigned int *tllp_bpt;
tllp_table tllp;

static void tllp_set_budget_constants(budget_size budget)
{
	switch (budget) {
		case BUDGET_8K:
			tllp_size 	 = TLLP_SIZE_8K;
			tllp_bpt_mid = TLLP_BPT_MID_8K;
			tllp_lht_max = TLLP_LHT_MAX_8K;
			tllp_bpt_max = TLLP_BPT_MAX_8K;
			tllp_lht     = tllp.lht.b8k;
			tllp_bpt     = tllp.bpt.b8k;
			break;

		case BUDGET_16K:
			tllp_size 	 = TLLP_SIZE_16K;
			tllp_bpt_mid = TLLP_BPT_MID_16K;
			tllp_lht_max = TLLP_LHT_MAX_16K;
			tllp_bpt_max = TLLP_BPT_MAX_16K;
			tllp_lht     = tllp.lht.b16k;
			tllp_bpt     = tllp.bpt.b16k;
			break;

		case BUDGET_32K:
			tllp_size 	 = TLLP_SIZE_32K;
			tllp_bpt_mid = TLLP_BPT_MID_32K;
			tllp_lht_max = TLLP_LHT_MAX_32K;
			tllp_bpt_max = TLLP_BPT_MAX_32K;
			tllp_lht     = tllp.lht.b32k;
			tllp_bpt     = tllp.bpt.b32k;
			break;

		case BUDGET_64K:
			tllp_size 	 = TLLP_SIZE_32K;
			tllp_bpt_mid = TLLP_BPT_MID_32K;
			tllp_lht_max = TLLP_LHT_MAX_32K;
			tllp_bpt_max = TLLP_BPT_MAX_32K;
			tllp_lht     = tllp.lht.b32k;
			tllp_bpt     = tllp.bpt.b32k;
			break;

		case BUDGET_128K:
			tllp_size 	 = TLLP_SIZE_128K;
			tllp_bpt_mid = TLLP_BPT_MID_128K;
			tllp_lht_max = TLLP_LHT_MAX_128K;
			tllp_bpt_max = TLLP_BPT_MAX_128K;
			tllp_lht     = tllp.lht.b128k;
			tllp_bpt     = tllp.bpt.b128k;
			break;

		case BUDGET_1M:
			tllp_size 	 = TLLP_SIZE_1M;
			tllp_bpt_mid = TLLP_BPT_MID_1M;
			tllp_lht_max = TLLP_LHT_MAX_1M;
			tllp_bpt_max = TLLP_BPT_MAX_1M;
			tllp_lht     = tllp.lht.b1m;
			tllp_bpt     = tllp.bpt.b1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_tllp_predictor (budget_size budget)
{
	tllp_set_budget_constants(budget);	

	memset(tllp_lht, 0, sizeof(unsigned int) * tllp_size);
	for (int i = 0; i < tllp_size; i++) {
		tllp_bpt[i] = tllp_bpt_max;
	}
}

bool make_tllp_prediction (unsigned int pc)
{
	unsigned int lsb_addr = (pc & tllp_lht_max);
	unsigned int bpt_idx = tllp_lht[lsb_addr];

	if (tllp_bpt[bpt_idx] > tllp_bpt_mid) {
		return true;
	} else {
		return false;
	}
}

void train_tllp_predictor (unsigned int pc, bool outcome)
{
	unsigned int lsb_addr = (pc & tllp_lht_max);
	unsigned int bpt_idx = tllp_lht[lsb_addr];

	if (outcome) {
		if (tllp_bpt[bpt_idx] < tllp_bpt_max) {
			tllp_bpt[bpt_idx]++;
		}

		bpt_idx = ((bpt_idx << 1) | 0x01);
	} else {
		if (tllp_bpt[bpt_idx] > 0) {
			tllp_bpt[bpt_idx]--;
		}

		bpt_idx = ((bpt_idx << 1) | 0x00);
	}

	tllp_lht[lsb_addr] = (bpt_idx & tllp_lht_max);
}
