import json
import threading
import websocket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

SOCKET = "ws://192.168.0.100:7891/yoga_mat"
closes = np.array([])

#Function that executes on receiving a message
def on_message(ws, message):
    global closes
    message = json.loads(message)
    close = message["Data"]
    closes = np.array(close)

#Initiate a websocket running forever
def wsthread(closes):
    ws = websocket.WebSocketApp(SOCKET, on_message=on_message)
    ws.run_forever()

#Run the websocket in a background thread.
t = threading.Thread(target=wsthread, args=(closes,))
t.start()

#The matplotlib plots run in the main thread.
fig = plt.figure(figsize=plt.figaspect(1))

def animate(i):
    #Render plots as a matplotlib animation
    print(closes)
    data_arr = closes

    rows = 5
    columns = 5
    
    Z = data_arr.reshape(columns,rows)

    ax = fig.add_subplot(1,1,1)

    plt.imshow(Z,origin='lower',interpolation='bilinear', cmap='autumn',extent = [0,rows-1,0,columns-1], vmin=0, vmax = 4096)
    plt.colorbar()
    

anim = FuncAnimation(
    fig, animate, interval=1000
)

plt.show()