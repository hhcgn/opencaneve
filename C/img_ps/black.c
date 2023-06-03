/*
 * @Author: CanEve
 * @Date: 2023-05-12 00:00:00
 * @LastEditTime: 2023-05-12 21:35:11
 * @FilePath: /io/black.c
 * @Description: 只实现了部分功能，
		命令white <img1> <img2>img1转为白色img2;  
			gray <img1> <img2> 转为黑白; 
			cat <img1> <img2> <img3> img1上半张与img2下半张拼接
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include<stdlib.h>
typedef struct
{
    unsigned char r,g,b;
    /* data */
} COLOR;

typedef struct 
{
    int fd_s,fd_t,fd_1,fd_2,fd_3;
    int w,h;
    long image_size;
    unsigned char *rgb;
    char f1[20],f2[20],f3[20];
}IMAGE;
IMAGE image;

int func=0;

void executeCMD(const char *cmd, char *result)
{
    char buf_ps[1024];
    char ps[1024]={0};
    FILE *ptr;

    strcpy(ps, cmd);

    if((ptr=popen(ps, "r"))!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
           strcat(result, buf_ps);
           if(strlen(result)>1024)
               break;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error", ps);
    }
}
/**
 * @description: 手动设置rgb值
 * @param {unsigned char} *src
 * @param {u_int8_t} r
 * @param {u_int8_t} g
 * @param {u_int8_t} b
 * @return {*}
 */
void sets(unsigned char *src,u_int8_t r,u_int8_t g,u_int8_t b,long imsize){

}
/**
 * @description: 两图像叠加
 * @param {unsigned char} *src1
 * @param {u_int8_t} *src2
 * @return {*}
 */
void image_or(unsigned char *src1,u_int8_t *src2,u_int8_t *tar){
     IMAGE im1,im2,im3;
    int fd_c1= open(src2, O_RDWR, 0666);
    // int fd_c2= open(tar, O_RDWR, 0666);
    if(-1 == fd_c1)
    {
        perror("open");
        return ;
    }
    char args[100]={0};
    int h,w;
    executeCMD("identify 2.bmp",args);
    sscanf(args, "%*s%*s%dx%d", &im1.w, &im1.h);
    long isize=im1.w*im1.h*3;
    im1.image_size=isize;
   im1.rgb=(char*)malloc(isize);
//    im2.rgb=(char*)malloc(isize);
    //  image.color=(char*)malloc(isize/3);
    // lseek(fd_c2, 54, SEEK_SET);
    // read(fd_c2, im1.rgb, im1.image_size);
    lseek(fd_c1, 54, SEEK_SET);
    read(fd_c1, im1.rgb, im1.image_size/2);
    close(fd_c1);
    // close(fd_c2);
    // for(int i=0;i<im1.image_size;i+=3){
    //         if(i<im1.image_size/2){
    //             im3.rgb[i]=im1.rgb[i];
    //             im3.rgb[i+1]=im1.rgb[i+1];
    //             im3.rgb[i+2]=im1.rgb[i+2];
    //         }else{
    //             im3.rgb[i]=im2.rgb[i];
    //             im3.rgb[i+1]=im2.rgb[i+1];
    //             im3.rgb[i+2]=im2.rgb[i+2];
    //         }
    // }
    char cmd[30]={0};
    strcat(cmd,"cp ");
    strcat(cmd,src1);
    strcat(cmd," ");
    strcat(cmd,tar);
    system(cmd);
    sleep(1);
    int fd_tar= open(tar, O_RDWR, 0666);
    if(-1 == fd_tar)
    {
        perror("open");
        return ;
    }
    lseek(fd_tar, 54/*+im1.image_size/2*/, SEEK_SET);
    write(fd_tar,im1.rgb,im1.image_size/2);
    close(fd_tar);
}
/**
 * @description: 半张图像拼接
 * @param {u_int8_t} *up_half
 * @param {u_int8_t} *down_half
 * @return {*}
 */
void image_half(u_int8_t *up_half,u_int8_t *down_half){

}
/**
 * @description: 添加马赛克
 * @param {u_int8_t} *src
 * @param {int} x
 * @param {int} y
 * @param {int} size
 * @return {*}
 */
void image_mosaic(u_int8_t *src,int x,int y,int size){

}
/**
 * @description: 设置为黑白色
 * @param {u_int8_t} *src
 * @return {*}
 */
void set_gray(u_int8_t *src,long isize){
    for(int i=0;i<isize;i+=3){//3
        unsigned char t=0,y;
        y=src[i]*0.114+src[i+1]*0.59+src[i+2]*0.3;
        src[i]=y;
        src[i+1]=y;
        src[i+2]=y;
    }
}
void image_white(u_int8_t *tar,long isize){
    for(int i=0;i<isize;i+=3){//3
        unsigned char t=0,y;
        // y=image.rgb[i]*0.114+src[i+1]*0.59+src[i+2]*0.3;
        image.rgb[i]=254;
        image.rgb[i+1]=254;
        image.rgb[i+2]=254;
    }
    strcpy(image.f3,tar);
    if(tar[0]==0){
        // printf("no tar");
        set_gray(image.rgb,image.image_size);
        lseek(image.fd_s, 54, SEEK_SET);
        write(image.fd_s,image.rgb,image.image_size);
        close(image.fd_s);
    }else{
        // printf("tar");
        close(image.fd_s);
        char cmd[20]={0};
        strcat(cmd,"cp ");
        strcat(cmd,image.f1);
        strcat(cmd," ");
        strcat(cmd,image.f3);
        system(cmd);
        sleep(1);
        image.fd_t= open(image.f3, O_RDWR, 0666);
        if(-1 == image.fd_t)
        {
            perror("open");
            return ;
        }
        set_gray(image.rgb,image.image_size);
        lseek(image.fd_t, 54, SEEK_SET);
        write(image.fd_t,image.rgb,image.image_size);
        close(image.fd_t);
    }

}
void image_help()
{ 
    printf("#####################################################\n");
    printf("#  help: print help message!                        #\n");
    printf("#       #\n");
    printf("#   #\n");
    printf("#            #\n");
    printf("#                  #\n");
    printf("#    #\n");
    printf("#####################################################\n\n");
}
void image_cat(char *src1,char *src2,char *tar ){
    IMAGE im1,im2,im3;
    int fd_c1= open(src2, O_RDWR, 0666);
    // int fd_c2= open(tar, O_RDWR, 0666);
    if(-1 == fd_c1)
    {
        perror("open");
        return ;
    }
    char args[100]={0};
    int h,w;
    char cmd1[30]={0};
    strcat(cmd1,"identify ");
    strcat(cmd1,src1);
    executeCMD(cmd1,args);
    sscanf(args, "%*s%*s%dx%d", &im1.w, &im1.h);
    long isize=im1.w*im1.h*3;
    im1.image_size=isize;
   im1.rgb=(char*)malloc(isize);
//    im2.rgb=(char*)malloc(isize);
    //  image.color=(char*)malloc(isize/3);
    // lseek(fd_c2, 54, SEEK_SET);
    // read(fd_c2, im1.rgb, im1.image_size);
    lseek(fd_c1, 54, SEEK_SET);
    read(fd_c1, im1.rgb, im1.image_size/2);
    close(fd_c1);
    // close(fd_c2);
    // for(int i=0;i<im1.image_size;i+=3){
    //         if(i<im1.image_size/2){
    //             im3.rgb[i]=im1.rgb[i];
    //             im3.rgb[i+1]=im1.rgb[i+1];
    //             im3.rgb[i+2]=im1.rgb[i+2];
    //         }else{
    //             im3.rgb[i]=im2.rgb[i];
    //             im3.rgb[i+1]=im2.rgb[i+1];
    //             im3.rgb[i+2]=im2.rgb[i+2];
    //         }
    // }
    char cmd[30]={0};
    strcat(cmd,"cp ");
    strcat(cmd,src1);
    strcat(cmd," ");
    strcat(cmd,tar);
    system(cmd);
    sleep(1);
    int fd_tar= open(tar, O_RDWR, 0666);
    if(-1 == fd_tar)
    {
        perror("open");
        return ;
    }
    lseek(fd_tar, 54/*+im1.image_size/2*/, SEEK_SET);
    write(fd_tar,im1.rgb,im1.image_size/2);
    close(fd_tar);
}
void image_read(unsigned char *buf,int *fd){
    *fd = open(buf, O_RDWR, 0666);
    if(-1 == *fd)
    {
        perror("open");
        return ;
    }
    // strcpy(image.f1,buf);
    char args[100]={0};
    
    int h,w;
    char cmd[30]={0};
    strcat(cmd,"identify ");
    strcat(cmd,buf);
    executeCMD(cmd,args);
    // sscanf(args, "%*s%*s%dx%d", &w, &h);
    sscanf(args, "%*s%*s%dx%d", &image.w, &image.h);
    printf("weigth :%d\theight %d\n",image.w,image.h);
    long isize=image.w*image.h*3;
    // long isize=image.w*image.h;
    image.image_size=isize;
   image.rgb=(char*)malloc(isize);
    //  image.color=(char*)malloc(isize/3);
    lseek(*fd, 54, SEEK_SET);
    read(*fd, image.rgb, image.image_size);

}
void image_write(char *tar){
    strcpy(image.f3,tar);
    if(tar[0]==0){
        // printf("no tar");
        set_gray(image.rgb,image.image_size);
        lseek(image.fd_s, 54, SEEK_SET);
        write(image.fd_s,image.rgb,image.image_size);
        close(image.fd_s);
    }else{
        // printf("tar");
        close(image.fd_s);
        char cmd[20]={0};
        strcat(cmd,"cp ");
        strcat(cmd,image.f1);
        strcat(cmd," ");
        strcat(cmd,image.f3);
        system(cmd);
        sleep(1);
        image.fd_t= open(image.f3, O_RDWR, 0666);
        if(-1 == image.fd_t)
        {
            perror("open");
            return ;
        }
        set_gray(image.rgb,image.image_size);
        lseek(image.fd_t, 54, SEEK_SET);
        write(image.fd_t,image.rgb,image.image_size);
        close(image.fd_t);
    }
    
}
int main(int argc, char *argv[])
{
while(1)
    {
        printf("IMAGE_TOOL> ");
        fflush(stdout);

        char buf[100] = {0};
        fgets(buf, 100, stdin);
 
        if(0 == strncmp(buf, "help", 4))
        {
            image_help();
        }
        else if(0 == strncmp(buf, "ls", 2))
        {
            system("ls");
            
        }else if(0 == strncmp(buf, "gray", 4)){
           func=1;
            char src[20] = {0};
            char tar[20] = {0};
            sscanf(buf, "%*s%s%s", src,tar);
            strcpy(image.f1,src);
            image_read(src,&image.fd_s);
           
            image_write(tar);
        }else if(0 == strncmp(buf, "cat", 3)){
            func=2;
            char src1[20] = {0};
            char src2[20] = {0};
            char tar[20] = {0};
            sscanf(buf, "%*s%s%s%s", src1,src2,tar);
            image_cat(src1,src2,tar);
            
        }else if(0 == strncmp(buf, "size", 3)){
                char src1[20] = {0};
                sscanf(buf, "%*s%s", src1);
                int fd = open(src1, O_RDWR, 0666);
                    if(-1 == fd)
                    {
                        perror("open");
                        return -1;
                    }
                unsigned char data[54]={0};
                lseek(fd, 0, SEEK_SET);
                read(fd, data, 54);
                for(int i=0;i<54;i++){
                    printf("%x\n",data[i]);
                }
        }else if(0 == strncmp(buf, "white", 4)){
           func=1;
            char src[20] = {0};
            char tar[20] = {0};
            sscanf(buf, "%*s%s%s", src,tar);
            strcpy(image.f1,src);
            image_read(src,&image.fd_s);
           
            image_white(tar,image.image_size);
        }
        else{
            func=0;
        }
        }
    
    if(argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    //1. 打开文件
    int fd = open(argv[1], O_RDWR, 0666);
    if(-1 == fd)
    {
        perror("open");
        return -1;
    }
    char args[100]={0};
    IMAGE image;
    int h,w;
    executeCMD("identify 2.bmp",args);
    // sscanf(args, "%*s%*s%dx%d", &w, &h);
    sscanf(args, "%*s%*s%dx%d", &image.w, &image.h);


// printf("%s\n",strtok(args," "));
// printf("%s\n",args);
printf("weigth :%d\theight %d\n",image.w,image.h);
long isize=image.w*image.h*3;
// long isize=image.w*image.h;
unsigned char *rgb=(char*)malloc(isize);
//  image.color=(char*)malloc(isize/3);
lseek(fd, 54, SEEK_SET);
read(fd, rgb, isize);
// printf("%ld\n",isize);

// for(int i=0;i<isize;i+=3){//3
//     unsigned char t=0;
//     // printf("S%d",rgb[i]);
//     // printf("M%d",rgb[i+1]);
//     // printf("Y%d\t",rgb[i+2]);

//     t= rgb[i];
//     rgb[i]=rgb[i+2];
//     rgb[i+2]=t;
//     //  t= image.color[i].b;
//     // image.color[i].b=image.color[i].r;
//     // image.color[i].r=t;
   
// }
set_gray(rgb,isize);

lseek(fd, 54, SEEK_SET);
write(fd, rgb,isize);
    //2. 写入hello
/*    lseek(fd, 54, SEEK_SET);
    
    char buf[640*480*3/2] = {0};
    printf("%d\n", write(fd, buf, sizeof(buf)));
*/
    //3. 关闭文件
    close(fd);

    return 0;
}
