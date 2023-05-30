// Bow design Knurl 2022


/* Libraries --------------------------------------------------------------------------------------------- */  

  // Basic libraries for ESP32 OSC
    #if defined(ESP8266)
    #include <ESP8266WiFi.h>
    #else
    #include <WiFi.h>
    #endif
    #include <WiFiUdp.h>
    #include <OSCMessage.h>
  
  // Basic demo for accelerometer readings from Adafruit MPU6050
    #include <Adafruit_MPU6050.h>
    #include <Adafruit_Sensor.h>
    #include <Wire.h>

/* Definitions --------------------------------------------------------------------------------------------- */

Adafruit_MPU6050 mpu;

  // Wifi
    char ssid[] = "KnurlAlpha"; // your network SSID (name)
    char pass[] = "alpha1234";  // your network password
    //char ssid[] = "WD4X"; // your network SSID (name)
    //char pass[] = "w1f1p4$$";  // your network password
    
    WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
    const IPAddress outIp(192,168,30,1);        // remote IP of your computer
    const unsigned int outPort = 2000;          // remote port to receive OSC
    const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

  // button
    // We assigned a name LED pin to pin number 23
      const int LEDPIN = 23; 
    // this will assign the name PushButton to pin numer 15
      const int PushButton = 15; 
 
/* Set up --------------------------------------------------------------------------------------------- */

void setup() {
      Serial.begin(115200);
      
  // Accelerometer
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  
  // OSC

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("send port: ");
#ifdef ESP32
    Serial.println(outPort);
#else
    Serial.println(Udp.outPort());
#endif

// This statement will declare pin 22 as digital output 
pinMode(LEDPIN, OUTPUT);
// This statement will declare pin 15 as digital input 
pinMode(PushButton, INPUT);

  Serial.println("");
  delay(100);
}

void loop() {


//button

// digitalRead function stores the Push button state 
// in variable push_button_state
int Push_button_state = digitalRead(PushButton);
// if condition checks if push button is pressed
// if pressed LED will turn on otherwise remain off 
if ( Push_button_state == HIGH )
{ 
digitalWrite(LEDPIN, HIGH); 

 // Send values thourgh OSC
   /* Get new sensor events with the readings */
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
    
      /* Print out the values */
      OSCMessage msg("/bowknurl");
      msg.add(a.acceleration.x);
      msg.add(a.acceleration.y);
      msg.add(a.acceleration.z);

      msg.add(g.gyro.x);
      msg.add(g.gyro.x);
      msg.add(g.gyro.x);
    
      Serial.println("");

 //test
    msg.add("hello, osc.");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(500);
    
}
else 
{
digitalWrite(LEDPIN, LOW); 
};


}
