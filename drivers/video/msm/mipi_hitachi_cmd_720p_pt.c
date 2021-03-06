/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_renesas.h"

static struct msm_panel_info pinfo;

static struct mipi_dsi_phy_ctrl dsi_cmd_mode_phy_db = {
	/* regulator */
	{0x03, 0x0a, 0x04, 0x00, 0x20},
	/* timing */
	{0xb3, 0x8c, 0x1d, 0x00, 0x20, 0x94, 0x20, 0x8e, 
	0x20, 0x03, 0x04, 0x00},
    /* phy ctrl */
	{0x5f, 0x00, 0x00, 0x10},
    /* strength */
	{0xff, 0x00, 0x06, 0x00},
	/* pll control */
	{0x0, 0xB0, 0x1, 0x19, 0x0, 0x50, 0x48, 0x63, 
	0x77, 0x88, 0x99, 
	0x0, 0x14, 0x3, 0x0, 0x2, 0x0, 0x20, 0x0, 0x1, 0x0 },
};

static int __init mipi_cmd_hitachi_720p_pt_init(void)
{
	int ret;

#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_cmd_hitachi_720p"))
		return 0;
#endif

	pinfo.xres = 720;
	pinfo.yres = 1280;
	pinfo.bpp = 24;
	pinfo.type = MIPI_CMD_PANEL;
	pinfo.wait_cycle = 0;
	pinfo.pdest = DISPLAY_1;
	pinfo.bl_max = 127;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;
	pinfo.clk_rate = 450000000;

	pinfo.lcd.vsync_enable = TRUE;
	pinfo.lcd.refx100 = 6000; /* adjust refx100 to prevent tearing */
	pinfo.lcd.v_back_porch = 7;
	pinfo.lcd.v_front_porch = 7;
	pinfo.lcd.v_pulse_width = 1;
	pinfo.lcd.hw_vsync_mode = TRUE;
	pinfo.lcd.vsync_notifier_period = 0;
	pinfo.lcd.rev = 14;
	pinfo.lcdc.h_back_porch = 14;
	pinfo.lcdc.h_front_porch = 220;
	pinfo.lcdc.h_pulse_width = 12;
	pinfo.lcdc.v_back_porch = 7;
	pinfo.lcdc.v_front_porch = 7;
	pinfo.lcdc.v_pulse_width = 1;
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff; 	/* blue */
	pinfo.lcdc.hsync_skew = 0;

	pinfo.mipi.mode = DSI_CMD_MODE;
	pinfo.mipi.dst_format = DSI_CMD_DST_FORMAT_RGB888;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.data_lane2 = TRUE;
	pinfo.mipi.data_lane3 = TRUE;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.t_clk_post = 0x22;
	pinfo.mipi.t_clk_pre = 0x3b;
	pinfo.mipi.vc = 0;
	pinfo.mipi.interleave_max = 1;
	pinfo.mipi.insert_dcs_cmd = TRUE;
	pinfo.mipi.wr_mem_continue = 0x3c;
	pinfo.mipi.wr_mem_start = 0x2c;
	pinfo.mipi.te_sel = 1; /* TE from vsync gpio */
	pinfo.mipi.stream = 0;	/* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW_TE;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.esc_byte_ratio = 4;


	pinfo.mipi.dsi_phy_db = &dsi_cmd_mode_phy_db;

	ret = mipi_renesas_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_720P_PT);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_cmd_hitachi_720p_pt_init);
