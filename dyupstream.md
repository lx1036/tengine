

# 安装 http_lua_module
```shell
# 先安装 luajit
brew install openresty/brew/openresty
brew install lua@5.1

# 编译 http_lua_module
./configure --prefix=./bin --with-stream --with-debug --add-module=./modules/ngx_http_echo_module/ --add-module=./modules/ngx_http_curl_module/ --with-http_lua_module --with-luajit-inc=/usr/local/Cellar/openresty/1.21.4.2_1/luajit/include/luajit-2.1 --with-luajit-lib=/usr/local/Cellar/openresty/1.21.4.2_1/luajit/lib
make && make install

```




https://tengine.taobao.org/document_cn/http_dyups_cn.html

编译包含 module:

```
# 添加 stream TCP/UDP 四层编译
./configure --prefix=./bin --add-module=modules/ngx_http_upstream_dyups_module --with-stream --with-debug
make && make install

./bin/sbin/nginx -c conf/nginx-dyup.conf -p .
./bin/sbin/nginx -c conf/proxy-protocol.conf -p .
```


# 本地运行 ./objs/nginx -c ./conf/nginx.conf -p .

```
#user  nobody;
worker_processes  1;

daemon off;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


error_log logs/error.log debug;


http {

    # dyups_upstream_conf  upstream.conf;

    include upstream.conf;

    server {
        listen   9090;

        location / {
            proxy_pass http://$host;
        }
    }

    server {
        listen 8088;
        location / {
            # echo 8088;
            root   html;
            index  index.html;
        }
    }

    server {
        listen 8089;
        location / {
            # echo 8089;
            root   html;
            index  index2.html;
        }
    }

    server {
        listen 9091;
        location / {
            dyups_interface;
        }
    }
}


upstream host1 {
    server 127.0.0.1:8088;
}

upstream host2 {
    server 127.0.0.1:8089;
}
```

然后运行测试：

```
# 目前没有 dyhost upstream
curl -H "Host: dyhost" localhost:9090

# 增加
curl -d "server 127.0.0.1:8089;server 127.0.0.1:8088;" 127.0.0.1:9091/upstream/dyhost

curl localhost:9091/detail
curl -H "Host: host2" 127.0.0.1:9090
curl -H "Host: dyhost" 127.0.0.1:9090
curl -i -X DELETE 127.0.0.1:9091/upstream/host1
```
