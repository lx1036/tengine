


import socket
from ssl import SSLContext,create_default_context,CERT_NONE
import os

HOST = 'localhost'
PORT = 5005

# 创建基础 TCP 套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 创建 SSL 套接字包装基础套接字
ca_pem = os.path.abspath("{}/../../ssl/ca.pem".format(os.path.dirname(__file__)))
sslcontext = create_default_context(cafile=ca_pem)
# sslcontext.verify_mode = CERT_NONE
alpn_protocols = ['http/1.1']
sslcontext.set_alpn_protocols(alpn_protocols)
ssl_sock = sslcontext.wrap_socket(sock, server_hostname="127.0.0.1")

# sslcontext.verify_mode(CERT_NONE)
# 连接服务器
ssl_sock.connect((HOST, PORT))

# 发送数据
ssl_sock.sendall(b'hello server!!!')
# ssl_sock.sendall(b'GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n')

# 接收响应数据
response = b''
while True:
    data = ssl_sock.recv(4096)
    if not data:
        break
    response += data

# 打印响应数据
print(response.decode())

# 关闭连接
ssl_sock.close()
