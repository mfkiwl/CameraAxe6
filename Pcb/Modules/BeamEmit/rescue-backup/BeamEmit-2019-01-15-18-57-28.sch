EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:CA6
LIBS:BeamEmit-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L C_Small C3
U 1 1 59FF8067
P 1450 5350
F 0 "C3" H 1460 5420 50  0000 L CNN
F 1 "10uF" H 1460 5270 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 1450 5350 50  0001 C CNN
F 3 "" H 1450 5350 50  0001 C CNN
	1    1450 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 5A21D01D
P 1050 5600
F 0 "#PWR01" H 1050 5350 50  0001 C CNN
F 1 "GND" H 1050 5450 50  0000 C CNN
F 2 "" H 1050 5600 50  0000 C CNN
F 3 "" H 1050 5600 50  0000 C CNN
	1    1050 5600
	1    0    0    -1  
$EndComp
$Comp
L CAP_ELEC C1
U 1 1 5A21D205
P 1050 5350
F 0 "C1" H 1100 5450 50  0000 L CNN
F 1 "100uF" H 1100 5250 50  0000 L CNN
F 2 "CA6:c_elec_6.3x5.7" H 1050 5350 60  0001 C CNN
F 3 "" H 1050 5350 60  0000 C CNN
	1    1050 5350
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5A21EBE0
P 2450 4000
F 0 "R2" V 2530 4000 50  0000 C CNN
F 1 "100K" V 2450 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2380 4000 50  0001 C CNN
F 3 "" H 2450 4000 50  0001 C CNN
	1    2450 4000
	-1   0    0    1   
$EndComp
$Comp
L C_Small C2
U 1 1 5A21ED48
P 1800 5350
F 0 "C2" H 1810 5420 50  0000 L CNN
F 1 "0.1uF" H 1810 5270 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1800 5350 50  0001 C CNN
F 3 "" H 1800 5350 50  0001 C CNN
	1    1800 5350
	1    0    0    -1  
$EndComp
$Comp
L HOLE X1
U 1 1 5A22D73E
P 4050 5500
F 0 "X1" H 4050 5400 60  0000 C CNN
F 1 "HOLE" H 4050 5600 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 4050 5500 60  0001 C CNN
F 3 "" H 4050 5500 60  0000 C CNN
	1    4050 5500
	1    0    0    -1  
$EndComp
$Comp
L HOLE X2
U 1 1 5A22E2CB
P 4350 5500
F 0 "X2" H 4350 5400 60  0000 C CNN
F 1 "HOLE" H 4350 5600 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 4350 5500 60  0001 C CNN
F 3 "" H 4350 5500 60  0000 C CNN
	1    4350 5500
	1    0    0    -1  
$EndComp
$Comp
L HOLE X3
U 1 1 5A22E316
P 4650 5500
F 0 "X3" H 4650 5400 60  0000 C CNN
F 1 "HOLE" H 4650 5600 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 4650 5500 60  0001 C CNN
F 3 "" H 4650 5500 60  0000 C CNN
	1    4650 5500
	1    0    0    -1  
$EndComp
$Comp
L HOLE X4
U 1 1 5A22E364
P 4950 5500
F 0 "X4" H 4950 5400 60  0000 C CNN
F 1 "HOLE" H 4950 5600 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 4950 5500 60  0001 C CNN
F 3 "" H 4950 5500 60  0000 C CNN
	1    4950 5500
	1    0    0    -1  
$EndComp
$Comp
L JACK_RJ45 J1
U 1 1 5ABE5A75
P 1400 3950
F 0 "J1" H 1575 4450 39  0000 C CNN
F 1 "JACK_RJ45" H 1225 4450 39  0000 C CNN
F 2 "CA6:RJ45_pcbwing" H 1375 3950 60  0001 C CNN
F 3 "" H 1375 3950 60  0000 C CNN
	1    1400 3950
	0    -1   1    0   
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 5ABE5EDB
P 1050 4800
F 0 "#PWR02" H 1050 4650 50  0001 C CNN
F 1 "+3.3V" H 1050 4940 50  0000 C CNN
F 2 "" H 1050 4800 50  0001 C CNN
F 3 "" H 1050 4800 50  0001 C CNN
	1    1050 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59FD7258
P 3550 5000
F 0 "#PWR03" H 3550 4750 50  0001 C CNN
F 1 "GND" H 3550 4850 50  0000 C CNN
F 2 "" H 3550 5000 50  0000 C CNN
F 3 "" H 3550 5000 50  0000 C CNN
	1    3550 5000
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 59FD89F4
P 3550 3600
F 0 "R1" V 3630 3600 50  0000 C CNN
F 1 "4R7" V 3550 3600 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 3480 3600 50  0001 C CNN
F 3 "" H 3550 3600 50  0001 C CNN
	1    3550 3600
	-1   0    0    1   
$EndComp
$Comp
L LED D2
U 1 1 5A221E37
P 3550 3950
F 0 "D2" H 3500 4050 50  0000 L CNN
F 1 "IR_LED" H 3550 3800 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm_Horizontal_O1.27mm_Z15.0mm" H 3550 4125 50  0001 C CNN
F 3 "" H 3500 3950 50  0001 C CNN
	1    3550 3950
	0    -1   -1   0   
$EndComp
$Comp
L LED D1
U 1 1 59CD43E5
P 3550 4300
F 0 "D1" H 3500 4400 50  0000 L CNN
F 1 "IR_LED" H 3550 4150 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm_Horizontal_O1.27mm_Z15.0mm" H 3550 4475 50  0001 C CNN
F 3 "" H 3500 4300 50  0001 C CNN
	1    3550 4300
	0    -1   -1   0   
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 59FD80EC
P 3450 4650
F 0 "Q1" H 3650 4700 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 3650 4600 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3650 4750 50  0001 C CNN
F 3 "" H 3450 4650 50  0000 C CNN
	1    3450 4650
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 59D0E70C
P 2750 3800
F 0 "R4" V 2830 3800 50  0000 C CNN
F 1 "1K" V 2750 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2680 3800 50  0001 C CNN
F 3 "" H 2750 3800 50  0001 C CNN
	1    2750 3800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1050 5550 1050 5600
Connection ~ 1050 5550
Wire Wire Line
	1450 5550 1450 5450
Wire Wire Line
	1450 4850 1450 5250
Wire Wire Line
	3550 4850 3550 5000
Wire Wire Line
	3550 4100 3550 4150
Wire Wire Line
	3550 3750 3550 3800
Wire Wire Line
	3550 3400 3550 3450
Wire Wire Line
	3100 4650 3250 4650
Wire Wire Line
	3100 3800 3100 4650
Wire Wire Line
	2900 3800 3100 3800
Wire Wire Line
	1800 5550 1800 5450
Connection ~ 1450 5550
Wire Wire Line
	1800 4850 1800 5250
Connection ~ 1450 4850
Wire Wire Line
	1050 4800 1050 5150
Connection ~ 1050 4850
Wire Wire Line
	1050 4850 1800 4850
Wire Wire Line
	1050 5550 1800 5550
$Comp
L +3.3V #PWR05
U 1 1 5ABE7EAD
P 1900 3550
F 0 "#PWR05" H 1900 3400 50  0001 C CNN
F 1 "+3.3V" H 1900 3690 50  0000 C CNN
F 2 "" H 1900 3550 50  0001 C CNN
F 3 "" H 1900 3550 50  0001 C CNN
	1    1900 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5ABE7EE2
P 1900 4350
F 0 "#PWR06" H 1900 4100 50  0001 C CNN
F 1 "GND" H 1900 4200 50  0000 C CNN
F 2 "" H 1900 4350 50  0000 C CNN
F 3 "" H 1900 4350 50  0000 C CNN
	1    1900 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 3600 1900 3600
Wire Wire Line
	1900 3600 1900 3550
Wire Wire Line
	1850 4300 1900 4300
Wire Wire Line
	1900 4300 1900 4350
$Comp
L GND #PWR07
U 1 1 5ABE81CF
P 2450 4250
F 0 "#PWR07" H 2450 4000 50  0001 C CNN
F 1 "GND" H 2450 4100 50  0000 C CNN
F 2 "" H 2450 4250 50  0000 C CNN
F 3 "" H 2450 4250 50  0000 C CNN
	1    2450 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 3800 2600 3800
Wire Wire Line
	2450 3850 2450 3800
Connection ~ 2450 3800
Wire Wire Line
	2450 4250 2450 4150
NoConn ~ 1850 3700
NoConn ~ 1850 3900
NoConn ~ 1850 4000
NoConn ~ 1850 4100
NoConn ~ 1850 4200
$Comp
L +3.3V #PWR?
U 1 1 5ABE5DD5
P 3550 3400
F 0 "#PWR?" H 3550 3250 50  0001 C CNN
F 1 "+3.3V" H 3550 3540 50  0000 C CNN
F 2 "" H 3550 3400 50  0001 C CNN
F 3 "" H 3550 3400 50  0001 C CNN
	1    3550 3400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
