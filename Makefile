CC = gcc
INCLUDE = -I./scheduler/api/include -I./scheduler/internal/include
CFLGAGS = -Wall -g $(INCLUDE)
OBJDIR = ./obj
APP_1 = app_normal_sem
APP_2 = app_deadlock_release

SRCROOT = ./scheduler
SRCDIRS := $(SRCROOT)/api $(SRCROOT)/internal
SRCS = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o)) 

all: $(APP_1) $(APP_2)

$(APP_1): $(OBJS)
	$(CC) $(CFLGAGS) -o $@ $^ $(SRCROOT)/$(APP_1).c

$(APP_2): $(OBJS)
	$(CC) $(CFLGAGS) -o $@ $^ $(SRCROOT)/$(APP_2).c

$(OBJDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLGAGS) -o $@ -c $<

clean:
	rm -rf $(OBJDIR) $(APP_1) $(APP_2)
