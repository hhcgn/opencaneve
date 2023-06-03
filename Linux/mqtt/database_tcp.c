/*
 * @Author: CanEve
 * @Date: 2023-05-13 03:24:43
 * @LastEditTime: 2023-05-14 02:39:45
 * @FilePath: /day5/server/tcp_server.c
 * @Description: tcp server
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "database.h"
#include <string.h>
int database_start(){
    
    printf("database_start!\n");
    

    MYDATABASE db;
    database_init(&db);
    read_line("save.txt",&db);
    int socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if(-1==socket_fd ){
        perror  ("socket");
        return  -1;
    }
    struct sockaddr_in serveraddress={0},clientaddr={0};
    serveraddress.sin_family= AF_INET;
    serveraddress.sin_addr.s_addr= inet_addr("0.0.0.0");//0.0.0.0all
    serveraddress.sin_port=   htons(8888);//设置为大端（小端：先存储低位，大端：先存储高位）
    
    
    int conn_fd=bind(socket_fd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));
    if(-1==conn_fd){
        perror  ("connect");
        return -1;
    }

    listen(socket_fd,10);
    printf("Server listening on\n");
    while(1){
    int client=accept(socket_fd,NULL,NULL/*(struct sockaddr *)&clientaddr,sizeof clientaddr*/);
    printf("Client listening on\n");
     char buf[1024];
    while (1)
    {
    int i=0;
        for(;i<1024;i++){
            buf[i]=0;
        }
        if(read(client,buf,1024)>0){
            printf("rec -?> %s\n",buf);
            run_database(buf,client,&db);
        }else{
            break;
        }
        data_clear(&db);
        
            
        
        
        
        //write(client,buf,strlen(buf));
        
        //
        
        // char repeat[1024]={"hello\n"};
        // write(client,repeat,6);
        /* code */
    }
    }
    

    return 0;
}
