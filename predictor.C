// A very stupid predictor.  It will always predict not taken.
#include "predictor.h"

/* Budget specific globals set during init. */
unsigned int tllp_size;
unsigned int tllp_bpt_mid;
unsigned int tllp_lht_max;
unsigned int  tllp_bpt_max;
unsigned int *tllp_lht;
unsigned int *tllp_bpt;
tllp_table tllp;

void init_predictor (budget_size budget)
{
	tllp_size = TLLP_SIZE_8K;
	tllp_bpt_mid = TLLP_BPT_MID_8K;
	tllp_lht_max = TLLP_LHT_MAX_8K;
	tllp_bpt_max = TLLP_BPT_MAX_8K;
	tllp_lht = tllp.lht.b8k;
	tllp_bpt = tllp.bpt.b8k;

	memset(tllp.lht.b8k, 0, sizeof(unsigned int) * tllp_size);
	for (int i = 0; i < tllp_size; i++) {
		tllp.bpt.b8k[i] = tllp_bpt_max;
	}
}

bool make_prediction (unsigned int pc)
{
	unsigned int lsb_addr = (pc & tllp_lht_max);
	unsigned int bpt_idx = tllp.lht.b8k[lsb_addr];

	if (tllp.bpt.b8k[bpt_idx] > tllp_bpt_mid) {
		return true;
	} else {
		return false;
	}
}

void train_predictor (unsigned int pc, bool outcome)
{
	unsigned int lsb_addr = (pc & tllp_lht_max);
	unsigned int bpt_idx = tllp.lht.b8k[lsb_addr];

	if (outcome) {
		if (tllp.bpt.b8k[bpt_idx] < tllp_bpt_max) {
			tllp.bpt.b8k[bpt_idx]++;
		}

		bpt_idx = ((bpt_idx << 1) | 0x01);
	} else {
		if (tllp.bpt.b8k[bpt_idx] > 0) {
			tllp.bpt.b8k[bpt_idx]--;
		}

		bpt_idx = ((bpt_idx << 1) | 0x00);
	}

	tllp.lht.b8k[lsb_addr] = (bpt_idx & tllp_lht_max);
}
