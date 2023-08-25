import socket


# 服务器的 IP 地址和端口号
host = '127.0.0.1'
port = 5001

# 创建 TCP 客户端套接字
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # 连接到服务器
    client_socket.connect((host, port))
    print(f"已连接到服务器 {host}:{port}")

    for i in range(10):
        # 发送数据到服务器
        message = "Hello, Server!"
        client_socket.send(message.encode())

        # 接收服务器的响应数据
        response = client_socket.recv(1024)
        print(f"收到服务器的响应: {response.decode()}")

except socket.error as e:
    print(f"与服务器 {host}:{port} 的连接发生错误: {e}")

finally:
    # 关闭客户端套接字
    client_socket.close()
