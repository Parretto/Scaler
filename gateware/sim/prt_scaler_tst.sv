/*
     __        __   __   ___ ___ ___  __  
    |__)  /\  |__) |__) |__   |   |  /  \ 
    |    /~~\ |  \ |  \ |___  |   |  \__/ 


    Module: Scaler Test Bench
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

`timescale 1ns / 1ps
`default_nettype none

module scaler_tst ();

// Parameters
localparam P_SYS_FREQ = 'd100000000;    // System frequency
localparam P_BPC = 8;
localparam P_PPC = 4;
localparam P_VENDOR = "xilinx";         // Vendor

// Interfaces

// DPTX (dangeling)
prt_dp_lb_if
#(
  .P_ADR_WIDTH  (16)
)
dptx_if();

// DPRX (dangeling)
prt_dp_lb_if
#(
  .P_ADR_WIDTH  (16)
)
dprx_if();

// VTB
prt_dp_lb_if
#(
  .P_ADR_WIDTH  (16)
)
vtb_if();

prt_dp_lb_if
#(
  .P_ADR_WIDTH  (16)
)
phy_if();

prt_dp_lb_if
#(
  .P_ADR_WIDTH  (16)
)
scaler_if();

// Signals
logic sys_clk;
logic vid_clk;
logic rst;

// Scaler
wire                        cke_from_scaler;
wire                        vs_from_scaler;
wire                        hs_from_scaler;
wire [(P_PPC*P_BPC)-1:0]    r_from_scaler;
wire [(P_PPC*P_BPC)-1:0]    g_from_scaler;
wire [(P_PPC*P_BPC)-1:0]    b_from_scaler;
wire                        de_from_scaler;

// VTB
wire                        vs_from_vtb;
wire                        hs_from_vtb;
wire [(P_PPC*P_BPC)-1:0]    r_from_vtb;
wire [(P_PPC*P_BPC)-1:0]    g_from_vtb;
wire [(P_PPC*P_BPC)-1:0]    b_from_vtb;
wire                        de_from_vtb;

// System clock 100 MHz
initial
begin
    sys_clk <= 0;
    forever
        #5ns sys_clk <= ~sys_clk;
end

// Reset
initial
begin
    rst <= 1;
    #500ns
    rst <= 0;
end

// Video clock 74.25 MHz
initial
begin
    vid_clk <= 0;
    forever
        #6.734ns vid_clk <= ~vid_clk;
end

// DUT
     prt_scaler_top
     #(
          .P_PPC    (P_PPC),      // Pixels per clock
          .P_BPC    (P_BPC)       // Bits per component
     )
     DUT_INST
     (
          // System
          .SYS_RST_IN         (rst),
          .SYS_CLK_IN         (sys_clk),

          // Local bus
          .LB_IF              (scaler_if),

          // Reset and clock
          .VID_CLK_IN   (vid_clk),

          // Video in
          .VID_VS_IN    (vs_from_vtb),                               // Vertical sync    
          .VID_HS_IN    (hs_from_vtb),                               // Horizontal sync    
          .VID_R_IN     (r_from_vtb),      // Data
          .VID_G_IN     (g_from_vtb),      // Data
          .VID_B_IN     (b_from_vtb),      // Data
          .VID_DE_IN    (de_from_vtb),                               // Data enable

          // Video out
          .VID_CKE_OUT  (cke_from_scaler),
          .VID_VS_OUT   (vs_from_scaler),
          .VID_HS_OUT   (hs_from_scaler),       // Horizontal sync    
          .VID_R_OUT    (r_from_scaler),       // Data
          .VID_G_OUT    (g_from_scaler),       // Data
          .VID_B_OUT    (b_from_scaler),       // Data
          .VID_DE_OUT   (de_from_scaler)        // Data enable
     );

// Video toolbox
    prt_vtb_top
    #(
        .P_SYS_FREQ         (P_SYS_FREQ),
        .P_PPC              (P_PPC),        // Pixels per clock
        .P_BPC              (P_BPC),        // Bits per component
        .P_AXIS_DAT         (96)
    )
    VTB_INST
    (
        // System
        .SYS_RST_IN         (rst),
        .SYS_CLK_IN         (sys_clk),

        // Local bus
        .LB_IF              (vtb_if),

        // Direct I2C Access
        .DIA_RDY_IN         (1'b0),
        .DIA_DAT_OUT        (),
        .DIA_VLD_OUT        (),

        // Link
        .TX_LNK_CLK_IN      (1'b0),           // TX link clock
        .RX_LNK_CLK_IN      (1'b0),           // RX link clock
        .LNK_SYNC_IN        (1'b0),
        
        // Axi-stream Video
        .AXIS_SOF_IN        (1'b0),  // Start of frame
        .AXIS_EOL_IN        (1'b0),  // End of line
        .AXIS_DAT_IN        (96'd0),  // Data
        .AXIS_VLD_IN        (1'b0),  // Valid       

        // Native video
        .VID_CLK_IN         (vid_clk),
        .VID_CKE_IN         (cke_from_scaler),
        .VID_VS_OUT         (vs_from_vtb),
        .VID_HS_OUT         (hs_from_vtb),
        .VID_R_OUT          (r_from_vtb),
        .VID_G_OUT          (g_from_vtb),
        .VID_B_OUT          (b_from_vtb),
        .VID_DE_OUT         (de_from_vtb)
    );

// Application
     dp_app_top
     #(
          .P_VENDOR (P_VENDOR),
          .P_ROM_INIT ("../../software/build/sim/bin/scaler_sim_rom.mem"),
          .P_RAM_INIT ("../../software/build/sim/bin/scaler_sim_ram.mem")
     )
     APP_INST
     (
        // Reset and clock
        .RST_IN        (rst),
        .CLK_IN        (sys_clk),

        // PIO
        .PIO_DAT_IN     (0),
        .PIO_DAT_OUT    (),

        // Uart
        .UART_RX_IN    (1'b0),
        .UART_TX_OUT   (),

        // I2C
        .I2C_SCL_INOUT  (),
        .I2C_SDA_INOUT  (),

        // Direct I2C Access
        .DIA_RDY_OUT    (),
        .DIA_DAT_IN     (32'h0),
        .DIA_VLD_IN     (1'b0),

        // DPTX interface (not used)
        .DPTX_IF        (dptx_if),
        .DPTX_IRQ_IN    (1'b0),

        // DPRX interface (not used)
        .DPRX_IF        (dprx_if),
        .DPRX_IRQ_IN    (1'b0),

        // VTB interface (VTB TX)
        .VTB_IF         (vtb_if),

        // PHY interface
        .PHY_IF         (phy_if),

        // Scaler interface
        .SCALER_IF      (scaler_if),

        // Aqua
        .AQUA_SEL_IN   (1'b0),
        .AQUA_CTL_IN   (1'b0),
        .AQUA_CLK_IN   (1'b0),
        .AQUA_DAT_IN   (1'b0)
     );

endmodule

`default_nettype wire
