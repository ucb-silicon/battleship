CC = 		cc
OPT =		-O3 -msse2

CC_FLAGS = 	$(OPT) -Wall  -I/usr/include/eigen3 -I/usr/local/include/eigen3

LD_FLAGS = 	
LIBS = 	-lstdc++

SRCS	= $(wildcard *.cpp)
OBJS	= $(filter-out test%, $(SRCS:.cpp=.o))
TOBJS	= $(filter test%, $(SRCS:.cpp=.o))
TESTS	= $(subst .cpp,,$(filter test%, $(SRCS)))
BINARIES= battleship

%.o:%.cc
	$(CC) $(CC_FLAGS) -c $< 

all: $(OBJS) $(BINARIES)

tests: $(TESTS)

$(TESTS): $(OBJS) $(TOBJS)
	$(CC) $(LD_FLAGS) $(LIBS) $(OBJS) $(TOBJS) -o $@

battleship: battleship.o
	$(CC) $(LD_FLAGS) $@.o $(LINKOBJS) $(LIBS) -o $@

clean:
	@rm -rf *.a *.o *.so $(TESTS) $(BINARIES)

clean-binaries:
	@rm -rf $(TESTS) $(BINARIES)
