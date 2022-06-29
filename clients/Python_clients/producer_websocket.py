
import json
import threading
import websocket
from kafka import KafkaProducer
from kafka.errors import KafkaError

SOCKET = "ws://192.168.43.14:7891/"



# Function that executes on receiving a message
producer = KafkaProducer(bootstrap_servers=[
                             '192.168.43.89:49155'], value_serializer=lambda m: json.dumps(m).encode('ascii'))

json_array = []
count = 0
def on_message(ws, message): 
    global json_array, count
    msg_json = json.loads(message)
    json_array.append(msg_json)
    count+=1
    if count%1000 == 0:
        future = producer.send('t1', {"sensor": json_array})
        json_array.clear()
        
   

# Successful result returns assigned partition and offset



# Initiate a websocket running forever
ws = websocket.WebSocketApp(SOCKET, on_message=on_message)
ws.run_forever()


# Run the websocket in a background thread.



# Asynchronous by default


# The matplotlib plots run in the main thread.
