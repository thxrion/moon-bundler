# config
TARGET = bundler.exe
CC = gcc
LINKER = $(CC)
CFLAGS = -O0 -Wall

# directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/.obj

SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

build: $(BIN_DIR) $(BIN_DIR)/$(TARGET)

$(BIN_DIR):
	mkdir -p $@

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_DIR)/$(TARGET): $(OBJ)
	$(LINKER) $(OBJ) -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: build clean