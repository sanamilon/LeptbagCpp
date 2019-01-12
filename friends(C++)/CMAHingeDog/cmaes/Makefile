PROG := app
SRCS := main.cpp
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d)

CC := g++
CCFLAGS := -std=c++14

all: $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) -c $(CCFLAGS) -MMD -MP $<

clean:
	rm -f $(PROG) $(OBJS) $(DEPS)
