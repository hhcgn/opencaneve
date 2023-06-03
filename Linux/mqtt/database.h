/*
 * @Author: CanEve
 * @Date: 2023-05-13 04:52:52
 * @LastEditTime: 2023-05-14 02:39:36
 * @FilePath: /day5/server/database.h
 * @Description: 
 */
/*===============================================
*   文件名称：database.h
*   创 建 者：     
*   创建日期：2023年05月09日
*   描    述：
================================================*/
#ifndef MD
#define MD

#define MAXSIZE 100

typedef struct{
    struct{
        char name[20];
        char pass[20];
    }user_pass[MAXSIZE];
    int lenth;
    char msg_back[1024];
}MYDATABASE;

void database_help(MYDATABASE *db);
void database_init(MYDATABASE *db);
void database_show(MYDATABASE *db);
int database_insert(MYDATABASE *db, const char *name, const char *pass);
int database_delete(MYDATABASE *db, const char *name, const char *pass);
int database_update(MYDATABASE *db, const char *name,  const char *oldpass,const char *newpass);
int database_select(MYDATABASE *db, const char *name, const char *pass);
void database_print(MYDATABASE *db,int len);
int database_save(MYDATABASE *db);
int run_database(char *buf,int client,    MYDATABASE *db);
void send_client(char *dat);
int read_line(char *path,MYDATABASE *db);
void data_clear(MYDATABASE *db);

#endif
