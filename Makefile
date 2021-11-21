CC = gcc
INCLUDE = -I./scheduler/api/include -I./scheduler/internal/include
CFLGAGS = -Wall -g $(INCLUDE)
OBJDIR = ./obj
BINDIR = ./bin
APP_1 = normal_sem
APP_2 = single_sem_deadlock_and_release
APP_3 = multi_sem_deadlock_and_release

T_APP_1 := $(addprefix $(BINDIR)/, $(APP_1))
T_APP_2 := $(BINDIR)/$(APP_2)
T_APP_3 := $(BINDIR)/$(APP_3)

SRCROOT = ./scheduler
SRCDIRS := $(SRCROOT)/api $(SRCROOT)/internal
SRCS = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o)) 

all: $(T_APP_1) $(T_APP_2) $(T_APP_3)

$(T_APP_1): $(OBJS)
	$(CC) $(CFLGAGS) -o $@ $^ $(SRCROOT)/$(APP_1).c

$(T_APP_2): $(OBJS)
	$(CC) $(CFLGAGS) -o $@ $^ $(SRCROOT)/$(APP_2).c

$(T_APP_3): $(OBJS)
	$(CC) $(CFLGAGS) -o $@ $^ $(SRCROOT)/$(APP_3).c

$(OBJDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLGAGS) -o $@ -c $<
	@mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
