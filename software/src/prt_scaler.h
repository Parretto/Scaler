/*
     __        __   __   ___ ___ ___  __  
    |__)  /\  |__) |__) |__   |   |  /  \ 
    |    /~~\ |  \ |  \ |___  |   |  \__/ 

    Module: Scaler Header
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

#pragma once

// Contorl register
#define PRT_SCALER_CTL_RUN			(1 << 0)
#define PRT_SCALER_CTL_VPS_SHIFT		1

// VPS
#define PRT_SCALER_VPS_HTOTAL			0
#define PRT_SCALER_VPS_HWIDTH			1
#define PRT_SCALER_VPS_HSTART			2
#define PRT_SCALER_VPS_HSW			3
#define PRT_SCALER_VPS_VTOTAL			4
#define PRT_SCALER_VPS_VHEIGHT		5
#define PRT_SCALER_VPS_VSTART			6
#define PRT_SCALER_VPS_VSW			7

// Device structure
typedef struct {
	prt_u32 ctl;
	prt_u32 vps;		// Video parameters
} prt_scaler_dev_struct;

// Timing parameters
typedef struct {
	prt_u16 htotal;
	prt_u16 hwidth;
	prt_u16 hstart;
	prt_u16 hsw;
	prt_u16 vtotal;
	prt_u16 vheight;
	prt_u16 vstart;
	prt_u16 vsw;
} prt_scaler_tp_struct;

// Data structure
typedef struct {
	volatile prt_scaler_dev_struct 	*dev;	// Device
	prt_scaler_tp_struct 			tp;		// Timing parameters
} prt_scaler_ds_struct;


// Prototypes
void prt_scaler_set_base (prt_scaler_ds_struct *scaler, prt_u32 base);
void prt_scaler_set_tp (prt_scaler_ds_struct *scaler, prt_scaler_tp_struct *tp);
void prt_scaler_vps_wr (prt_scaler_ds_struct *scaler, prt_u8 vps, prt_u32 dat);
void prt_scaler_str (prt_scaler_ds_struct *scaler);
void prt_scaler_stp (prt_scaler_ds_struct *scaler);
