import os
import socket


def send(SERVER_SOCKET_NAME):
    # Must match the C string exactly, starting with a null byte
    ABSTRACT_SOCKET = "\0" + SERVER_SOCKET_NAME

    client_fd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
        print("Connecting to Linux abstract socket...")
        client_fd.connect(ABSTRACT_SOCKET)

        # Send a large 4MB payload
        # Recommended practice: use 'with' to ensure the file closes automatically
        with open("/home/ooh/Projects/MOC/maths/basic.math", "rb") as file:
            large_message = file.read()

        print(f"Sending payload ({len(large_message)} bytes)...")

        client_fd.sendall(large_message)

    finally:
        print("Closing connection.")
        client_fd.close()
