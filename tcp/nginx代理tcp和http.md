
# nginx 代理测试



http client -> tcp nginx -> tcp server: not ok，必然不可以的，tcp server 发的包 http client 无法解析



tcp client(10次请求) -> tcp nginx(1次日志) -> tcp server(10次日志): ok



目前模拟的 python 代码不正确
http client(10次请求) -> tcp nginx(1次日志) -> http server(10次日志): ok,期望应该是1次日志
http client(10次请求) -> http nginx(10次日志) -> http server(10次日志): ok


tcp client -> tcp nginx -> tcp nginx -> tcp server: 



