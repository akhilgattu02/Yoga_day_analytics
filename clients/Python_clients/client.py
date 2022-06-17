import websocket
import json
import time
import numpy as np


count =0
data = []
def on_message(wsapp, message):
    print(message)
    count +=1
    if count == 5:
        with open('output.json',"w") as f:
            json.dumps(data,f)
    data.append(message)

wsapp = websocket.WebSocketApp("ws://192.168.0.92:7892/", on_message=on_message)
wsapp.run_forever()

if count == 5:
    with open('output.json',"w") as f:
        json.dumps(data,f)
