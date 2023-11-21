//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_HEADER_H
#define TCGR_HEADER_H


// 网络编程头文件

// Windows环境下头文件
#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
// Linux环境下头文件
#else
#include <cstring>
#include <sys/wait.h>		// Linux进程等待
#include <cerrno>      	// 错误处理
#include <csignal>     	// 信号
#include <sys/types.h>  	// 提供各种数据类型
#include <sys/socket.h> 	// 套结字函数和结构
#include <netinet/in.h> 	// sockaddr_in 结构
#include <arpa/inet.h>  	// IP 地址转换 htons
#include <netdb.h>      	// 域名转换 inet_ntop
#include <sys/ioctl.h>  	// I/O控制的函数
#include <sys/poll.h>   	// socket 等待测试机制
#include <unistd.h>     	// 标准IO，进程等等
#include <pwd.h>        	// 对系统某些文件的访问
#include <sys/epoll.h>		// epoll IO多路复用
#include <sys/stat.h>		// 获取文件信息
#include <fcntl.h>			// open 等关于文件的函数
#include <sys/sendfile.h>	// 零拷贝，在两个内核之间传输数据
#endif

// 跨平台通用头文件
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <cstdio>
#include <memory>
#include <ctime>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <unordered_map>



#endif //TCGR_HEADER_H
