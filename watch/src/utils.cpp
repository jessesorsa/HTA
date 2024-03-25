#include "utils.h"

/****************** FS *******************/

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

void writeFile2(fs::FS &fs, const char * path, const char * message){
    if(!fs.exists(path)){
		if (strchr(path, '/')) {
            Serial.printf("Create missing folders of: %s\r\n", path);
			char *pathStr = strdup(path);
			if (pathStr) {
				char *ptr = strchr(pathStr, '/');
				while (ptr) {
					*ptr = 0;
					fs.mkdir(pathStr);
					*ptr = '/';
					ptr = strchr(ptr+1, '/');
				}
			}
			free(pathStr);
		}
    }

    Serial.printf("Writing file to: %s\r\n", path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void deleteFile2(fs::FS &fs, const char * path){
    Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }

    char *pathStr = strdup(path);
    if (pathStr) {
        char *ptr = strrchr(pathStr, '/');
        if (ptr) {
            Serial.printf("Removing all empty folders on path: %s\r\n", path);
        }
        while (ptr) {
            *ptr = 0;
            fs.rmdir(pathStr);
            ptr = strrchr(pathStr, '/');
        }
        free(pathStr);
    }
}

void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}


void saveIdToFile(uint16_t id)
{
    char buffer[10];
    itoa(id, buffer, 10);
    writeFile(LITTLEFS, "/id.txt", buffer);
}

void saveStepsToFile(uint32_t step_count)
{
    char buffer[10];
    itoa(step_count, buffer, 10);
    writeFile(LITTLEFS, "/steps.txt", buffer);
}

void saveDistanceToFile(float distance)
{
    std::string dist = std::to_string(distance);
    size_t len = dist.length();
    char buffer[len+10];
    strcpy(buffer,dist.c_str());
    writeFile(LITTLEFS, "/distance.txt", buffer);
}


void saveStartTimeToFile(RTC_Date t){

    char buffer[20]; // format length 10+1+8+1(/n) = 20
    snprintf(buffer, sizeof(buffer), "%02d-%02d-%02d/%02d:%02d:%02d", t.year, t.month, t.day, t.hour, t.minute, t.second);
    writeFile(LITTLEFS, "/start_time.txt", buffer);
}

void saveDurationToFile(char* formatted_duration){
    // format "HH:MM:SS"
    writeFile(LITTLEFS, "/duration.txt", formatted_duration);

}


void deleteSession()
{
    deleteFile(LITTLEFS, "/id.txt");
    deleteFile(LITTLEFS, "/distance.txt");
    deleteFile(LITTLEFS, "/steps.txt");
    deleteFile(LITTLEFS, "/start_time.txt");
    deleteFile(LITTLEFS, "/duration.txt");

}



bool readLineAsSteps(fs::FS &fs, const char * path, uint32_t& steps){
    
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return false;
    }
    Serial.println("- read from file:");
    if(file.available()) {
        String line = file.readStringUntil('\n'); // Read until newline character
        file.close();
        steps = line.toInt(); // Convert read line to integer
        return true;
    } else {
        Serial.println("- file is empty");
        file.close();
        return false;
    }
}


bool parseTimeToSeconds(const String& timeStr, int& duration) {
    int hours, minutes, seconds;
    if (sscanf(timeStr.c_str(), "%2d:%2d:%2d", &hours, &minutes, &seconds) == 3) {
        duration = hours * 3600 + minutes * 60 + seconds;
        return true;
    } else {
        return false;
    }
}

bool readLineAsDuration(fs::FS &fs, const char * path, int& duration){

    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return false; // Return -1 to indicate failure
    }

    Serial.println("- read from file:");
    if(file.available()) {
        String line = file.readStringUntil('\n'); // Read until newline character
        file.close();
        return parseTimeToSeconds(line, duration); 
    } else {
        Serial.println("- file is empty");
        file.close();
        return false; // Return -1 if file is empty
    }
}


bool loadLastSession(fs::FS &fs, uint32_t& steps,  int& duration){    
    return (readLineAsSteps(fs, "/steps.txt", steps) && readLineAsDuration(fs, "/duration.txt", duration));
}


bool Quectel_L76X_Probe(HardwareSerial* GNSS)
{
    // FROM TTGO watch examlpe
    bool result = false;
    GNSS->write("$PCAS03,0,0,0,0,0,0,0,0,0,0,,,0,0*02\r\n");
    delay(5);
    // Get version information
    GNSS->write("$PCAS06,0*1B\r\n");
    uint32_t startTimeout = millis() + 500;
    while (millis() < startTimeout) {
        if (GNSS->available()) {
            String ver = GNSS->readStringUntil('\r');
            // Get module info , If the correct header is returned,
            // it can be determined that it is the MTK chip
            int index = ver.indexOf("$");
            if (index != -1) {
                ver = ver.substring(index);
                if (ver.startsWith("$GPTXT,01,01,02")) {
                    Serial.println("L76K GNSS init succeeded, using L76K GNSS Module");
                    result = true;
                }
            }
        }
    }
    // Initialize the L76K Chip, use GPS + GLONASS
    GNSS->write("$PCAS04,5*1C\r\n");
    delay(250);
    // only ask for RMC and GGA
    GNSS->write("$PCAS03,1,0,0,0,1,0,0,0,0,0,,,0,0*02\r\n");
    delay(250);
    // Switch to Vehicle Mode, since SoftRF enables Aviation < 2g
    GNSS->write("$PCAS11,3*1E\r\n");
    return result;
}

bool syncRTC(HardwareSerial *GNSS, TinyGPSPlus *gps, PCF8563_Class* rtc){
     
     // updates rtc with gps date & time
    while (GNSS->available()) {
        int r = GNSS->read();
        Serial.write(r);
        gps->encode(r);
    }

    if(gps->time.isUpdated()){
            // add correction for Helsinki time / summer time

        
        TimeChangeRule finEET = {"EET", Last, Sun, Mar,  3, +180}; // UTC+3
        TimeChangeRule finEEST = {"EEST", Last, Sun, Oct,  4, +120}; // UTC+2
        Timezone europeanEastern(finEET, finEEST);

        struct tm t_tm;
        t_tm.tm_hour = gps->time.hour();
        t_tm.tm_min = gps->time.minute();
        t_tm.tm_sec = gps->time.second() ;
        t_tm.tm_year = gps->date.year() - 1900;    //Year, whose value starts from 1900
        t_tm.tm_mon = gps->date.month() - 1;       //Month (starting from January, 0 for January) - Value range is [0,11]
        t_tm.tm_mday = gps->date.day();

        time_t timeSinceEpoch = mktime(&t_tm);

        
        time_t eetTime = europeanEastern.toLocal(timeSinceEpoch);

        Serial.println("hours:");
        Serial.println(hour(eetTime));
        
        Serial.println("minutes:");
        Serial.println(minute(eetTime));

        Serial.println("seconds:");  
        Serial.println(second(eetTime));

        Serial.println("d:");
        Serial.println(day(eetTime));
        
        Serial.println("m:");
        Serial.println(month(eetTime));

        Serial.println("y:");  
        Serial.println(year(eetTime));

        rtc->setDateTime(year(eetTime), month(eetTime), day(eetTime), hour(eetTime), minute(eetTime), second(eetTime));
        return true;

    }else{

        return false;
    }
}


void sendDataBT(fs::FS &fs, const char * path, BluetoothSerial& sbt)
{
    /* Sends data via SerialBT */
    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }
    Serial.println("- read from file:");
    while(file.available()){
        sbt.write(file.read());
    }
    file.close();
}

void sendSessionBT(BluetoothSerial& sbt)
{
    
    // Sending session id
    sendDataBT(LITTLEFS, "/id.txt", sbt);
    sbt.write(';');

    // Sending steps
    sendDataBT(LITTLEFS, "/steps.txt", sbt);
    sbt.write(';');

    // Sending distance
    sendDataBT(LITTLEFS, "/distance.txt", sbt);
    sbt.write(';');

    // sending start time
    sendDataBT(LITTLEFS, "/start_time.txt", sbt);
    sbt.write(';');

    // sending duration
    sendDataBT(LITTLEFS, "/duration.txt", sbt);
    sbt.write(';');

    // Send connection termination char
    sbt.write('\n');
}
