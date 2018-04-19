ASM_OBJ += 

C_OBJ += 

CXX_OBJ += $(MBED_LIB_DIR)/HttpServer_snapshot/HTTPRequestHandler.o
CXX_OBJ += $(MBED_LIB_DIR)/HttpServer_snapshot/Handler/FSHandler.o
CXX_OBJ += $(MBED_LIB_DIR)/HttpServer_snapshot/Handler/RPCHandler.o
CXX_OBJ += $(MBED_LIB_DIR)/HttpServer_snapshot/Handler/SimpleHandler.o
CXX_OBJ += $(MBED_LIB_DIR)/HttpServer_snapshot/Handler/SnapshotHandler.o

INCLUDE_PATHS += -I$(MBED_LIB_DIR)/HttpServer_snapshot
INCLUDE_PATHS += -I$(MBED_LIB_DIR)/HttpServer_snapshot/Handler

CDEFS += 
