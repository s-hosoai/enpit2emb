#
#  ターゲットの指定（Makefile.targetで上書きされるのを防ぐため）
#
all:

#
#  アプリケーションファイル
#
APPLNAME = app
USE_CXX = true
APPL_CFG = $(APPLNAME).cfg
USE_TRUESTUDIO = true

#DEBUG = 1
DEBUG = 0
#OMIT_OPTIMIZATION = true

#
#  MBEDライブラリのディレクトリの定義
#
MBED_LIB_DIR = ../../../mbed-lib

#
#  ASPソースファイルのディレクトリの定義
#
SRCDIR = ../../../asp-1.9.2-utf8

#
#  MBEDライブラリのビルド
#
include $(MBED_LIB_DIR)/mbed-src/Makefile.src
include $(MBED_LIB_DIR)/EthernetInterface/EthernetInterface.mk
include $(MBED_LIB_DIR)/GraphicsFramework/GraphicsFramework.mk
include $(MBED_LIB_DIR)/GR-PEACH_Camera/GR-PEACH_Camera.mk
include $(MBED_LIB_DIR)/GR-PEACH_video/GR-PEACH_video.mk
include $(MBED_LIB_DIR)/GR-PEACH_WlanBP3595STA/GR-PEACH_WlanBP3595STA.mk
include $(MBED_LIB_DIR)/HttpServer_snapshot/HttpServer_snapshot.mk
include $(MBED_LIB_DIR)/i2c/i2c.mk
include $(MBED_LIB_DIR)/mbed-rpc/mbed-rpc.mk
include $(MBED_LIB_DIR)/R_BSP/R_BSP.mk
include $(MBED_LIB_DIR)/SDFileSystem/SDFileSystem.mk
include $(MBED_LIB_DIR)/TextLCD_SB1602E/TextLCD_SB1602E.mk
include $(MBED_LIB_DIR)/common/Makefile.cmn
include $(MBED_LIB_DIR)/Milkcocoa/Milkcocoa.mk
include $(MBED_LIB_DIR)/Zumo/Zumo.mk
include ../Makefile.mbd

#
#  ASPカーネルライブラリ
#
KERNEL_LIB = .
include ../Makefile.asp
