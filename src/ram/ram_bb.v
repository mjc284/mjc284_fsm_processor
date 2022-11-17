// megafunction wizard: %ALTUFM_NONE Intel FPGA IP%VBB%
// GENERATION: STANDARD
// VERSION: WM1.0
// MODULE: ALTUFM_NONE 

// ============================================================
// File Name: ram.v
// Megafunction Name(s):
// 			ALTUFM_NONE
//
// Simulation Library Files(s):
// 			maxii
// ============================================================
// ************************************************************
// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
//
// 18.1.0 Build 625 09/12/2018 SJ Lite Edition
// ************************************************************

//Copyright (C) 2018  Intel Corporation. All rights reserved.
//Your use of Intel Corporation's design tools, logic functions 
//and other software and tools, and its AMPP partner logic 
//functions, and any output files from any of the foregoing 
//(including device programming or simulation files), and any 
//associated documentation or information are expressly subject 
//to the terms and conditions of the Intel Program License 
//Subscription Agreement, the Intel Quartus Prime License Agreement,
//the Intel FPGA IP License Agreement, or other applicable license
//agreement, including, without limitation, that your use is for
//the sole purpose of programming logic devices manufactured by
//Intel and sold by Intel or its authorized distributors.  Please
//refer to the applicable agreement for further details.

module ram (
	arclk,
	ardin,
	arshft,
	drclk,
	drdin,
	drshft,
	erase,
	oscena,
	program,
	busy,
	drdout,
	osc,
	rtpbusy)/* synthesis synthesis_clearbox = 1 */;

	input	  arclk;
	input	  ardin;
	input	  arshft;
	input	  drclk;
	input	  drdin;
	input	  drshft;
	input	  erase;
	input	  oscena;
	input	  program;
	output	  busy;
	output	  drdout;
	output	  osc;
	output	  rtpbusy;

endmodule

// ============================================================
// CNX file retrieval info
// ============================================================
// Retrieval info: LIBRARY: altera_mf altera_mf.altera_mf_components.all
// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "MAX II"
// Retrieval info: CONSTANT: ERASE_TIME NUMERIC "500000000"
// Retrieval info: CONSTANT: INTENDED_DEVICE_FAMILY STRING "MAX II"
// Retrieval info: CONSTANT: LPM_FILE STRING "program.mif"
// Retrieval info: CONSTANT: LPM_HINT STRING "UNUSED"
// Retrieval info: CONSTANT: LPM_TYPE STRING "altufm_none"
// Retrieval info: CONSTANT: OSC_FREQUENCY NUMERIC "180000"
// Retrieval info: CONSTANT: PORT_ARCLKENA STRING "PORT_UNUSED"
// Retrieval info: CONSTANT: PORT_DRCLKENA STRING "PORT_UNUSED"
// Retrieval info: CONSTANT: PROGRAM_TIME NUMERIC "1600000"
// Retrieval info: CONSTANT: WIDTH_UFM_ADDRESS NUMERIC "9"
// Retrieval info: USED_PORT: arclk 0 0 0 0 INPUT NODEFVAL "arclk"
// Retrieval info: CONNECT: @arclk 0 0 0 0 arclk 0 0 0 0
// Retrieval info: USED_PORT: ardin 0 0 0 0 INPUT NODEFVAL "ardin"
// Retrieval info: CONNECT: @ardin 0 0 0 0 ardin 0 0 0 0
// Retrieval info: USED_PORT: arshft 0 0 0 0 INPUT NODEFVAL "arshft"
// Retrieval info: CONNECT: @arshft 0 0 0 0 arshft 0 0 0 0
// Retrieval info: USED_PORT: busy 0 0 0 0 OUTPUT NODEFVAL "busy"
// Retrieval info: CONNECT: busy 0 0 0 0 @busy 0 0 0 0
// Retrieval info: USED_PORT: drclk 0 0 0 0 INPUT NODEFVAL "drclk"
// Retrieval info: CONNECT: @drclk 0 0 0 0 drclk 0 0 0 0
// Retrieval info: USED_PORT: drdin 0 0 0 0 INPUT NODEFVAL "drdin"
// Retrieval info: CONNECT: @drdin 0 0 0 0 drdin 0 0 0 0
// Retrieval info: USED_PORT: drdout 0 0 0 0 OUTPUT NODEFVAL "drdout"
// Retrieval info: CONNECT: drdout 0 0 0 0 @drdout 0 0 0 0
// Retrieval info: USED_PORT: drshft 0 0 0 0 INPUT NODEFVAL "drshft"
// Retrieval info: CONNECT: @drshft 0 0 0 0 drshft 0 0 0 0
// Retrieval info: USED_PORT: erase 0 0 0 0 INPUT NODEFVAL "erase"
// Retrieval info: CONNECT: @erase 0 0 0 0 erase 0 0 0 0
// Retrieval info: USED_PORT: osc 0 0 0 0 OUTPUT NODEFVAL "osc"
// Retrieval info: CONNECT: osc 0 0 0 0 @osc 0 0 0 0
// Retrieval info: USED_PORT: oscena 0 0 0 0 INPUT NODEFVAL "oscena"
// Retrieval info: CONNECT: @oscena 0 0 0 0 oscena 0 0 0 0
// Retrieval info: USED_PORT: program 0 0 0 0 INPUT NODEFVAL "program"
// Retrieval info: CONNECT: @program 0 0 0 0 program 0 0 0 0
// Retrieval info: USED_PORT: rtpbusy 0 0 0 0 OUTPUT NODEFVAL "rtpbusy"
// Retrieval info: CONNECT: rtpbusy 0 0 0 0 @rtpbusy 0 0 0 0
// Retrieval info: GEN_FILE: TYPE_NORMAL ram.v TRUE FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL ram.qip TRUE FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL ram.bsf TRUE TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL ram_inst.v TRUE TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL ram_bb.v TRUE TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL ram.inc TRUE TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL ram.cmp TRUE TRUE
// Retrieval info: LIB_FILE: maxii
