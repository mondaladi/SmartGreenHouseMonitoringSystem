#define BLYNK_TEMPLATE_ID "TMPL33KTOM6jK"
#define BLYNK_TEMPLATE_NAME "Smart Greenhouse Monitoring System"
#define BLYNK_AUTH_TOKEN "YA5VjFUFTQVu45KKvZPFWjgaruKjweXx"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D4           // GPIO pin for DHT11 sensor
#define DHTTYPE DHT11       // Define sensor type as DHT11

#define SOIL_MOISTURE_PIN A0   // Analog pin for soil moisture sensor
#define LDR_PIN D3             // Digital pin for LDR sensor
#define PUMP_RELAY_PIN D5      // Relay pin for water pump (active LOW)
#define LIGHT_PIN D6           // Relay pin for LED light (active LOW)

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C address and dimensions

char auth[] = BLYNK_AUTH_TOKEN;    // Blynk authentication token
char ssid[] = "Adi";               // WiFi SSID
char pass[] = "Aditya@129";        // WiFi password

bool manualPumpControl = false;  
bool manualLightControl = false; 

unsigned long previousMillis = 0;  
const long interval = 500;         

// Blynk function to control the pump manually via Virtual Pin V0
BLYNK_WRITE(V0) {  
  manualPumpControl = param.asInt();
  digitalWrite(PUMP_RELAY_PIN, manualPumpControl ? LOW : HIGH); 
}

// Blynk function to control the light manually via Virtual Pin V3
BLYNK_WRITE(V3) {  
  manualLightControl = param.asInt();
  digitalWrite(LIGHT_PIN, manualLightControl ? HIGH : LOW);  
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  
  dht.begin();
  
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT); 

  digitalWrite(PUMP_RELAY_PIN, HIGH); 
  digitalWrite(LIGHT_PIN, LOW);        

  lcd.init();
  lcd.backlight();
}

// Function to reconnect WiFi and Blynk if disconnected
void reconnectBlynk() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000) {
      delay(500);
      Serial.print(".");
    }
  }
  if (!Blynk.connected()) {
    Blynk.connect();
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Validate sensor readings before sending to Blynk
    if (!isnan(h) && !isnan(t)) {
      Blynk.virtualWrite(V1, t);
      Blynk.virtualWrite(V2, h); 
    }

    int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);  // Read soil moisture level
    int soilMoisturePercent = map(soilMoistureValue, 0, 1023, 100, 0); // Convert to percentage
    Blynk.virtualWrite(V4, soilMoisturePercent);  

    int ldrState = digitalRead(LDR_PIN);  // Read LDR sensor state (Using digitalRead as the only available analog pin is connected to the soil moisture sensor)

    // Automatic Pump Control if manual mode is OFF
    if (!manualPumpControl) {
      if (soilMoistureValue < 500) {
        digitalWrite(PUMP_RELAY_PIN, LOW);
      } else {
        digitalWrite(PUMP_RELAY_PIN, HIGH);
      }
    }

    // Automatic Light Control if manual mode is OFF
    if (!manualLightControl) {
      if (ldrState == LOW) {  // Assuming LOW means it's dark
        digitalWrite(LIGHT_PIN, LOW);
      } else {
        digitalWrite(LIGHT_PIN, HIGH); 
      }
    }

    // Update LCD display with temperature, humidity, soil moisture, and light status
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t, 1);
    lcd.print("  H:");
    lcd.print(h, 1); 

    lcd.setCursor(0, 1);
    lcd.print("M:");
    lcd.print(soilMoisturePercent); 
    lcd.print("%  L:");
    lcd.print(ldrState ? "Bright" : "Dark");
  }

  // Maintain Blynk connection
  if (Blynk.connected()) {
    Blynk.run();
  } else {
    reconnectBlynk();
  }
}