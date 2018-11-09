PROG := app
SRCS := main.cpp export.cpp
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d)

CC := g++

all: $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) -c -std=c++11 -MMD -MP $<

clean:
	rm -f *.o $(PROG) $(OBJS) $(DEPS)
