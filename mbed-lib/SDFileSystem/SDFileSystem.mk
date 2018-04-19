ASM_OBJ += 

C_OBJ += 

CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/SDFileSystem.o
CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/FATDirHandle.o
CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/FATFileHandle.o
CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/FATFileSystem.o
CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/ChaN/ccsbcs.o
CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/ChaN/diskio.o
CXX_OBJ += $(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/ChaN/ff.o

INCLUDE_PATHS += -I$(MBED_LIB_DIR)/SDFileSystem
INCLUDE_PATHS += -I$(MBED_LIB_DIR)/SDFileSystem/FATFileSystem
INCLUDE_PATHS += -I$(MBED_LIB_DIR)/SDFileSystem/FATFileSystem/ChaN

CDEFS += 
