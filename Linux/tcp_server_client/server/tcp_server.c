/*
 * @Author: CanEve
 * @Date: 2023-05-13 03:24:43
 * @LastEditTime: 2023-06-02 05:32:28
 * @FilePath: /day5/server/tcp_server.c
 * @Description: tcp server
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include "database.h"
#include <string.h>
#include <stdlib.h>
int main(int argc, char **argv){
    
    if(argc!=2){
        printf("usage: %s  <port>\n",argv[0]);
        return -1;
    }
    // MYDATABASE db;
    // database_init(&db);
    // read_line("save.txt",&db);
    int socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if(-1==socket_fd ){
        perror  ("socket");
        return  -1;
    }
    struct sockaddr_in serveraddress={0},clientaddr={0};
    serveraddress.sin_family= AF_INET;//ipv4
    serveraddress.sin_addr.s_addr= inet_addr("0.0.0.0");//0.0.0.0是监听所有ip
    serveraddress.sin_port=   htons(atoi(argv[1]));//设置为大端（小端：先存储低位，大端：先存储高位）
    
    
    int conn_fd=bind(socket_fd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));//绑定ip和端口
    if(-1==conn_fd){
        perror  ("connect");
        return -1;
    }
    listen(socket_fd,10);//可接受10个（但不能同时连接）
    printf("Server listening on\n");
    while(1){//多次连接
        int client=accept(socket_fd,NULL,NULL/*(struct sockaddr *)&clientaddr,sizeof clientaddr*/);//等待客户端接入
        printf("Client listening on\n");
        char buf[1024];
        while (1)
        {
           memset(buf,0,1024);
            if(read(client,buf,1024)){
                printf("rec -> %s\n",buf);
                if(write(client,buf,strlen(buf))<=0){//返回给客户端
                    break;//客户端断开后等待下一个连接
                }   
                // run_database(buf,client,&db);
            }else{
                break;//客户端断开后等待下一个连接
            }
           
        }
        close(client);
    }
    close(socket_fd);
        //write(client,buf,strlen(buf));
        // char repeat[1024]={"hello\n"};
        // write(client,repeat,6);
    

    return 0;
}
