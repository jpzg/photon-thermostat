#!/usr/bin/env python

# A server for my thermostat project.
# Accepts websocket connections from client webpages
# Connects to the photon
# Exchanges commands and data between the two
# Determines the proper temperature for the house
# Looks like the Yun can't handle python3, so python2 it is... That means, no async and no websockets lib

import websockets
import socket

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind((socket.gethostname(),8889))
sock.listen()

clients = [] # store current connections in an array
data = [] # store (temp,hum) tuples of the last few minutes

async def on_client_connect():
    # Accept the connection, add it to clients
    # Send info including photon connection status, recent temp data, current settings

async def on_client_command():
    # Parse the command, execute it

async def on_client_disconnect():
    # Remove from clients

async def on_photon_connect():
    # Handle connections

async def on_photon_data():
    # Add it to data, send it to clients

async def on_photon_disconnect():
    # Notify clients