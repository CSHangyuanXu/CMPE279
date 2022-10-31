// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    // process data
    pid_t pid = fork();
    
    // process separation
    if (pid == 0) { 
    	// child process
    	struct passwd *pwd;
    	uid_t uid;
    	if((pwd = getpwnam("nobody")) == NULL) {
		perror("UID found failed");
		exit(EXIT_FAILURE);
    	} else {
    		uid = pwd->pw_uid;
    		printf("Child UID is: %d\n", uid);
    		setuid(uid); // drop privilege
    	}
    	valread = read( new_socket , buffer, 1024);
    	printf("%s\n",buffer );
    	send(new_socket , hello , strlen(hello) , 0 );
    	printf("Hello message sent\n");
    	printf("Child exit\n");
    } else if (pid > 0) {
    	wait(NULL); // wait all children to exit
    	printf("Parent exit\n");
    } else {
    	perror("Fork error");
    	exit(EXIT_FAILURE);
    }
 
    return 0;
}
