#!/usr/bin/perl -w
# Filename : server.pl
 
use strict;
use Socket;
 
# 使用端口 7890 作为默认值
my $port = shift || 7890;
my $proto = getprotobyname('tcp');
my $server = "localhost";  # 设置本地地址
 
# 创建 socket, 端口可重复使用，创建多个连接
socket(SOCKET, PF_INET, SOCK_STREAM, $proto)
   or die "无法打开 socket $!\n";
setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR, 1)
   or die "无法设置 SO_REUSEADDR $!\n";
 
# 绑定端口并监听
bind( SOCKET, pack_sockaddr_in($port, inet_aton($server)))
   or die "无法绑定端口 $port! \n";
 
listen(SOCKET, 5) or die "listen: $!";
print "访问启动：$port\n";
 
# 接收请求
my $client_addr;
while ($client_addr = accept(NEW_SOCKET, SOCKET)) {
   # send them a message, close connection
   # my $name = gethostbyaddr($client_addr, AF_INET );
   print NEW_SOCKET "我是来自服务端的信息";
   # print "Connection recieved from $name\n";
   close NEW_SOCKET;
}