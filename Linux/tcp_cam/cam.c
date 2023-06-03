/*
 * @Author: CanEve 2785546297@qq.com
 * @Date: 2023-06-02 00:55:19
 * @LastEditors: CanEve 2785546297@qq.com
 * @LastEditTime: 2023-06-02 00:56:45
 * @FilePath: \undefinedd:\Git\GIT_UPDATE\opencaneve\opencaneve\Linux\tcp_cam\cam.c
 * @Description: Linux tcp_cam Ĭ����ʹ��dev/video0,�˿�Ϊ6666
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
#include<stdio.h>      /*��׼�����������*/
#include<stdlib.h>     /*��׼�����ⶨ��*/
#include<unistd.h>     /*Unix ��׼��������*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*�ļ����ƶ���*/
#include<termios.h>    /*PPSIX �ն˿��ƶ���*/
#include<errno.h>      /*����Ŷ���*/
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
	/*open ���豸�ļ�*/
	if((fd = open(devpath, O_RDWR)) == -1){
		perror("open:");	
		return -1;		
	}
	/*ioctl �鿴֧�ֵ�����*/
	ret = ioctl(fd, VIDIOC_QUERYCAP, &capability);
	if (ret == -1) {
		perror("camera->init");
		return -1;
	}
	/*�ж��豸�Ƿ�֧����Ƶ�ɼ�*/
	if(!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "camera->init: device can not support V4L2_CAP_VIDEO_CAPTURE\n");
		close(fd);
		return -1;
	}
	/*�ж��豸�Ƿ�֧����Ƶ���ɼ�*/
	if(!(capability.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "camera->init: device can not support V4L2_CAP_STREAMING\n");
		close(fd);
		return -1;
	}
	/*���ò������Ƶ��ʽ MYJPEG*/
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
	/*���ò������Ƶ��ʽ YUYV*/
	memset(&format, 0, sizeof(format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;			//��Զ�����������
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;		//���òɼ�ͼƬ�ĸ�ʽ
	format.fmt.pix.width = *width;
	format.fmt.pix.height = *height;
	format.fmt.pix.field = V4L2_FIELD_ANY;				//����ͼƬһ��һ�еĲɼ�
	ret = ioctl(fd, VIDIOC_S_FMT, &format);				//ioctl	��������Ч
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
	/*���������뻺��*/
	memset(&reqbufs, 0, sizeof(struct v4l2_requestbuffers));
	reqbufs.count	= REQBUFS_COUNT;					//����������
	reqbufs.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbufs.memory	= V4L2_MEMORY_MMAP;					//���ò������뻺��ķ�ʽ:ӳ�� MMAP
	ret = ioctl(fd, VIDIOC_REQBUFS, &reqbufs);			
	if (ret == -1) {	
		perror("camera init");
		close(fd);
		return -1;
	}
	/*ѭ��ӳ�䲢���*/
	for (i = 0; i < reqbufs.count; i++){
		/*������ȡ����ĵ�ַ��С*/
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
		/*ӳ�仺�浽�û��ռ�,ͨ��mmap���ں˵Ļ����ַӳ�䵽�û��ռ�,���к��ļ�������fd�����*/
		bufs[i].length = vbuf.length;
		bufs[i].start = mmap(NULL, vbuf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, vbuf.m.offset);
		if (bufs[i].start == MAP_FAILED) {
			perror("camera init");
			close(fd);
			return -1;
		}
		/*ÿ��ӳ�䶼�����,���뻺�����*/
		vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vbuf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_QBUF, &vbuf);
		if (ret == -1) {
			perror("camera init");
			close(fd);
			return -1;
		}
	}
	/*�����������óɹ��Ŀ�.��.��С*/
	*width = format.fmt.pix.width;
	*height = format.fmt.pix.height;
	*size = bufs[0].length;

	return fd;
}

int camera_start(int fd)
{
	int ret;
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	/*ioctl��������ͷ��ʼ�ɼ�*/
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
		ret = select(fd + 1, &fds, NULL, NULL, &timeout);	//ʹ��select����,��֤fd��ͼƬ��ʱ����ܳ���
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
			ret = ioctl(fd, VIDIOC_DQBUF, &vbuf);	//����,Ҳ���Ǵ��û��ռ�ȡ��ͼƬ
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
	ret = ioctl(fd, VIDIOC_QBUF, &vbuf);		//���
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
	// �ɼ�����ͼƬ���� 
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
    //1.��������
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socket_fd)
    {
        perror("socket");
        return -1;
    }

    //2.���ü�������׼��
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
    //3.��ȡ�ͻ��˽���
        printf("waiting for client........\n");
        int clientfd = accept(socket_fd, NULL, NULL);
        printf("client is accepted!\n");
	while(1)
	{
		//���շ����ڴ��jpeg_ptrָ��ָ���λ�ö��ҳ���Ϊsize
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

		//����ϵͳ�Ѿ�ȡ��
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
