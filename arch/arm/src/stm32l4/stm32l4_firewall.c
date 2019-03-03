
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/arch.h>

#include "up_arch.h"
#include "chip/stm32l4_syscfg.h"

#include "stm32l4_firewall.h"

int stm32l4_firewallsetup(FAR struct stm32l4_firewall_t *setup)
  {
    uint32_t reg;

    /* code and nvdata must be aligned to 256 bytes
     * data must be aligned to 64 bytes
     */
    if( (setup->codestart & 0xFF) || (setup->nvdatastart & 0xFF) || (setup->datastart & 0x3F) )
      {
        return -1;
      }

    /* code and nvdata length must be a multiple of 256 bytes
     * data length must be a multiple of 64 bytes
     */
    if( (setup->codelen & 0xFF) || (setup->nvdatalen & 0xFF) || (setup->datalen & 0x3F) )
      {
        return -1;
      }

    /*
     * code and nvdata must be in flash
     * data must be in SRAM1
     */
    if( (setup->codestart & STM32L4_REGION_MASK) != STM32L4_FLASH_BASE)
      {
        return -1;
      }

    if( (setup->nvdatastart & STM32L4_REGION_MASK) != STM32L4_FLASH_BASE)
      {
        return -1;
      }

    /* Define address and length registers */
    modifyreg32(STM32L4_FIREWALL_CSSA  , FIREWALL_CSSADD_MASK  , setup->codestart  );
    modifyreg32(STM32L4_FIREWALL_CSL   , FIREWALL_CSSLENG_MASK , setup->codelen    );
    modifyreg32(STM32L4_FIREWALL_NVDSSA, FIREWALL_NVDSADD_MASK , setup->nvdatastart);
    modifyreg32(STM32L4_FIREWALL_NVDSL , FIREWALL_NVDSLENG_MASK, setup->nvdatalen  );
    modifyreg32(STM32L4_FIREWALL_VDSSA , FIREWALL_VDSADD_MASK  , setup->datastart  );
    modifyreg32(STM32L4_FIREWALL_VDSL  , FIREWALL_VDSLENG_MASK , setup->datalen    );

    /* Define access options */
    reg = getreg32(STM32L4_FIREWALL_CR);
    if(setup->datashared)
      {
        reg |= FIREWALL_CR_VDS;
      }
    if(setup->dataexec)
      {
        reg |= FIREWALL_CR_VDE;
      }

    putreg32(reg, STM32L4_FIREWALL_CR);

    /* Enable firewall */
    reg = getreg32(STM32L4_SYSCFG_CFGR1);
    reg &= ~SYSCFG_CFGR1_FWDIS;
    putreg32(reg, STM32L4_SYSCFG_CFGR1);

    /* Now protected code can only be accessed by jumping to the FW gate */
    return 0;
  }

