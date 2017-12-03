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
LIBS:Beam-cache
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
L TSOP585xx U3
U 1 1 59CD457D
P 6850 1650
F 0 "U3" H 6450 1950 50  0000 L CNN
F 1 "TSOP585xx" H 6450 1350 50  0000 L CNN
F 2 "Opto-Devices:IRReceiver_Vishay_MINICAST-3pin" H 6800 1275 50  0001 C CNN
F 3 "" H 7500 1950 50  0001 C CNN
	1    6850 1650
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR01
U 1 1 59FCE3A1
P 4500 2050
F 0 "#PWR01" H 4500 1900 50  0001 C CNN
F 1 "+3.3V" H 4500 2190 50  0000 C CNN
F 2 "" H 4500 2050 50  0000 C CNN
F 3 "" H 4500 2050 50  0000 C CNN
	1    4500 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 59FCE5A5
P 4500 2850
F 0 "#PWR02" H 4500 2600 50  0001 C CNN
F 1 "GND" H 4500 2700 50  0000 C CNN
F 2 "" H 4500 2850 50  0000 C CNN
F 3 "" H 4500 2850 50  0000 C CNN
	1    4500 2850
	1    0    0    -1  
$EndComp
$Comp
L C_Small C2
U 1 1 59FD6427
P 6100 1900
F 0 "C2" H 6110 1970 50  0000 L CNN
F 1 "0.1uF" H 6110 1820 50  0000 L CNN
F 2 "Resistors_SMD:R_0603" H 6100 1900 50  0001 C CNN
F 3 "" H 6100 1900 50  0001 C CNN
	1    6100 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59FD6B4A
P 6100 2100
F 0 "#PWR03" H 6100 1850 50  0001 C CNN
F 1 "GND" H 6100 1950 50  0000 C CNN
F 2 "" H 6100 2100 50  0000 C CNN
F 3 "" H 6100 2100 50  0000 C CNN
	1    6100 2100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR04
U 1 1 59FD6C0D
P 5150 1300
F 0 "#PWR04" H 5150 1150 50  0001 C CNN
F 1 "+3.3V" H 5150 1440 50  0000 C CNN
F 2 "" H 5150 1300 50  0000 C CNN
F 3 "" H 5150 1300 50  0000 C CNN
	1    5150 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1450 6450 1450
Wire Wire Line
	5150 1300 5150 1450
Wire Wire Line
	6100 1450 6100 1800
Connection ~ 6100 1450
Wire Wire Line
	6100 2000 6100 2100
$Comp
L GND #PWR05
U 1 1 59FD6E29
P 6450 2100
F 0 "#PWR05" H 6450 1850 50  0001 C CNN
F 1 "GND" H 6450 1950 50  0000 C CNN
F 2 "" H 6450 2100 50  0000 C CNN
F 3 "" H 6450 2100 50  0000 C CNN
	1    6450 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 1850 6450 2100
Wire Wire Line
	6450 1650 4750 1650
Wire Wire Line
	4750 1650 4750 2200
Wire Wire Line
	4750 2200 4350 2200
$Comp
L JACK_RJ45 J1
U 1 1 59FCD911
P 3900 2450
F 0 "J1" H 4075 2950 39  0000 C CNN
F 1 "JACK_RJ45" H 3725 2950 39  0000 C CNN
F 2 "CA6:RJ45_pcbwing" H 3875 2450 60  0001 C CNN
F 3 "" H 3875 2450 60  0000 C CNN
	1    3900 2450
	0    -1   1    0   
$EndComp
Wire Wire Line
	4500 2050 4500 2100
Wire Wire Line
	4500 2100 4350 2100
Wire Wire Line
	4350 2800 4500 2800
Wire Wire Line
	4500 2800 4500 2850
NoConn ~ 4350 2400
NoConn ~ 4350 2500
NoConn ~ 4350 2600
NoConn ~ 4350 2700
$Comp
L R R1
U 1 1 5A21DC3C
P 5300 1450
F 0 "R1" V 5380 1450 50  0000 C CNN
F 1 "100R" V 5300 1450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5230 1450 50  0001 C CNN
F 3 "" H 5300 1450 50  0001 C CNN
	1    5300 1450
	0    -1   -1   0   
$EndComp
$Comp
L CAP_ELEC C1
U 1 1 5A21DD7D
P 5650 2050
F 0 "C1" H 5700 2150 50  0000 L CNN
F 1 "100uF" H 5700 1950 50  0000 L CNN
F 2 "CA6:c_elec_6.3x5.7" H 5650 2050 60  0001 C CNN
F 3 "" H 5650 2050 60  0000 C CNN
	1    5650 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1850 5650 1450
Connection ~ 5650 1450
$Comp
L GND #PWR06
U 1 1 5A21DE51
P 5650 2250
F 0 "#PWR06" H 5650 2000 50  0001 C CNN
F 1 "GND" H 5650 2100 50  0000 C CNN
F 2 "" H 5650 2250 50  0000 C CNN
F 3 "" H 5650 2250 50  0000 C CNN
	1    5650 2250
	1    0    0    -1  
$EndComp
NoConn ~ 4350 2300
$Comp
L HOLE X1
U 1 1 5A22D009
P 5750 3150
F 0 "X1" H 5750 3050 60  0000 C CNN
F 1 "HOLE" H 5750 3250 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 5750 3150 60  0001 C CNN
F 3 "" H 5750 3150 60  0000 C CNN
	1    5750 3150
	1    0    0    -1  
$EndComp
$Comp
L HOLE X2
U 1 1 5A22D141
P 6050 3150
F 0 "X2" H 6050 3050 60  0000 C CNN
F 1 "HOLE" H 6050 3250 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 6050 3150 60  0001 C CNN
F 3 "" H 6050 3150 60  0000 C CNN
	1    6050 3150
	1    0    0    -1  
$EndComp
$Comp
L HOLE X3
U 1 1 5A22D16C
P 6350 3150
F 0 "X3" H 6350 3050 60  0000 C CNN
F 1 "HOLE" H 6350 3250 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 6350 3150 60  0001 C CNN
F 3 "" H 6350 3150 60  0000 C CNN
	1    6350 3150
	1    0    0    -1  
$EndComp
$Comp
L HOLE X4
U 1 1 5A22D1A0
P 6650 3150
F 0 "X4" H 6650 3050 60  0000 C CNN
F 1 "HOLE" H 6650 3250 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 6650 3150 60  0001 C CNN
F 3 "" H 6650 3150 60  0000 C CNN
	1    6650 3150
	1    0    0    -1  
$EndComp
$EndSCHEMATC
