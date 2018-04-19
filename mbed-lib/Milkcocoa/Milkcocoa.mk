ASM_OBJ += 

C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTConnectClient.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTConnectServer.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTDeserializePublish.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTPacket.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTSerializePublish.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTSubscribeClient.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTSubscribeServer.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTUnsubscribeClient.o
C_OBJ += $(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket/MQTTUnsubscribeServer.o

CXX_OBJ += $(MBED_LIB_DIR)/Milkcocoa/Milkcocoa.o

INCLUDE_PATHS += -I$(MBED_LIB_DIR)/Milkcocoa/MQTT/MQTTPacket
INCLUDE_PATHS += -I$(MBED_LIB_DIR)/Milkcocoa/MQTT/FP
INCLUDE_PATHS += -I$(MBED_LIB_DIR)/Milkcocoa/MQTT
INCLUDE_PATHS += -I$(MBED_LIB_DIR)/Milkcocoa

CDEFS += 
