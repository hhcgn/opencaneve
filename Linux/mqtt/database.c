#include "database.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>
// #define printf send_client

void data_clear(MYDATABASE *db){
	int ix=0;
    for(;ix<1024;ix++){
    	db->msg_back[ix]=0;
    }
}

void database_init(MYDATABASE *db)
{
    bzero(db, sizeof(MYDATABASE));
}

void database_print(MYDATABASE *db,int len)
{
    // printf("%s\t%s\n", db->user_pass[len].name, db->user_pass[len].pass);
    // printf("%d\n",db->lenth);
    strcat(db->msg_back,db->user_pass[len].name);
    strcat(db->msg_back,"\t");
    strcat(db->msg_back,db->user_pass[len].pass);
    strcat(db->msg_back,"\n");

    
    // send_client(db->msg_back);
}
int database_save(MYDATABASE *db){
    int i = 0;
    FILE* file = fopen("save.txt", "w");

    //进行判定
    if (file == NULL)
    {
       
        return -1;
    }
    for(i=0; i<db->lenth; i++)
    {
        
        fputs(db->user_pass[i].name, file);
        fputc('\t', file);
        fputs(db->user_pass[i].pass, file);
        fputc('\n', file);
    }
    fclose(file);
    
    return 0;
}

void database_show(MYDATABASE *db)
{
    int i = 0;
    for(i=0; i<db->lenth; i++)
    {
        database_print(db,i);
    }
    //printf("%s",db->msg_back);
    send_client(db->msg_back);
}

int database_insert(MYDATABASE *db, const char *name, const char *pass)
{
    if(db->lenth<MAXSIZE-1){
        strcpy(db->user_pass[db->lenth].name, name);
        strcpy(db->user_pass[db->lenth].pass, pass);
        db->lenth++;
        database_save(db);
        return 0;
    }else{
        return -1;
    }
    
    //printf("insert successful !\n");
}
int database_delete(MYDATABASE *db, const char *name, const char *pass)
{
    int i=database_select(db,name,pass);
    int j=0;
    if(i>=0){
        for(j=i;j<(db->lenth-1);j++){
            strcpy(db->user_pass[j].name, db->user_pass[j+1].name);
            strcpy(db->user_pass[j].pass, db->user_pass[j+1].pass);
        }
        db->lenth--;
        database_save(db);
        return 0;
        
    }else{
        return -1;
        
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
int database_update(MYDATABASE *db, const char *name, const char *oldpass,const char *newpass)
{
    int i=database_select(db,name,oldpass);
    if(i>=0){
        strcpy(db->user_pass[i].pass,newpass);
       database_save(db);
        return 0;
    }else{
        
        return -1;
    }
}

void database_help(MYDATABASE *db)
{ 
    //printf("#####################################################\n");
    //printf("#  help: print help message!                        #\n");
    //printf("#  insert <username> <passwd>: add a new user       #\n");
    //printf("#  delete <username> <passwd>: delete a  user       #\n");
    //printf("#  update <username> <oldpasswd> <newpass>          #\n");
    //printf("#  select <username> <passwd>                       #\n");
    //printf("#  show                      : show all the user    #\n");
    //printf("#  save                      : save to txt file     #\n");
    //printf("#  exit                      : exit                 #\n");
    //printf("#####################################################\n\n");
    db->msg_back[0]=0;
    strcat(db->msg_back,"#####################################################\n");
    strcat(db->msg_back,"#  help: print help message!                        #\n");
    strcat(db->msg_back,"#  insert <username> <passwd>: add a new user       #\n");
    strcat(db->msg_back,"#  delete <username> <passwd>: delete a  user       #\n");
    strcat(db->msg_back,"#  update <username> <oldpasswd> <newpass>          #\n");
    strcat(db->msg_back,"#  select <username> <passwd>                       #\n");
    strcat(db->msg_back,"#  show                      : show all the user    #\n");
    // strcat(db->msg_back,);

}
#define LINE_MAX 1024
int read_line(char *path,MYDATABASE *db)
{
	FILE *fp;
	int line_num = 0; // 文件行数
	int line_len = 0; // 文件每行的长度
	char buf[LINE_MAX] = {0}; // 行数据缓存

	fp = fopen(path, "r+");
	if (NULL == fp) {
		printf("open %s failed.\n", path);
		
		return -1;
	}

	while(fgets(buf, LINE_MAX, fp)) {
		line_num++;
		line_len = strlen(buf);
		// 排除换行符
		if ('\n' == buf[line_len - 1]) {
			buf[line_len - 1] = '\0';
			line_len--;
			if (0 == line_len) {
				//空行
				continue;
			}
		}
		// windos文本排除回车符
		if ('\r' == buf[line_len - 1]) {
			buf[line_len - 1] = '\0';
			line_len--;
			if (0 == line_len) {
				//空行
				continue;
			}
		}
		// printf("%s\n", buf);
         sscanf(buf,"%s%s",db->user_pass[line_num-1].name,db->user_pass[line_num-1].pass);
         //printf("name %s pass : %s\n",db->user_pass[line_num-1].name,db->user_pass[line_num-1].pass);
         if(db->user_pass[line_num-1].name !=0)
         db->lenth=line_num;
		/** 对每行数据(buf)进行处理 **/
	}

	if (0 == feof) {
		// 未读到文件末尾
		//printf("fgets error\n");	
		return -1;
	}
	fclose(fp);

	return line_num;
}
