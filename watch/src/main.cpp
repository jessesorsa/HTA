#include "config.h"

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// Watch objects
TTGOClass *watch;
TFT_eSPI *tft;
BMA *sensor;
PCF8563_Class *rtc;
//CapacitiveTouch *tp;

#if defined(LILYGO_WATCH_2020_V2)
TinyGPSPlus *gps = nullptr;
HardwareSerial *GNSS = NULL;
#endif

// session data
uint16_t sessionId = 30;
uint32_t steps = 0;
RTC_Date startTime;
Duration sessionDuration;

volatile uint8_t state;
volatile bool irqBMA = false;
volatile bool irqButton = false;

bool sessionStored = false;
bool sessionSent = false;
bool dataDisplayable = false;

bool  rtcGpsSynced = false;


//must have initialized watch before usage
bool isShortSideBtnPress(void){
    bool read = 1;
    do {
        read = watch->power->readIRQ();
    } while(!(read==0));
    return watch->power->isPEKShortPressIRQ();
}


//calculates traveled distance in KM
float calculateDistance(uint32_t steps){
    float stepLenght = 0.74; 
    return steps*stepLenght/1000;
}



void displayWelcomeMsg(){
    

    // Basic interface
    tft->fillScreen(TFT_BLACK);
    tft->setTextFont(4);
    tft->setTextDatum(TC_DATUM); //top center datum
    tft->setTextColor(TFT_GREEN,TFT_BLACK); 
    tft->drawString("Press button to", tft->width()/2, 65);
    tft->drawString("start hiking session", tft->width()/2, 95);
    tft->setTextColor(TFT_WHITE,TFT_BLACK);

    if(dataDisplayable){
        tft->setTextFont(2);
        tft->setTextDatum(TC_DATUM); //top left datum
        tft->drawString("Previous session",tft->width()/2,tft->width()/2 + 5) ;
        
        tft->setTextFont(4);
        tft->setCursor(10, 150);
        tft->print("Steps: ");
        tft->print(steps);

        tft->setCursor(10, 180);
        tft->print("Dist: ");
        tft->print(calculateDistance(steps)); //converted to km
        tft->print(" km");  

        tft->setCursor(10, 210);
        tft->print("Duration: ");
        tft->print(sessionDuration.getAsCString());
    }
    
    return;
}

void initHikeWatch(){

    // LittleFS
    if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("LITTLEFS Mount Failed");
        return;
    }

    // Stepcounter
    // Configure IMU
    Acfg cfg;
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;  //Output data rate
    cfg.range = BMA4_ACCEL_RANGE_2G;        //G-rage 
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4; //Bandwith parameter
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;   //Filter performance mode
    sensor->accelConfig(cfg);
    sensor->enableAccel();
    
    //Set pin mode and attach Interrupt
    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        // Set interrupt to set irq value to 1
        irqBMA = 1;
    }, RISING); //It must be a rising edge
    
    
    // Enable BMA423 step count feature
    sensor->enableFeature(BMA423_STEP_CNTR, true);

    // Reset steps
    sensor->resetStepCounter();

    // Turn on step interrupt
    sensor->enableStepCountInterrupt();

    // Side button
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irqButton = true;
    }, FALLING);

    //!Clear IRQ unprocessed first
    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    watch->power->clearIRQ();


    //GPS
    #if defined(LILYGO_WATCH_2020_V2)
    watch->trunOnGPS();
    watch->gps_begin();
    gps = watch->gps;
    GNSS = watch->hwSerial;

    if (!Quectel_L76X_Probe(GNSS)) {
        watch->tft->setCursor(0, 0);
        watch->tft->setTextColor(TFT_RED);
        watch->tft->println("GNSS Probe failed!"); while (1);
    }
    #endif

    
    // keep previous time in RTC by not reseting


    return;
}


void setup(){

    Serial.begin(115200);
    watch = TTGOClass::getWatch();
    watch->begin();
    watch->openBL();

    //Receive objects for easy writing
    tft = watch->tft;
    sensor = watch->bma;
    rtc = watch->rtc;
    initHikeWatch();

    state = 1;

    SerialBT.begin("Hiking Tour Assistant");


    // Load values from previous session in startup // 
    // steps, duration as reference
    int duration_in_secs = 0;
    if(loadLastSession(LITTLEFS, steps, duration_in_secs)){
        dataDisplayable = true;
        sessionStored = true;
        sessionDuration.setCount(duration_in_secs);
    }
}

void loop(){

    switch (state)
    {
        case 1:
        {   
            if(!rtcGpsSynced){
                // sync RTC only once 
            rtcGpsSynced = syncRTC(GNSS, gps, rtc);
            } 

            /* Initial stage */
            //Basic interface
            displayWelcomeMsg();

            // display time
            watch->tft->drawString(rtc->formatDateTime(PCF_TIMEFORMAT_HMS) , 5, 10, 7);

            bool exitSync = false;

            // TODO CHECK THAT WATCH WORKS CORRECTLY IN BLUETOOTH SYNC / WELCOME SCREEN maybe add rtc draw above also
            //Bluetooth discovery
            while (1){
                // display time here also!
                watch->tft->drawString(rtc->formatDateTime(PCF_TIMEFORMAT_HMS) , 5, 10, 7);

                
                //TODO figure out how sync behaves in different situations
                /* Bluetooth sync */
                if (SerialBT.available()){

                    char incomingChar = SerialBT.read();
                    if (incomingChar == 'c' and sessionStored and not sessionSent){   
                        // Read session and send it via SerialBT
                        tft->setTextDatum(TC_DATUM); //top center datum
                        tft->fillRect(0, 0, 240, 240, TFT_BLACK);
                        tft->drawString("Sending session", tft->width()/2, 80);
                        tft->drawString("to Hub", tft->width()/2, 110);
                        tft->setTextDatum(TL_DATUM); //top left datum

                        sendSessionBT(SerialBT);
                        sessionSent = true;
                    }

                    if (sessionSent && sessionStored){
                        // Update timeout before blocking while
                        unsigned long updateTimeout = millis();
                        while(1){

                            if (SerialBT.available())
                                incomingChar = SerialBT.read();
                            if (incomingChar == 'r'){

                                Serial.println("Got an R");
                                // Delete session
                                deleteSession();
                                sessionStored = false;
                                sessionSent = false;
                                incomingChar = 'q';
                                exitSync = true;

                                //Inform user
                                tft->setTextDatum(CC_DATUM); //top center datum
                                tft->fillRect(0, 0, 240, 240, TFT_BLACK);
                                tft->drawString("Session sent", tft->width()/2, tft->height()/2);
                                tft->setTextDatum(TL_DATUM); //top left datum


                                break;
                                
                            }else if ((millis() - updateTimeout > 2000)) {  //sync has taken more than 2 seconds
    
                            
                                Serial.println("Timeout expired");
                                updateTimeout = millis();
                                sessionSent = false;
                                
                                //Inform user
                                tft->fillRect(0, 0, 240, 240, TFT_BLACK);
                                tft->setTextColor(TFT_RED,TFT_BLACK);
                                tft->setTextDatum(BC_DATUM); //bottom center datum
                                tft->drawString("Sending session", tft->width()/2, (tft->height()/2) - 5);
                                tft->setTextDatum(TC_DATUM); //top center datum
                                tft->drawString("failed!", tft->width()/2, (tft->height()/2) + 5);
                                tft->setTextDatum(TL_DATUM); //top left datum
                                tft->setTextColor(TFT_WHITE,TFT_BLACK);

                                exitSync = true;
                                break;
                            }
                        }
                    }
                }
                if (exitSync)
                {
                    delay(2000);
                    exitSync = false;
                    displayWelcomeMsg();
                }

                /*      IRQ     */
                if (irqButton) {
                    irqButton = false;
                    /*
                    watch->power->readIRQ();
                    */
                    if(isShortSideBtnPress()){
                        if (state == 1)
                        {
                            state = 2;
                        }
                    }
                    watch->power->clearIRQ();
                }
                if (state == 2){

                    if (sessionStored){
                        
                        tft->setTextDatum(TL_DATUM);
                        tft->fillRect(0, 0, 240, 240, TFT_BLACK);
                        tft->drawString("Overwriting",  55, 100, 4);
                        tft->drawString("session", 70, 130);
                        
                        deleteSession();
                        sessionStored = 0;
                        delay(1000); // shows message to user
                    }

                    break;
                }

                delay(1000);
            }

            break;
        }
        case 2:
        {   
            /* Hiking session initalisation */
            watch->tft->fillRect(0, 0, 240, 240, TFT_BLACK);
            watch->tft->setTextDatum(CC_DATUM); //center center datum
            watch->tft->drawString("Starting hike", 120, 120);
            delay(1000);

            // makes unique id
            sessionId = millis();

            //reset steps
            sensor->resetStepCounter();

            // save start time
            startTime = rtc->getDateTime();

            // start stop watch
            sessionDuration.startCount();
            
            watch->tft->fillRect(0, 0, 240, 240, TFT_BLACK);

            watch->tft->setCursor(10, 150);
            watch->tft->print("Steps: 0");

            watch->tft->setCursor(10, 180);
            watch->tft->print("Dist: 0.00 km");

            watch->tft->setCursor(10, 210);
            watch->tft->print("Duration: ");        

            state = 3;
            break;
        }
        case 3:
        {   

            // Basic interface
            tft->setTextFont(4);
            tft->setTextDatum(TC_DATUM); //top center datum
            tft->setTextColor(TFT_RED,TFT_BLACK); 
            tft->drawString("Press button to", tft->width()/2, 65);
            tft->drawString("stop hiking session", tft->width()/2, 95);
            tft->setTextColor(TFT_WHITE,TFT_BLACK);

            tft->setTextFont(2);
            tft->setTextDatum(TC_DATUM);
            tft->drawString("Ongoing session",tft->width()/2,tft->width()/2 + 5) ;

            // display watch in screen
            tft->drawString(rtc->formatDateTime(PCF_TIMEFORMAT_HMS) , 5, 10, 7);

            // update and duration regardless of steps update
            sessionDuration.update();

            tft->setTextDatum(TL_DATUM);
            tft->setTextFont(4);
            tft->setCursor(10, 210);
            tft->print("Duration: ");
            tft->print(sessionDuration.getAsCString());

            /* IRQ to stop hiking session */
            if (irqButton) {
                irqButton = false;
                
                if(isShortSideBtnPress()){
                    if (state == 3)
                    {
                        state = 4;
                    }
                    watch->power->clearIRQ();
                break;
                }
                watch->power->clearIRQ();
            }
            
            /* Hiking session ongoing */
            if (irqBMA) {

                irqBMA = 0;
                bool  rlst;
                do {
                    // Read the BMA423 interrupt status,
                    // need to wait for it to return to true before continuing
                    rlst =  sensor->readInterrupt();
                } while (!rlst); 

                // Check if it is a step interrupt
                if (sensor->isStepCounter()) {
                    // Get step data from register
                    steps = sensor->getCounter();
                    
                    tft->fillRect(0, 150, 240, 60, TFT_BLACK); 
                

                    tft->setTextDatum(TL_DATUM); //top left datum
                    tft->setTextFont(4);
                    tft->setCursor(10, 150);
                    tft->print("Steps: ");
                    tft->print(steps);

                    tft->setCursor(10, 180);
                    tft->print("Dist: ");
                    tft->print(calculateDistance(steps)); //converted to km
                    tft->print(" km");  
                }
            }
                
            delay(500);
            
            break;
        }
        case 4:
        {
            //Save hiking session data
            tft->fillRect(0, 0, 240, 240, TFT_BLACK);
            tft->setTextDatum(CC_DATUM); //center center datum
            tft->drawString("Saving session!",tft->width()/2,tft->height()/2);
            
            // end count
            sessionDuration.endCount();

            // save variables to txt files using LITTLEFS 
            saveIdToFile(sessionId);
            saveStepsToFile(steps);
            saveDistanceToFile(calculateDistance(steps));
            saveStartTimeToFile(startTime);
            saveDurationToFile(sessionDuration.getAsCString());
            sessionStored = 1;
            dataDisplayable = true;
            
            delay(1000);

            tft->fillRect(0, 0, 240, 240, TFT_BLACK);
            tft->drawString("Session saved!",tft->width()/2, tft->height()/2);
            tft->setTextDatum(TL_DATUM);

            delay(2000);
            
            state = 1;  
            
            break;
        }
        default:
        {
            // Restart watch
            ESP.restart();
            break;
        }
    }        
}