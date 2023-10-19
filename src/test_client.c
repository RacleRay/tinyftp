#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/**
 * @brief 只用作测试服务器选项功能，没有建立数据通道，有时间再测
 *        
 * 
 */


int init_client(const char *host, uint16_t port) {
	int tcp_socket;
	struct sockaddr_in dest;
	int ret;

	tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_socket == -1) {
		perror("socket");
		return -1;
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr.s_addr = inet_addr(host);

	ret = connect(tcp_socket,(const struct sockaddr *)&dest,sizeof(dest));
	if (ret < 0) {
		perror("connect");
		return -1;
	}

	printf("connect success!\n");
	return tcp_socket;
}

void msg_handler(int client_socket) {
	char buf[1024 * 8] = {0};
    char rbuf[1024] = {0};
	ssize_t ret;

    printf("=============");        
	printf("\n<client>: ");
	while (fgets(buf, sizeof(buf), stdin)) {
		// 把标准输入获取到的回车符去除掉
		buf[strlen(buf) - 1] = 0;
		if (strncmp(buf, "quit", 4) == 0) {
			break;
		}
		ret = send(client_socket, buf, strlen(buf), 0);
		if (ret < 0) {
			perror("send");
			break;
		}
        
        printf("\n<server>: \n");
        // while (1) {
        ret = recv(client_socket, rbuf, sizeof(rbuf), 0);
        if (ret <= 0) {
            printf("%ld\n", ret);
            perror("recv");
            break;
        }
        printf("%ld, %s\n", ret, rbuf);
        // }
        
        printf("=============");        
		printf("\n<client>: ");
	}
}


int main(int argc, char *argv[]) {
    // ./program ip port

	uint16_t port = 8899;
	char *dest_ip = "192.168.44.128";
	char *end = NULL;
	int client_fd;

	if (argc == 3) {
		dest_ip = argv[1];
		port = strtoul(argv[2], &end, 10);
		if (end && *end) {
			fprintf(stderr, "Invalid port format!\n");
			port = 8899;
		}
	}

	client_fd = init_client(dest_ip, port);
	if (client_fd < 0) {
		return -1;
	}

	msg_handler(client_fd);

	printf("client closed!\n");
	close(client_fd);
	return 0;
}
