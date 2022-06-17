# Yoga_day_analytics
## Websocket server for transmitting data from multiple sensors
1. Run the python server in the servers folder after installing "websockets" and "websocket-client" libraries.
2. After ensuring that the server is switched, go to Sensor_clients folder and burn the code for the desired sensor on connecting the sensor to ESP32 and using Arduino IDE.
3. The server currently runs to collect information posted from multiple sensors simultaneously at a speed of nearly 250 samples/second.
4. The flask server collects 8 samples of sensor data per second.
## Sensors:
### Pulse oximeter and GSR:
#### Sensor data format
#### Plots
![alt text](https://github.com/akhilgattu02/Yoga_day_analytics/blob/main/pictures/Pulse_ox.png)
### Yoga mat heatmap:
#### Sensor data format
#### Plots
![alt_text](https://github.com/akhilgattu02/Yoga_day_analytics/blob/main/pictures/heatmap.png)
### FSR:
#### Sensor data format
#### Plots
![alt_text](https://github.com/akhilgattu02/Yoga_day_analytics/blob/main/pictures/fsr_plot.png)
### EMG:
#### Sensor data format
#### Plots
![alt_text](https://github.com/akhilgattu02/Yoga_day_analytics/blob/main/pictures/emg.png)
## Issues to fix:
1. Database integration
2. Faster data transmission of multiple sensors(using threading or queues)
