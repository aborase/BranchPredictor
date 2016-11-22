#include <stdio.h>
#include"perceptron_predictor.h"

/* Budget specific globals set during init. */
unsigned int perceptron_size;
unsigned int perceptron_bpt_idx_max;
unsigned int perceptron_bpt_mid;
unsigned int perceptron_bpt_max;
unsigned int *perceptron_bpt;
perceptron_table perceptron;

static void perceptron_set_budget_constants(budget_size budget)
{
	switch (budget) {
		case BUDGET_8K:
			perceptron_size 	 = PERCEPTRON_SIZE_8K;
			perceptron_bpt_idx_max = PERCEPTRON_BPT_IDX_MAX_8K;
			perceptron_bpt_mid = PERCEPTRON_BPT_MID_8K;
			perceptron_bpt_max = PERCEPTRON_BPT_MAX_8K;
			perceptron_bpt     = perceptron.bpt.b8k;
			break;

		case BUDGET_16K:
			perceptron_size 	 = PERCEPTRON_SIZE_16K;
			perceptron_bpt_idx_max = PERCEPTRON_BPT_IDX_MAX_16K;
			perceptron_bpt_mid = PERCEPTRON_BPT_MID_16K;
			perceptron_bpt_max = PERCEPTRON_BPT_MAX_16K;
			perceptron_bpt     = perceptron.bpt.b16k;
			break;

		case BUDGET_32K:
			perceptron_size 	 = PERCEPTRON_SIZE_32K;
			perceptron_bpt_idx_max = PERCEPTRON_BPT_IDX_MAX_32K;
			perceptron_bpt_mid = PERCEPTRON_BPT_MID_32K;
			perceptron_bpt_max = PERCEPTRON_BPT_MAX_32K;
			perceptron_bpt     = perceptron.bpt.b32k;
			break;

		case BUDGET_64K:
			perceptron_size 	 = PERCEPTRON_SIZE_64K;
			perceptron_bpt_idx_max = PERCEPTRON_BPT_IDX_MAX_64K;
			perceptron_bpt_mid = PERCEPTRON_BPT_MID_64K;
			perceptron_bpt_max = PERCEPTRON_BPT_MAX_64K;
			perceptron_bpt     = perceptron.bpt.b64k;
			break;

		case BUDGET_128K:
			perceptron_size 	 = PERCEPTRON_SIZE_128K;
			perceptron_bpt_idx_max = PERCEPTRON_BPT_IDX_MAX_128K;
			perceptron_bpt_mid = PERCEPTRON_BPT_MID_128K;
			perceptron_bpt_max = PERCEPTRON_BPT_MAX_128K;
			perceptron_bpt     = perceptron.bpt.b128k;
			break;

		case BUDGET_1M:
			perceptron_size 	 = PERCEPTRON_SIZE_1M;
			perceptron_bpt_idx_max = PERCEPTRON_BPT_IDX_MAX_1M;
			perceptron_bpt_mid = PERCEPTRON_BPT_MID_1M;
			perceptron_bpt_max = PERCEPTRON_BPT_MAX_1M;
			perceptron_bpt     = perceptron.bpt.b1m;
			break;

		default:
			printf("Incorrect Budget size");
			break;
	}
}

void init_perceptron_predictor (budget_size budget)
{
	perceptron_set_budget_constants(budget);	

	perceptron.pp = 0;
	for (int i = 0; i < perceptron_size; i++) {
		perceptron_bpt[i] = perceptron_bpt_max;
	}
}

bool make_perceptron_prediction (unsigned int pc)
{
	unsigned int bpt_idx = ((pc ^ perceptron.pp) & perceptron_bpt_idx_max);
	assert(bpt_idx <= perceptron_bpt_idx_max);

	if (perceptron_bpt[bpt_idx] > perceptron_bpt_mid) {
		return true;
	} else {
		return false;
	}
}

void train_perceptron_predictor (unsigned int pc, bool outcome)
{
	unsigned int bpt_idx = ((pc ^ perceptron.pp) & perceptron_bpt_idx_max);
	assert(bpt_idx <= perceptron_bpt_idx_max);

	if (outcome) {
		if (perceptron_bpt[bpt_idx] < perceptron_bpt_max) {
			perceptron_bpt[bpt_idx]++;
		}

		perceptron.pp = ((perceptron.pp << 1) | 0x01);
	} else {
		if (perceptron_bpt[bpt_idx] > 0) {
			perceptron_bpt[bpt_idx]--;
		}

		perceptron.pp = ((perceptron.pp << 1) | 0x00);
	}
}

#define SIZE_N 62 //or whatever see section 5.3
float history[n] = {0}; //Put branch history here, -1 not taken, 1 taken.
float weight[n] = {0};  //storage for weights

float percepatron(void )
{
    int i;
    float y=0;
    for (i=0;i<SIZE_N;i++) { y+= weight[i] * history[i];}
    return y;
}

void train(float result, float y, float theta) //passed result of last branch (-1 not taken, 1 taken), and perceptron value
{
    int i;
    if ((y<0) != (result<0)) || (abs(y) < theta))
    {
     for (i=0;i<SIZE_N;i++;) {
          weight[i] = weight[i] + result*history[i];
       }
    }
}

float theta = (1.93 * SIZE_N) + 14;

y = percepatron();
//make prediction:
if (y < 0) predict_not_taken();
else predict_taken();
//get actual result
result = get_actual_branch_taken_result();//must return -1 not taken, 1 taken
//train for future predictions
train(y,result,theta);

//Then you need to shift everything down....
for (i=1;i<SIZE_N;i++)
{
  history[i] = history[i-1];
  //weight[i] = history[i-1]; //toggle this and see what happens :-)
}
history[0] = 1; //weighting - see section 3.2
