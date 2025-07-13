/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00027000, LENGTH = 0x5A000
  EXTFLASH (rx) : ORIGIN = 0x12000000, LENGTH = 0x8000000
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x40000
  CODE_RAM (rwx) : ORIGIN = 0x800000, LENGTH = 0x40000
}


INCLUDE "nRF5_SDK_17.1.0/modules/nrfx/mdk/nrf_common.ld"
