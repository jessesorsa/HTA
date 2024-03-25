// => Hardware select
#define LILYGO_WATCH_2020_V2             //To use T-Watch2020 V2, please uncomment this line
//#define LILYGO_WATCH_2020_V3           //To use T-Watch2020 V3, please uncomment this line


#include <LilyGoWatch.h>
#include <Timezone.h>
#include "BluetoothSerial.h"


#include "FS.h"
#include <LITTLEFS.h>


#define FORMAT_LITTLEFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS &fs, const char * path, const char * message);
void deleteFile2(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);


// save to LITTLEFS
void saveIdToFile(uint16_t id);
void saveStepsToFile(uint32_t step_count);
void saveDistanceToFile(float distance);
void saveStartTimeToFile(RTC_Date t);
void saveDurationToFile(char* formatted_duration);


void deleteSession();



// functions to init session varaibles from files
// note distance is not neaded as its calculated from steps
bool readLineAsSteps(fs::FS &fs, const char * path, uint32_t& steps);
bool readLineAsDuration(fs::FS &fs, const char * path, int& duration);
bool loadLastSession(fs::FS &fs, uint32_t& steps,  int& duration);


// GPS
bool Quectel_L76X_Probe(HardwareSerial* GNSS);
bool syncRTC(HardwareSerial *GNSS, TinyGPSPlus *gps, PCF8563_Class *rtc);


// BT
void sendDataBT(fs::FS &fs, const char * path, BluetoothSerial& sbt);
void sendSessionBT(BluetoothSerial& sbt);

// distance
