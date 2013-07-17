TARGET_NAME = prog
TARGET_EXT = exe

BIN_DIR = ./bin
OBJ_DIR = ./obj
SRC_DIR = ./src
INC_DIR = ./inc
TARGET_PATH = $(BIN_DIR)/$(TARGET_NAME).$(TARGET_EXT)

prog : main.o
	gcc $(OBJ_DIR)/main.o -o $(TARGET_PATH)

main.o :
	gcc -c $(SRC_DIR)/main.c -I$(INC_DIR) -o $(OBJ_DIR)/main.o

clean :
	rm -rf $(BIN_DIR)/*.exe
	rm -rf $(OBJ_DIR)/*.obj
