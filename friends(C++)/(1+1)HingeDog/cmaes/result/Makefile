PROG := app
SRCS := main.cpp export.cpp
OBJS := $(SRCS:%.c=%.o)
DEPS := $(SRCS:%.c=%.d)

CC := g++

all: $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
        $(CC) -o $@ $^

%.o: %.c
        $(CC) -c -std=c++11 -MMD -MP $<

clean:
        rm -f $(PROG) $(OBJS) $(DEPS)
