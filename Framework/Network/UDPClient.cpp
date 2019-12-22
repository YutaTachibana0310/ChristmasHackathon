//=============================================================================
//
// UDPクライアントクラス [UDPClient.cpp]
// Author : HAL東京 GP12B332 41 頼凱興
//
//=============================================================================
#include "../../main.h"
#include <process.h>
#include "UDPClient.h"

//*****************************************************************************
// スタティック変数宣言
//*****************************************************************************
HANDLE UDPClient::Thread = 0;
SOCKET UDPClient::ClientSocket = 0;
sockaddr_in UDPClient::ServerAddress;

//=============================================================================
// コンストラクタ
//=============================================================================
UDPClient::UDPClient()
{
	// WinSock初期化
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// ソケット生成
	ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// 送信先アドレス
	ServerAddress.sin_port = htons(ServerPort);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr("255.255.255.255");//ブロードキャストアドレス

	// ブロードキャスト許可
	int value = 1;
	setsockopt(ClientSocket, SOL_SOCKET, SO_BROADCAST, (char*)&value, sizeof(value));

	// クライアント受信スレッド開始
	Thread = (HANDLE)_beginthreadex(NULL, 0, ThreadEntryPoint, this, 0, NULL);
}

//=============================================================================
// デストラクタ
//=============================================================================
UDPClient::~UDPClient()
{
	ThreadStop = true;

	// WinSock終了処理
	closesocket(ClientSocket);
	WSACleanup();

	// ===============================================================
	// 注意。きちんとスレッド終了関数(_endthreadex)を呼ばないと
	// メインスレッドが終了した後に他のスレッドはそのまま実行します
	// 例外スローの可能性が高い
	// ===============================================================
	// スレッドが解放するまで待つ
	DWORD ThreadResult = WaitForSingleObject(Thread, INFINITE);
	if (ThreadResult == WAIT_OBJECT_0)
	{
		// スレッド終了
		CloseHandle(Thread);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UDPClient::Update(void)
{
	if (!InLoading)
		return;
}

//=============================================================================
// 描画処理
//=============================================================================
void UDPClient::Draw(void)
{
	if (!InLoading)
		return;
}

//=============================================================================
// ランキング追加パケット送信
//=============================================================================
void UDPClient::SendRankPacket(string NameStr, string AILevel)
{
	string Message;

	// 入力メッセージ処理
	Message = PacketHeader + "," + std::to_string(Packet::InsertRank) + "," + NameStr + "," + AILevel;

	// 送信
	sendto(ClientSocket, Message.c_str(), Message.length() + 1, 0, (sockaddr*)&ServerAddress, sizeof(ServerAddress));
}

//=============================================================================
// ランキング追加パケット送信
//=============================================================================
void UDPClient::SendRankPacket(string NameStr, unsigned long long AILevel)
{
	string AILevelStr = std::to_string(AILevel);
	SendRankPacket(NameStr, AILevelStr);
}

//=============================================================================
// イベント中継パケット送信
//=============================================================================
void UDPClient::SendEventPacket(string EventNo, string FieldLevel)
{
	string Message;

	// 入力メッセージ処理
	Message = PacketHeader + "," + std::to_string(Packet::EventLive) + "," + EventNo + "," + FieldLevel;

	// 送信
	sendto(ClientSocket, Message.c_str(), Message.length() + 1, 0, (sockaddr*)&ServerAddress, sizeof(ServerAddress));
}

//=============================================================================
// イベント中継パケット送信
//=============================================================================
void UDPClient::SendEventPacket(int EventNo, int FieldLevel)
{
	string EventNoStr = std::to_string(EventNo);
	string FieldLevelStr = std::to_string(FieldLevel);
	SendEventPacket(EventNoStr, FieldLevelStr);
}

//=============================================================================
// フィールドレベルアップパケット送信
//=============================================================================
void UDPClient::SendLevelUpPacket(void)
{
	string Message;

	// 入力メッセージ処理
	Message = PacketHeader + "," + std::to_string(Packet::LevelUp);

	// 送信
	sendto(ClientSocket, Message.c_str(), Message.length() + 1, 0, (sockaddr*)&ServerAddress, sizeof(ServerAddress));
}

//=============================================================================
// セカンドスレッド
//=============================================================================
unsigned __stdcall UDPClient::ThreadEntryPoint(void* This)
{
	UDPClient* Temp = (UDPClient*)This;
	Temp->GetScorePacket();					// 本当のセカンドスレッドの処理関数
	_endthreadex(0);						// スレッドの実行停止
	return 0;
}

//=============================================================================
// サーバーからランキング最下位のスコアを取得
//=============================================================================
void UDPClient::GetScorePacket(void)
{
	// データ送信
	char data[256];
	string Message;
	int AddressLength;

	AddressLength = sizeof(ServerAddress);

	while (!ThreadStop)
	{
		memset(data, 0, sizeof(data));

		// メッセージ受信
		recvfrom(ClientSocket, (char*)data, sizeof(data), 0, (sockaddr*)&ServerAddress, &AddressLength);

		// データ処理
		Message = data;
		if (!Message.empty())
		{
			LastScore = std::stoull(Message);
			ReceiveSuccess = true;
		}
	}
}

//=============================================================================
// リクエスト
//=============================================================================
void UDPClient::TryGetLastScore(void)
{
	// データ送信
	string Message;
	int AddressLength;

	AddressLength = sizeof(ServerAddress);

	// 入力メッセージ処理
	Message = PacketHeader + "," + std::to_string(Packet::GetLastScore);

	// 送信
	sendto(ClientSocket, Message.c_str(), Message.length() + 1, 0, (sockaddr*)&ServerAddress, AddressLength);
}

//=============================================================================
// イベント中継パケット送信
//=============================================================================
unsigned long long UDPClient::GetLastScore(void)
{
	ReceiveSuccess = false;
	InLoading = true;
	TimeoutCount = 0;

	// スレッドまだ解放されないので、もう一度サーバーにリクエスト
	TryGetLastScore();

	while (TimeoutCount <= 60)
	{
		DWORD Result = WaitForSingleObject(Thread, 1);

		// スレッド正常終了
		if (ReceiveSuccess)
		{
			break;
		}
		// タイムアウト
		else
		{
			TimeoutCount++;

			// リクエスト
			if (TimeoutCount % 3 == 0)
				TryGetLastScore();
		}
	}

	if (ReceiveSuccess)
	{
		return LastScore;
	}
	else
	{
		return 0;
	}
}
