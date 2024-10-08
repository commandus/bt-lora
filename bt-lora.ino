#include <heltec.h>

#include "LoRaWan_APP.h"
#include "Arduino.h"

#include "lora-keys.h"
#include "hex-string.h"

class CollarState {
  public:
    enum eDeviceState_LoraWan state;
    uint16_t txCount;
    CollarState()
      : state(DEVICE_STATE_INIT), txCount(0)
      {

      }
      CollarState &operator=(const CollarState &v) {
        txCount = v.txCount;
        state = v.state;
        return *this;
      }
      bool operator==(const CollarState &v) {
        return (state == v.state) && (txCount == v.txCount);
      }

      bool operator!=(const CollarState &v) {
        return (state != v.state) || (txCount != v.txCount);
      }
};

CollarState state;
CollarState oldState;

void updateState()
{
  state.state = deviceState;
  if (state == oldState)
    return;

  String s;
  if (overTheAirActivation) {
    s = "OTAA "; // + hexString(appKey, 16);
  } else {
    s =  "ABP "; // + hexString(appSKey, 16) + " N: " + hexString(nwkSKey, 16);
  }
  s += String(devAddr, HEX); // + hexString(devEui, 8); // + " A: " + hexString(appEui, 8);
  s += " Tx: " + String(state.txCount, DEC) + " ";
  switch (deviceState) {
    case DEVICE_STATE_INIT:
        s += "Init";
        break;
    case DEVICE_STATE_JOIN:
        s += "Join";
        break;
    case DEVICE_STATE_SEND:
        s +="Send";
        break;
    case DEVICE_STATE_CYCLE:
        s +="Cycle";
        break;
    case DEVICE_STATE_SLEEP:
        s +="Sleep";
        break;
    default:
        break;
  }
  Serial.println(s);  
	
  digitalWrite(LED, (state.txCount %2 ? LOW: HIGH));

  oldState = state;
}

void setup() {
  Serial.begin(115200);
  Mcu.setlicense(license);
  Mcu.begin();
  deviceState = DEVICE_STATE_INIT;
  pinMode(LED, OUTPUT);

  // The chip ID is essentially its MAC address(length: 6 bytes).
  uint64_t chipid = ESP.getEfuseMac();
  Serial.println("DeviceId=" + String(chipid, HEX));
}

// Prepares the payload of the frame 
static void prepareTxFrame(
  uint8_t port
)
{
  appDataSize = 4;
  appData[0] = 0xAA;
  appData[1] = 0x11;
  auto puc = (unsigned char *)(&state.txCount);
  appData[2] = puc[0];
  appData[3] = puc[1];
}

void loop() {
  switch (deviceState) {
    case DEVICE_STATE_INIT:
      updateState();
#if(LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif
      LoRaWAN.init(loraWanClass, loraWanRegion);
      updateState();
      break;
    case DEVICE_STATE_JOIN:
      updateState();
      LoRaWAN.join();
      break;
    case DEVICE_STATE_SEND:
      prepareTxFrame(appPort);
      updateState();
      LoRaWAN.send();
      state.txCount++;
      deviceState = DEVICE_STATE_CYCLE;
      updateState();
      break;
    case DEVICE_STATE_CYCLE:
      updateState();
      txDutyCycleTime = appTxDutyCycle;// appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      updateState();
      break;
    case DEVICE_STATE_SLEEP:
      updateState();
      LoRaWAN.sleep(loraWanClass);
      break;
    default:
      deviceState = DEVICE_STATE_INIT;
      updateState();
      break;
  }
}
