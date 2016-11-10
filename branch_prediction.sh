#!/bin/bash

budget_types=6
pred_types=4

make clean
make all

# 2-level local predictor
i=1
j=1
while [ $i -le $pred_types ]
do
	printf "\n\n*******************************\n"
	printf "*      Predictor Type: $i      *\n"
	printf "*******************************\n"
	j=1
	while [ $j -le $budget_types ]
	do
		printf "\n**** Budget: $j ,Predictor: $i and Trace: DIST-INT-1.\n"	
		./predictor $j $i ./traces/DIST-INT-1
		printf "\n**** Budget: $j ,Predictor: $i and Trace: DIST-FP-1.\n"
		./predictor $j $i ./traces/DIST-FP-1
		printf "\n**** Budget: $j ,Predictor: $i and Trace: DIST-MM-1.\n"
		./predictor $j $i ./traces/DIST-MM-1
		printf "\n**** Budget: $j ,Predictor: $i and Trace: DIST-SERV-1.\n"
		./predictor $j $i ./traces/DIST-SERV-1
		j=`expr $j + 1`
	done
	i=`expr $i + 1`

done
