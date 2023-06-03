/*
 * @Author: CanEve 2785546297@qq.com
 * @Date: 2023-06-02 00:55:19
 * @LastEditors: CanEve 2785546297@qq.com
 * @LastEditTime: 2023-06-02 00:56:45
 * @FilePath: \undefinedd:\Git\GIT_UPDATE\opencaneve\opencaneve\Linux\tcp_cam\cam.c
 * @Description: Linux tcp_cam 默认是使用dev/video0,端口为6666
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/
#include<string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define	REQBUFS_COUNT	4

struct cam_buf {
	void *start;
	size_t length;
};

struct v4l2_requestbuffers reqbufs;
struct cam_buf bufs[REQBUFS_COUNT];

int camera_init(char *devpath, unsigned int *width, unsigned int *height, unsigned int *size, unsigned int *ismjpeg)
{
	int i;
	int fd = -1;;
	int ret;
	struct v4l2_buffer vbuf;
	struct v4l2_format format;
	struct v4l2_capability capability;
	/*open 打开设备文件*/
	if((fd = open(devpath, O_RDWR)) == -1){
		perror("open:");	
		return -1;		
	}
	/*ioctl 查看支持的驱动*/
	ret = ioctl(fd, VIDIOC_QUERYCAP, &capability);
	if (ret == -1) {
		perror("camera->init");
		return -1;
	}
	/*判断设备是否支持视频采集*/
	if(!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "camera->init: device can not support V4L2_CAP_VIDEO_CAPTURE\n");
		close(fd);
		return -1;
	}
	/*判断设备是否支持视频流采集*/
	if(!(capability.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "camera->init: device can not support V4L2_CAP_STREAMING\n");
		close(fd);
		return -1;
	}
	/*设置捕获的视频格式 MYJPEG*/
	memset(&format, 0, sizeof(format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = *width;
	format.fmt.pix.height = *height;
	format.fmt.pix.field = V4L2_FIELD_ANY;
	ret = ioctl(fd, VIDIOC_S_FMT, &format);
	if(ret == -1) {
		perror("camera init");
		return -1;
	} else {
		*ismjpeg = 0;
		fprintf(stdout, "camera->init: picture format is mjpeg\n");
		goto get_fmt;
	}
	/*设置捕获的视频格式 YUYV*/
	memset(&format, 0, sizeof(format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;			//永远都是这个类型
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;		//设置采集图片的格式
	format.fmt.pix.width = *width;
	format.fmt.pix.height = *height;
	format.fmt.pix.field = V4L2_FIELD_ANY;				//设置图片一行一行的采集
	ret = ioctl(fd, VIDIOC_S_FMT, &format);				//ioctl	是设置生效
	if(ret == -1)
		perror("camera init");
	else {
		fprintf(stdout, "camera->init: picture format is yuyv\n");
		*ismjpeg = 1;
		goto get_fmt;
	}

get_fmt:
	ret = ioctl(fd, VIDIOC_G_FMT, &format);
	if (ret == -1) {
		perror("camera init");
		return -1;
	}
	/*向驱动申请缓存*/
	memset(&reqbufs, 0, sizeof(struct v4l2_requestbuffers));
	reqbufs.count	= REQBUFS_COUNT;					//缓存区个数
	reqbufs.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbufs.memory	= V4L2_MEMORY_MMAP;					//设置操作申请缓存的方式:映射 MMAP
	ret = ioctl(fd, VIDIOC_REQBUFS, &reqbufs);			
	if (ret == -1) {	
		perror("camera init");
		close(fd);
		return -1;
	}
	/*循环映射并入队*/
	for (i = 0; i < reqbufs.count; i++){
		/*真正获取缓存的地址大小*/
		memset(&vbuf, 0, sizeof(struct v4l2_buffer));
		vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vbuf.memory = V4L2_MEMORY_MMAP;
		vbuf.index = i;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &vbuf);
		if (ret == -1) {
			perror("camera init");
			close(fd);
			return -1;
		}
		/*映射缓存到用户空间,通过mmap讲内核的缓存地址映射到用户空间,并切和文件描述符fd相关联*/
		bufs[i].length = vbuf.length;
		bufs[i].start = mmap(NULL, vbuf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, vbuf.m.offset);
		if (bufs[i].start == MAP_FAILED) {
			perror("camera init");
			close(fd);
			return -1;
		}
		/*每次映射都会入队,放入缓冲队列*/
		vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vbuf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_QBUF, &vbuf);
		if (ret == -1) {
			perror("camera init");
			close(fd);
			return -1;
		}
	}
	/*返回真正设置成功的宽.高.大小*/
	*width = format.fmt.pix.width;
	*height = format.fmt.pix.height;
	*size = bufs[0].length;

	return fd;
}

int camera_start(int fd)
{
	int ret;
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	/*ioctl控制摄像头开始采集*/
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if (ret == -1) {
		perror("camera->start");
		return -1;
	}
	fprintf(stdout, "camera->start: start capture\n");

	return 0;
}

int camera_dqbuf(int fd, void **buf, unsigned int *size, unsigned int *index)
{
	int ret;
	fd_set fds;
	struct timeval timeout;
	struct v4l2_buffer vbuf;

	while (1) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		ret = select(fd + 1, &fds, NULL, NULL, &timeout);	//使用select机制,保证fd有图片的时候才能出对
		if (ret == -1) {
			perror("camera->dbytesusedqbuf");
			if (errno == EINTR)
				continue;
			else
				return -1;
		} else if (ret == 0) {
			fprintf(stderr, "camera->dqbuf: dequeue buffer timeout\n");
			return -1;
		} else {
			vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			vbuf.memory = V4L2_MEMORY_MMAP;
			ret = ioctl(fd, VIDIOC_DQBUF, &vbuf);	//出队,也就是从用户空间取出图片
			if (ret == -1) {
				perror("camera->dqbuf");
				return -1;
			}
			*buf = bufs[vbuf.index].start;
			*size = vbuf.bytesused;
			*index = vbuf.index;

			return 0;
		}
	}
}

int camera_eqbuf(int fd, unsigned int index)
{
	int ret;
	struct v4l2_buffer vbuf;

	vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vbuf.memory = V4L2_MEMORY_MMAP;
	vbuf.index = index;
	ret = ioctl(fd, VIDIOC_QBUF, &vbuf);		//入队
	if (ret == -1) {
		perror("camera->eqbuf");
		return -1;
	}

	return 0;
}

int camera_stop(int fd)
{
	int ret;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
	if (ret == -1) {
		perror("camera->stop");
		return -1;
	}
	fprintf(stdout, "camera->stop: stop capture\n");

	return 0;
}

int camera_exit(int fd)
{
	int i;
	int ret;
	struct v4l2_buffer vbuf;
	vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vbuf.memory = V4L2_MEMORY_MMAP;
	for (i = 0; i < reqbufs.count; i++) {
		ret = ioctl(fd, VIDIOC_DQBUF, &vbuf);
		if (ret == -1)
			break;
	}
	for (i = 0; i < reqbufs.count; i++)
		munmap(bufs[i].start, bufs[i].length);
	fprintf(stdout, "camera->exit: camera exit\n");
	return close(fd);
}

int main()
{
	
	int i;
	int fd;
	int ret;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int index;
	unsigned int ismjpeg;
	width = 640;
	height = 480;
	char pic_size[12]={0};
	
	
	fd = camera_init("/dev/video0", &width, &height, &size, &ismjpeg);
	if (fd == -1)
		return -1;
	ret = camera_start(fd);
	if (ret == -1)
		return -1;
	// 采集几张图片丢弃 
	char *jpeg_ptr = NULL;
	for (i = 0; i < 8; i++) {
		ret = camera_dqbuf(fd, (void **)&jpeg_ptr, &size, &index);
		if (ret == -1)
			exit(EXIT_FAILURE);

		ret = camera_eqbuf(fd, index);
		if (ret == -1)
			exit(EXIT_FAILURE);
	}

	fprintf(stdout, "init camera success\n");
	signal(SIGPIPE, SIG_IGN);
    //1.申请网卡
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socket_fd)
    {
        perror("socket");
        return -1;
    }

    //2.做好监听连接准备
    struct sockaddr_in serveraddr = {0};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("0.0.0.0");//All IP
    serveraddr.sin_port = htons(6666);
    if(-1 == bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) )
    {
        perror("connect");
        return -1;
    }
    listen(socket_fd, 10);
    int count_send=0;
while(1){
    //3.提取客户端接入
        printf("waiting for client........\n");
        int clientfd = accept(socket_fd, NULL, NULL);
        printf("client is accepted!\n");
	while(1)
	{
		//拍照放入内存的jpeg_ptr指针指向的位置而且长度为size
		ret = camera_dqbuf(fd, (void **)&jpeg_ptr, &size, &index); //tack picture
		if (ret == -1)
			return -1;

#if 0
        //save pic
        int fdpix = open("1.jpg", O_WRONLY|O_CREAT, 0666);

        write(fdpix, jpeg_ptr, size);

        close(fdpix);
        break;
#else
	count_send++;
	//if(count_send>5){
	count_send=0;
	sprintf(pic_size,"%d",size);
	printf("%s\n",pic_size);
	if(write(clientfd,pic_size,6)<0){
	break;
	}
	
        if(write(clientfd, jpeg_ptr, size)<0){
        break;
        }
       // }
#endif

		//告诉系统已经取走
		ret = camera_eqbuf(fd, index); //prepare for next tacking
		if (ret == -1)
			return -1;

	}
	}
	ret = camera_stop(fd); //stop the camera!!!! do not call this!!!
	if (ret == -1)
		return -1;

	ret = camera_exit(fd);
	if (ret == -1)
		return -1;
}
