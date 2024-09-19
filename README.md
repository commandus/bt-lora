
Install Arduino
In Linux:
pip3 install pyserial

Add to Preferences, Additiopnal Board Manager URL
Input the last ESP32 package URL: https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/1.0.0/package_heltec_esp32_index.json

Tools - Board - Heltec ESP32 Series Dev boards - WiFi Lora 32 (v2) or WiFi Lora 32 (v3) ?

// First device
uint32_t license[4] = { 0xAD8973C6, 0xEE5887FB, 0x8464951F, 0x180A219F };


Attempting to boot anyway...
entry 0x403c98d8

LoRaWAN EU868 Class A start!

+OTAA=0

+Class=A

+ADR=1

+IsTxConfirmed=0

+AppPort=2

+DutyCycle=1000

+ConfirmedNbTrials=4

+ChMask=0000000000000000000000FF

+DevEui=2232330000000223(For OTAA Mode)

+AppEui=0000000000000000(For OTAA Mode)

+AppKey=88888888888888888888888888888888(For OTAA Mode)

+NwkSKey=15B1D0EFA463DFBE3D11181E1EC7DA85(For ABP Mode)

+AppSKey=D72C78758CDCCABF55EE4A778D16EF67(For ABP Mode)

+DevAddr=007E6AE2(For ABP Mode)




ABP 7e6ae2 Tx: 0 Join
ABP 7e6ae2 Tx: 0 Send
unconfirmed uplink sending ...

ABP 7e6ae2 Tx: 1 Cycle
ABP 7e6ae2 Tx: 1 Sleep
ABP 7e6ae2 Tx: 1 Send