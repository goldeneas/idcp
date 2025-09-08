CC := gcc
CC_FLAGS := -Wall -Wextra -pedantic

PROTOC := protoc
PROTOC_FLAGS := -Inanopb -Iproto

OUTPUT_DIR := build

SERVER_OUTPUT_NAME := server
CLIENT_OUTPUT_NAME := client

CLIENT_SRC_DIR := client/src
SERVER_SRC_DIR := server/src
COMMON_SRC_DIR := common/src

PROTO_DIR := proto

PROTO_FILES := $(wildcard $(PROTO_DIR)/*.proto)

SERVER_SRC_FILES := $(shell find $(SERVER_SRC_DIR) -name "*.c")
CLIENT_SRC_FILES := $(shell find $(CLIENT_SRC_DIR) -name "*.c") 
COMMON_SRC_FILES := $(shell find $(COMMON_SRC_DIR) -name "*.c") 

HEADER_INCLUDE_FLAGS := -Icommon/src -Icommon/src/nanopb -Icommon/src/generated

LIBRARY_FLAGS = -luv

.PHONY: all client server proto run-client run-server

all: proto server client

proto:
	mkdir -p $(COMMON_SRC_DIR)/generated
	$(PROTOC) $(PROTOC_FLAGS) --nanopb_out=$(COMMON_SRC_DIR)/generated $(PROTO_FILES)

server: $(SERVER_SRC_FILES)
	$(CC) $(CC_FLAGS) $(HEADER_INCLUDE_FLAGS) $(COMMON_SRC_FILES) $(SERVER_SRC_FILES) $(LIBRARY_FLAGS) -o $(OUTPUT_DIR)/$(SERVER_OUTPUT_NAME)

client: $(CLIENT_SRC_FILES)
	$(CC) $(CC_FLAGS) $(HEADER_INCLUDE_FLAGS) $(COMMON_SRC_FILES) $(CLIENT_SRC_FILES) $(LIBRARY_FLAGS) -o $(OUTPUT_DIR)/$(CLIENT_OUTPUT_NAME)

run-server: server
	./$(OUTPUT_DIR)/$(SERVER_OUTPUT_NAME)

run-client: client
	./$(OUTPUT_DIR)/$(CLIENT_OUTPUT_NAME)
