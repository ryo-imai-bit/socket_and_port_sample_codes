# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

// www.example.comにover tcpでhttpリクエストを送る
int main(void) {
	char *hostname = "www.example.com";
	char *service = "http";
	struct addrinfo hints, *res0, *res;
	char buf[2048];
	char req[] = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
	int err, n;
	int sock;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = PF_UNSPEC; /* v4とv6両方を取得 */
	if ((err = getaddrinfo(hostname, service, &hints, &res0)) != 0) {
		printf("error %d\n", err);
		return 1;
	}

	// 接続できるaddrinfoが来るまで探す
	for (res=res0; res!=NULL; res=res->ai_next) {
		sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sock < 0) {
			continue;
		}

		if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
			close(sock);
			continue;
		}

		break;
	}

	if (res == NULL) {
		printf("failed\n");
		return 1;
	}

	freeaddrinfo(res0);

	// C言語にてポインタ型は指す先のサイズという情報を含まない
	// https://teratail.com/questions/41701
	write(sock, req, sizeof(req));
	memset(buf, 0, sizeof(buf));
	n = read(sock, buf, sizeof(buf));

	printf("%d, %s\n", n, buf);

	close(sock);

	return 0;
}
