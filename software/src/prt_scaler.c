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
	scaler->tp.htotal = tp->htotal;
	scaler->tp.hwidth = tp->hwidth;
	scaler->tp.hstart = tp->hstart;
	scaler->tp.hsw = tp->hsw;
	scaler->tp.vtotal = tp->vtotal + 4;	// The scaler has vertical latency of 4 lines
	scaler->tp.vheight = tp->vheight + 4;	// See vtotal
	scaler->tp.vstart = tp->vstart;
	scaler->tp.vsw = tp->vsw;
}

// Write video parameter set
void prt_scaler_vps_wr (prt_scaler_ds_struct *scaler, prt_u8 vps, prt_u32 dat)
{
	scaler->dev->ctl = vps << PRT_SCALER_CTL_VPS_SHIFT;
	scaler->dev->vps = dat;
}

// Start
void prt_scaler_str (prt_scaler_ds_struct *scaler)
{
	// Copy timing parameters to device
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_HTOTAL, scaler->tp.htotal);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_HWIDTH, scaler->tp.hwidth);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_HSTART, scaler->tp.hstart);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_HSW, scaler->tp.hsw);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_VTOTAL, scaler->tp.vtotal);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_VHEIGHT, scaler->tp.vheight);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_VSTART, scaler->tp.vstart);
	prt_scaler_vps_wr (scaler, PRT_SCALER_VPS_VSW, scaler->tp.vsw);

	// Assert run flag
	scaler->dev->ctl = PRT_SCALER_CTL_RUN;
}

// Stop
void prt_scaler_stp (prt_scaler_ds_struct *scaler)
{
	// Clear run flag
	scaler->dev->ctl = 0;
}
