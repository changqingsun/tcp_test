/*
 * Name: tcp_server.c
 * Author: Changqing.Sun
 * Data : 2016/09/03
 * Update:
 * 	1. Add support QUICKACK   2016/9/4
 *    2. Use select to accept and recv data   2016/9/4
 *    3. Test Nagle Algorithm [Kernel default Open]
 *    4. Add support NODELAY disable Nagle algorithm
 */

 #include <sys/types.h>
 #include <sys/socket.h>
 #include <string.h>
 #include <stdio.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <errno.h>
 #include <unistd.h>

/*TCP option*/
#include <netinet/tcp.h>

#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>


//#define ENABLE_QUICKACK 0
//#define DISABLE_NAGLE 0


#define SERVER_PORT 6666

int main()
 {
 	int err = 0;

	struct sockaddr_in server_bind_addr;
	struct sockaddr_in client_accept_addr;
	int client_len =  sizeof(client_accept_addr);
	int sock_for_client_fd = 0;

	/*1. Create a Server socket*/
	int sock_server_fd = socket(AF_INET,SOCK_STREAM,0);
	ssize_t recv_len = 0;
	if(sock_server_fd < 0) {
 		printf("[tcp_test] Create server socket fail err(%s)\n",strerror(errno));
		return -1;
	}

	server_bind_addr.sin_family = AF_INET;
	server_bind_addr.sin_port = htons(SERVER_PORT);
	server_bind_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	/*2. Bind port 6666*/
	err = bind(sock_server_fd,(struct sockaddr *)&server_bind_addr,sizeof(server_bind_addr));
	if(err < 0){
  		printf("[tcp_test] Bind server socket fail err(%s)\n",strerror(errno));
		return -1;
	}

  	printf("[tcp_test] Bind server socket successfully\n");

	/*3. Listen this port, and set backlog 5*/
	err = listen(sock_server_fd,5);
	if (err < 0){
  		printf("[tcp_test] Listen server socket fail err(%s)\n",strerror(errno));
		return -1;
	}
  	printf("[tcp_test] Listen server socket successfully\n");

       fd_set read_set;
	fd_set write_set;

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);

	FD_SET(sock_server_fd,&read_set);

	err = select(sock_server_fd + 1,&read_set,&write_set,NULL,NULL);
	if(err < 0) {
  		printf("[tcp_test] Select fail err(%s)\n",strerror(errno));
	}

	if(FD_ISSET(sock_server_fd,&read_set)){
	/*4. Accept Client socket*/
	sock_for_client_fd = accept(sock_server_fd,(struct sockaddr *)&client_accept_addr,&client_len);
	if (sock_for_client_fd < 0){
		  	printf("[tcp_test] Accpet client fail err(%s)\n",strerror(errno));
			return -1;
		}
	
	printf("[tcp_test] Accpet client ip  %s port %d \n",inet_ntoa(client_accept_addr.sin_addr),htons(client_accept_addr.sin_port));

	}
	

#ifdef ENABLE_QUICKACK
	/*Enable QUICKACK option*/
	err = setsockopt(sock_for_client_fd, IPPROTO_TCP,TCP_QUICKACK, (int[]){1}, sizeof(int));
#endif

#ifdef DISABLE_NAGLE
       setsockopt(sock_for_client_fd, SOL_TCP, TCP_NODELAY,(int[]){1}, sizeof(int));
#endif
      char recv_buff[100] = {0};

      while(1){

	   recv_len = recv(sock_for_client_fd,recv_buff,100,0);
	   
#ifdef ENABLE_QUICKACK
	  err = setsockopt(sock_for_client_fd, IPPROTO_TCP,TCP_QUICKACK, (int[]){1}, sizeof(int));
#endif
	   if (recv_len > 0) {
             printf("Recv : %s\n",recv_buff);
	   }

	 // sleep 30ms
        usleep(30000);

	 //Using Test Nagle 
	// int i = 0;
       //for(i =0; i<strlen(recv_buff); i++){
  	 err = send(sock_for_client_fd,recv_buff,strlen(recv_buff),0);
       	//}
         memset(recv_buff,0,strlen(recv_buff));

	 }    


	
 }
