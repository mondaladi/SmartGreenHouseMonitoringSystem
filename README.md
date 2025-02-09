# Smart Greenhouse Monitoring System
![1733845374160](https://github.com/user-attachments/assets/b89abed2-1702-483c-8f2f-66445e0545a0)

## Overview
This project is an **IoT-based Smart Greenhouse Monitoring System** using an **ESP8266** microcontroller. The system monitors temperature, humidity, soil moisture, and light intensity. It controls a water pump and LED light based on sensor readings and can be manually controlled via the **Blynk IoT app**.

## Wiring Connection
| ESP8266 Pin | Component |
|------------|------------|
| D4         | DHT11 Data |
| A0         | Soil Moisture Sensor Output |
| D3         | LDR Sensor Output |
| D5         | Water Pump Relay |
| D6         | Light Relay |
| SDA (D2)   | LCD SDA |
| SCL (D1)   | LCD SCL |

## Blynk Dashboard
![Capture](https://github.com/user-attachments/assets/555f60c0-55ea-44f4-9291-b3055cf904ea)

## Future Improvements
- **Alerts:** Push notifications for temperature, soil moisture, air quality, and pH levels via Blynk.  
- **Exhaust Fan:** Automatic control based on temperature thresholds with manual override through Blynk.  
- **Air Quality:** Integration of MQ-135 sensor for CO₂/VOCs detection; triggers auto fan control if levels are high.  
- **Soil Acidity:** Real-time pH monitoring with alerts for pH imbalance using a pH sensor.  
- **MUX (CD74HC4067):** Expand analog inputs to connect multiple sensors (moisture, pH) to the single ADC pin of the NodeMCU.  
- **Light and Fan Switch:** Physical switches for manual light and fan control, fully synchronized with Blynk.  
