/*
 * @Author: CanEve
 * @Date: 2023-05-10 20:03:32
 * @LastEditTime: 2023-05-14 02:13:27
 * @FilePath: /day5/server/mydatabase.c
 * @Description: 
 */
/*===============================================
*   文件名称：mydatabase.c
*   创 建 者：     
*   创建日期：2023年05月09日
*   描    述：
================================================*/
#include <stdio.h>
#include <string.h>
#include "database.h"
#include <unistd.h>

int client_s;
void send_client(char *dat){
    // puts(dat);
    
    write(client_s,dat,strlen(dat));
    
}
int run_database(char *buf,int client,    MYDATABASE *db)
{
    strcpy(db->msg_back,"");
    client_s=client;
    if(buf[0]==0) return -1;

    // while(1)
    // {
        // printf("DATABASE> ");
        // fflush(stdout);

        // char buf[100] = {0};
        // fgets(buf, 100, stdin);
 
        if(0 == strncmp(buf, "help", 4))
        {
            database_help(db);
            send_client(db->msg_back);
        }
        else
        if(0 == strncmp(buf, "show", 4))
        {
            database_show(db);
        }
        else
        if(0 == strncmp(buf, "insert", 6))
        {
            char name[20] = {0};
            char pass[20] = {0};
            sscanf(buf, "%*s%s%s", name, pass);
            if(database_insert(db, name, pass)!=-1){
                send_client("insertok\n");
            }else{
                 send_client("insertfail\n");
            }
        }
        else if(0 == strncmp(buf, "delete", 6)){
            char name[20] = {0};
            char pass[20] = {0};
            sscanf(buf, "%*s%s%s", name, pass);
            if(database_delete(db, name, pass)!=-1){
                //printf("delete successful !\n");
                send_client("deleteok!\n");
            }else{
                //printf("delete fail! user may not exist or password not correct\n");
                send_client("deletefail!\n");
            }
        }
        else if(0 == strncmp(buf, "select", 6)){
            char name[20] = {0};
            char pass[20] = {0};
            sscanf(buf, "%*s%s%s", name,pass);
            int pos=database_select(db, name,pass);
            if(pos>=0){
                //printf("find it !\n");
                send_client("selectok\n");
                database_print(db,pos);
            }else{
                //printf("not found !\n");
                send_client("selectfail!\n");
            }
        }
        else if(0 == strncmp(buf, "update", 6)){
            char name[20] = {0};
            char oldpass[20] = {0};
            char newpass[20] = {0};
            sscanf(buf, "%*s%s%s%s", name,oldpass,newpass);
            if(database_update(db,name,oldpass,newpass)!=-1){
                 //printf("update successful ! \n");
                 send_client("updateok\n");
            }else{
                //printf("update fail! user may not exist or password not correct\n");
                send_client("updatefail\n");
            }
        }
        else if(0 == strncmp(buf, "exit", 4)){
            //printf("BYE !\n");
            send_client("BYE !\n");
            return 0;
        }
        else if(0 == strncmp(buf, "save", 4)){
            if(database_save(db)!=-1){
                //printf("save successful !\n");
                send_client("save successful !\n");
            }else{
                 //printf("save fail !\n");
                 send_client("save fail !\n");
            }
        }
        else{
            //printf("cmd err! try help!\n");
            send_client("cmd err! try help!\n");
        }
            
    // }


    return 0;
} 
