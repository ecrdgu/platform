
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_netinitialize (stub)
 *
 * Description:
 *   This is a stub version os up_netinitialize.  Normally, up_netinitialize
 *   is defined in board/xyz_network.c for board-specific Ethernet
 *   implementations, or chip/xyx_ethernet.c for chip-specific Ethernet
 *   implementations.  The stub version here is used in the corner case where
 *   the network is enable yet there is no Ethernet driver to be initialized.
 *   In this case, up_initialize will still try to call up_netinitialize()
 *   when one does not exist.  This corner case would occur if, for example,
 *   only a USB network interface is being used or perhaps if a SLIP is
 *   being used).
 *
 *   Use of this stub is deprecated.  The preferred mechanism is to use
 *   CONFIG_NETDEV_LATEINIT=y to suppress the call to up_netinitialize() in
 *   up_initialize().  Then this stub would not be needed.
 *
 ****************************************************************************/

void up_netinitialize(void)
{
}
