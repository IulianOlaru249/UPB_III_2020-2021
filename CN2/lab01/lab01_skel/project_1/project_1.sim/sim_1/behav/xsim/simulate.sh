#!/bin/bash -f
# ****************************************************************************
# Vivado (TM) v2019.2 (64-bit)
#
# Filename    : simulate.sh
# Simulator   : Xilinx Vivado Simulator
# Description : Script for simulating the design by launching the simulator
#
# Generated by Vivado on Fri Oct 23 18:07:48 EEST 2020
# SW Build 2708876 on Wed Nov  6 21:39:14 MST 2019
#
# Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
#
# usage: simulate.sh
#
# ****************************************************************************
set -Eeuo pipefail
echo "xsim task04_test_behav -key {Behavioral:sim_1:Functional:task04_test} -tclbatch task04_test.tcl -log simulate.log"
xsim task04_test_behav -key {Behavioral:sim_1:Functional:task04_test} -tclbatch task04_test.tcl -log simulate.log

