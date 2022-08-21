import sys
import time
import socket


IP = "183.173.24.61"
PORT = 8897


if __name__ == "__main__":
    barrage_server = socket.socket()

    while True:
        try:
            barrage_server.connect((IP, PORT))
            break
        except socket.error as e:
            print(e)
            time.sleep(1)

    print(f"Connect {IP}:{PORT} successfully.")

    while True:
        try:
            lines = sys.stdin.readlines()
        except KeyboardInterrupt:
            break

        for line in lines:
            barrage_server.send(line.encode("utf-8"))
            time.sleep(0.3)

    barrage_server.close()
    print(f"Disconnect {IP}:{PORT} successfully.")
