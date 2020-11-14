#include <Wire.h>
#include "SparkFun_VL53L1X.h"


#define TCAADDR 0x70
#define TOFADDR 0x29

uint8_t roiID = 0;

typedef struct {
  uint8_t x;
  uint8_t y;
} point;


/**Table of Optical Centers**
  *
  * 128,136,144,152,160,168,176,184,  192,200,208,216,224,232,240,248
  * 129,137,145,153,161,169,177,185,  193,201,209,217,225,233,241,249
  * 130,138,146,154,162,170,178,186,  194,202,210,218,226,234,242,250
  * 131,139,147,155,163,171,179,187,  195,203,211,219,227,235,243,251
  * 132,140,148,156,164,172,180,188,  196,204,212,220,228,236,244,252
  * 133,141,149,157,165,173,181,189,  197,205,213,221,229,237,245,253
  * 134,142,150,158,166,174,182,190,  198,206,214,222,230,238,246,254
  * 135,143,151,159,167,175,183,191,  199,207,215,223,231,239,247,255
  
  * 127,119,111,103, 95, 87, 79, 71,  63, 55, 47, 39, 31, 23, 15, 7
  * 126,118,110,102, 94, 86, 78, 70,  62, 54, 46, 38, 30, 22, 14, 6
  * 125,117,109,101, 93, 85, 77, 69,  61, 53, 45, 37, 29, 21, 13, 5
  * 124,116,108,100, 92, 84, 76, 68,  60, 52, 44, 36, 28, 20, 12, 4
  * 123,115,107, 99, 91, 83, 75, 67,  59, 51, 43, 35, 27, 19, 11, 3
  * 122,114,106, 98, 90, 82, 74, 66,  58, 50, 42, 34, 26, 18, 10, 2
  * 121,113,105, 97, 89, 81, 73, 65,  57, 49, 41, 33, 25, 17, 9, 1
  * 120,112,104, 96, 88, 80, 72, 64,  56, 48, 40, 32, 24, 16, 8, 0             ^ Y
  *                                                                  pin1      |
  *                                                                        X <-+
  */

SFEVL53L1X distanceSensor;

void setup(void)
{
  Wire.begin();
  tcaselect(0);

  Serial.begin(115200);

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");

  // Short mode max distance is limited to 1.3 m but has a better ambient immunity.
  // Above 1.3 meter error 4 is thrown (wrap around).
  distanceSensor.setDistanceModeShort();
//  distanceSensor.setDistanceModeLong(); // default

  /*
     * The minimum timing budget is 20 ms for the short distance mode and 33 ms for the medium and long distance modes.
     * Predefined values = 15, 20, 33, 50, 100(default), 200, 500.
     * This function must be called after SetDistanceMode.
     */
  distanceSensor.setTimingBudgetInMs(20);

  // measure periodically. Intermeasurement period must be >/= timing budget.
  distanceSensor.setIntermeasurementPeriod(25);
  distanceSensor.startRanging(); // Start once
}

void loop(void)
{
  distanceSensor.setROI(4,4, roiID);
  delay(1);
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  byte rangeStatus = distanceSensor.getRangeStatus();
  unsigned int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
    Serial.print(roiID);
    point p = getPosition();
    Serial.print("\t");
    Serial.print(p.x);
    Serial.print("\t");
    Serial.print(p.y);
    Serial.print("\t");

  if (rangeStatus == 0) {
    Serial.println(distance);
  } else {
    Serial.println(-1);
  }
  
    nextROI();
}

void nextROI()
{
  if (roiID == 255) {
    roiID = 0;
    return;
  }

  roiID++;
}

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


point getPosition()
{
  uint8_t x = 0;
  uint8_t y = 0;
  
  if (roiID < 128) {
    x = roiID / 8;
    y = roiID - x*8;
  } else {
    x = (255 - roiID) / 8;
    y = (255 - roiID) - x * 8 + 8;
  }

  x = 15 - x;

  return {x,y};
}
