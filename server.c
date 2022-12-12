# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>

int main(void) {
	// listen用ソケット
	int sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	socklen_t len;
	int sock1, sock2;

	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定

	// IPv4を利用することを指定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(11111);
	// 任意のインターフェースを指定
	addr.sin_addr.s_addr = INADDR_ANY;
	// sockaddr_in構造体のサイズを設定
	addr.sin_len = sizeof(addr);

	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	listen(sock0, 5);

	// TCPクライアントからの接続要求を受け付ける（１回目）
	len = sizeof(client);
	// acceptは接続待ちソケット宛の保留状態の接続要求が入っているキューから先頭の接続要求を取り出し、接続済みソケットを新規に作成し、そのソケットを参照する新しいファイルディスクリプタを返す。新規に接続されたソケットはlisten状態でなく、元のソケットは呼び出しによって影響を受けない
	// https://linuxjm.osdn.jp/html/LDP_man-pages/man2/accept.2.html
	sock1 = accept(sock0, (struct sockaddr *)&client, &len);

	write(sock1, "HELLO", 6);
	close(sock1);

	// 2回目
	len = sizeof(client);
	sock2 = accept(sock0, (struct sockaddr *)&client, &len);

	write(sock2, "HOGE", 5);

	close(sock2);

	close(sock0);

	return 0;
}
