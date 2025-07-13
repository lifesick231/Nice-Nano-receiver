PROJECT_NAME     := udevices_base
TARGETS          := ubase
OUTPUT_DIRECTORY := build

RECV_ROOT := ./receiver/nrf_usdk52
SDK_PATH := ./nRF5_SDK_17.1.0
URF_DIR := ./receiver/urf

$(OUTPUT_DIRECTORY)/ubase.out: \
  LINKER_SCRIPT  := nrf52840_xxaa.ld

# Source files common to all targets
SRC_FILES += \
  ./udevices_base_main.c \
  $(URF_DIR)/urf_radio.c \
  $(URF_DIR)/urf_timer.c \
  $(URF_DIR)/urf_star_protocol.c \
  $(RECV_ROOT)/device/gcc_startup_nrf52840.S \
  $(RECV_ROOT)/system_nrf52840.c \

# Include folders common to all targets
INC_FOLDERS += \
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


# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10056
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF52
CFLAGS += -DNRF52840_XXAA
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums 

# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10056
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52840_XXAA

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs


# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default

# Default target - first one defined
default: ubase

TEMPLATE_PATH := $(RECV_ROOT)/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

