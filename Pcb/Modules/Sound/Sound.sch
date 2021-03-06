EESchema Schematic File Version 4
LIBS:Sound-cache
EELAYER 26 0
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
L Sound-rescue:OPA376AIDCK U1
U 1 1 59BF073B
P 3650 1850
F 0 "U1" H 3700 2050 50  0000 C CNN
F 1 "MAX4466" H 3850 1650 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-353_SC-70-5" H 3600 1550 50  0001 L CNN
F 3 "" H 3700 2050 50  0000 C CNN
	1    3650 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:Microphone_Condenser MK1
U 1 1 59BF08F8
P 1350 3600
F 0 "MK1" H 1220 3650 50  0000 R CNN
F 1 "Mic" H 1220 3575 50  0000 R CNN
F 2 "CA6:MIC_CMA-4544PF" V 1350 3700 50  0001 C CNN
F 3 "" V 1350 3700 50  0000 C CNN
	1    1350 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 59BF092D
P 1750 2400
F 0 "R1" V 1830 2400 50  0000 C CNN
F 1 "1K" V 1750 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1680 2400 50  0001 C CNN
F 3 "" H 1750 2400 50  0000 C CNN
	1    1750 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 59BF0968
P 2050 2800
F 0 "C1" H 2075 2900 50  0000 L CNN
F 1 "47uF" H 2075 2700 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 2088 2650 50  0001 C CNN
F 3 "" H 2050 2800 50  0000 C CNN
	1    2050 2800
	1    0    0    -1  
$EndComp
$Comp
L CA6:JACK_RJ45 J1
U 1 1 59BF1311
P 2850 5000
F 0 "J1" H 3025 5500 39  0000 C CNN
F 1 "JACK_RJ45" H 2675 5500 39  0000 C CNN
F 2 "CA6:RJ45_pcbwing" H 2825 5000 60  0001 C CNN
F 3 "" H 2825 5000 60  0000 C CNN
	1    2850 5000
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 59BFD37D
P 1750 3150
F 0 "R2" V 1830 3150 50  0000 C CNN
F 1 "2.2K" V 1750 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1680 3150 50  0001 C CNN
F 3 "" H 1750 3150 50  0000 C CNN
	1    1750 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 59BFD42E
P 2050 3050
F 0 "#PWR01" H 2050 2800 50  0001 C CNN
F 1 "GND" H 2050 2900 50  0000 C CNN
F 2 "" H 2050 3050 50  0000 C CNN
F 3 "" H 2050 3050 50  0000 C CNN
	1    2050 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR02
U 1 1 59BFD6E2
P 1750 2150
F 0 "#PWR02" H 1750 2000 50  0001 C CNN
F 1 "+3.3V" H 1750 2290 50  0000 C CNN
F 2 "" H 1750 2150 50  0000 C CNN
F 3 "" H 1750 2150 50  0000 C CNN
	1    1750 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 59BFDB2D
P 1350 3850
F 0 "#PWR03" H 1350 3600 50  0001 C CNN
F 1 "GND" H 1350 3700 50  0000 C CNN
F 2 "" H 1350 3850 50  0000 C CNN
F 3 "" H 1350 3850 50  0000 C CNN
	1    1350 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 59BFE43A
P 2700 1550
F 0 "R3" V 2780 1550 50  0000 C CNN
F 1 "100K" V 2700 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2630 1550 50  0001 C CNN
F 3 "" H 2700 1550 50  0000 C CNN
	1    2700 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 59BFE510
P 2700 1950
F 0 "R4" V 2780 1950 50  0000 C CNN
F 1 "100K" V 2700 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2630 1950 50  0001 C CNN
F 3 "" H 2700 1950 50  0000 C CNN
	1    2700 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR04
U 1 1 59BFED19
P 2700 1300
F 0 "#PWR04" H 2700 1150 50  0001 C CNN
F 1 "+3.3V" H 2700 1440 50  0000 C CNN
F 2 "" H 2700 1300 50  0000 C CNN
F 3 "" H 2700 1300 50  0000 C CNN
	1    2700 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 59BFED42
P 2700 2200
F 0 "#PWR05" H 2700 1950 50  0001 C CNN
F 1 "GND" H 2700 2050 50  0000 C CNN
F 2 "" H 2700 2200 50  0000 C CNN
F 3 "" H 2700 2200 50  0000 C CNN
	1    2700 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 59BFEF22
P 3050 3150
F 0 "C3" H 3075 3250 50  0000 L CNN
F 1 "0.1uF" H 3075 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3088 3000 50  0001 C CNN
F 3 "" H 3050 3150 50  0000 C CNN
	1    3050 3150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 59BFF023
P 3550 2250
F 0 "#PWR06" H 3550 2000 50  0001 C CNN
F 1 "GND" H 3550 2100 50  0000 C CNN
F 2 "" H 3550 2250 50  0000 C CNN
F 3 "" H 3550 2250 50  0000 C CNN
	1    3550 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 59BFF149
P 3550 3000
F 0 "C4" H 3575 3100 50  0000 L CNN
F 1 "47pF" H 3575 2900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3588 2850 50  0001 C CNN
F 3 "" H 3550 3000 50  0000 C CNN
	1    3550 3000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R5
U 1 1 59BFF2B5
P 3050 2750
F 0 "R5" V 3130 2750 50  0000 C CNN
F 1 "20K" V 3050 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2980 2750 50  0001 C CNN
F 3 "" H 3050 2750 50  0000 C CNN
	1    3050 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 59BFF4E5
P 3550 2550
F 0 "R6" V 3630 2550 50  0000 C CNN
F 1 "1M" V 3550 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3480 2550 50  0001 C CNN
F 3 "" H 3550 2550 50  0000 C CNN
	1    3550 2550
	0    1    1    0   
$EndComp
Text Label 4550 1850 0    60   ~ 0
AN_VAL
$Comp
L power:GND #PWR07
U 1 1 59C00872
P 1850 4600
F 0 "#PWR07" H 1850 4350 50  0001 C CNN
F 1 "GND" H 1850 4450 50  0000 C CNN
F 2 "" H 1850 4600 50  0000 C CNN
F 3 "" H 1850 4600 50  0000 C CNN
	1    1850 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 59C009A9
P 3850 4400
F 0 "#PWR08" H 3850 4250 50  0001 C CNN
F 1 "+3.3V" H 3850 4540 50  0000 C CNN
F 2 "" H 3850 4400 50  0000 C CNN
F 3 "" H 3850 4400 50  0000 C CNN
	1    3850 4400
	0    1    1    0   
$EndComp
Text Label 3100 4250 1    60   ~ 0
AN_VAL
Text Notes 3650 3400 0    60   ~ 0
Gain = (R6/R5)+1 = 51
$Comp
L power:+3.3V #PWR09
U 1 1 59C0230A
P 3550 1450
F 0 "#PWR09" H 3550 1300 50  0001 C CNN
F 1 "+3.3V" H 3550 1590 50  0000 C CNN
F 2 "" H 3550 1450 50  0000 C CNN
F 3 "" H 3550 1450 50  0000 C CNN
	1    3550 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 59BFDB54
P 2500 1950
F 0 "C2" H 2525 2050 50  0000 L CNN
F 1 "1uF" H 2525 1850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2538 1800 50  0001 C CNN
F 3 "" H 2500 1950 50  0000 C CNN
	1    2500 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	1350 3850 1350 3800
Wire Wire Line
	1350 3400 1350 3350
Wire Wire Line
	1750 3300 1750 3350
Connection ~ 1750 3350
Wire Wire Line
	2050 3050 2050 2950
Wire Wire Line
	1750 2150 1750 2250
Wire Wire Line
	1750 2550 1750 2600
Wire Wire Line
	1750 2600 2050 2600
Wire Wire Line
	2050 2600 2050 2650
Connection ~ 1750 2600
Wire Wire Line
	2700 1700 2700 1750
Connection ~ 2700 1750
Wire Wire Line
	2700 2100 2700 2150
Wire Wire Line
	2700 1400 2700 1300
Wire Wire Line
	3550 2150 3550 2250
Wire Wire Line
	3050 2550 3250 2550
Connection ~ 3050 2550
Wire Wire Line
	3950 1850 4200 1850
Wire Wire Line
	4200 2550 4200 1850
Wire Wire Line
	3700 2550 3900 2550
Wire Wire Line
	3250 2550 3250 3000
Wire Wire Line
	3250 3000 3400 3000
Connection ~ 3250 2550
Wire Wire Line
	3700 3000 3900 3000
Wire Wire Line
	3900 3000 3900 2550
Connection ~ 3900 2550
Connection ~ 4200 1850
Wire Wire Line
	3200 4400 3650 4400
Wire Wire Line
	3200 4400 3200 4550
Wire Wire Line
	2500 4400 2500 4550
Wire Wire Line
	1850 4400 2500 4400
Wire Wire Line
	1850 4400 1850 4600
Wire Wire Line
	3100 4550 3100 4250
Wire Wire Line
	3050 2900 3050 3000
Wire Wire Line
	3350 1950 3050 1950
Wire Wire Line
	3050 1950 3050 2550
Wire Wire Line
	3550 1550 3550 1450
Wire Wire Line
	2500 1750 2700 1750
Wire Wire Line
	2500 1800 2500 1750
Wire Wire Line
	2500 2100 2500 2150
Wire Wire Line
	2500 2150 2700 2150
Connection ~ 2700 2150
Wire Wire Line
	1350 3350 1750 3350
Wire Wire Line
	3050 3350 3050 3300
NoConn ~ 3000 4550
NoConn ~ 2900 4550
NoConn ~ 2800 4550
NoConn ~ 2700 4550
NoConn ~ 2600 4550
NoConn ~ 6050 2750
$Comp
L CA6:HOLE X1
U 1 1 5A23D852
P 5050 4050
F 0 "X1" H 5050 3950 60  0000 C CNN
F 1 "HOLE" H 5050 4150 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 5050 4050 60  0001 C CNN
F 3 "" H 5050 4050 60  0000 C CNN
	1    5050 4050
	1    0    0    -1  
$EndComp
$Comp
L CA6:HOLE X2
U 1 1 5A23DBBB
P 5350 4050
F 0 "X2" H 5350 3950 60  0000 C CNN
F 1 "HOLE" H 5350 4150 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 5350 4050 60  0001 C CNN
F 3 "" H 5350 4050 60  0000 C CNN
	1    5350 4050
	1    0    0    -1  
$EndComp
$Comp
L CA6:HOLE X3
U 1 1 5A23DC00
P 5650 4050
F 0 "X3" H 5650 3950 60  0000 C CNN
F 1 "HOLE" H 5650 4150 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 5650 4050 60  0001 C CNN
F 3 "" H 5650 4050 60  0000 C CNN
	1    5650 4050
	1    0    0    -1  
$EndComp
$Comp
L CA6:HOLE X4
U 1 1 5A23DC48
P 5950 4050
F 0 "X4" H 5950 3950 60  0000 C CNN
F 1 "HOLE" H 5950 4150 60  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm_Pad" H 5950 4050 60  0001 C CNN
F 3 "" H 5950 4050 60  0000 C CNN
	1    5950 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5A241B9D
P 3650 4650
F 0 "C5" H 3675 4750 50  0000 L CNN
F 1 "47uF" H 3675 4550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3688 4500 50  0001 C CNN
F 3 "" H 3650 4650 50  0000 C CNN
	1    3650 4650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5A241E23
P 3650 4850
F 0 "#PWR010" H 3650 4600 50  0001 C CNN
F 1 "GND" H 3650 4700 50  0000 C CNN
F 2 "" H 3650 4850 50  0000 C CNN
F 3 "" H 3650 4850 50  0000 C CNN
	1    3650 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4850 3650 4800
Wire Wire Line
	3650 4500 3650 4400
Connection ~ 3650 4400
Wire Wire Line
	1750 3350 3050 3350
Wire Wire Line
	1750 2600 1750 3000
Wire Wire Line
	2700 1750 2700 1800
Wire Wire Line
	2700 1750 3350 1750
Wire Wire Line
	3050 2550 3050 2600
Wire Wire Line
	3250 2550 3400 2550
Wire Wire Line
	3900 2550 4200 2550
Wire Wire Line
	4200 1850 4550 1850
Wire Wire Line
	2700 2150 2700 2200
Wire Wire Line
	3650 4400 3850 4400
$EndSCHEMATC
