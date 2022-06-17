import json
import threading
import websocket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

SOCKET = "ws://192.168.0.237:7891/"

time_emg = []
channel_1 = []
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
    global time_emg, channel_1, channel_2, channel_3, channel_4, channel_5, channel_6, channel_7, channel_8, channel_9, channel_10, channel_11, channel_12, channel_13, channel_14, channel_15, channel_16
    msg_json = json.loads(message)

    sensor_type = msg_json['sensor_type']
    if(sensor_type == 2):

        time_emg.append(msg_json["time"])
        channel_1.append(msg_json["emg"][0])
        channel_2.append(msg_json["emg"][1])
        channel_3.append(msg_json["emg"][2])
        channel_4.append(msg_json["emg"][3])
        channel_5.append(msg_json["emg"][4])
        channel_6.append(msg_json["emg"][5])
        channel_7.append(msg_json["emg"][6])
        channel_8.append(msg_json["emg"][7])
        channel_9.append(msg_json["emg"][8])
        channel_10.append(msg_json["emg"][9])
        channel_11.append(msg_json["emg"][10])
        channel_12.append(msg_json["emg"][11])
        channel_13.append(msg_json["emg"][12])
        channel_14.append(msg_json["emg"][13])
        channel_15.append(msg_json["emg"][14])
        channel_16.append(msg_json["emg"][15])


# Initiate a websocket running forever
def wsthread(time_emg, channel_1, channel_2, channel_3, channel_4, channel_5, channel_6, channel_7, channel_8, channel_9, channel_10, channel_11,channel_12,channel_13,channel_14,channel_15,channel_16):
    ws = websocket.WebSocketApp(SOCKET, on_message=on_message)
    ws.run_forever()


# Run the websocket in a background thread.
t = threading.Thread(target=wsthread, args=(
    time_emg, channel_1, channel_2, channel_3, channel_4, channel_5, channel_6, channel_7, channel_8, channel_9, channel_10, channel_11,channel_12,channel_13,channel_14,channel_15,channel_16,))
t.start()

# The matplotlib plots run in the main thread.
fig, axs = plt.subplots(8, 2)


def animate(i):
    n = 100
    # Render plots as a matplotlib animation
    axs[0][0].cla()
    axs[0][0].plot(time_emg[-n:], channel_1[-n:])
    axs[0][0].set_title('Channel 1')
    
    #axs[0][0].set_ylim([-4000, 4000])
    axs[1][0].cla()
    axs[1][0].plot(time_emg[-n:], channel_2[-n:])
    axs[1][0].set_title('Channel 2')
    #axs[1][0].set_ylim([-4000, 4000])
    axs[2][0].cla()
    axs[2][0].plot(time_emg[-n:], channel_3[-n:])
    axs[2][0].set_title('Channel 3')
    
    #axs[2][0].set_ylim([-4000, 4000])
    axs[3][0].cla()
    axs[3][0].plot(time_emg[-n:], channel_4[-n:])
    axs[3][0].set_title('Channel 4')
    
    #axs[3][0].set_ylim([-4000, 4000])
    axs[4][0].cla()
    axs[4][0].plot(time_emg[-n:], channel_5[-n:])
    axs[4][0].set_title('Channel 5')
    
    #axs[4][0].set_ylim([-4000, 4000])
    axs[5][0].cla()
    axs[5][0].plot(time_emg[-n:], channel_6[-n:])
    axs[5][0].set_title('Channel 6')
    #axs[5][0].set_ylim([-4000, 4000])
    axs[6][0].cla()
    axs[6][0].plot(time_emg[-n:], channel_7[-n:])
    axs[6][0].set_title('Channel 7')
    #axs[6][0].set_ylim([-4000, 4000])
    axs[7][0].cla()
    axs[7][0].plot(time_emg[-n:], channel_8[-n:])
    axs[7][0].set_title('Channel 8')
    #axs[7][0].set_ylim([-4000, 4000])
    axs[0][1].cla()
    axs[0][1].plot(time_emg[-n:], channel_9[-n:])
    axs[0][1].set_title('Channel 9')
    #axs[0][1].set_ylim([-4000, 4000])
    axs[1][1].cla()
    axs[1][1].plot(time_emg[-n:], channel_10[-n:])
    axs[1][1].set_title('Channel 10')
    
    #axs[1][1].set_ylim([-4000, 4000])
    axs[2][1].cla()

    axs[2][1].plot(time_emg[-n:], channel_11[-n:])
    axs[2][1].set_title('Channel 11')
    #axs[2][1].set_ylim([-4000, 4000])
    axs[3][1].cla()
    axs[3][1].plot(time_emg[-n:], channel_12[-n:])
    axs[3][1].set_title('Channel 12')
    #axs[3][1].set_ylim([-4000, 4000])
    axs[4][1].cla()
    axs[4][1].plot(time_emg[-n:], channel_13[-n:])
    axs[4][1].set_title('Channel 13')
    #axs[4][1].set_ylim([-4000, 4000])
    axs[5][1].cla()
    axs[5][1].plot(time_emg[-n:], channel_14[-n:])
    axs[5][1].set_title('Channel 14')
    #axs[5][1].set_ylim([-4000, 4000])
    axs[6][1].cla()
    axs[6][1].plot(time_emg[-n:], channel_15[-n:])
    axs[6][1].set_title('Channel 15')
    #axs[6][1].set_ylim([-4000, 4000])
    axs[7][1].cla()
    axs[7][1].plot(time_emg[-n:], channel_16[-n:])
    axs[7][1].set_title('Channel 16')
    #axs[7][1].set_ylim([-4000, 4000])
    

anim = FuncAnimation(
    fig, animate, interval=1000
)

plt.show()
