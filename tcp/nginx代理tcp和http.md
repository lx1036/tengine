
# nginx 代理测试



http client -> tcp nginx -> tcp server: not ok，必然不可以的，tcp server 发的包 http client 无法解析



tcp client(10次请求) -> tcp nginx(1次日志) -> tcp server(10次日志): ok




http client(10次请求) -> tcp nginx(0次日志) -> http server(10次日志): ok

http client(10次请求) -> http nginx(10次日志) -> http server(10次日志): ok


tcp client -> tcp nginx -> tcp nginx -> tcp server: 



