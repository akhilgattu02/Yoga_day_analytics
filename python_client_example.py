import json
import threading
import websocket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

SOCKET = "ws://192.168.0.237:7891/"
time_pulse = []
pulse = []
avg_pulse = []
gsr = []
time_fsr = []
fsr = []




#Function that executes on receiving a message
def on_message(ws, message):
    global time_pulse, pulse, avg_pulse, gsr, time_fsr, fsr
    msg_json = json.loads(message)
    print(msg_json['sensor_type'])
    sensor_type=msg_json['sensor_type']
    if(sensor_type==0): # pulseox
        time_pulse.append(msg_json["Time"])
        pulse.append(msg_json["Pulse"])
        avg_pulse.append(msg_json["Avg_pulse"])
        gsr.append(msg_json["gsr"])

    elif(sensor_type==1):   #fsr
        time_fsr.append(msg_json["Time"])
        fsr.append(msg_json["Force_FSR"])
    

#Initiate a websocket running forever
def wsthread(time_pulse, pulse, avg_pulse, gsr, time_fsr, fsr):
    ws = websocket.WebSocketApp(SOCKET, on_message=on_message)
    ws.run_forever()

#Run the websocket in a background thread.
t = threading.Thread(target=wsthread, args=(time_pulse, pulse, avg_pulse, gsr, time_fsr, fsr,))
t.start()

#The matplotlib plots run in the main thread.
fig, axs = plt.subplots(2)

def animate(i):
    #Render plots as a matplotlib animation
    
    axs[0].plot(fsr)
    axs[1].plot(pulse)

anim = FuncAnimation(
    fig, animate, interval=1000
)

plt.show()