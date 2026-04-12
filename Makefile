LIB_NAME ?= spm
OBJ_DIR ?= build
WASM_OBJ_DIR ?= build/wasm
LIB_DIR ?= $(realpath ./)/lib
WASM_DIR ?= $(realpath ./)/dist
WASI_SDK_PATH ?= ${WASI_SDK_PATH}
SRC_DIR = $(realpath ./)/src
INCLUDE_DIR = $(realpath ./)/include


INCLUDES = \
	-I${INCLUDE_DIR} \
	-I${SRC_DIR} \

#
#
#

CC ?= gcc
AR ?= ar

CFLAGS ?= -Wall -Wextra -g -fPIC

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
WASM_OBJS = $(patsubst $(SRC_DIR)/%.c, $(WASM_OBJ_DIR)/%.o, $(SRC))

STATIC_LIB = $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB = $(LIB_DIR)/lib$(LIB_NAME).so
WASM_STATIC_LIB = $(WASM_DIR)/lib/lib$(LIB_NAME).a

#
# WASM (clang)
#

# 64KiB
# WASM_MEM=65536
# 128KiB
# WASM_MEM=131072
# 256KiB
WASM_MEM=262144

WASM_CFLAGS = \
	--target=wasm32-unknown-unknown \
	-O2 \
	-ffreestanding \
	-fno-builtin \
	-fvisibility=hidden \
	-nostdlib

WASM_LDFLAGS = \
	-Wl,--no-entry \
	-Wl,--export-dynamic \
	-Wl,--import-memory \
	-Wl,--export-memory \
	-Wl,--initial-memory=$(WASM_MEM) \
	-Wl,--max-memory=$(WASM_MEM) \
	-Wl,--allow-undefined

WASM_OUT = $(WASM_DIR)/$(LIB_NAME).wasm

#
#
#

all: shared static

wasm: $(WASM_OUT) $(WASM_STATIC_LIB)

static: $(OBJS)
	mkdir -p $(LIB_DIR)
	$(AR) rcs $(STATIC_LIB) $(OBJS)

shared: $(OBJS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $(SHARED_LIB) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


$(WASM_OUT): $(SRC)
	mkdir -p $(WASM_DIR)
	$(WASI_SDK_PATH)/bin/clang $(WASM_CFLAGS) $(INCLUDES) \
		$(SRC) \
		-o $@ \
		$(WASM_LDFLAGS)

$(WASM_STATIC_LIB): $(WASM_OBJS)
	mkdir -p $(@D)
	$(WASI_SDK_PATH)/bin/ar rcs $@ $^

$(WASM_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(WASI_SDK_PATH)/bin/clang \
		-O2 \
		--target=wasm32-wasi \
		--sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot \
		$(INCLUDES) -c $< -o $@


run-tests: tests
	./bin/tests

tests:
	$(MAKE) clean
	$(MAKE) all
	$(MAKE) clean -C ./tests
	$(MAKE) -C ./tests \
		MAIN_DIR=$(realpath ./) \
		EXTERNAL_INCLUDES="$(INCLUDES)" \
		EXTERNAL_LIBRARIES="-L$(LIB_DIR) -lspm"

clean:
	$(MAKE) clean -C ./tests
	rm -rf $(WASM_OUT)
	rm -rf $(LIB_DIR)
	rm -rf $(OBJ_DIR) $(SHARED_LIB) $(STATIC_LIB)

.PHONY: all clean wasm tests run-tests
