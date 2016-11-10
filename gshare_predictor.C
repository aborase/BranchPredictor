// A very stupid predictor.  It will always predict not taken.
#include <stdio.h>
#include"gshare_predictor.h"

/* Budget specific globals set during init. */
unsigned int gshare_size;
unsigned int gshare_ghr_lsb;
unsigned int gshare_addr_sz;
unsigned int gshare_bpt_idx_max;
unsigned int gshare_bpt_mid;
unsigned int gshare_bpt_max;
unsigned int *gshare_bpt;
gshare_table gshare;

static void gshare_set_budget_constants(budget_size budget)
{
	switch (budget) {
		case BUDGET_8K:
			gshare_size 	 = GSHARE_SIZE_8K;
			gshare_ghr_lsb = GSHARE_GHR_LSB_8K;
			gshare_addr_sz = GSHARE_ADDR_SZ_8K;
			gshare_bpt_idx_max = GSHARE_BPT_IDX_MAX_8K;
			gshare_bpt_mid = GSHARE_BPT_MID_8K;
			gshare_bpt_max = GSHARE_BPT_MAX_8K;
			gshare_bpt     = gshare.bpt.b8k;
			break;

		case BUDGET_16K:
			gshare_size 	 = GSHARE_SIZE_16K;
			gshare_ghr_lsb = GSHARE_GHR_LSB_16K;
			gshare_addr_sz = GSHARE_ADDR_SZ_16K;
			gshare_bpt_idx_max = GSHARE_BPT_IDX_MAX_16K;
			gshare_bpt_mid = GSHARE_BPT_MID_16K;
			gshare_bpt_max = GSHARE_BPT_MAX_16K;
			gshare_bpt     = gshare.bpt.b16k;
			break;

		case BUDGET_32K:
			gshare_size 	 = GSHARE_SIZE_32K;
			gshare_ghr_lsb = GSHARE_GHR_LSB_32K;
			gshare_addr_sz = GSHARE_ADDR_SZ_32K;
			gshare_bpt_idx_max = GSHARE_BPT_IDX_MAX_32K;
			gshare_bpt_mid = GSHARE_BPT_MID_32K;
			gshare_bpt_max = GSHARE_BPT_MAX_32K;
			gshare_bpt     = gshare.bpt.b32k;
			break;

		case BUDGET_64K:
			gshare_size 	 = GSHARE_SIZE_64K;
			gshare_ghr_lsb = GSHARE_GHR_LSB_64K;
			gshare_addr_sz = GSHARE_ADDR_SZ_64K;
			gshare_bpt_idx_max = GSHARE_BPT_IDX_MAX_64K;
			gshare_bpt_mid = GSHARE_BPT_MID_64K;
			gshare_bpt_max = GSHARE_BPT_MAX_64K;
			gshare_bpt     = gshare.bpt.b64k;
			break;

		case BUDGET_128K:
			gshare_size 	 = GSHARE_SIZE_128K;
			gshare_ghr_lsb = GSHARE_GHR_LSB_128K;
			gshare_addr_sz = GSHARE_ADDR_SZ_128K;
			gshare_bpt_idx_max = GSHARE_BPT_IDX_MAX_128K;
			gshare_bpt_mid = GSHARE_BPT_MID_128K;
			gshare_bpt_max = GSHARE_BPT_MAX_128K;
			gshare_bpt     = gshare.bpt.b128k;
			break;

		case BUDGET_1M:
			gshare_size 	 = GSHARE_SIZE_1M;
			gshare_ghr_lsb = GSHARE_GHR_LSB_1M;
			gshare_addr_sz = GSHARE_ADDR_SZ_1M;
			gshare_bpt_idx_max = GSHARE_BPT_IDX_MAX_1M;
			gshare_bpt_mid = GSHARE_BPT_MID_1M;
			gshare_bpt_max = GSHARE_BPT_MAX_1M;
			gshare_bpt     = gshare.bpt.b1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_gshare_predictor (budget_size budget)
{
	gshare_set_budget_constants(budget);	

	gshare.ghr = 0;
	for (int i = 0; i < gshare_size; i++) {
		gshare_bpt[i] = gshare_bpt_max;
	}
}

bool make_gshare_prediction (unsigned int pc)
{
	unsigned int lsb_addr = (pc << gshare_addr_sz);
	unsigned int lsb_ghr = (gshare.ghr & gshare_ghr_lsb);
	unsigned int bpt_idx = ((lsb_addr | lsb_ghr) & gshare_bpt_idx_max);
	assert(bpt_idx <= gshare_bpt_idx_max);

	if (gshare_bpt[bpt_idx] > gshare_bpt_mid) {
		return true;
	} else {
		return false;
	}
}

void train_gshare_predictor (unsigned int pc, bool outcome)
{
	unsigned int lsb_addr = (pc << gshare_addr_sz);
	unsigned int lsb_ghr = (gshare.ghr & gshare_ghr_lsb);
	unsigned int bpt_idx = ((lsb_addr | lsb_ghr) & gshare_bpt_idx_max);
	assert(bpt_idx <= gshare_bpt_idx_max);

	if (outcome) {
		if (gshare_bpt[bpt_idx] < gshare_bpt_max) {
			gshare_bpt[bpt_idx]++;
		}

		gshare.ghr = ((gshare.ghr << 1) | 0x01);
	} else {
		if (gshare_bpt[bpt_idx] > 0) {
			gshare_bpt[bpt_idx]--;
		}

		gshare.ghr = ((gshare.ghr << 1) | 0x00);
	}
}
