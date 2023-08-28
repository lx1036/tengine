

# 这个 python 代码不是 connection 复用，是每次都创建新的连接

# import http.client

# # 创建连接
# conn = http.client.HTTPConnection("localhost", 4001)


# for i in range(10):
#     # 发送 GET 请求
#     conn.request("GET", "/")

#     # 获取响应
#     response = conn.getresponse()

#     # 打印响应状态码和内容
#     print("Response status:", response.status)
#     print("Response content:", response.read().decode())




# # 关闭连接
# conn.close()


# 这个代码抓包，也不是TCP链接复用
# import requests

# session = requests.session()

# response1 = session.get("http://localhost:4001")
# print("Response 1 Status:", response1.status_code, response1.content)

# response2 = session.get("http://localhost:4001")
# print("Response 2 Status:", response2.status_code, response2.content)

# session.close()


# 这个是链接复用，但是抓包第二个包，127.0.0.1:4001 会发 reset 包，不知道为啥???
# 所以，模拟链接复用，使用连接池
import socket

# 创建一个 TCP 连接
def create_connection():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("localhost", 4001))
    return s

# 创建一个 TCP 连接池
def create_connection_pool(pool_size):
    return [create_connection() for _ in range(pool_size)]

# 使用 TCP 链接池发送请求
def send_request(connection_pool):
    # 从连接池中取出一个链接
    s = connection_pool.pop()
    # 发送请求
    s.sendall(b"GET / HTTP/1.1\r\nHost: localhost:4001\r\nAccept-Encoding: gzip, deflate\r\nAccept: */*\r\nConnection: keep-alive\r\n\r\n")
    # 接收响应
    response = s.recv(1024)
    # 将链接放回连接池
    connection_pool.append(s)
    return response

# 创建连接池
connection_pool = create_connection_pool(1)

# 发送请求并打印响应
for i in range(2):
    print("Response", i+1, ":", send_request(connection_pool))

