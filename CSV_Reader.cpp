/**********************************************************************
[[�O���t�@�C���v���O����(CSV_Reader.cpp)]]	Version 0.0.5
	��ҁF���c�@�^�K

	CSV�t�@�C���ǂݍ��݂Ɋւ���v���O����
***********************************************************************/

#include <Windows.h>

#include <stdio.h>
#include <tchar.h>
#include <malloc.h>

#include "CSV_Reader.h"


//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define MAX_READ_COLUMN	(255)	// ��̍ő吔

// ���ꕶ��
#define MOJI_COMMA			(',')
#define MOJI_INDENT			('\n')
#define MOJI_DQUOTATION		('"')

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


// �����^�\����(���Ƃŕ����Q�ɂ��邽�߂̈ꎞ�ۊǏ�)
typedef struct STR_LIST {
	char		*Str;
	int			Line;
	int			Column;
	STR_LIST	*next_pt;
}STR_LIST;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

int CheckSpecificCSVMoji(FILE *fp, char *findchar);

HRESULT CreateCSVFromFile_ErrorCheckArgument(const char *filename, char *ReadTypeArray, CSV_CELRANGE *CSV_CelRange, CSV_FILE **Csv, DWORD flag);

HRESULT CreateCSVFromFile_OutputError(CSV_FILE **Csv, const char *String = NULL);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

/*=====================================================================
����CSV�쐬�֐�����
�߂�l : HRESULT : �G���[���b�Z�̏o��
�����@ : 
	IN	const char		*filename		:�t�@�C����
	IN  char			*ReadTypeArray	:�ǂݍ��ރt�H�[�}�b�g��z��ɂ������
	IN  CSV_CELRANGE	CSV_CelRange	:CSV�t�@�C����ł̓ǂݍ��ޏꏊ���\���̂Ŏw��
	OUT CSV_FILE		**Csv			:�ǂݍ��݂��I�������G�N�Z���f�[�^(���s������A�h���X��NULL)
	IN  DWORD			flag			:[�K�{�����ł͂Ȃ�]flag	(�f�t�H���g=0)
=====================================================================*/
HRESULT CreateCSVFromFile(const char *filename, char *ReadTypeArray, CSV_CELRANGE CSV_CelRange, CSV_FILE **Csv, DWORD flag)
{
	LARGE_INTEGER	tBegin, tEnd, tFreq;				// ������\�@�p�ϐ��錾
	FILE			*fp;								// �t�@�C���|�C���^
	HRESULT			hr;									// ���ʏo��
	int				TotalCelSize = 0;					// ���Z����
	CSV_FILE		*In_Pack_pt		= NULL;				// �o�͂���CSV�t�@�C��
	bool			isNOTeof = true;					// �e�L�X�g�I�[�܂ōs���Ă��܂�����
	int				SheetStrLength = 0;					// ������̑��v�f��
	STR_LIST		*Top_pt = NULL, *work_pt = NULL;	// �����񃊃X�g


	// ���ԕ\���I�v�V����
	if (flag&CSV_OPTION_SHOWTIME)
	{
		QueryPerformanceFrequency(&tFreq);	// CPU�p�t�H�[�}���X�J�E���^�i1�b������̃J�E���g���j�擾
		QueryPerformanceCounter(&tBegin);	// �J�n���Ԃ̕ۑ�
	}

	// �����̃`�F�b�N
	hr = CreateCSVFromFile_ErrorCheckArgument(filename, ReadTypeArray, &CSV_CelRange, Csv, flag);
	if (FAILED(hr))return hr;

	fopen_s(&fp, filename, "r"); // �e�L�X�g�ǂݍ��݃��[�h

	// �t�@�C�����J���Ȃ������ꍇ(���݂��Ȃ�)
	if (fp == NULL)
		return CreateCSVFromFile_OutputError(Csv,
		(flag&CSV_OPTION_SHOWERR) ? _T("ERR:�t�@�C�������݂��܂���\n����:\n\t�t�@�C���A�h���X���̊m�F\n\tCSV���݂̊m�F\n\t�Y���t�@�C���𑼃A�v���P�[�V�����ŊJ���Ă��Ȃ����m�F") : NULL);


	// �X�^�[�g�s�܂Ńt�@�C���|�C���^�������Ă���
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
					sprintf_s(Errstr, _T("ERR:�J�n�s�܂œ��B�ł��܂���ł����B(�F���s:%d)\n����:\n\tCSV�t�@�C���̃`�F�b�N\n\t����[CSV_CelRange.Line_Start]�̃`�F�b�N"), i + 1);
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

	// �����̍s�J�E���g��0�Ȃ�T���ŋ��߂�
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
						return CreateCSVFromFile_OutputError(Csv, _T("ERR:�ǂݎ��s�����݂��܂���B\n����:\n\tCSV�t�@�C���̃`�F�b�N\n\t����[CSV_CelRange.Line_Start]�ƈ���[CSV_CelRange.Line_Count]�̔z���~�X�m�F"));
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


	// 1�������I�z�񃁃����m��
	TotalCelSize = CSV_CelRange.Line_Count*CSV_CelRange.Column_Count;
	In_Pack_pt = (CSV_FILE*)malloc(sizeof(CSV_FILE) + (sizeof(CEL_DATA)*TotalCelSize) + 
		(sizeof(CSVREAD_TYPE)*CSV_CelRange.Column_Count));
	ZeroMemory(In_Pack_pt, sizeof(In_Pack_pt));

	In_Pack_pt->Array = (CEL_DATA*)(In_Pack_pt + 1);
	In_Pack_pt->TypeArray = (CSVREAD_TYPE *)(In_Pack_pt->Array + TotalCelSize);

	//CSV_FILE	�ɒl����
	In_Pack_pt->Column_Size = CSV_CelRange.Column_Count;
	In_Pack_pt->Line_Size = CSV_CelRange.Line_Count;
	// �Z���ǂݍ��݃^�C�v��z��ő��
	for (int i = 0; i < CSV_CelRange.Column_Count; i++)
	{
		In_Pack_pt->TypeArray[i] = (CSVREAD_TYPE)ReadTypeArray[i];
	}


	// �s���Ƃɑ�����s��
	for (int i = 0; i < CSV_CelRange.Line_Count; i++)
	{
		bool isColumn_Data = true;	// ��f�[�^�͂܂��L����

		if (isNOTeof == false)
		{
			isColumn_Data = false;
		}

		// �X�^�[�g��܂Ńt�@�C���|�C���^�������Ă���
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

		// CSV��̓ǂݎ��
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

			// �Z���ǂݍ���
			if (isColumn_Data&&result > 0)
			{
				// �X�^�b�N�m��
				char *PullString = (char *)_malloca(sizeof(char)*result);
				ZeroMemory(PullString, sizeof(char)*result);

				// �ǂݍ���
				fread(PullString, sizeof(char), result - 1, fp);
				fseek(fp, 1, SEEK_CUR);

				// "�`�F�b�N
				if (PullString[0] == MOJI_DQUOTATION)
				{
					int DoubleQuotation_Count = 0;

					// �ŏ��̕����͏���
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
								// ��ڂ�"��������
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

				// �Z���^�C�v���Ƃ̏���
				switch (ReadTypeArray[j])
				{
				case CSV_INT:
					sscanf_s(PullString, "%d", &_ARRAY(In_Pack_pt, i, j)._int);
					break;
				case CSV_FLOAT:
					sscanf_s(PullString, "%f", &_ARRAY(In_Pack_pt, i, j)._float);
					break;
				case CSV_STRING:
					// ������^�̓f�[�^���s��̂��߈�x���X�g�ɓ���Ă��當����Q�ɑ������
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

				_freea(PullString);	// �J��
			}
			else
			{
				// �f�[�^�Ȃ�������
				_ARRAY(In_Pack_pt, i, j)._int = 0;
			}

			if (readchar == MOJI_INDENT)
			{
				isColumn_Data = false;
			}

		}

		fseek(fp, -1, SEEK_CUR);

		// ���s��T��
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

	fclose(fp);		// �t�@�C�������

	// ������̊i�[(�����񂪑��݂���ꍇ)
	if (SheetStrLength > 0)
	{
		STR_LIST *keep_pt = NULL;
		int StringArray_Move = 0;

		// �傫����ς���(�ꏊ��)
		In_Pack_pt = (CSV_FILE *)realloc(In_Pack_pt, (sizeof(char)*SheetStrLength) + (sizeof(CSV_FILE)) +
			(sizeof(CEL_DATA)*TotalCelSize) + (sizeof(CSVREAD_TYPE)*CSV_CelRange.Column_Count));

		In_Pack_pt->Array = (CEL_DATA*)(In_Pack_pt + 1);
		In_Pack_pt->TypeArray = (CSVREAD_TYPE *)(In_Pack_pt->Array + TotalCelSize);

		In_Pack_pt->StringTop = (char *)(In_Pack_pt->TypeArray + CSV_CelRange.Column_Count);
		ZeroMemory(In_Pack_pt->StringTop, sizeof(char)*SheetStrLength);

		work_pt = Top_pt;

		// ���ԂɃ|�C���^
		while (work_pt != NULL)
		{
			strcpy_s(&In_Pack_pt->StringTop[StringArray_Move], sizeof(char)*(strlen(work_pt->Str) + 1), work_pt->Str);
			
			// ������Q�Ƃ̃����N
			_ARRAY(In_Pack_pt, work_pt->Line, work_pt->Column)._str = &In_Pack_pt->StringTop[StringArray_Move];

			// ������̒������Z
			StringArray_Move += (int)strlen(work_pt->Str) + 1;

			// �J��
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

	*Csv = In_Pack_pt;		// �f�[�^�o��

	// ���ԕ\���I�v�V����
	if (flag&CSV_OPTION_SHOWTIME)
	{
		char TimeStr[128];
		QueryPerformanceCounter(&tEnd);
		sprintf_s(TimeStr, _T("CSV�f�[�^��������:\n %f (s)"), (double)(tEnd.QuadPart - tBegin.QuadPart) / (double)tFreq.QuadPart);
		MessageBox(NULL, TimeStr, _T("CSV�V�X�e��"), NULL);
	}

	return S_OK;
}

/*=====================================================================
����CSV_FILE:: Release()�֐�����
CSV_FILE�J���֐�
�߂�l :void
�����@ :void
=====================================================================*/
void CSV_FILE::Release()
{

	// �������J�� �T��...�E....�i��......
	free(this);

	return;
}

/*=====================================================================
����CSV_FILE:: Sum()�֐�����
CSV_FILE ���v�Z�o�֐�
�߂�l :CEL_DATA
�����@ :
IN	int Column	:���Z�����
=====================================================================*/
CEL_DATA CSV_FILE::Sum(int Column)
{
	CEL_DATA Sum_Data;
	Sum_Data._int = 0;

	if (Column<0 || Column>Column_Size)return Sum_Data;

	// ���v���Z����
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
����CSV_FILE:: Average()�֐�����
CSV_FILE ���ώZ�o�֐�
�߂�l :CEL_DATA
�����@ :
IN	int Column	:���ώZ�o�����
=====================================================================*/
CEL_DATA CSV_FILE::Average(int Column)
{
	CEL_DATA Average_Data = Sum(Column);

	if (Column<0 || Column>Column_Size)return Average_Data;

	// ���ώZ�o����
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
����CSV_FILE:: Min()�֐�����
CSV_FILE ���ώZ�o�֐�
�߂�l :void
�����@ :
IN	int Column	:�ŏ��l�����߂��
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

		// �ŏ��l�����߂鏈��
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

		// �ŏ��l�����߂鏈��
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
����CSV_FILE:: Max()�֐�����
CSV_FILE ���ώZ�o�֐�
�߂�l :void
�����@ :
IN	int Column	:�ő�l�����߂��
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

		// �ő�l�����߂鏈��
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

		// �ő�l�����߂鏈��
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
�������ꕶ���܂Ŕ�Ԋ֐�����(cpp������)
�߂�l :int :�ړ������t�@�C���|�C���^�̃o�C�g���@(EOF��-1)
�����@ :
	IN	FILE *fp			: �g�p���Ă���t�@�C���|�C���^
	OUT	char *findchar		: �ǂ̓��ꕶ�������������o��(1�o�C�g)
=====================================================================*/
int CheckSpecificCSVMoji(FILE *fp, char *findchar)
{
	int move = 0;
	char CheckChar;
	bool isDoubleQuotation_Area = false;

	// ���蕶���{��
	while (1)
	{
		CheckChar = fgetc(fp);

		move++;

		// �ǂ����Ă��`�F�b�N
		if (feof(fp))
		{
			*findchar = NULL;
			return -1;
		}

		// �}���`�o�C�g�����������m
		else if (!IsDBCSLeadByte(CheckChar))
		{
			// ���ꕶ�����m
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
	


	// ���ʂ�Ԃ�
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
���������~�X�m�F�ϐ�����(cpp������)
�G���[�֐�
�߂�l : HRESULT : �~�X������΃G���[��Ԃ�
�����@ :
	CreateCSVFromFile �̑S����
=====================================================================*/
HRESULT CreateCSVFromFile_ErrorCheckArgument(const char *filename, char *ReadTypeArray, CSV_CELRANGE *CSV_CelRange, CSV_FILE **Csv, DWORD flag)
{
	DWORD Error = 0x0;
	char *ErrStr = NULL;

	if (flag&CSV_OPTION_SHOWERR)
	{
		ErrStr = (char *)calloc(1024, sizeof(char));

		strcpy_s(ErrStr, 1024, _T("ERR:����������������܂���\n�ڍ�:"));
	}

	// ---------- �G���[�`�F�b�N ----------

	// 1,�t�@�C���������̃k���`�F�b�N
	if (filename == NULL)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[filename]��null���Q�Ƃ��Ă��܂��B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x1;
	}

	// 1,�g���q�̃`�F�b�N
	if (_strcmpi((filename + (strlen(filename) - 4)), _T(".csv")) && (_strcmpi((filename + (strlen(filename) - 4)), _T(".txt"))))
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[filename]�ɂ���t�@�C���A�h���X�̊g���q��.csv(����.txt)�ł͂���܂���B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x2;
	}

	// 2,�k���`�F�b�N
	if (ReadTypeArray == NULL)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[ReadTypeArray]��null���Q�Ƃ��Ă��܂��B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x4;
	}

	// 3,Column_Start�`�F�b�N
	if (CSV_CelRange->Column_Start < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Column_Start]�ɕ��̐����w�肵�Ă��܂��B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x8;
	}

	// 3,Line_Start�`�F�b�N
	if (CSV_CelRange->Line_Start < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Line_Start]�ɕ��̐����w�肵�Ă��܂��B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x10;
	}

	// 3,Column_Count�`�F�b�N
	if (CSV_CelRange->Column_Count > MAX_READ_COLUMN)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char ErrTxt[128];
			sprintf_s(ErrTxt, _T("\n\t[CSV_CelRange.Column_Count]�̐��l��%d�܂łł�(����:%d)"), MAX_READ_COLUMN, CSV_CelRange->Column_Count);
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x20;
	}
	else if (CSV_CelRange->Column_Count < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Column_Count]->�T�C�Y�O�����͖����ł��B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x40;
	}

	// 3,Line_Count�`�F�b�N
	if (CSV_CelRange->Line_Count < 0)
	{
		if (flag&CSV_OPTION_SHOWERR)
		{
			char *ErrTxt = (char *)_T("\n\t[CSV_CelRange.Line_Count]->�T�C�Y�O�����͖����ł��B");
			strcat_s(ErrStr, 1024, ErrTxt);
		}
		Error |= 0x80;
	}

	// ����v�f���G���[�łȂ����
	if (!((Error & 0x4) || ((Error & 0x20) ^ (Error & 0x40))))
	{

		// �T�C�Y��0�w��ł���Έ����̔z�����ōl����
		if (CSV_CelRange->Column_Count == 0)
		{
			CSV_CelRange->Column_Count = (int)strlen(ReadTypeArray);
		}


		// 2��3,Column_Count�Ɣz��̃`�F�b�N
		for (int i = 0; i < CSV_CelRange->Column_Count; i++)
		{
			// �t�H�[�}�b�g�̃`�F�b�N
			switch (ReadTypeArray[i])
			{
			case CSV_INT:
			case CSV_FLOAT:
			case CSV_STRING:
				break;

			default:
				Error |= 0x100;
				Error |= (i & 0xff) << 24;	// ���V�t�g
				break;
			}

			// �G���[�o���烋�[�v������
			if (Error & 0x100)
			{
				if (flag&CSV_OPTION_SHOWERR)
				{
					char ErrTxt[128];
					int val = (0xff & (Error >> 24));	// ����̂̉E�V�t�g
					sprintf_s(ErrTxt, _T("\n\t�z��[ReadTypeArray[%d]]�̒l���t�H�[�}�b�g�ɑΉ����Ă��܂���B(����:%d)"), val, ReadTypeArray[val]);
					strcat_s(ErrStr, 1024, ErrTxt);
				}
				break;
			}
		}
	}

	// ---------- �G���[�m�F ----------
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
�����G���[�����֐�����(cpp������)
�G���[�֐�
�߂�l : HRESULT : �ݒ肷��G���[��Ԃ�
�����@ :
	IN  const char	*String	:�\�镶��
	OUT CSV_FILE	**Csv	:�ǂݍ��݂��I�������G�N�Z���f�[�^(���s������A�h���X��NULL)
=====================================================================*/
HRESULT CreateCSVFromFile_OutputError( CSV_FILE **Csv, const char *String)
{

	Csv = NULL;	// �A�h���X���k��

	if (String != NULL)
	{
		MessageBox(NULL, String, _T("CSV�V�X�e��"), NULL); // ���b�Z�{�b�N�X���o��
	}

	return E_FAIL;
}

