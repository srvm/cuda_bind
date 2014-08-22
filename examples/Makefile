all: test

INCLUDES = -I ../
INCLUDES += -I ${TUPLE_CAT_PATH}
CXXARGS = -std=c++11

test: test.cu
	nvcc $(CXXARGS) $(INCLUDES) test.cu -o test

clean:
	rm -f *.o test
