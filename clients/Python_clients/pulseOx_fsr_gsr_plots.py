import json
import threading
import websocket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

SOCKET = "ws://192.168.0.237:7891/"
time_pulse = []
pulse = []
gsr = []
time_fsr = []
fsr = []

# Function that executes on receiving a message


def on_message(ws, message):
    global time_pulse, pulse, gsr, time_fsr, fsr
    msg_json = json.loads(message)
    print(message)

    sensor_type = msg_json['sensor_type']
    if(sensor_type == 0):  # pulseox
        time_pulse.append(msg_json["time"])
        pulse.append(msg_json["pulse"])
        gsr.append(msg_json["gsr"])

    elif(sensor_type == 1):  # fsr
        time_fsr.append(msg_json["time"])
        fsr.append(msg_json["fsr"])
   


# Initiate a websocket running forever
def wsthread(time_pulse, pulse, gsr, time_fsr, fsr):
    ws = websocket.WebSocketApp(SOCKET, on_message=on_message)
    ws.run_forever()


# Run the websocket in a background thread.
t = threading.Thread(target=wsthread, args=(
    time_pulse, pulse, gsr, time_fsr, fsr, ))
t.start()

# The matplotlib plots run in the main thread.
fig, axs = plt.subplots(3)


def animate(i):
    # Render plots as a matplotlib animation
    axs[0].cla()
    axs[0].set_title('Pulse Oximeter')
    axs[0].plot(time_pulse, pulse)
    axs[1].cla()
    axs[1].plot(time_pulse, gsr)
    axs[1].set_title('GSR')
    axs[2].cla()
    axs[2].plot(time_fsr, fsr)
    axs[2].set_title('FSR')


anim = FuncAnimation(
    fig, animate, interval=1
)

plt.show()
