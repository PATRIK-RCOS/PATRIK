<QucsStudio Schematic 5.8>
<Properties>
View=-67.3717,-8.76395,1105.72,759.39,1.14525,106,56
Grid=10,10,1
DataSet=*.dat
DataDisplay=*.dpl
OpenDisplay=1
showFrame=0
FrameText0=Title \n @PATH@@FILE@
FrameText1=Drawn By:
FrameText2=Date: @DATE@
FrameText3=Revision:
</Properties>
<Symbol>
.PortSym 40 20 3 0
.PortSym 40 60 1 0
.PortSym 40 100 2 0
.PortSym 40 140 4 0
</Symbol>
<Components>
Port RFin 1 160 300 -46 -48 3 "3"0"in"0"smd_point"0
Port SMAconnector 1 840 290 15 -27 3 "2"0"out"0"smd_point"0
Port RFout 1 820 270 40 -23 2 "4"0"analog"0"smd_point"0
GND * 1 840 560 0 0 0
Eqn L1plusL2_is_6plus6_winds_around_2_ferrite_beads_with_center_tap 1 130 640 0 8 0 "y=="0"yes"0
_BJT T1 1 330 400 8 -26 0 "npn"0"1e-16"0"1"0"1"0"0"0"0"0"0"1"0"0"0"0"1.5"0"0"0"2"0"100"1"1"0"0"0"0"0"0"0"0"0"0"0"0"0"0.75"0"0.33"0"0"0"0.75"0"0.33"0"1.0"0"0"0"0.75"0"0"0"0.5"0"0.0"0"0.0"0"0.0"0"0.0"0"0.0"0"26.85"0"0.0"0"1.0"0"1.0"0"0.0"0"1.0"0"1.0"0"0.0"0"0.0"0"3.0"0"1.11"0"26.85"0"1.0"0"7.02e-4"0"1108.0"0"SOT23"0
R R1 1 330 480 9 -20 1 "50 Ω"1"26.85"0"US"0"SMD0603"0
R R2 1 240 510 -26 8 0 "220 Ω"1"26.85"0"US"0"SMD0603"0
Port connector 1 180 280 -101 -11 0 "1"0"analog"0"con_1"0
L L2 1 190 490 -62 -26 3 "3.6uH"1"0"0""0"inductor_1mH"0
GND * 1 120 560 0 0 0
C C1 1 240 440 -33 7 0 "10nF"1"0"0""0"neutral"0"SMD0603"0
L L1 1 190 400 -66 -20 1 "3.6uH"1"0"0""0"inductor_1mH"0
C C2 1 270 290 -60 -12 1 "10nF"1"0"0""0"neutral"0"SMD0603"0
R R3 1 270 350 -68 -15 1 "330Ω"1"26.85"0"US"0"SMD0603"0
</Components>
<Wires>
840 290 840 560
190 560 210 560
190 430 190 440
330 560 840 560
330 430 330 450
330 510 330 560
270 400 300 400
270 400 270 440
210 560 330 560
210 510 210 560
270 380 270 400
270 250 270 260
270 250 330 250
330 250 330 370
190 520 190 560
120 560 190 560
120 300 160 300
120 300 120 560
190 280 190 370
180 280 190 280
190 440 190 460
190 440 210 440
270 440 270 510
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
