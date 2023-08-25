


from http.server import BaseHTTPRequestHandler, HTTPServer

# 定义处理请求的类
class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # 设置响应状态码和头部信息
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

        # 响应内容
        response = "Hello, client!"
        self.wfile.write(response.encode())

# 启动服务器
def main():
    host = 'localhost'
    port = 4002
    server = HTTPServer((host, port), RequestHandler)
    print(f"Server running on {host}:{port}")

    server.serve_forever()

if __name__ == '__main__':
    main()
