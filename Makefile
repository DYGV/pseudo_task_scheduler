CC := gcc
INCLUDE := -I./scheduler/api/include -I./scheduler/internal/include
CFLGAGS := -Wall -g $(INCLUDE)
OBJDIR := ./obj
BINDIR := ./bin
SRCROOT := ./scheduler
SRCDIRS := $(SRCROOT)/api $(SRCROOT)/internal

DEPS := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJS := $(addprefix $(OBJDIR)/, $(DEPS:.c=.o))

APPS := single_sem single_sem_idle multi_sem multi_sem_idle
BINS := $(addprefix $(BINDIR)/, $(APPS))

.PHONY: all clean

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CFLGAGS) -o $@ $^ $(SRCROOT)/$(notdir $@).c

$(OBJDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLGAGS) -o $@ -c $<
	@mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

code_format:
	astyle --style=1tbs -s4 -S -N -Y -M80 -p -j -k1 -U -H -A2 --recursive *.c *.h
