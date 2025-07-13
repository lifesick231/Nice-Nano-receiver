include ../../build_system/make/make.mk

RECV_ROOT := ../../../../receiver/nrf_usdk52
SDK_PATH := ../../../../nRF5_SDK_17.1.0
URF_DIR := ../../../../receiver/urf

INC += \
	src \
	$(TOP)/hw \
	$(TOP)/hw/mcu/cmsis/include \
	$(URF_DIR) \
	$(RECV_ROOT)/device \
	$(RECV_ROOT)/cmsis/include \
	$(RECV_ROOT) \
	$(SDK_PATH)/config/nrf52840/config \
	$(SDK_PATH)/modules/nrfx/mdk \
	$(SDK_PATH)/modules/nrfx/drivers/include \
	$(SDK_PATH)/components/toolchain/cmsis/include \
	$(SDK_PATH)/modules/nrfx/hal \
	$(SDK_PATH)/modules/nrfx \
	$(SDK_PATH)/integration/nrfx \
	$(SDK_PATH)/components/libraries/util \
	$(SDK_PATH)/components/drivers_nrf/nrf_soc_nosd \
	$(SDK_PATH)/components/libraries/delay \
	
# Example source
EXAMPLE_SOURCE += \
	src/main.c \
	src/msc_disk.c \
	src/usb_descriptors.c \
	$(URF_DIR)/urf_radio.c \
	$(URF_DIR)/urf_timer.c \
	$(URF_DIR)/urf_star_protocol.c \

SRC_C += $(addprefix $(CURRENT_PATH)/, $(EXAMPLE_SOURCE))

include ../../build_system/make/rules.mk
