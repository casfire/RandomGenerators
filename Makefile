WARNINGS:=-Wall -Wextra
FILES:=main RandomGenerators
BINARY:=benchmark
LIN_CFLAGS:=$(WARNINGS) -O
LIN_LFLAGS:=-static-libgcc -static-libstdc++
WIN_CFLAGS:=$(WARNINGS) -O
WIN_LFLAGS:=-static-libgcc -static-libstdc++

####################

UNAME:=$(shell uname)
ifeq ($(UNAME), Linux)
CFLAGS:=$(LIN_CFLAGS)
LFLAGS:=$(LIN_LFLAGS)
else
CFLAGS:=$(WIN_CFLAGS)
LFLAGS:=$(WIN_LFLAGS)
endif
OBJS:=$(patsubst %,obj/%.o,$(FILES))
.PHONY: all clean dirs
all: dirs $(OBJS)
	@echo "Linking..."
	@g++ $(OBJS) $(LFLAGS) -o bin/$(BINARY)
	@echo "Done."
clean:
	@rm -rf *.o bin/$(BINARY) bin/$(BINARY).exe obj/
	-@rmdir bin
	@echo "Cleaned."
dirs: obj/ bin/
obj/:
	@echo "Creating obj/ directory"
	@mkdir -p obj/
bin/:
	@echo "Creating bin/ directory"
	@mkdir -p bin/
obj/%.o: src/%.cpp
	@echo "Compiling $<"
	@g++ -c $< $(CFLAGS) -o $@
obj/%.o: src/%.c
	@echo "Compiling $<"
	@gcc -c $< $(CFLAGS) -o $@
