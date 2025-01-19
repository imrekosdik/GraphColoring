# Compiler and flags
CC = mpicc
CFLAGS = -Wall -Iinclude -O2

# Directories
SRC_DIR = src
OUT_DIR = out
INCLUDE_DIR = include

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SRCS))

# Output executable
TARGET = out/graph_coloring

# Default rule
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule to compile source files into object files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the output directory if it doesn't exist
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Run the program
run: all
	mpirun ./$(TARGET)

# Clean rule
clean:
	rm -rf $(OUT_DIR) $(TARGET)

# Phony targets
.PHONY: all clean run
