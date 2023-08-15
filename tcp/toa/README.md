


# 源码安装 toa

操作系统：目前验证 ubuntu 20, >= ubuntu 22 以上系统不支持，函数需要适配。

步骤：https://support.huaweicloud.com/intl/en-us/usermanual-elb/en-us_elb_06_0001.html

```shell
sudo apt-get install -y gcc make linux-headers-`uname -r`
git clone https://github.com/Huawei/TCP_option_address.git
cd src
make
# load toa module into kernel
sudo insmod toa.ko
dmesg | grep TOA
lsmod | grep toa
```

永久生效：
```shell
mkdir -p /etc/sysconfig/modules/

touch toa.modules

#!/bin/sh
/sbin/modinfo -F filename /root/toa/toa.ko > /dev/null 2>&1
if [ $? -eq 0 ]; then
/sbin/insmod /root/toa/toa.ko
fi
```

verify:
```shell
# 启动一个 http server
python2 -m SimpleHTTPServer port
# 日志打印 clientIP
192.168.0.90 - - [06/Aug/2020 14:24:21] "GET / HTTP/1.1" 200 –
```

