TARGET=benchmark
CFLAGS:=-O3 -std=c++11 -Wall -Wextra
LFLAGS:=-static -static-libgcc -static-libstdc++
FILES=main.cpp Random.cpp

OBJS=$(patsubst %,obj/%.o,$(basename $(FILES)))
.PHONY: all clean
all: $(TARGET)
$(TARGET): $(OBJS)
	@echo "Linking..."
	@g++ $(OBJS) $(LFLAGS) -o $(TARGET)
	@echo "Done."
$(OBJS): obj/%.o: src/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@g++ -c $< $(CFLAGS) -o $@
clean:
	@rm -rf *.o $(TARGET) $(TARGET).exe obj/
	@echo "Cleaned."
