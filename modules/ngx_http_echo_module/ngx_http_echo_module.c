

// Compile: 
// ./configure --prefix=./bin --with-stream --with-debug --add-module=./modules/ngx_http_echo_module/
// make && make install
// @see https://github.com/wujunze/nginx-http-echo-module/blob/master/src/ngx_http_echo_module.c

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// 定义模块配置结构 命名规则为ngx_http_[module-name]_[main|srv|loc]_conf_t。其中main、srv和loc分别用于表示同一模块在三层block中的配置信息。
typedef struct {
    ngx_str_t ed;  //该结构体定义在这里 https://github.com/nginx/nginx/blob/master/src/core/ngx_string.h
} ngx_http_echo_loc_conf_t;

static char *ngx_http_echo(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void *ngx_http_echo_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_echo_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_echo_init(ngx_conf_t *cf);

/**
 * 定义echo模块的指令
 */
static ngx_command_t ngx_http_echo_commands[] = {
        {
            ngx_string("echo"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_http_echo,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_echo_loc_conf_t, ed),
            NULL,
        },
        ngx_null_command,
};


/**
 * 定义ngx_http_module_t类型的结构体变量   
 * 命名规则为ngx_http_[module-name]_module_ctx，这个结构主要用于定义各个Hook函数
 *
 * 可以看到一共有8个Hook注入点，分别会在不同时刻被Nginx调用，
 * 由于我们的模块仅仅用于location域，这里将不需要的注入点设为NULL即可。
 *
 * ngx_http_echo_create_loc_conf  ngx_http_echo_merge_loc_conf 
 * 这两个函数会被Nginx自动调用。注意这里的命名规则：ngx_http_[module-name]_[create|merge]_[main|srv|loc]_conf。
 */
static ngx_http_module_t ngx_http_echo_module_ctx = {
        NULL,                                  /* preconfiguration */
        ngx_http_echo_init,                    /* postconfiguration */
        NULL,                                  /* create main configuration */
        NULL,                                  /* init main configuration */
        NULL,                                  /* create server configuration */
        NULL,                                  /* merge server configuration */
        ngx_http_echo_create_loc_conf,         /* create location configuration */
        ngx_http_echo_merge_loc_conf           /* merge location configuration */
};

/**
 * 组合Nginx Module
 *
 * 上面完成了Nginx模块各种组件的开发下面就是将这些组合起来了。
 * 一个Nginx模块被定义为一个ngx_module_t结构体
 * https://github.com/nginx/nginx/blob/master/src/core/ngx_module.h，
 * 这个结构体的字段很多，不过开头和结尾若干字段一般可以通过Nginx内置的宏去填充
 *
 * 开头和结尾分别用NGX_MODULE_V1和NGX_MODULE_V1_PADDING 填充了若干字段，就不去深究了。
 * 这里主要需要填入的信息从上到下以依次为context、指令数组、
 * 模块类型以及若干特定事件的回调处理函数（不需要可以置为NULL），
 * 其中内容还是比较好理解的，注意我们的echo是一个HTTP模块，
 * 所以这里类型是NGX_HTTP_MODULE，其它可用类型还有NGX_EVENT_MODULE（事件处理模块）和NGX_MAIL_MODULE（邮件模块）。
 *
 */
ngx_module_t ngx_http_echo_module = {
        NGX_MODULE_V1,
        &ngx_http_echo_module_ctx,             /* module context */
        ngx_http_echo_commands,                /* module directives */
        NGX_HTTP_MODULE,                       /* module type */
        NULL,                                  /* init master */
        NULL,                                  /* init module */
        NULL,                                  /* init process */
        NULL,                                  /* init thread */
        NULL,                                  /* exit thread */
        NULL,                                  /* exit process */
        NULL,                                  /* exi master */
        NGX_MODULE_V1_PADDING
};

/**
 * Handler会接收一个ngx_http_request_t指针类型的参数，这个参数指向一个ngx_http_request_t结构体，此结构体存储了这次HTTP请求的一些信息，这个结构定义在https://github.com/nginx/nginx/blob/master/src/http/ngx_http_request.h中：
 *
 * 第一步是获取模块配置信息，这一块只要简单使用ngx_http_get_module_loc_conf就可以了。
 *
 * 第二步是功能逻辑，因为echo模块非常简单，只是简单输出一个字符串，所以这里没有功能逻辑代码。
 *
 * 第三步是设置response header。Header内容可以通过填充headers_out实现，我们这里只设置了Content-type和Content-length等基本内容，ngx_http_headers_out_t定义了所有可以设置的HTTP Response Header信息 这个结构体在https://github.com/nginx/nginx/blob/master/src/http/ngx_http_request.h 设置好头信息后使用ngx_http_send_header就可以将头信息输出，ngx_http_send_header接受一个ngx_http_request_t类型的参数。
 *
 * 第四步也是最重要的一步是输出Response body。   Nginx的I/O机制  Nginx允许handler一次产生一组输出，可以产生多次，Nginx将输出组织成一个单链表结构，链表中的每个节点是一个chain_t，定义在https://github.com/nginx/nginx/blob/master/src/core/ngx_buf.h
 * @param r   ngx_http_request_t指针
 * @return
 */
static ngx_int_t
ngx_http_echo_handler(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_buf_t *b;
    ngx_chain_t out;
    ngx_http_echo_loc_conf_t *elcf;
    elcf = ngx_http_get_module_loc_conf(r,ngx_http_echo_module); // "for me"
    if(!(r->method & (NGX_HTTP_HEAD|NGX_HTTP_GET|NGX_HTTP_POST))){
        return NGX_HTTP_NOT_ALLOWED;
    }
    r->headers_out.content_type.len= sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = elcf->ed.len;
    if(r->method == NGX_HTTP_HEAD){
        rc = ngx_http_send_header(r);
        if(rc != NGX_OK){
            return rc;
        }
    }
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if(b == NULL){
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    out.buf = b;
    out.next = NULL;
    b->pos = elcf->ed.data;
    b->last = elcf->ed.data + (elcf->ed.len);
    b->memory = 1;
    b->last_buf = 1;
    rc = ngx_http_send_header(r);
    if(rc != NGX_OK){
        return rc;
    }
    return ngx_http_output_filter(r, &out);
}

/**
 * 参数转化函数
 * @param f
 * @param cmd
 * @param conf
 * @return ngx status code
 */
static char *
ngx_http_echo(ngx_conf_t *cf,ngx_command_t *cmd , void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    //修改核心模块配置(也就是当前location),将其handler替换为我们自己定义的ngx_http_echo_handler
    clcf->handler = ngx_http_echo_handler;
    ngx_conf_set_str_slot(cf,cmd,conf);
    return NGX_CONF_OK;
}

/**
 * 初始化一个配置结构体
 * @param cf
 * @return
 */
static void *
ngx_http_echo_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_echo_loc_conf_t *conf;
    //gx_pcalloc用于在Nginx内存池中分配一块空间，是pcalloc的一个包装
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_echo_loc_conf_t)); 
    if(conf == NULL) {
        return NGX_CONF_ERROR;
    }
    conf->ed.len = 0;
    conf->ed.data = NULL;
    return conf;
}

/**
 * 将其父block的配置信息合并到此结构体 实现了配置的继承
 * @param cf
 * @param parent
 * @param child
 * @return ngx status code
 *
 * ngx_conf_merge_str_value不是一个函数，而是一个宏，
 * 其定义在 https://github.com/nginx/nginx/blob/master/src/core/ngx_conf_file.h#L205中
 */
static char *
ngx_http_echo_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_echo_loc_conf_t *prev = parent;
    ngx_http_echo_loc_conf_t *conf = child;
    ngx_conf_merge_str_value(conf->ed, prev->ed, '"');
    return NGX_CONF_OK;
}

/**
 * init echo模块
 * @param cf
 * @return
 */
static ngx_int_t
ngx_http_echo_init(ngx_conf_t *cf)
{
    return NGX_OK;
}