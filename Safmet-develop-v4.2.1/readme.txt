For any features modification only have to edit in "dev_id.h" file

1.for changing safmet UIDS make changes in below line:
byte dev_ID[4] = {0x00,0x00,0x11,0x58};   ///example if UID is 1158

2.For controlling vibration motor make changes in below line:
bool vib_motor = 1;//to ON vibration motor make it 1 and OFF vibration motor make it 0

3.For controlling buzzer make changes in below line:
bool buzz = 0;//to ON buzzer make it 1 and OFF buzzer make it 0


The following changes were made to the Safmet_V3 firmware : Touch 3pin connector added(1-GND,2-TX(touch data pin,I/O),3-T_PWR(touch supply pin,VCC)) At USB Connector TX and T_PWR pin using for touch data and supply pin respectively. At atmega328p ic pin PC3(ADC3)using for T_PWR. Tilt sensor removed from schematic because for low battery led we required one gpio pin. Q1 FDN360P MOSFET added across TPS (due to unavaibility of TPS we can use this mosfet.

The following changes were made to the Safmet_P3.0.2 hardware: Q2 mosfet placed between battery and circuit. Resistors value replaced -r13(4.7k), r20(1.5k), and r37(1.5k). led added(LED,PC1/ADC1) for low battery status. Touch 3pin connector added(1-GND,2-TX(touch data pin,I/O),3-T_PWR(touch supply pin,VCC)) At USB Connector TX and T_PWR pin using for touch data and supply pin respectively. At atmega328p ic pin PC3(ADC3)using for T_PWR. Tilt sensor removed from schematic because for low battery led we required one gpio pin. Q1 FDN360P MOSFET added across TPS (due to unavaibility of TPS we can use this mosfet.

Please reffer file inside document folders for more detailed information
