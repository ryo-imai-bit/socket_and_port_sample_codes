#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
	int sock;
	struct sockaddr_in s, t;
	int n;

	// 送信元socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	s.sin_family = AF_INET;
	s.sin_port = htons(11111);
	s.sin_len = sizeof(s);

	t.sin_family = AF_INET;
	t.sin_port = htons(11112);
	t.sin_len = sizeof(s);

	n = inet_pton(AF_INET, "127.0.0.1", &s.sin_addr);
	if (n < 1) {
		perror("inet_pton");
		return 1;
	}

	if (bind(sock, (struct sockaddr *)&t, sizeof(t)) != 0) {
		perror("bind");
		return 1;
	}

	// UDPパケットを送信
	n = sendto(sock, "TEST", 5, 0, (struct sockaddr *)&s, sizeof(s));
	if (n < 1) {
		perror("sendto");
		return 1;
	}

	close(sock);

	return 0;
}
