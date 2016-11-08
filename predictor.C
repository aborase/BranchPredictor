// A very stupid predictor.  It will always predict not taken.

void init_predictor ()
{
	for (int i =0; i<TLLP_SIZE;i++){ 
		TLLP_GHT[i]=0;
		TLLP_BPT[i]=0x7f;
	}
	//memset(TLLP_GHT, 0, sizeof(unsigned int)*TLLP_SIZE);
	//memset(TLLP_BPT, 0x7f, sizeof(unsigned int)*TLLP_SIZE); 
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
