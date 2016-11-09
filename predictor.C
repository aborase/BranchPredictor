// A very stupid predictor.  It will always predict not taken.
#include "predictor.h"

uint16_t TLLP_GHT[TLLP_SIZE];
uint8_t TLLP_BPT[TLLP_SIZE];

void init_predictor ()
{
	memset(TLLP_GHT, 0, sizeof(uint16_t) * TLLP_SIZE);
	memset(TLLP_BPT, 0x7f, sizeof(uint8_t) * TLLP_SIZE); 
}

bool make_prediction (unsigned int pc)
{
	uint16_t lsb_addr = (pc & 0x01ff);
	uint16_t bpt_idx = TLLP_GHT[lsb_addr];

	if ((TLLP_BPT[bpt_idx] & 0x7f) > TLLP_BPT_MID) {
		return true;
	} else {
		return false;
	}
}

void train_predictor (unsigned int pc, bool outcome)
{
	uint16_t lsb_addr = (pc & 0x01ff);
	uint16_t bpt_idx = TLLP_GHT[lsb_addr];

	if (outcome) {
		if ((TLLP_BPT[bpt_idx] & 0x7f) < TLLP_BPT_MAX) {
			TLLP_BPT[bpt_idx]++;
		}

		bpt_idx = ((bpt_idx << 1) | 0x0001);
	} else {
		if ((TLLP_BPT[bpt_idx] & 0x7f) > TLLP_BPT_MIN) {
			TLLP_BPT[bpt_idx]--;
		}

		bpt_idx = ((bpt_idx << 1) | 0x0000);
	}

	TLLP_GHT[lsb_addr] = bpt_idx;
}
