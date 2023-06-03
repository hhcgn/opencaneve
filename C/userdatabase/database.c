#include "database.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>


void database_init(MYDATABASE *db)
{
    bzero(db, sizeof(MYDATABASE));
}
void database_print(MYDATABASE *db,int len)
{
    printf("%s\t%s\n", db->user_pass[len].name, db->user_pass[len].pass);
}
void database_save(MYDATABASE *db){
    int i = 0;
    FILE* file = fopen("save.txt", "w");

    //进行判定
    if (file == NULL)
    {
        printf("\033[1;31;40m save fail !\033[0m\n");
        return;
    }
    for(i=0; i<db->lenth; i++)
    {
        
        fputs(db->user_pass[i].name, file);
        fputc('\t', file);
        fputs(db->user_pass[i].pass, file);
        fputc('\n', file);
    }
    fclose(file);
    printf("save successful !\n");
}

void database_show(MYDATABASE *db)
{
    int i = 0;
    for(i=0; i<db->lenth; i++)
    {
        database_print(db,i);
    }
}

void database_insert(MYDATABASE *db, const char *name, const char *pass)
{
    strcpy(db->user_pass[db->lenth].name, name);
    strcpy(db->user_pass[db->lenth].pass, pass);

    db->lenth++;
    //printf("insert successful !\n");
}
void database_delete(MYDATABASE *db, const char *name, const char *pass)
{
    int i=database_select(db,name,pass);
    int j=0;
    if(i>=0){
        for(j=i;j<(db->lenth-1);j++){
            strcpy(db->user_pass[j].name, db->user_pass[j+1].name);
            strcpy(db->user_pass[j].pass, db->user_pass[j+1].pass);
        }
        db->lenth--;
        printf("delete successful !\n");
    }else{
        printf("\033[1;31;40m delete fail! user may not exist or password not correct\033[0m\n");
    }
}
int database_select(MYDATABASE *db, const char *name,const char *pass)
{
    
    int i=0;
    
    for(i=0; i<db->lenth; i++){
        //printf("%d\t%d\n",strlen(db->user_pass[i].name),strlen(db->user_pass[i].pass));
        if((0 == strncmp(db->user_pass[i].name, name, strlen(db->user_pass[i].name)))&&(0 == strncmp(db->user_pass[i].pass, pass, strlen(db->user_pass[i].pass)))){
            return i;
        }
    }

    return -1;
}
void database_update(MYDATABASE *db, const char *name, const char *oldpass,const char *newpass)
{
    int i=database_select(db,name,oldpass);
    if(i>=0){
        strcpy(db->user_pass[i].pass,newpass);
        printf("update successful ! \n");
    }else{
        printf("\033[1;31;40m update fail! user may not exist or password not correct\033[0m\n");
    }
}

void database_help()
{ 
    printf("#####################################################\n");
    printf("#  help: print help message!                        #\n");
    printf("#  insert <username> <passwd>: add a new user       #\n");
    printf("#  delete <username> <passwd>: delete a  user       #\n");
    printf("#  update <username> <oldpasswd> <newpass>          #\n");
    printf("#  select <username> <passwd>                       #\n");
    printf("#  show                      : show all the user    #\n");
    printf("#  save                      : save to txt file     #\n");
    printf("#  exit                      : exit                 #\n");

    printf("#####################################################\n\n");
}
