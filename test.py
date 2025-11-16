import socket
import random
import time

HOST = "127.0.0.1"
PORT = 8080

# Iran approximate center
IRAN_LAT = 32.4279
IRAN_LON = 53.6880


def random_packet(valid=True):
    packet = bytearray()
    if valid:
        packet += bytes.fromhex("A5A5A5A5")  # Header
    else:
        packet += (
            random.randbytes(4)
            if hasattr(random, "randbytes")
            else bytes([random.randint(0, 255) for _ in range(4)])
        )
    # Type ID
    packet += bytes(
        [
            random.choice(
                [
                    0xA1,
                    0xA2,
                    0xB1,
                    0xB2,
                    0xB3,
                    0xB4,
                    0xB5,
                    0xB6,
                    0xB7,
                    0xC1,
                    0xD1,
                    0xD2,
                    0xD5,
                    0xE1,
                    0xE2,
                    0xE8,
                    0xF0,
                    0xF1,
                    0xF2,
                    0xF5,
                    random.randint(0, 255),
                ]
            )
        ]
    )
    # Source/Dest Airport ID
    packet += bytes(
        [
            random.choice(
                [
                    0x1A,
                    0x1B,
                    0x1C,
                    0x1D,
                    0x1E,
                    0x1F,
                    0x3A,
                    0xA2,
                    0x10,
                    0x2B,
                    0x3C,
                    random.randint(0, 255),
                ]
            )
        ]
    )
    packet += bytes(
        [
            random.choice(
                [
                    0x1A,
                    0x1B,
                    0x1C,
                    0x1D,
                    0x1E,
                    0x1F,
                    0x3A,
                    0xA2,
                    0x10,
                    0x2B,
                    0x3C,
                    random.randint(0, 255),
                ]
            )
        ]
    )
    # Latitude, Latitude Factor
    lat_factor = random.randint(1000, 10000)
    lat = int((IRAN_LAT + random.uniform(-5, 5)) * lat_factor)
    packet += lat.to_bytes(4, "little", signed=True)
    packet += lat_factor.to_bytes(2, "little")
    # Longitude, Longitude Factor
    lon_factor = random.randint(1000, 10000)
    lon = int((IRAN_LON + random.uniform(-5, 5)) * lon_factor)
    packet += lon.to_bytes(4, "little", signed=True)
    packet += lon_factor.to_bytes(2, "little")
    # Altitude
    packet += random.randint(0, 50000).to_bytes(4, "little")
    # Flight ID
    packet += random.randint(0, 100000).to_bytes(4, "little")
    # Reserved
    packet += random.randint(0, 2**32 - 1).to_bytes(4, "little")
    packet += random.randint(0, 2**32 - 1).to_bytes(4, "little")
    if valid:
        packet += bytes.fromhex("55555555")  # Footer
    else:
        packet += (
            random.randbytes(4)
            if hasattr(random, "randbytes")
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
        for i in range(50):
            valid = random.choice([True, False])
            pkt = random_packet(valid)
            conn.sendall(pkt)
            print(f"Sent {'valid' if valid else 'invalid'} packet {i+1}")
            time.sleep(0.5)
