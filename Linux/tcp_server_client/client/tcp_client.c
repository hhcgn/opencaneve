/*
 * @Author: CanEve
 * @Date: 2023-05-12 22:52:38
 * @LastEditTime: 2023-06-02 05:04:20
 * @FilePath: /undefined/home/kali/cprogrames/hqyj_learn/day5/client/tcp_client.c
 * @Description: 网络客户端
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char **argv){
    if(argc!=3){
        printf("usage: %s <host> <port>\n",argv[0]);
        return -1;
    }
    int socket_fd = socket(AF_INET, SOCK_STREAM,0);//使用ipv4
    if(-1==socket_fd ){
        perror  ("socket");//可同时打印最近的报错信息
        return  -1;
    }
    struct sockaddr_in serveraddress={0};
    serveraddress.sin_family= AF_INET;//ipv4
    serveraddress.sin_addr.s_addr= inet_addr(/*"127.0.0.1"*/argv[1]);
    serveraddress.sin_port=   htons(/*8888*/atoi(argv[2]));//设置为大端（小端：先存储低位，大端：先存储高位,这里统一一下）
    

    int conn_fd=connect(socket_fd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));//连接
    if(-1==conn_fd){
        perror  ("connect");
        return -1;
    }
    while(1){
        printf("input : ");//前部提示
        fflush(stdout);//刷新缓冲区，printf需要\n刷新
        //gets有bug，没有长度限制
        char buf[1024];
        // gets(buf);
        fgets(buf,100,stdin);//获取键盘输入
        write(socket_fd,buf,strlen(buf));//发送
        if(read(socket_fd,buf,sizeof buf)){//接收
            printf(buf);
            fflush(stdout);
        }
        
    }
    close(socket_fd);//关闭socket
    
    return 0;
}
