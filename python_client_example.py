import websockets
import asyncio
import json
# The main funciton that will handle connectioin and communication with the server
async def listen():
    url = 'ws://192.168.0.237:7891'
    #url = 'ws://localhost:7890'
    i = 0

    #Connect to the server
    async with websockets.connect(url) as ws:
        #stay alive forever to incoming messages
        await ws.send("Hello Server!")
        while True:
            #ws.send(json.dump(i))
            msg = await ws.recv()
            print(msg)
            i = i+1
#Start the connection
asyncio.get_event_loop().run_until_complete(listen())
