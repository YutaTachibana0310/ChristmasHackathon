/**********************************************************************
[[外部ファイルプログラム(CSV_Reader.cpp)]]	Version 0.0.5
	作者：奥田　真規

	CSVファイル読み込みに関するプログラム
***********************************************************************/

#include <Windows.h>

#include <stdio.h>
#include <tchar.h>
#include <malloc.h>

#include "CSV_Reader.h"


//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define MAX_READ_COLUMN	(255)	// 列の最大数

// 特殊文字
#define MOJI_COMMA			(',')
#define MOJI_INDENT			('\n')
#define MOJI_DQUOTATION		('"')

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


// 文字型構造体(あとで文字群にするための一時保管所)
typedef struct STR_LIST {
	char		*Str;
	int			Line;
	int			Column;
	STR_LIST	*next_pt;
}STR_LIST;

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

int CheckSpecificCSVMoji(FILE *fp, char *findchar);

HRESULT CreateCSVFromFile_ErrorCheckArgument(const char *filename, char *ReadTypeArray, CSV_CELRANGE *CSV_CelRange, CSV_FILE **Csv, DWORD flag);

HRESULT CreateCSVFromFile_OutputError(CSV_FILE **Csv, const char *String = NULL);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

/*=====================================================================
＊＊CSV作成関数＊＊
戻り値 : HRESULT : エラーメッセの出力
引数　 : 
	IN	const char		*filename		:ファイル名
	IN  char			*ReadTypeArray	:読み込むフォーマットを配列にしたやつ
	IN  CSV_CELRANGE	CSV_CelRange	:CSVファイル基準での読み込む場所を構造体で指定
	OUT CSV_FILE		**Csv			:読み込みが終了したエクセルデータ(失敗したらアドレスがNULL)
	IN  DWORD			flag			:[必須引数ではない]flag	(デフォルト=0)
=====================================================================*/
HRESULT CreateCSVFromFile(const char *filename, char *ReadTypeArray, CSV_CELRANGE CSV_CelRange, CSV_FILE **Csv, DWORD flag)
{
	LARGE_INTEGER	tBegin, tEnd, tFreq;				// 高分解能　用変数宣言
	FILE			*fp;								// ファイルポインタ
	HRESULT			hr;									// 結果出力
	int				TotalCelSize = 0;					// 総セル数
	CSV_FILE		*In_Pack_pt		= NULL;				// 出力するCSVファイル
	bool			isNOTeof = true;					// テキスト終端まで行ってしまったか
	int				SheetStrLength = 0;					// 文字列の総要素数
	STR_LIST		*Top_pt = NULL, *work_pt = NULL;	// 文字列リスト


	// 時間表示オプション
	if (flag&CSV_OPTION_SHOWTIME)
	{
		QueryPerformanceFrequency(&tFreq);	// CPUパフォーマンスカウンタ（1秒あたりのカウント数）取得
		QueryPerformanceCounter(&tBegin);	// 開始時間の保存
	}

	// 引数のチェック
	hr = CreateCSVFromFile_ErrorCheckArgument(filename, ReadTypeArray, &CSV_CelRange, Csv, flag);
	if (FAILED(hr))return hr;

	fopen_s(&fp, filename, "r"); // テキスト読み込みモード

	// ファイルが開けなかった場合(存在しない)
	if (fp == NULL)
		return CreateCSVFromFile_OutputError(Csv,
		(flag&CSV_OPTION_SHOWERR) ? _T("ERR:ファイルが存在しません\n推奨:\n\tファイルアドレス名の確認\n\tCSV存在の確認\n\t該当ファイルを他アプリケーションで開いていないか確認") : NULL);


	// スタート行までファイルポインタを持ってくる
	for (int i = 0; i < CSV_CelRange.Line_Start; i++)
	{
		while (1)
		{
			char readChar;
			int result = CheckSpecificCSVMoji(fp, &readChar);

			if (result < 0)
			{
				fclose(fp);

				if (flag&CSV_OPTION_SHOWERR)
				{
					char Errstr[128];
					sprintf_s(Errstr, _T("ERR:開始行まで到達できませんでした。(認識行:%d)\n推奨:\n\tCSVファイルのチェック\n\t引数[CSV_CelRange.Line_Start]のチェック"), i + 1);
					return CreateCSVFromFile_OutputError(Csv, Errstr);
				}
				else
				{
					return CreateCSVFromFile_OutputError(Csv);
				}
			}

			else if (readChar == MOJI_INDENT)break;
		}
	}

	// 引数の行カウントが0なら探索で求める
	if (CSV_CelRange.Line_Count == 0)
	{
		long Backup_fp = ftell(fp);
		int Line_Count = 0;

		while (1)
		{
			char readChar;
			int result = CheckSpecificCSVMoji(fp, &readChar);

			if (result < 0)
			{
				if (Line_Count == 0)
				{
					fclose(fp);

					if (flag&CSV_OPTION_SHOWERR)
					{
						return CreateCSVFromFile_OutputError(Csv, _T("ERR:読み取り行が存在しません。\n推奨:\n\tCSVファイルのチェック\n\t引数[CSV_CelRange.Line_Start]と引数[CSV_CelRange.Line_Count]の配分ミス確認"));
					}
					else
					{
						return CreateCSVFromFile_OutputError(Csv);
					}
				}

				break;
			}

			else if (readChar == MOJI_INDENT)Line_Count++;
		}

		fseek(fp, Backup_fp, SEEK_SET);
		CSV_CelRange.Line_Count = Line_Count;
	}


	// 1次元動的配列メモリ確保
	TotalCelSize = CSV_CelRange.Line_Count*CSV_CelRange.Column_Count;
	In_Pack_pt = (CSV_FILE*)malloc(sizeof(CSV_FILE) + (sizeof(CEL_DATA)*TotalCelSize) + 
		(sizeof(CSVREAD_TYPE)*CSV_CelRange.Column_Count));
	ZeroMemory(In_Pack_pt, sizeof(In_Pack_pt));

	In_Pack_pt->Array = (CEL_DATA*)(In_Pack_pt + 1);
	In_Pack_pt->TypeArray = (CSVREAD_TYPE *)(In_Pack_pt->Array + TotalCelSize);

	//CSV_FILE	に値を代入
	In_Pack_pt->Column_Size = CSV_CelRange.Column_Count;
	In_Pack_pt->Line_Size = CSV_CelRange.Line_Count;
	// セル読み込みタイプを配列で代入
	for (int i = 0; i < CSV_CelRange.Column_Count; i++)
	{
		In_Pack_pt->TypeArray[i] = (CSVREAD_TYPE)ReadTypeArray[i];
	}


	// 行ごとに代入を行う
	for (int i = 0; i < CSV_CelRange.Line_Count; i++)
	{
		bool isColumn_Data = true;	// 列データはまだ有効か

		if (isNOTeof == false)
		{
			isColumn_Data = false;
		}

		// スタート列までファイルポインタを持ってくる
		for (int j = 0; j < CSV_CelRange.Column_Start; j++)
		{
			while (1)
			{
				char readChar;
				int result = CheckSpecificCSVMoji(fp, &readChar);

				if (result < 0)
				{					
					isNOTeof = false;
					break;
				}

				else if (readChar == MOJI_INDENT)
				{
					isColumn_Data = false;
					break;
				}

				else if (readChar == MOJI_COMMA)break;
			}
		}

		// CSV列の読み取り
		for (int j = 0; j < CSV_CelRange.Column_Count; j++)
		{
			char readchar = 0;
			int result = 0;
			int original_result = 0;

			if (isNOTeof)
			{
				long Backup_fp = ftell(fp);

				original_result = result =
					CheckSpecificCSVMoji(fp, &readchar);
				
				if (result < 0)
				{
					isNOTeof = false;
				}
				else
				{
					fseek(fp, Backup_fp, SEEK_SET);

				}

			}

			// セル読み込み
			if (isColumn_Data&&result > 0)
			{
				// スタック確保
				char *PullString = (char *)_malloca(sizeof(char)*result);
				ZeroMemory(PullString, sizeof(char)*result);

				// 読み込み
				fread(PullString, sizeof(char), result - 1, fp);
				fseek(fp, 1, SEEK_CUR);

				// "チェック
				if (PullString[0] == MOJI_DQUOTATION)
				{
					int DoubleQuotation_Count = 0;

					// 最初の文字は消す
					strcpy_s(&PullString[0], (rsize_t)result - 1, &PullString[1]);
					result --;

					for (int k = 0; k < result; k++)
					{
						if (IsDBCSLeadByte(PullString[k]))
						{
							k++;
						}
						else
						{
							if (PullString[k] == MOJI_DQUOTATION)
							{
								// 奇数個目の"だけ消す
								if (++DoubleQuotation_Count & 0x1)
								{
									strcpy_s(&PullString[k], (rsize_t)result - k, &PullString[k + 1]);
									result--;
									k--;
								}
							}
						}
					}

				}

				// セルタイプごとの処理
				switch (ReadTypeArray[j])
				{
				case CSV_INT:
					sscanf_s(PullString, "%d", &_ARRAY(In_Pack_pt, i, j)._int);
					break;
				case CSV_FLOAT:
					sscanf_s(PullString, "%f", &_ARRAY(In_Pack_pt, i, j)._float);
					break;
				case CSV_STRING:
					// 文字列型はデータが不定のため一度リストに入れてから文字列群に代入する
					if (Top_pt == NULL)
					{
						Top_pt = (STR_LIST*)calloc(1, sizeof(STR_LIST));
						work_pt = Top_pt;
					}
					else
					{
						work_pt->next_pt = (STR_LIST*)calloc(1, sizeof(STR_LIST));
						work_pt = work_pt->next_pt;
					}

					work_pt->next_pt = NULL;
					work_pt->Line = i;
					work_pt->Column = j;
					work_pt->Str = (char *)calloc(original_result, sizeof(char));

					strcpy_s(work_pt->Str, sizeof(char)*original_result, PullString);

					SheetStrLength += result;
					break;
				}

				_freea(PullString);	// 開放
			}
			else
			{
				// データなしを入れる
				_ARRAY(In_Pack_pt, i, j)._int = 0;
			}

			if (readchar == MOJI_INDENT)
			{
				isColumn_Data = false;
			}

		}

		fseek(fp, -1, SEEK_CUR);

		// 改行を探す
		while (1)
		{
			char readChar;
			int result = CheckSpecificCSVMoji(fp, &readChar);

			if (result < 0)
			{
				isNOTeof = false;
				break;
			}

			else if(readChar== MOJI_INDENT)break;
		}

	}

	fclose(fp);		// ファイルを閉じる

	// 文字列の格納(文字列が存在する場合)
	if (SheetStrLength > 0)
	{
		STR_LIST *keep_pt = NULL;
		int StringArray_Move = 0;

		// 大きさを変える(場所も)
		In_Pack_pt = (CSV_FILE *)realloc(In_Pack_pt, (sizeof(char)*SheetStrLength) + (sizeof(CSV_FILE)) +
			(sizeof(CEL_DATA)*TotalCelSize) + (sizeof(CSVREAD_TYPE)*CSV_CelRange.Column_Count));

		In_Pack_pt->Array = (CEL_DATA*)(In_Pack_pt + 1);
		In_Pack_pt->TypeArray = (CSVREAD_TYPE *)(In_Pack_pt->Array + TotalCelSize);

		In_Pack_pt->StringTop = (char *)(In_Pack_pt->TypeArray + CSV_CelRange.Column_Count);
		ZeroMemory(In_Pack_pt->StringTop, sizeof(char)*SheetStrLength);

		work_pt = Top_pt;

		// 順番にポインタ
		while (work_pt != NULL)
		{
			strcpy_s(&In_Pack_pt->StringTop[StringArray_Move], sizeof(char)*(strlen(work_pt->Str) + 1), work_pt->Str);
			
			// 文字列群とのリンク
			_ARRAY(In_Pack_pt, work_pt->Line, work_pt->Column)._str = &In_Pack_pt->StringTop[StringArray_Move];

			// 文字列の長さ加算
			StringArray_Move += (int)strlen(work_pt->Str) + 1;

			// 開放
			free(work_pt->Str);
			keep_pt = work_pt->next_pt;
			free(work_pt);
			work_pt = keep_pt;
		}

	}
	else
	{
		In_Pack_pt->StringTop = NULL;
	}

	*Csv = In_Pack_pt;		// データ出力

	// 時間表示オプション
	if (flag&CSV_OPTION_SHOWTIME)
	{
		char TimeStr[128];
		QueryPerformanceCounter(&tEnd);
		sprintf_s(TimeStr, _T("CSVデータ処理時間:\n %f (s)"), (double)(tEnd.QuadPart - tBegin.QuadPart) / (double)tFreq.QuadPart);
		MessageBox(NULL, TimeStr, _T("CSVシステム"), NULL);
	}

	return S_OK;
}

/*=====================================================================
＊＊CSV_FILE:: Release()関数＊＊
CSV_FILE開放関数
戻り値 :void
引数　 :void
=====================================================================*/
void CSV_FILE::Release()
{

	// 自分を開放 サヨ...ウ....ナラ......
	free(this);

	return;
}

/*=====================================================================
＊＊CSV_FILE:: Sum()関数＊＊
CSV_FILE 合計算出関数
戻り値 :CEL_DATA
引数　 :
IN	int Column	:加算する列
=====================================================================*/
CEL_DATA CSV_FILE::Sum(int Column)
{
	CEL_DATA Sum_Data;
	Sum_Data._int = 0;

	if (Column<0 || Column>Column_Size)return Sum_Data;

	// 合計加算処理
	if (TypeArray[Column] == CSV_INT)
	{
		for (int i = 0; i < Line_Size; i++)
		{
			Sum_Data._int += Array[(i*Column_Size) + Column]._int;
		}
	}
	else if (TypeArray[Column] == CSV_FLOAT)
	{
		for (int i = 0; i < Line_Size; i++)
		{
			Sum_Data._float += Array[(i*Column_Size) + Column]._float;
		}
	}
	
	return Sum_Data;
}

/*=====================================================================
＊＊CSV_FILE:: Average()関数＊＊
CSV_FILE 平均算出関数
戻り値 :CEL_DATA
引数　 :
IN	int Column	:平均算出する列
=====================================================================*/
CEL_DATA CSV_FILE::Average(int Column)
{
	CEL_DATA Average_Data = Sum(Column);

	if (Column<0 || Column>Column_Size)return Average_Data;

	// 平均算出処理
	if (TypeArray[Column] == CSV_INT)
	{
		Average_Data._int /= Line_Size;
	}
	else if (TypeArray[Column] == CSV_FLOAT)
	{
		Average_Data._float /= Line_Size;
	}

	return Average_Data;
}

/*=====================================================================
＊＊CSV_FILE:: Min()関数＊＊
CSV_FILE 平均算出関数
戻り値 :void
引数　 :
IN	int Column	:最小値を求める列
=====================================================================*/
CEL_DATA CSV_FILE::Min(int Column)
{
	CEL_DATA MinData;
	MinData._int = 0;
	if (Column<0 || Column>Column_Size)return MinData;

	switch (TypeArray[Column])
	{
	case CSV_INT:
		MinData._int = Array[(0 * Column_Size) + Column]._int;

		// 最小値を求める処理
		for (int i = 0; i < Line_Size; i++)
		{
			if (MinData._int > Array[(i * Column_Size) + Column]._int)
			{
				MinData._int = Array[(i * Column_Size) + Column]._int;
			}
		}
		break;
	case CSV_FLOAT:
		MinData._float = Array[(0 * Column_Size) + Column]._float;

		// 最小値を求める処理
		for (int i = 0; i < Line_Size; i++)
		{
			if (MinData._float > Array[(i * Column_Size) + Column]._float)
			{
				MinData._float = Array[(i * Column_Size) + Column]._float;
			}
		}
		break;
	default:
		break;
	}

	return MinData;
}

/*=====================================================================
＊＊CSV_FILE:: Max()関数＊＊
CSV_FILE 平均算出関数
戻り値 :void
引数　 :
IN	int Column	:最大値を求める列
=====================================================================*/
CEL_DATA CSV_FILE::Max(int Column)
{
	CEL_DATA MaxData;
	MaxData._int = 0;
	if (Column<0 || Column>Column_Size)return MaxData;

	switch (TypeArray[Column])
	{
	case CSV_INT:
		MaxData._int = Array[(0 * Column_Size) + Column]._int;

		// 最大値を求める処理
		for (int i = 0; i < Line_Size; i++)
		{
			if (MaxData._int < Array[(i * Column_Size) + Column]._int)
			{
				MaxData._int = Array[(i * Column_Size) + Column]._int;
			}
		}
		break;
	case CSV_FLOAT:
		MaxData._float = Array[(0 * Column_Size) + Column]._float;

		// 最大値を求める処理
		for (int i = 0; i < Line_Size; i++)
		{
			if (MaxData._float < Array[(i * Column_Size) + Column]._float)
			{
				MaxData._float = Array[(i * Column_Size) + Column]._float;
			}
		}
		break;
	default:
		break;
	}

	return MaxData;
}

/*=====================================================================
＊＊特殊文字まで飛ぶ関数＊＊(cpp内限定)
戻り値 :int :移動したファイルポインタのバイト数　(EOF時-1)
引数　 :
	IN	FILE *fp			: 使用しているファイルポインタ
	OUT	char *findchar		: どの特殊文字を見つけたか出力(1バイト)
=====================================================================*/
int CheckSpecificCSVMoji(FILE *fp, char *findchar)
{
	int move = 0;
	char CheckChar;
	bool isDoubleQuotation_Area = false;

	// 特定文字捜索
	while (1)
	{
		CheckChar = fgetc(fp);

		move++;

		// どこ見てるんチェック
		if (feof(fp))
		{
			*findchar = NULL;
			return -1;
		}

		// マルチバイト文字文字検知
		else if (!IsDBCSLeadByte(CheckChar))
		{
			// 特殊文字検知
			if (CheckChar == MOJI_INDENT || CheckChar == MOJI_COMMA)
			{
				if (!isDoubleQuotation_Area)
				{
					break;
				}
			}
			else if (CheckChar == MOJI_DQUOTATION)
			{
				if (isDoubleQuotation_Area)
				{
					char next_moji = fgetc(fp);

					if (next_moji != MOJI_DQUOTATION)
					{
						isDoubleQuotation_Area = false;
						fseek(fp, -1, SEEK_CUR);
					}
					else
					{
						move++;
					}
				}
				else
				{
					isDoubleQuotation_Area = true;
				}
			}
		}
		else
		{
			fseek(fp, 1, SEEK_CUR);
			move++;
		}


	}
	


	// 結果を返す
	switch (CheckChar)
	{
	case MOJI_INDENT:
	case MOJI_COMMA:
		*findchar = CheckChar;
		return move;
	default:
		return -1;
	}

}

/*=====================================================================
＊＊引数ミス確認変数＊＊(cpp内限定)
エラー関数
戻り値 : HRESULT : ミスがあればエラーを返す
引数　 :
	CreateCSVFromFile の全引数
=====================================================================*/
HRESULT CreateCSVFromFile_ErrorCheckArgument(const char *filename, char *ReadTypeArray, CSV_CELRANGE *CSV_CelRange, CSV_FILE **Csv, DWORD flag)
{
	DWORD Error = 0x0;
	char *ErrStr = NULL;

	if (flag&CSV_OPTION_SHOWERR)
	{
		ErrStr = (char *)calloc(1024, sizeof(char));

		strcpy_s(ErrStr, 1024, _T("ERR:引数が正しくありません\n詳細:"));
	}

	// ---------- エラーチェック ----------

	// 1,ファイル名引数のヌルチェック
	if (filename == NULL)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[filename]がnullを参照しています。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x1;
	}

	// 1,拡張子のチェック
	if (_strcmpi((filename + (strlen(filename) - 4)), _T(".csv")) && (_strcmpi((filename + (strlen(filename) - 4)), _T(".txt"))))
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[filename]にあるファイルアドレスの拡張子が.csv(又は.txt)ではありません。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x2;
	}

	// 2,ヌルチェック
	if (ReadTypeArray == NULL)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[ReadTypeArray]がnullを参照しています。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x4;
	}

	// 3,Column_Startチェック
	if (CSV_CelRange->Column_Start < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Column_Start]に負の数を指定しています。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x8;
	}

	// 3,Line_Startチェック
	if (CSV_CelRange->Line_Start < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Line_Start]に負の数を指定しています。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x10;
	}

	// 3,Column_Countチェック
	if (CSV_CelRange->Column_Count > MAX_READ_COLUMN)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char ErrTxt[128];
			sprintf_s(ErrTxt, _T("\n\t[CSV_CelRange.Column_Count]の数値は%dまでです(現在:%d)"), MAX_READ_COLUMN, CSV_CelRange->Column_Count);
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x20;
	}
	else if (CSV_CelRange->Column_Count < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Column_Count]->サイズ０未満は無効です。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x40;
	}

	// 3,Line_Countチェック
	if (CSV_CelRange->Line_Count < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Line_Count]->サイズ０未満は無効です。");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x80;
	}

	// 特定要素がエラーでなければ
	if (!((Error & 0x4) || ((Error & 0x20) ^ (Error & 0x40))))
	{

		// サイズが0指定であれば引数の配列を基準で考える
		if (CSV_CelRange->Column_Count == 0)
		{
			CSV_CelRange->Column_Count = (int)strlen(ReadTypeArray);
		}


		// 2と3,Column_Countと配列のチェック
		for (int i = 0; i < CSV_CelRange->Column_Count; i++)
		{
			// フォーマットのチェック
			switch (ReadTypeArray[i])
			{
			case CSV_INT:
			case CSV_FLOAT:
			case CSV_STRING:
				break;

			default:
				Error |= 0x100;
				Error |= (i & 0xff) << 24;	// 左シフト
				break;
			}

			// エラー出たらループ抜ける
			if (Error & 0x100)
			{
				if (flag&CSV_OPTION_SHOWERR)
				{
					char ErrTxt[128];
					int val = (0xff & (Error >> 24));	// からのの右シフト
					sprintf_s(ErrTxt, _T("\n\t配列[ReadTypeArray[%d]]の値がフォーマットに対応していません。(現在:%d)"), val, ReadTypeArray[val]);
					strcat_s(ErrStr, 1024, ErrTxt);
				}
				break;
			}
		}
	}

	// ---------- エラー確認 ----------
	if (Error)
	{
		CreateCSVFromFile_OutputError(Csv, ErrStr);

		if (flag&CSV_OPTION_SHOWERR)
		{
			ZeroMemory(ErrStr, sizeof(char) * 1024);
			free(ErrStr);
		}
			return E_INVALIDARG;
	}
	else
	{
		return S_OK;
	}
}

/*=====================================================================
＊＊エラー処理関数＊＊(cpp内限定)
エラー関数
戻り値 : HRESULT : 設定するエラーを返す
引数　 :
	IN  const char	*String	:貼る文字
	OUT CSV_FILE	**Csv	:読み込みが終了したエクセルデータ(失敗したらアドレスがNULL)
=====================================================================*/
HRESULT CreateCSVFromFile_OutputError( CSV_FILE **Csv, const char *String)
{

	Csv = NULL;	// アドレスをヌル

	if (String != NULL)
	{
		MessageBox(NULL, String, _T("CSVシステム"), NULL); // メッセボックスを出す
	}

	return E_FAIL;
}

