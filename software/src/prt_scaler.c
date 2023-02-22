/*
     __        __   __   ___ ___ ___  __  
    |__)  /\  |__) |__) |__   |   |  /  \ 
    |    /~~\ |  \ |  \ |___  |   |  \__/ 

    Module: Scaler Driver
    (c) 2022 by Parretto B.V.

    History
    =======
    v1.0 - Initial release

    License
    =======
    This License will apply to the use of the IP-core (as defined in the License). 
    Please read the License carefully so that you know what your rights and obligations are when using the IP-core.
    The acceptance of this License constitutes a valid and binding agreement between Parretto and you for the use of the IP-core. 
    If you download and/or make any use of the IP-core you agree to be bound by this License. 
    The License is available for download and print at www.parretto.com/license.html
    Parretto grants you, as the Licensee, a free, non-exclusive, non-transferable, limited right to use the IP-core 
    solely for internal business purposes for the term and conditions of the License. 
    You are also allowed to create Modifications for internal business purposes, but explicitly only under the conditions of art. 3.2.
    You are, however, obliged to pay the License Fees to Parretto for the use of the IP-core, or any Modification, in, or embodied in, 
    a physical or non-tangible product or service that has substantial commercial, industrial or non-consumer uses. 
*/

// Includes
#include "prt_types.h"
#include "prt_scaler.h"

// Set base address
void prt_scaler_set_base (prt_scaler_ds_struct *scaler, prt_u32 base)
{
	// Base address
	scaler->dev = (prt_scaler_dev_struct *) base;
}

// Set timing parameters
void prt_scaler_set_tp (prt_scaler_ds_struct *scaler, prt_scaler_tp_struct *tp)
{
	scaler->tp.dst_htotal 	= tp->dst_htotal;
	scaler->tp.dst_hwidth 	= tp->dst_hwidth;
	scaler->tp.dst_hstart 	= tp->dst_hstart;
	scaler->tp.dst_hsw 		= tp->dst_hsw;
	scaler->tp.dst_vtotal 	= tp->dst_vtotal;	
	scaler->tp.dst_vheight 	= tp->dst_vheight;	
	scaler->tp.dst_vstart 	= tp->dst_vstart;
	scaler->tp.dst_vsw 		= tp->dst_vsw;
	scaler->tp.src_hwidth 	= tp->src_hwidth;
	scaler->tp.src_vheight 	= tp->src_vheight;	
}

// Write video parameter set
void prt_scaler_vps_wr (prt_scaler_ds_struct *scaler, prt_u8 vps, prt_u32 dat)
{
	// Variables
	prt_u32 ctl;
	prt_u32 msk;

	// Read control register
	ctl = scaler->dev->ctl;
	
	// Mask bits
	msk = ~(0xf << PRT_SCALER_CTL_VPS_SHIFT);
	ctl &= msk;

	// Set VPS address
	ctl |= vps << PRT_SCALER_CTL_VPS_SHIFT;

	// Write VPS address
	scaler->dev->ctl = ctl;

	// Write data
	scaler->dev->vps = dat;
}

// Start
void prt_scaler_str (prt_scaler_ds_struct *scaler, prt_u8 cr, prt_u8 mode)
{
	// Variables
	prt_u32 dat;
	prt_u16 ver_len;
	prt_u16 hor_len;

	// Clock ratio
	dat = (cr << PRT_SCALER_CTL_CR_SHIFT);

	// Set mode
	dat |= (mode << PRT_SCALER_CTL_MODE_SHIFT);

	// Write control register
	scaler->dev->ctl = dat;

	// Copy timing parameters to device
	// Destination
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_HTOTAL, scaler->tp.dst_htotal);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_HWIDTH, scaler->tp.dst_hwidth);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_HSTART, scaler->tp.dst_hstart);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_HSW, scaler->tp.dst_hsw);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_VTOTAL, scaler->tp.dst_vtotal);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_VHEIGHT, scaler->tp.dst_vheight);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_VSTART, scaler->tp.dst_vstart);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_DST_VSW, scaler->tp.dst_vsw);
	
	// Source
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_SRC_HWIDTH, scaler->tp.src_hwidth);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_SRC_VHEIGHT, scaler->tp.src_vheight);

	// Read control register
	dat = scaler->dev->ctl;

	// Set run flag
	dat |= PRT_SCALER_CTL_RUN;
	
	// Write control register
	scaler->dev->ctl = dat;
}

// Stop
void prt_scaler_stp (prt_scaler_ds_struct *scaler)
{
	// Clear run flag
	scaler->dev->ctl = 0;
}
