import asyncio
import websockets
import json
import time
HOST = "192.168.0.237"  # Empty denotes a localhost.
PORT = 7891
CONNECTIONS = set()


async def handler(websocket):
    CONNECTIONS.add(websocket)

    count = 0
    while True:
        try:
            message = await websocket.recv() 
            count+=1
            print(count)
            print(message)
            # Send a response to all connected client except the server
            for conn in CONNECTIONS:
                if conn != websocket:
                    await conn.send(message)
        except websockets.exceptions.ConnectionClosedError as error1:
            print(f'Server Error: {error1}')
            CONNECTIONS.remove(websocket)

async def main():
    server1 = await websockets.serve(handler, HOST, PORT, ping_interval=None, ping_timeout=None)
    server2 = await websockets.serve(handler, HOST, 7892, ping_interval=None, ping_timeout=None)
    await asyncio.gather(server1.wait_closed(), server2.wait_closed())
      # run forever




if __name__ == "__main__":
   asyncio.run(main())


