/* 簡易チャットプログラム */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h> /* WinSockのヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment ( lib, "WSock32.lib" ) /* WinSockライブラリの指定 */

// チャットプログラム　サーバー関数
void ChatServer(void)
{
	char szBuf[512];
	int fromlen;
	int recv_cnt;
	int send_cnt;
	struct sockaddr_in addr, recv_addr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？

	// 受信ソケットにIPアドレスとポート設定
	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(8080);   //通信ポート番号設定
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANYはすべてのアドレスからのパケットを受信する
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));

	while (1) {
		fromlen = sizeof(recv_addr);
		recv_cnt = 0;
		send_cnt = 0;

		memset(szBuf, 0, sizeof(szBuf));

		puts("受信待ち…");
		recv_cnt = recvfrom(sock, szBuf, sizeof(szBuf) - 1, 0, (struct sockaddr*)&recv_addr, &fromlen);
		printf("受信 : %s \n", szBuf);

		printf("送信 --> ");
		scanf_s("%s", szBuf, sizeof(szBuf));
		fflush(stdin);

		while (send_cnt == 0)
		{
			//データ送信
			send_cnt = sendto(sock, szBuf, sizeof(szBuf), 0, (struct sockaddr*)&recv_addr, sizeof(recv_addr));
		}
	}

	closesocket(sock);
}

// チャットプログラム メイン関数
int main(void)
{
    WSADATA wsaData;

    // WinSockの初期化
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        // 初期化エラー
        printf("WinSockの初期化に失敗しました\n");
        return 1;
    }

    // サーバーとして起動
    ChatServer();

    // WinSockの終了処理
    WSACleanup();

    return 0;
}
//eof