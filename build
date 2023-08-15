


# --with-stream_ssl_module
# --with-pcre --with-pcre=/usr/local/Cellar/pcre/8.45 \
# --with-pcre-opt="-L/usr/local/Cellar/pcre/8.45/lib -lpcre" 这个配置貌似没有配置正确

# --add-module=./modules/ngx_http_curl_module/ \ 这个模块总是导致 worker 进程重启

./configure --prefix=./bin --with-stream --with-stream_ssl_module --with-debug --with-pcre  \
--add-module=./modules/ngx_http_echo_module \
--add-module=./modules/ngx_stream_lua_module_0.0.13 \
--add-module=./modules/ngx_http_upstream_dyups_module \
--with-http_lua_module \
--with-luajit-inc=/usr/local/Cellar/openresty/1.21.4.2_1/luajit/include/luajit-2.1 \
--with-luajit-lib=/usr/local/Cellar/openresty/1.21.4.2_1/luajit/lib \
--with-lua-inc=/usr/local/Cellar/lua@5.1/5.1.5_8/include/lua-5.1 \
--with-lua-lib=/usr/local/Cellar/lua@5.1/5.1.5_8/lib

make && make install
