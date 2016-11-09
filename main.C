#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "traceread.h"
#include "predictor.h"
#include "defs.h"

FILE * stream;

void setup_trace (const char * filename)
{
  if (filename == NULL)
    stream = stdin;
  else {
	stream = fopen (filename, "r");
  }
}

void close_trace ()
{
  fclose (stream);
}


int main (int argc, char * argv[])
{
  int mis_preds = 0;
  int num_branches = 0;
  uint32_t pc = 0;
  bool outcome = false;
  budget_size budget;
  pred_type pred;

  if (argc != 4) {
	printf("Usage: predictor <BUDGET_TYPE> <PRED_TYPE> <filename>\n");
	printf("   where,\n");
	printf("       BUDGET_TYPE: 1 (for 8K + 64 bits)\n");
	printf("                    2 (for 16K + 128 bits)\n");
	printf("                    3 (for 32K + 256 bits)\n");
	printf("                    4 (for 64K + 512 bits)\n");
	printf("                    5 (for 128K + 1K bits)\n");
	printf("                    6 (for 1M + 4K bits)\n");
	printf("       PRED_TYPE : 1 (for 2-level local predictor)\n");
	printf("                   2 (for alpha 21264 -like predictor)\n");
	printf("                   3 (for perceptron predictor)\n");
	printf("                   4 (for g-share predictor)\n");
	return 0;
  } else {
	budget = (budget_size) atoi(argv[1]);
	pred = (pred_type) atoi(argv[2]);
  }

  // Initialize the predictor
  init_predictor (budget);

  setup_trace (argv[3]);

  // Read the number of instructions from the trace
  uint32_t stat_num_insts = 0;
  if (fread (&stat_num_insts, sizeof (uint32_t), 1, stream) != 1) {
    printf ("Could not read intput file\n");
    return 1;
  }
  stat_num_insts = ntohl (stat_num_insts);

  // Read each branch from the trace
  while (read_branch (&pc, &outcome)) {

    pc = ntohl (pc);

    num_branches ++;
    
    // Make a prediction and compare with actual outcome
    if (make_prediction (pc) != outcome)
      mis_preds ++;

    // Train the predictor
    train_predictor (pc, outcome);
  }

  // Print out the mispredict statistics
  printf ("Branches\t\t%8d\n", num_branches);
  printf ("Incorrect\t\t%8d\n", mis_preds);
  float mis_pred_rate = (float)mis_preds / float(stat_num_insts / 1000);
  printf ("1000*wrong_cc_predicts/total insts 1000 * %8d / %8d = %7.3f\n", mis_preds, stat_num_insts, mis_pred_rate);

  if (argc == 2)
    close_trace ();
  
  return 0;
}
