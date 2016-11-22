#include <stdio.h>
#include <stdlib.h>
#include"perceptron_predictor.h"


/* Budget specific globals set during init. */
unsigned int perceptron_size;
unsigned int perceptron_pt_idx_max;
unsigned int perceptron_threshold;
unsigned int perceptron_weight_count;
int weight_max;
int weight_min;
weight_set *perceptron_pt;
int total_weight;
perceptron_table perceptron;

static bool sign(int x) {
	return (x >= 0) ? true : false;
}

static void perceptron_set_budget_constants(budget_size budget)
{
	switch (budget) {
		case BUDGET_8K:
			perceptron_size 	 = PERCEPTRON_SIZE_8K;
			perceptron_pt_idx_max = PERCEPTRON_PT_IDX_MAX_8K;
			perceptron_threshold = PERCEPTRON_THRESHOLD_8K;
			perceptron_weight_count = PERCEPTRON_WEIGHT_CNT_8K;
			weight_max = PERCEPTRON_WEIGHT_MAX_8K;
			weight_min = PERCEPTRON_WEIGHT_MIN_8K;
			perceptron_pt 	  = perceptron.pt.p8k;
			break;

		case BUDGET_16K:
			perceptron_size 	 = PERCEPTRON_SIZE_16K;
			perceptron_pt_idx_max = PERCEPTRON_PT_IDX_MAX_16K;
			perceptron_threshold = PERCEPTRON_THRESHOLD_16K;
			perceptron_weight_count = PERCEPTRON_WEIGHT_CNT_16K;
			weight_max = PERCEPTRON_WEIGHT_MAX_16K;
			weight_min = PERCEPTRON_WEIGHT_MIN_16K;
			perceptron_pt     = perceptron.pt.p16k;
			break;

		case BUDGET_32K:
			perceptron_size 	 = PERCEPTRON_SIZE_32K;
			perceptron_pt_idx_max = PERCEPTRON_PT_IDX_MAX_32K;
			perceptron_threshold = PERCEPTRON_THRESHOLD_32K;
			perceptron_weight_count = PERCEPTRON_WEIGHT_CNT_32K;
			weight_max = PERCEPTRON_WEIGHT_MAX_32K;
			weight_min = PERCEPTRON_WEIGHT_MIN_32K;
			perceptron_pt     = perceptron.pt.p32k;
			break;

		case BUDGET_64K:
			perceptron_size 	 = PERCEPTRON_SIZE_64K;
			perceptron_pt_idx_max = PERCEPTRON_PT_IDX_MAX_64K;
			perceptron_threshold = PERCEPTRON_THRESHOLD_64K;
			perceptron_weight_count = PERCEPTRON_WEIGHT_CNT_64K;
			weight_max = PERCEPTRON_WEIGHT_MAX_64K;
			weight_min = PERCEPTRON_WEIGHT_MIN_64K;
			perceptron_pt     = perceptron.pt.p64k;
			break;

		case BUDGET_128K:
			perceptron_size 	 = PERCEPTRON_SIZE_128K;
			perceptron_pt_idx_max = PERCEPTRON_PT_IDX_MAX_128K;
			perceptron_threshold = PERCEPTRON_THRESHOLD_128K;
			perceptron_weight_count = PERCEPTRON_WEIGHT_CNT_128K;
			weight_max = PERCEPTRON_WEIGHT_MAX_128K;
			weight_min = PERCEPTRON_WEIGHT_MIN_128K;
			perceptron_pt     = perceptron.pt.p128k;
			break;

		case BUDGET_1M:
			perceptron_size 	 = PERCEPTRON_SIZE_1M;
			perceptron_pt_idx_max = PERCEPTRON_PT_IDX_MAX_1M;
			perceptron_threshold = PERCEPTRON_THRESHOLD_1M;
			perceptron_weight_count = PERCEPTRON_WEIGHT_CNT_1M;
			weight_max = PERCEPTRON_WEIGHT_MAX_1M;
			weight_min = PERCEPTRON_WEIGHT_MIN_1M;
			perceptron_pt     = perceptron.pt.p1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_perceptron_predictor (budget_size budget)
{
	perceptron_set_budget_constants(budget);	

	perceptron.ghr = 0;
	for (int i = 0; i < perceptron_size; i++) {
		for (int j = 0; j < perceptron_weight_count; j++) {
			perceptron_pt[i].weights[j] = 0;
		}
	}
}

bool make_perceptron_prediction (unsigned int pc)
{
	unsigned int pt_idx = (pc % perceptron_pt_idx_max);   //XXX

	assert(pt_idx <= perceptron_pt_idx_max);
	total_weight = 0;
	
	for (int i = 0; i < perceptron_weight_count - 1; i++) {		
		if ((perceptron.ghr >> i) & 0x01) {
			total_weight += perceptron_pt[pt_idx].weights[i];
		} else {
			total_weight -= perceptron_pt[pt_idx].weights[i];			
		}
	}

	total_weight += perceptron_pt[pt_idx].weights[perceptron_weight_count-1];
	return sign(total_weight);
}

void train_perceptron_predictor (unsigned int pc, bool outcome)
{
	unsigned int pt_idx = (pc % perceptron_pt_idx_max);   //XXX
	assert(pt_idx <= perceptron_pt_idx_max);
	
	if ((sign(total_weight) != outcome) || (abs(total_weight) <= perceptron_threshold)) {
		for (int i = 0; i < perceptron_weight_count - 1; i++) {		
			if (outcome != ((perceptron.ghr >> i) & 0x01)) {
				if (perceptron_pt[pt_idx].weights[i] > weight_min) { 
					perceptron_pt[pt_idx].weights[i] -= 1;
				}
			} else {
				if (perceptron_pt[pt_idx].weights[i] < weight_max) {
					perceptron_pt[pt_idx].weights[i] += 1;
				}
			}
		}

		if (outcome) {
			if (perceptron_pt[pt_idx].weights[perceptron_weight_count-1] < weight_max) {
				perceptron_pt[pt_idx].weights[perceptron_weight_count-1] += 1;
			}
		} else {
			if (perceptron_pt[pt_idx].weights[perceptron_weight_count-1] > weight_min) {
				perceptron_pt[pt_idx].weights[perceptron_weight_count-1] -= 1;
			}
		}
	}

	if (outcome) {
		perceptron.ghr = (perceptron.ghr << 1) | 0x01;
	} else {
		perceptron.ghr = (perceptron.ghr << 1) | 0x00;
	}
}
