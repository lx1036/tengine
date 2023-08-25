


import http.client

# 创建连接
conn = http.client.HTTPConnection("localhost", 4001)


for i in range(10):
    # 发送 GET 请求
    conn.request("GET", "/")

    # 获取响应
    response = conn.getresponse()

    # 打印响应状态码和内容
    print("Response status:", response.status)
    print("Response content:", response.read().decode())




# 关闭连接
conn.close()



