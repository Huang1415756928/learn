#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
uint8_t txHead = 0xff;

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0
// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13
// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000
// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN            2

int brightness = 0;    // 初始亮度how bright the LED is
int fadeAmount = 5;    // 变化等级how many points to fade the LED by
int i = 10;    //LED 闪烁频率，单个周期=i*255*2
long int k = 0;
uint8_t blink_s=0; //LED状态标志位，0为呼吸，1，2…为闪烁指定次数
int hall;
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();

            Serial.println("*********");
            Serial.print("Received Value: ");
            for (int i = 0; i < rxValue.length(); i++)
            Serial.print(rxValue[i]);
            Serial.println();
            Serial.println("*********");
            Serial.println(rxValue.length());
        }
    };
//LED呼吸效果函数，用三角函数的方式实现
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
    // calculate duty, 8191 from 2 ^ 13 - 1
    uint32_t duty = (8191 / valueMax) * min(value, valueMax);
    // write duty to LEDC
    ledcWrite(channel, duty);
}
//led呼吸灯输出
void led_breath()
{
    ledcAnalogWrite(LEDC_CHANNEL_0, brightness);
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(i);
}

//led以亮度闪烁指定次数
void led_blink_t(uint8_t t,uint8_t s)//t为次数（0-255）s为亮度（0-255）
{
    for (uint8_t t1 = 0;t1 < t;t1++)
    {
        ledcAnalogWrite(LEDC_CHANNEL_0, s);
        delay(100);
        ledcAnalogWrite(LEDC_CHANNEL_0, 0);
        delay(200);
    }
    delay(700);
}


// define two tasks for Blink & AnalogRead
void TaskBlink(void* pvParameters);
void TaskAnalogReadA3(void* pvParameters);



// the setup function runs once when you press reset or power the board
void setup() {

    //LED初始化
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);

    // 串口初始化initialize serial communication at 115200 bits per second:
    Serial.begin(115200);

    BLEDevice::init("Ble test");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pTxCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );

    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");


    // 任务函数初始化Now set up two tasks to run independently.
    xTaskCreatePinnedToCore(
        TaskBlink
        , "TaskBlink"   // 任务名称A name just for humans
        , 1024  //任务堆栈大小，单位字节数表示 This stack size can be checked & adjusted by reading the Stack Highwater
        , NULL  //
        , 1  // 任务优先级Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        , NULL
        , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        TaskAnalogReadA3
        , "AnalogReadA3"
        , 2048  // Stack size
        , NULL
        , 1  // Priority
        , NULL
        , ARDUINO_RUNNING_CORE);

    // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
    // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void* pvParameters)  // This is a task.
{

    for (;;)
    {
        switch (blink_s)
        {
        case 0: for (short int t2 = 0;t2 < 103;t2++) 
        {
            led_breath();
        }break;
        case 1:led_blink_t(1, 255); break;
        case 2:led_blink_t(2, 255); break;
        case 3:led_blink_t(3, 255); break;
        case 4:led_blink_t(4, 255); break;
        }
    }
}

void TaskAnalogReadA3(void* pvParameters)  // This is a task.
{

    /*
      AnalogReadSerial
      Reads an analog input on pin A3, prints the result to the serial monitor.
      Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
      Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

      This example code is in the public domain.
    */

    for (;;)
    {
        static uint8_t txhead = 0xff;
        if (deviceConnected) {
            hall = hallRead();
            //txValue=hallRead()+100;
            //pTxCharacteristic->setValue(&txValue, 2);
            pTxCharacteristic->setValue(hall);
            pTxCharacteristic->notify();
            txValue++;
            blink_s = 1;
            delay(5); // bluetooth stack will go into congestion, if too many packets are sent
        }

        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
            delay(500); // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising(); // restart advertising
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
            blink_s = 2;
        }
        // connecting
        if (deviceConnected && !oldDeviceConnected) {
            // do stuff here on connecting
            oldDeviceConnected = deviceConnected;
            blink_s = 3;
        }

    }
}
