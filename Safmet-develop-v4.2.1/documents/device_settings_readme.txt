For any features modification only have to edit in "dev_id.h" file

1.for changing safmet UIDS make changes in below line:
byte dev_ID[4] = {0x00,0x00,0x11,0x58};   ///example if UID is 1158

2.For controlling vibration motor make changes in below line:
bool vib_motor = 1;//to ON vibration motor make it 1 and OFF vibration motor make it 0

3.For controlling buzzer make changes in below line:
bool buzz = 0;//to ON buzzer make it 1 and OFF buzzer make it 0
