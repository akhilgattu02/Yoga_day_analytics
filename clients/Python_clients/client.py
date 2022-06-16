import websocket
import json
import time
import numpy as np
#def on_open(wsapp):
    #start = time.time()
    #while(1):
        #elapsed_time = time.time()
        #json_object = {
            #"time":elapsed_time-start,
            #"sin_time": np.sin(elapsed_time - start)
        #}
        #json_object = json.dumps(json_object)
        #print(json_object)
        #wsapp.send(json_object)
def on_message(wsapp, message):
    print(message)

wsapp = websocket.WebSocketApp("ws://192.168.0.92:7892/", on_message=on_message)
wsapp.run_forever()

