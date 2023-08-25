

import socket

# 创建 TCP 服务器套接字
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 监听的 IP 地址和端口号
host = '127.0.0.1'
port = 5002

# 绑定 IP 地址和端口号
server_socket.bind((host, port))

# 开始监听
server_socket.listen(1)
print(f"TCP server listening {host}:{port} ...")

while True:
    # 等待客户端的连接
    client_socket, addr = server_socket.accept()
    print(f"与客户端 {addr[0]}:{addr[1]} 建立连接")

    try:
        while True:
            # 接收客户端发送的数据
            data = client_socket.recv(1024)
            if not data:
                break

            # 处理接收到的数据
            # 这里可以根据业务逻辑进行相应的处理
            print("get data from client: {}".format(data))

            # 发送响应给客户端
            response = "Hello from the server!"
            client_socket.send(response.encode())

    except socket.error as e:
        print(f"与客户端 {addr[0]}:{addr[1]} 的连接发生错误: {e}")

    finally:
        # 关闭与客户端的连接
        client_socket.close()

# 关闭服务器套接字
server_socket.close()
