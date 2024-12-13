#include "gps.h"

void InitGPS() {
  for (unsigned int i = 0; i < sizeof(UBLOX_INIT); i++) {
    Serial.write(pgm_read_byte(UBLOX_INIT + i));
    delay(5);  // simulating a 38400baud pace (or less), otherwise commands are
               // not accepted by the device.
  }
}

bool ProcessGPS(NAV_PVT* pvt) {
  static int fpos = 0;
  static unsigned char checksum[2];
  const int payloadSize = sizeof(NAV_PVT);

  while (Serial.available()) {
    byte c = Serial.read();
    if (fpos < 2) {
      if (c == UBX_HEADER[fpos])
        fpos++;
      else
        fpos = 0;
    } else {
      if ((fpos - 2) < payloadSize) ((unsigned char*)(pvt))[fpos - 2] = c;

      fpos++;

      if (fpos == (payloadSize + 2)) {
        CalcChecksum(checksum, *pvt);
      } else if (fpos == (payloadSize + 3)) {
        if (c != checksum[0]) fpos = 0;
      } else if (fpos == (payloadSize + 4)) {
        fpos = 0;
        if (c == checksum[1]) {
          return true;
        }
      } else if (fpos > (payloadSize + 4)) {
        fpos = 0;
      }
    }
  }
  return false;
}

void CalcChecksum(unsigned char* CK, NAV_PVT pvt) {
  memset(CK, 0, 2);
  for (int i = 0; i < (int)sizeof(NAV_PVT); i++) {
    CK[0] += ((unsigned char*)(&pvt))[i];
    CK[1] += CK[0];
  }
}