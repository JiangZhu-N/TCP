# TCP

协议：

	一组规则。

分层模型结构： 

	OSI七层模型 ： 物  数  网  传  会  表  应
	TCP/IP四层模型 ： 网(链)  网  传  应
		应用层：http  ftp  nfs  ssh  telnet
		传输层：TCP  UDP
		网络层：IP ICMP IGMP
		链路层：以太网帧协议  ARP

C/S模型：

	client-server
B/S模型：

	browser-server

C/S与B/S比较：

			C/S  			B/S
	优点	缓存大量数据，协议	   	安全，跨平台，开发工作量小
		选择灵活，速度快
	缺点	安全性不足，不能跨平台    	 不能缓存大量数据，严格遵守http

网络传输流程：

	数据没有封装前不能在网络中传递

以太网帧协议：

	ARP协议：根据IP地址获取mac地址
	以太网帧协议：根据mac地址完成数据包传递

IP协议：

	版本：IPv4  IPv6
	TTL：设置数据包在路由节点中跳转上限，每经过一个路由节点，该值减一，减为零的路由，有义务将该数据包丢弃

	源IP：32位(4byte)
		↓
	192.168.1.108  点分十进制IP地址(string)
		↓
	     二进制
		↓
	目的IP：32位(4byte)

	IP地址：可以在网络环境中唯一标识一台主机
	端口号：可以在网络中的一台主机上唯一标识一个进程
	IP地址+端口号：在网络环境中唯一标识一个进程

UDP：

	16位源端口号  16位目的端口号

网络套接字：	socket

	一个文件描述符指向一个套接字（套接字内部由内核借助两个缓冲区实现）
	通信过程中，套接字成对出现

网络字节序：

	小端法：（PC本地存储）	高位存高地址，低位存低地址
	大端法：（网络存储）	高位存低地址，低位存高地址

	htonl：本地→网络（IP）
	htons：本地→网络（port）
	ntohl：网络→本地（IP）
	ntohs：网络→本地（port）

IP地址转换函数：

	int inet_pton(int af,const char *src,void *dst);	
	本地字节序(string)→网络字节序
	af:  AF_INET  AF_INET6
	src:(传入)点分十进制IP地址(string IP)
	dst:(传出)网络字节序IP地址
	返回值：
		成功：1
		异常：0（src指向的不是一个有效的地址）
		失败：-1
 
 
	const char *inet_ntop(int af,const void *src,char *dst,socklen_t size);
	af:  AF_INET  AF_INET6
	src：网络字节序IP地址
	dst：本地字节序IP地址（string IP）
	size：dst的大小
	返回值：
		成功：dst
		失败：NULL

sockadder地址结构：

	struct sockaddr_in addr;
	addr.sin_family=AF_INET/AF_INET6
	addr.sin_port=htons(0~65535)
	addr.sin_addr.s_addr=htonl(INADDR_ANY);

	bind(fd,(struct sockaddr *)&addr,size);

socket函数：

	#include<sys/socket.h>


	int socket(int domain,int type,int protocol);	创建一个套接字
		domain:  AF_INET  AF_INET6  AF_UNIX
		type:  SOCK_STREAM  SOCK_DGRAM
		protocol:  0
		返回值：
			成功：新套接字所对应的文件描述符
			失败：-1 errno


	int bind(int sockfd,const struct sockaddr *addr,socklen_t size);	给socket绑定一个地址结构(IP+port)
		sockfd:socket返回值
			struct sockaddr_in addr;
			addr.sin_family=AF_INET;
			addr.sin_port=htons(0~65535)
			addr.sin_addr.s_addr=htonl(INADDR_ANY);
		addr:(struct sockaddr *)&addr
		addrlen:sizeof(addr);	地址结构的大小
		返回值：
			成功：0
			失败：-1 errno


	int listen(int sockfd,int backlog);	设置同时与服务器建立连接的上限数（同时进行三次握手的客户端数量）
		sockfd:socket的返回值
		backlog:上限数值（最大128）
		返回值：
			成功：0
			失败：-1 errno


	int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);	阻塞等待客户端连接，成功则返回一个与客户端成功连接的socket的文件描述符
		sockfd:socket的返回值
		addr:传出参数  成功与服务器建立连接的客户端的地址结构（IP + port）
			socklen_t clit_addr_len = sizeof(addr)
		addrlen:传入传出  &clit_addr_len 
			入：addr的大小		出：客户端addr实际大小
		返回值：
			成功：能与服务器进行数据通信的socket的文件描述符
			失败：-1 errno
			
			
	int connect(int sockfd,const struct sockaddr *addr,socklen_t addrlen);	使现有的socket与服务器建立连接
		sockfd: socket函数返回值
		addr:传入参数  服务器的地址结构
		adrlen:服务器的地址结构大小
		返回值：
			成功：0
			失败：-1 errno
		如果不使用bind绑定客户端地址结构，则采用“隐式绑定”
	
TCP通信流程分析：

	server： 
		1.socket()	创建socket
		2.bind()	绑定服务器地址结构
		3.listen()	设置监听上限
		4.accept()	阻塞监听客户端连接
		5.read(fd)	读socket获取客户端数据
		6.代码
		7.write(fd)	
		8.close()
	
	client:
		1.socket()	创建socket
		2.connect()	与服务器建立连接
		3.write()	写数据到socket
		4.read()	读转换后的数据
		5.代码
		6.close()

