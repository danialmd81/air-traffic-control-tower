import socket
import random
import time
import math

HOST = "127.0.0.1"
PORT = 8080

IRAN_LAT = 32.4279
IRAN_LON = 53.6880


def make_packet(obj_id, lat, lon, lat_factor, lon_factor, valid=True):
    packet = bytearray()
    packet += (
        bytes.fromhex("A5A5A5A5")
        if valid
        else bytes([random.randint(0, 255) for _ in range(4)])
    )
    # Type ID (cycle through 5 types)
    type_ids = [0xA1, 0xA2, 0xB1, 0xB2, 0xC1]
    packet += bytes([type_ids[obj_id % len(type_ids)]])
    # Source/Dest Airport ID
    packet += bytes([0x1A + obj_id, 0x1B + obj_id])
    # Latitude, Latitude Factor
    packet += int(lat * lat_factor).to_bytes(4, "little", signed=True)
    packet += lat_factor.to_bytes(2, "little")
    # Longitude, Longitude Factor
    packet += int(lon * lon_factor).to_bytes(4, "little", signed=True)
    packet += lon_factor.to_bytes(2, "little")
    # Altitude
    packet += (10000 + obj_id * 1000).to_bytes(4, "little")
    # Flight ID
    packet += (1000 + obj_id).to_bytes(4, "little")
    # Reserved
    packet += (0).to_bytes(4, "little")
    packet += (0).to_bytes(4, "little")
    packet += (
        bytes.fromhex("55555555")
        if valid
        else bytes([random.randint(0, 255) for _ in range(4)])
    )
    return packet


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    print(f"Listening on {HOST}:{PORT} ...")
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        lat_factor = 10000
        lon_factor = 10000
        num_objs = 5
        steps = 30
        radius = 2.0  # degrees
        for step in range(steps):
            for obj_id in range(num_objs):
                angle = 2 * math.pi * (step + obj_id * steps // num_objs) / steps
                lat = IRAN_LAT + radius * math.sin(angle)
                lon = IRAN_LON + radius * math.cos(angle)
                pkt = make_packet(obj_id, lat, lon, lat_factor, lon_factor)
                conn.sendall(pkt)
            print(f"Step {step+1}/{steps} sent for all objects")
            time.sleep(0.3)
