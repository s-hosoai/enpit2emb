ASM_OBJ += 

C_OBJ += 

CXX_OBJ += $(MBED_LIB_DIR)/mbed-rpc/Arguments.o
CXX_OBJ += $(MBED_LIB_DIR)/mbed-rpc/RPCFunction.o
CXX_OBJ += $(MBED_LIB_DIR)/mbed-rpc/parse_pins.o
CXX_OBJ += $(MBED_LIB_DIR)/mbed-rpc/rpc.o

INCLUDE_PATHS += -I$(MBED_LIB_DIR)/mbed-rpc

CDEFS += 
