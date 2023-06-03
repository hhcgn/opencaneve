/*
 * @Author: CanEve
 * @Date: 2023-05-10 20:03:32
 * @LastEditTime: 2023-05-10 20:28:58
 * @FilePath: /undefined/home/kali/cprogrames/hqyj_learn/day4/mydatabase/mydatabase.c
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

int main(int argc, char *argv[])
{
    MYDATABASE db;
    database_init(&db);

    while(1)
    {
        printf("DATABASE> ");
        fflush(stdout);

        char buf[100] = {0};
        fgets(buf, 100, stdin);
 
        if(0 == strncmp(buf, "help", 4))
        {
            database_help();
        }
        else
        if(0 == strncmp(buf, "show", 4))
        {
            database_show(&db);
        }
        else
        if(0 == strncmp(buf, "insert", 6))
        {
            char name[20] = {0};
            char pass[20] = {0};
            sscanf(buf, "%*s%s%s", name, pass);
            database_insert(&db, name, pass);
        }
        else if(0 == strncmp(buf, "delete", 6)){
            char name[20] = {0};
            char pass[20] = {0};
            sscanf(buf, "%*s%s%s", name, pass);
            database_delete(&db, name, pass);
        }
        else if(0 == strncmp(buf, "select", 6)){
            char name[20] = {0};
            char pass[20] = {0};
            sscanf(buf, "%*s%s%s", name,pass);
            int pos=database_select(&db, name,pass);
            if(pos>=0){
                database_print(&db,pos);
            }else{
                printf("\033[1;31;40mnot found !\033[0m\n");
            }
        }
        else if(0 == strncmp(buf, "update", 6)){
            char name[20] = {0};
            char oldpass[20] = {0};
            char newpass[20] = {0};
            sscanf(buf, "%*s%s%s%s", name,oldpass,newpass);
            database_update(&db,name,oldpass,newpass);
        }else if(0 == strncmp(buf, "exit", 4)){
            printf("BYE !\n");
            return 0;
        }
        else if(0 == strncmp(buf, "save", 4)){
            database_save(&db);
        }
        else
            printf("\033[1;31;40mcmd err! try help!\033[0m\n");

    }


    return 0;
} 
