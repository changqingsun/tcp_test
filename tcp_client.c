/*
 * Name: tcp_client.c
 * Author: Changqing.Sun
 * Data : 2016/09/03
 */

 #include <sys/types.h>
 #include <sys/socket.h>
 #include <string.h>
 #include <stdio.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <errno.h>
 #include <netinet/tcp.h>

//#define DISABLE_NAGLE 0
#define SERVER_PORT 6666

 int main()
 {
 	int err = 0;
       struct sockaddr_in server_addr;
	int server_addr_len = sizeof(struct sockaddr_in);
	/*This is a client */
	int client_fd = socket(AF_INET,SOCK_STREAM,0);
	if(client_fd < 0) {
		printf("[tcp_test] Create client socket fail err(%s)\n",strerror(errno));
	return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

	err = connect(client_fd,(struct sockaddr *)&server_addr,server_addr_len);
	if(err < 0) {
 	//	printf("[tcp_test] connect server socket fail err(%s)\n",strerror(errno));
		return -1;
	}
	printf("[tcp_test] connect server ok \n");
	
#ifdef DISABLE_NAGLE
     //  err = setsockopt(client_fd, SOL_TCP, TCP_NODELAY,(int[]){1}, sizeof(int));
	if(err < 0) {
 		printf("[tcp_test] Disable Nagle  err(%s)\n",strerror(errno));
		return -1;
	}
#endif

	char send_buff[10];
	while(1){

         scanf("%s",send_buff);
	   printf("recv data : %s from studio \n",send_buff);
       int i =0;

	 for(i =0; i < strlen(send_buff); i++) {
	 printf("send %c\n",send_buff[i]);
	 err = send(client_fd,send_buff + i,1,0);
	 	}
	  memset(send_buff,0,strlen(send_buff));

	}
	
 }

