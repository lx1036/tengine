
import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 5001
MESSAGE = "Hello, UDP Server!"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
print("Message sent to {}:{}".format(UDP_IP, UDP_PORT))


data = sock.recv(1024)
# data, addr = sock.recvfrom(1024)
# print("Received response from {}: {}".format(addr, data.decode()))
print("Received response from: {}".format( data.decode()))

sock.close()
