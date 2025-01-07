CC = g++
CFLAGS = -Wall -Werror

OUT_DIR = out
OBJ_DIR = $(OUT_DIR)/obj

BST_SRC = solution_BST_tree.cpp
BST_OBJ = $(OBJ_DIR)/solution_BST_tree.o
BST_BIN = $(OUT_DIR)/bst

RBT_SRC = solution_RBT_tree.cpp
RBT_OBJ = $(OBJ_DIR)/solution_RBT_tree.o
RBT_BIN = $(OUT_DIR)/rbt

$(BST_OBJ): $(BST_SRC)
	$(CC) -g -c $(patsubst $(BST_OBJ), $(BST_SRC), $@) -o $@ $(CFLAGS)

bst: $(BST_OBJ)
	$(CC) -g -o $(BST_BIN) $^ $(CFLAGS)

$(RBT_OBJ): $(RBT_SRC)
	$(CC) -g -c $(patsubst $(RBT_OBJ), $(RBT_SRC), $@) -o $@ $(CFLAGS)

rbt: $(RBT_OBJ)
	$(CC) -g -o $(RBT_BIN) $^ $(CFLAGS)
