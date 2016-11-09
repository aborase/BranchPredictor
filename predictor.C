// A very stupid predictor.  It will always predict not taken.
#include "predictor.h"

/* Budget specific globals set during init. */
unsigned int tllp_size;
unsigned int tllp_bpt_mid;
unsigned int tllp_bpt_min;
unsigned int tllp_lht_max;
unsigned int  tllp_bpt_max;

unsigned int TLLP_LHT[TLLP_SIZE] = {0};
unsigned int TLLP_BPT[TLLP_SIZE] = {0};

void init_predictor ()
{
	tllp_size = TLLP_SIZE;
	tllp_bpt_mid = TLLP_BPT_MID;
	tllp_bpt_min = TLLP_BPT_MIN;
	tllp_lht_max = TLLP_LHT_MAX;
	tllp_bpt_max = TLLP_BPT_MAX;

	memset(TLLP_LHT, 0, sizeof(unsigned int) * tllp_size);
	for (int i=0; i<tllp_size;i++) {
		TLLP_BPT[i] = tllp_bpt_max;
	}
}

bool make_prediction (unsigned int pc)
{
	unsigned int lsb_addr = (pc & tllp_lht_max);
	unsigned int bpt_idx = TLLP_LHT[lsb_addr];

	if (TLLP_BPT[bpt_idx] > tllp_bpt_mid) {
		return true;
	} else {
		return false;
	}
}

void train_predictor (unsigned int pc, bool outcome)
{
	unsigned int lsb_addr = (pc & tllp_lht_max);
	unsigned int bpt_idx = TLLP_LHT[lsb_addr];

	if (outcome) {
		if (TLLP_BPT[bpt_idx] < tllp_bpt_max) {
			TLLP_BPT[bpt_idx]++;
		}

		bpt_idx = ((bpt_idx << 1) | 0x01);
	} else {
		if (TLLP_BPT[bpt_idx] > tllp_bpt_min) {
			TLLP_BPT[bpt_idx]--;
		}

		bpt_idx = ((bpt_idx << 1) | 0x00);
	}

	TLLP_LHT[lsb_addr] = (bpt_idx & tllp_lht_max);
}
