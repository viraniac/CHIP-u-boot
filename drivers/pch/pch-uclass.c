/*
 * Copyright (c) 2015 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <pch.h>
#include <dm/root.h>

DECLARE_GLOBAL_DATA_PTR;

int pch_get_sbase(struct udevice *dev, ulong *sbasep)
{
	struct pch_ops *ops = pch_get_ops(dev);

	*sbasep = 0;
	if (!ops->get_sbase)
		return -ENOSYS;

	return ops->get_sbase(dev, sbasep);
}

enum pch_version pch_get_version(struct udevice *dev)
{
	struct pch_ops *ops = pch_get_ops(dev);

	if (!ops->get_version)
		return -ENOSYS;

	return ops->get_version(dev);
}

int pch_set_spi_protect(struct udevice *dev, bool protect)
{
	struct pch_ops *ops = pch_get_ops(dev);

	if (!ops->set_spi_protect)
		return -ENOSYS;

	return ops->set_spi_protect(dev, protect);
}

static int pch_uclass_post_bind(struct udevice *bus)
{
	/*
	 * Scan the device tree for devices
	 *
	 * Before relocation, only bind devices marked for pre-relocation
	 * use.
	 */
	return dm_scan_fdt_node(bus, gd->fdt_blob, bus->of_offset,
				gd->flags & GD_FLG_RELOC ? false : true);
}

UCLASS_DRIVER(pch) = {
	.id		= UCLASS_PCH,
	.name		= "pch",
	.post_bind	= pch_uclass_post_bind,
};
