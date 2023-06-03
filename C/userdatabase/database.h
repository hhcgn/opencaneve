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
}MYDATABASE;

void database_help();
void database_init(MYDATABASE *db);
void database_show(MYDATABASE *db);
void database_insert(MYDATABASE *db, const char *name, const char *pass);
void database_delete(MYDATABASE *db, const char *name, const char *pass);
void database_update(MYDATABASE *db, const char *name,  const char *oldpass,const char *newpass);
int database_select(MYDATABASE *db, const char *name, const char *pass);
void database_print(MYDATABASE *db,int len);
void database_save(MYDATABASE *db);

#endif
