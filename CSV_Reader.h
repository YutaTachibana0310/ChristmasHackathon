/**********************************************************************
[[�O���t�@�C���w�b�_�[(CSV_Reader.h)]] Version 0.0.5
	��ҁF���c�@�^�K

	CSV_Reade.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once

#include <Windows.h>
/*==========================================================================
CSV_Reader �}�N���Ǘ�
==========================================================================*/

//---------------------------------------------------------------------
//	_ARRAY�}�N��
//		CSV���珑���o���ꂽ�t�@�C����1�����z��̂��߁A�}�N����
//		2�����z��Ɏ����Ă��܂�!
//
//		_ARRAY(a,b,c)	�� a->Array[b][c]�Ɠ��`�ł�
//		��a�́@CSV_FILE* �^�ł���K�v������܂��B
//---------------------------------------------------------------------
#define _ARRAY(a,b,c)		(a->Array[b * (a->Column_Size) + c])


//---------------------------------------------------------------------
//	_ARRAYSTR�}�N��
//		��L_ARRAY�}�N����txt�ǂݍ��ݔ�
//
//		 _ARRAYSTR(a,b,c)��a�V�[�gb�s�ɂ���0���c�����ڂ��o�͂��܂�
//		��a�́@CSV_FILE* �^�ł���K�v������܂��B
//---------------------------------------------------------------------
#define _ARRAYSTR(a,b,c)	(a->Array[b * (a->Column_Size)]._str[c])


//---------------------------------------------------------------------
//	CreateCSVFromFile�t���O�w��}�N��
//		�֐�CreateCSVFromFile�ł̓I�v�V����������flag�Ŏw�肷��
//		2�ȏ�I�v�V��������ꍇ��or���Z�q�őg�ݍ��킹��(|)
//		���ꕔ�}�N���͑g�ݍ��킹�s��
//
//		NO_CSV_OPTION			:�I�v�V�����w��Ȃ��B�f�t�H���g����
//									���S�Ă�ϸۂƑg�ݍ��킹�s��
//
//		CSV_OPTION_SHOWERR		:�G���[���Ƀ��b�Z�[�W�{�b�N�X
//		CSV_OPTION_SHOWTIME		:�������Ԃ��}�C�N���b�Ń��b�Z�[�W�{�b�N�X
//		CSV_OPTION_SHOWALL		:���b�Z�[�W�{�b�N�X�n�S�Ă��o�͂���
//
//---------------------------------------------------------------------

#define NO_CSV_OPTION			(0)

#define CSV_OPTION_SHOWERR		(0x1)
#define CSV_OPTION_SHOWTIME		(0x2)

#define CSV_OPTION_SHOWALL		(CSV_OPTION_SHOWERR|CSV_OPTION_SHOWTIME)



//---------------------------------------------------------------------
//	SAFE_RELEASE�}�N��
//		CreateCSVFromFile�ō쐬����CSV_FILE *�^�̃Z���V�[�g���J������
//		�}�N���B�@���łɃ|�C���^���k���ɂ��܂��B
//
//		SAFE_RELEASE(x) ��NULL�̏ꍇ,�J���֐������s���܂���(nullptr�̈�)
//		��x��CSV_FILE* �^�ł���K�v������܂��B
//---------------------------------------------------------------------
#define SAFE_RELEASE(x)			{ if(x) { (x)->Release(); (x) = NULL; } }

/*==========================================================================
CSV_Reader �\����,�񋓑́A���p�̐錾
==========================================================================*/

//---------------------------------------------------------------------
//	�Z���^�C�v�񋓌^
//		�֐�CreateCSVFromFile�ł�char�^�z��ł��̗񋓌^���w�肷��
//
//		CSV_INT		:�����^�f�[�^
//		CSV_FLOAT	:�����^�f�[�^
//		CSV_STRING	:������f�[�^
//---------------------------------------------------------------------
enum CSVREAD_TYPE {
	CSV_INT		= 'd',	
	CSV_FLOAT	= 'f',
	CSV_STRING	= 's'
};

//---------------------------------------------------------------------
//	�ǂݎ��Z���͈͎w��\����
//		�֐�CreateCSVFromFile�ł͖{�\���̂�p���ēǂݎ��͈͂��w�肷��
//
//		int Column_Start;	: �ǂݎ��O�ɓǂݔ�΂���
//		int Line_Start;		: �ǂݎ��O�ɓǂݔ�΂��s��
//
//		int Column_Count;	: �ǂݎ���̑傫��
//							0�w��̏ꍇ�A����ReadTypeArray�Ŕ��f���܂����A
//							������̍Ō��\0�K�{�ł�
//
//		int Line_Count;		: �ǂݎ��s�̑傫��
//							0�w��̏ꍇ�A��ԉ��̍s�܂Ŏ����I�ɓǂݍ���
//							�܂����A�����ׂł��B
//---------------------------------------------------------------------
typedef struct CSV_CELRANGE {
	int Column_Start;	
	int Line_Start;		
	int Column_Count;	
	int Line_Count;		
}CSV_CELRANGE;

//---------------------------------------------------------------------
//	�Z���f�[�^���p��
//		�����I�ȃZ���̃f�[�^
//		�� ���̒l�͕ύX���Ă͂����܂���I�I��
//
//		int		_int;	: �����^�f�[�^
//		float	_float;	: �����^�f�[�^
//		char *	_str	: ������f�[�^�i�擪�|�C���^�j
//---------------------------------------------------------------------
union CEL_DATA {
	int		_int;	
	float	_float;	
	char *	_str;
};

//---------------------------------------------------------------------
//	�V�[�g�f�[�^�\����
//		CreateCSVFromFile�֐���������\����
//
//	�������������� ���̕ϐ���Release()�Ń�������������Ă��������I
//
//		CEL_DATA		*Array;			: �Z���̐擪�f�[�^(�z��)
//		CSVREAD_TYPE	*TypeArray;		: �^�C�v�̗�^�C�v(�z��)
//		char			*StringTop;		: ������Q�i�|�C���^�z��j
//		int			Column_Size;		: ��T�C�Y
//		int			Line_Size;			: �s�T�C�Y
//
// ----�����o�֐� 
//		void Release();		:�Z���f�[�^���J������
//							(���s��SAFE_RELEASE�𐄏����܂�)
//							�� �֐����s��̃A�h���X�ɃA�N�Z�X�͋֎~�ł�
//		
//		CEL_DATA Sum(int Column);	
//							:�w�肵����̍��v�l��Ԃ�(������^�ł͎g�p�s��)
//
//		CEL_DATA Average(int Column);
//							:�w�肵����̕��ϒl��Ԃ�(������^�ł͎g�p�s��)
//
//		CEL_DATA Min(int Column);
//							:�w�肵����̍ŏ��l��Ԃ�(������^�ł͎g�p�s��)
//
//		CEL_DATA Max(int Column);
//							:�w�肵����̍ŏ��l��Ԃ�(������^�ł͎g�p�s��)
//---------------------------------------------------------------------
typedef struct CSV_FILE {
	CEL_DATA		*Array;
	CSVREAD_TYPE	*TypeArray;
	char			*StringTop;
	 int			Column_Size;	
	 int			Line_Size;		

//------ �ȉ��A�����o�֐� ------//

	void Release();
	CEL_DATA Sum(int Column);
	CEL_DATA Average(int Column);
	CEL_DATA Min(int Column);
	CEL_DATA Max(int Column);

}CSV_FILE;



/*==========================================================================
CSV_Reader �v���g�^�C�v�錾
==========================================================================*/
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
HRESULT CreateCSVFromFile(const char *filename, char *ReadTypeArray, CSV_CELRANGE CSV_CelRange, CSV_FILE **Csv, DWORD flag = 0x0);
