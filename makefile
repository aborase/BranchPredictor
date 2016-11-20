CC=g++
OPTS=-g

all: main.o traceread.o tllp_predictor.o global_predictor.o alpha_predictor.o perceptron_predictor.o gshare_predictor.o
	$(CC) $(OPTS) -lm -o predictor main.o traceread.o tllp_predictor.o global_predictor.o alpha_predictor.o perceptron_predictor.o gshare_predictor.o

main.o: main.C traceread.h tllp_predictor.h alpha_predictor.h perceptron_predictor.h gshare_predictor.h defs.h
	$(CC) $(OPTS) -c main.C

traceread.o: traceread.h traceread.C
	$(CC) $(OPTS) -c traceread.C

tllp_predictor.o: tllp_predictor.h tllp_predictor.C
	$(CC) $(OPTS) -c tllp_predictor.C

global_predictor.o: global_predictor.h global_predictor.C
	$(CC) $(OPTS) -c global_predictor.C

alpha_predictor.o: alpha_predictor.h alpha_predictor.C
	$(CC) $(OPTS) -c alpha_predictor.C

perceptron_predictor.o: perceptron_predictor.h perceptron_predictor.C
	$(CC) $(OPTS) -c perceptron_predictor.C

gshare_predictor.o: gshare_predictor.h gshare_predictor.C
	$(CC) $(OPTS) -c gshare_predictor.C

clean:
	rm -f *.o predictor;
