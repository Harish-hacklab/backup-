/** @file HSA.h
  * @brief This file declares all MACROS, variables and structs */ 



/** @brief GPIO to control Vibration Motor state */
#define VIB_MOTOR 7
/** @brief MACRO defining GPIO connected to ATTINY */
#define ATTINY_GPIO 5 
/** @brief MACRO for DW1000 Load Switch */
#define DW_LS 3
/** @brief ADC Pin used to check Input Power  */
#define PWR_ADC A7       
/** @brief GPIO to check state of TOUCH_SENSOR */
#define TOUCH_SENSOR 8 //TX
/** @brief  using as power supply for touch sensor */
#define T_PWR A3
/** @brief GPIO to control Buzzer state */
#define BUZZER 6

/** @brief GPIO control state of GREEN_LED */
#define GREEN_LED 4
/** @brief GPIO control state of BLUE_LED */
#define BLUE_LED A2 
/** @brief Not used anywhere */
#define CHR_DET 7

/** @brief ADC pin to check battery voltage */
#define VBAT_ADC A6 
#define R1 1.1 //Kohms
#define R2 2.0 //Kohms
#define VD_R1 12.0
#define VD_R2 3.9
#define BAT_LOW_ALERT 3.45// 0.852 for 2.92V, 0.890 for 3.05V,0.845 for 3.45V
#define V_SYS_SLEEP 3.10 //Voltage below which the complete system goes to sleep
byte sleep_counter_led =0;


#define SetupPrint

#define ENABLE true
#define DISABLE false
#define LEN_DATA 100

#define BEACON 0x05

#define max_process_size 3
#define broadcast_address 0xFFFFFFFF

#define ACK 0x01
#define POLL 0x02
#define POLLACK 0x03
#define RANGE 0x04
#define RANGEREPORT 0x05
//#define CONF 0x06
#define CAUTION 0x07
#define BROADCAST 0xFF
#define ERROR_REPORT 0xAF
#define PERCONFIG 0x21
#define TEMPCONFIG 0x29
#define PAYLOADSIZE 20
#define GREATER 1
#define LESSER 0

//Battery Voltage Indicator LEDs
#define LED_IND1 14 
#define LED_IND2 15
#define LED_IND3 3

#define LED1_TH_V 3.45
#define LED2_TH_V 3.75
#define LED3_TH_V 4.05


//Debug code for firmware 4.1.2
#define DEBUG_FW412

DW1000Time rx_time;

uint32_t printtime=0;   ///< Stores time when #check_vbat stores and prints battery voltage
float bat_voltage = 0;  ///< Stores voltage of battery
float volt = 0;   ///< Stores analog reading of #VBAT_ADC * bandgapReference(1.1V)/totalNumberOfSteps(1023)

//32 bit declaration
uint32_t lastBeaconTime = 300; // should not be '0' if so the device will not transmit and keep sleeping

//uint32_t last_indicate = 0;
//uint32_t listen_time = 25;//210 before - listens for ACKS from all CASnodes after transmitting BROADCAST
uint32_t source_uids_array[max_process_size];
uint32_t lastActivity = 0;  ///< `lastActivity` is not checked for anywhere
uint32_t source_UID ;
uint32_t destination_UID;
uint32_t current_active_src_uid;
uint32_t green_led_timer = 0;
uint32_t last_alive = 0;
uint32_t RangingTime = 0;
uint32_t TotalRangingTime = 0;

// 16 bit declarations
float VoltHist[10];
uint32_t green_init=0;
uint32_t VoltHistoryTime = 0;
uint32_t listenModeActiveTime = 0;
uint16_t sleep_timeout = 250; // should be leass than beacon_timeout
uint16_t alarm_timeout = 300; // for access control use 500 not more than that(after this time device goes to sleep to avoid conti communication) //for forklift use 2000
const uint16_t replyDelayTimeUS = 3000;
uint16_t network; ///< Store the Net_ID from received broadcast
uint16_t sub_network ;
uint16_t AckStore_Time = 0; // To check the time between receiving an ACK and completely processing it (storing the ack in source_uids_array[])
uint16_t listen_time = 25; //listens for ACKS from all CASnodes after transmitting BROADCAST
uint16_t BoradcastTimeout_Time = 30; //Goes to Broadcast Timeout Sleep
uint8_t sleep_test = 0;


// 8 bit declaration
byte sid_index = 0;
byte response_count = 0;
uint8_t payload[PAYLOADSIZE]; ///< Stores timestamps of transmission of DW1000
uint8_t packt_id;
static uint8_t  counter = 0;


//byte declaration
byte device_ID[4];//csk
byte data[LEN_DATA];
byte sent_packet_type;
byte packt_type;
byte temp_near;
byte temp_emerg;
byte dist_n_f;
byte dist_e_f;
byte dist_n_f_f;
byte dist_e_f_f;  
byte message[8];
byte BatteryVolts_IND;
//byte LoopDetector_Counter=0;

// float declarations

float distance;

//bool declarations

//bool RangeProcess_Lock = false; //Flag so that ranging process of individual CASnodes are not affected by other CASnodes - if this varaible is used the safmet waits and completes the twr with each casnode before moving to the next one
bool sleep_active = false;
bool beacon;
bool RangeProcess_Flag = false; //To track the TWR Ranging Process
bool CheckFlag = true; // To indicate whether the safemet can execute the check battery and check power functions
bool sleep_act = false;
bool SleepFlag = false; //for Atmega and DW1000 sleep during the working state (TWR)
//boolean InputFlag = false; //SerialInput Check Flag
volatile bool sentAck = false;
volatile bool receivedAck = false;
bool D_ZONE_DETECT = false;
bool enter = false;
bool all_ok = false;


//char[] declaration
//String SerialInput = ""; //Serial Input Buffer

//const declaration

const uint8_t PIN_RST = 9; ///< reset pin
const uint8_t PIN_IRQ = 2; ///< irq pin
const uint8_t PIN_SS = SS; ///< spi select pin

//function declarations
void transmit(uint8_t packt_type, uint8_t pack_id, uint32_t dest_id, uint8_t payload[], uint8_t payload_size);
void gpio_defaults();

void handleSent();
void handleReceived();

void fill_default_transmit();
void receiver();
void handleError();
void handleCaution();
void reInitDeca();
void decawaveInit();
void SerialHandle();
bool check_for_ignore_uid();
void green_setup(){
  green_init = millis;
}
/** @brief This struct stores device ID info */
struct dev_id
{
  uint16_t Net_ID;      ///< 65282 is the net id of 2 bytes used to check whether received packet belongs to the same network or not
  uint16_t Sub_Net_ID;  ///< This 2 bytes informs whether the transmitting device is a helmet or forklift.
  uint32_t Unique_ID;   ///< Each device has 4 byte software generated unique id to verify the responder(destination) has communicated to the respective sender in the complete transmission.
} device;

/** @brief This struct stores list of subnetwork IDs */ 
struct dist
{
  uint32_t dist_SID[5];

} dist_para;

/** @brief This struct stores various parameters related to DW1000 communication */
struct parameter
{
  //byte start = type_en_func;
  uint16_t en_func;//OUT1//OUT2//OUT3//RX-TX//can//i2c//led1//led2//led3//button1//button2//button3
  uint8_t tx_pwr;
  uint8_t rx_pwr;
  uint8_t op_mode;
  uint8_t sleep_time;
  uint8_t wait_time;
  uint16_t beacon_timeout;  ///< The time in milliseconds after which it broadcasts again upon receiving no response
} param;

struct Ignore
{
  uint32_t Ignore_UID[4];
} I_UD;


byte received_packet_type;

DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;       ///< Store the delay after which a DW1000 Range transmission is performed
DW1000Time timeRangeReceived;
// last computed range/time
DW1000Time timeComputedRange;

void reInitDeca()
{
  
  DW1000.reset();
  DW1000.spiWakeup();
  delay(2);
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);
  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(1);
  DW1000.setNetworkId(10);
  DW1000.enableMode(DW1000.MODE_SHORTDATA_FAST_ACCURACY); //MODE_SHORTDATA_FAST_ACCURACY);
  DW1000.commitConfiguration();
  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);
  receiver();

  digitalWrite(ATTINY_GPIO, HIGH);
  delay(1);
  digitalWrite(ATTINY_GPIO, LOW);
  fill_default_transmit();
}

void sendBeaconPacket() {
  if (RangeProcess_Flag == true) //if previously not cleared after rangereport, then clear the flag
  {
    RangeProcess_Flag = false;
  }
  //RangeProcess_Lock = false;

  Serial.println(F("Beacon Timeout Reached"));
  
  gpio_defaults();
  response_count = 0; //delete all the previous acks - source_uids_array will overwrite in the next cycle

#if TOUCH_ENABLE
  if (!checkTouchEnable()) return;
#endif
  packt_id++;
  Serial.println(F("BEACON"));

  //delay(random(0, 50));

  transmit(BROADCAST, packt_id, broadcast_address, payload, 0);
  Serial.println(F("BROADCAST sent"));
  lastBeaconTime = millis();
  listenModeActiveTime = millis();
}
