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
    SOCKET listen_s;
    SOCKET s;
    SOCKADDR_IN saddr;
    SOCKADDR_IN from;
    int fromlen;
    u_short uport = 8080;

    fflush(stdin);

    // リスンソケットをオープン
    listen_s = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_s == INVALID_SOCKET) {
        printf("リスンソケットオープンエラー");
        WSACleanup();
        return;
    }

    printf("リスンソケットをオープンしました\n");

    // ソケットに名前を付ける
    memset(&saddr, 0, sizeof(SOCKADDR_IN));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(uport);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_s, (struct sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
        printf("bindエラー");
        closesocket(listen_s);
        return;
    }
    printf("bind成功です\n");

    // クライアントからの接続待ちの状態にする
    if (listen(listen_s, SOMAXCONN) == SOCKET_ERROR) {
        printf("listenエラー\n");
        closesocket(listen_s);
        return;
    }

    printf("listen成功\n");

    // 接続待機する
    printf("acceptで待機します\n");

    fromlen = (int)sizeof(from);

    s = accept(listen_s, (struct sockaddr*)&from, &fromlen);
    if (s == INVALID_SOCKET) {
        printf("acceptエラー\n");
        closesocket(listen_s);
        return;
    }

    printf("%sが接続してきました\n", inet_ntoa(from.sin_addr));
    printf("accepet関数成功\n");

    //リスンソケットはもう不要
    closesocket(listen_s);

    // 会話開始
    printf("会話開始\n");

    while (1) {
        char recvBuf[1024];
        int recvLen = recv(s, recvBuf, sizeof(recvBuf) - 1, 0);
        if (recvLen == SOCKET_ERROR) {
            printf("データ受信エラー\n");
            break;
        }
        recvBuf[recvLen] = '\0';
        printf("受信: %s\n", recvBuf);

        printf("送信: ");
        char sendBuf[1024];
        scanf_s("%s", sendBuf, (unsigned)_countof(sendBuf));
        send(s, sendBuf, (int)strlen(sendBuf), 0);
    }

    // ソケットを閉じる
    closesocket(s);
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