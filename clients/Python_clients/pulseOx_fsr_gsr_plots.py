import json
import threading
import websocket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

SOCKET = "ws://192.168.223.209:7891/"
time_pulse = []
pulse = []
avg_pulse = []
gsr = []
time_fsr = []
fsr = []
channel_1 = []
time_emg = []
channel_2 = []
channel_3 = []
channel_4 = []
channel_5 = []
channel_6 = []
channel_7 = []
channel_8 = []
channel_9 = []
channel_10 = []
channel_11 = []
channel_12 = []
channel_13 = []
channel_14 = []
channel_15 = []
channel_16 = []
# Function that executes on receiving a message


def on_message(ws, message):
    global time_pulse, pulse, avg_pulse, gsr, time_fsr, fsr, time_emg, channel_1, channel_2, channel_3, channel_4, channel_5, channel_6, channel_7, channel_8, channel_9, channel_10, channel_11, channel_12, channel_13, channel_14, channel_15, channel_16
    msg_json = json.loads(message)
    print(message)

    sensor_type = msg_json['sensor_type']
    if(sensor_type == 0):  # pulseox
        time_pulse.append(msg_json["Time"])
        pulse.append(msg_json["Pulse"])
        avg_pulse.append(msg_json["Avg_pulse"])
        gsr.append(msg_json["gsr"])

    elif(sensor_type == 1):  # fsr
        time_fsr.append(msg_json["time"])
        fsr.append(msg_json["data_1"])
    elif(sensor_type == 2):
        time_emg.append(msg_json["time"])
        channel_1.append(msg_json["data"][0])
        channel_2.append(msg_json["data"][1])
        channel_3.append(msg_json["data"][2])
        channel_4.append(msg_json["data"][3])
        channel_5.append(msg_json["data"][4])
        channel_6.append(msg_json["data"][5])
        channel_7.append(msg_json["data"][6])
        channel_8.append(msg_json["data"][7])
        channel_9.append(msg_json["data"][8])
        channel_10.append(msg_json["data"][9])
        channel_11.append(msg_json["data"][10])
        channel_12.append(msg_json["data"][11])
        channel_13.append(msg_json["data"][12])
        channel_14.append(msg_json["data"][13])
        channel_15.append(msg_json["data"][14])
        channel_16.append(msg_json["data"][15])


# Initiate a websocket running forever
def wsthread(time_pulse, pulse, avg_pulse, gsr, time_fsr, fsr, time_emg, channel_1, channel_2, channel_3, channel_4, channel_5, channel_6, channel_7, channel_8, channel_9, channel_10, channel_11,channel_12,channel_13,channel_14,channel_15,channel_16):
    ws = websocket.WebSocketApp(SOCKET, on_message=on_message)
    ws.run_forever()


# Run the websocket in a background thread.
t = threading.Thread(target=wsthread, args=(
    time_pulse, pulse, avg_pulse, gsr, time_fsr, fsr, time_emg, channel_1, channel_2, channel_3, channel_4, channel_5, channel_6, channel_7, channel_8, channel_9, channel_10, channel_11,channel_12,channel_13,channel_14,channel_15,channel_16,))
t.start()

# The matplotlib plots run in the main thread.
fig, axs = plt.subplots(8, 2)


def animate(i):
    # Render plots as a matplotlib animation

    axs[0][0].plot(time_emg, channel_1)
    axs[1][0].plot(time_emg, channel_2)
    axs[2][0].plot(time_emg, channel_3)
    axs[3][0].plot(time_emg, channel_4)
    axs[4][0].plot(time_emg, channel_5)
    axs[5][0].plot(time_emg, channel_6)
    axs[6][0].plot(time_emg, channel_7)
    axs[7][0].plot(time_emg, channel_8)
    axs[0][1].plot(time_emg, channel_9)
    axs[1][1].plot(time_emg, channel_10)
    axs[2][1].plot(time_emg, channel_11)
    axs[3][1].plot(time_emg, channel_12)
    axs[4][1].plot(time_emg, channel_13)
    axs[5][1].plot(time_emg, channel_14)
    axs[6][1].plot(time_emg, channel_15)
    axs[7][1].plot(time_emg, channel_16)


anim = FuncAnimation(
    fig, animate, interval=1000
)

plt.show()
