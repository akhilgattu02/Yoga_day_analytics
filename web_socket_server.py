import asyncio
import websockets
import json
HOST = "192.168.0.100"  # Empty denotes a localhost.
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
    async with websockets.serve(handler, HOST, PORT):
        await asyncio.Future()  # run forever


if __name__ == "__main__":
   asyncio.run(main())



