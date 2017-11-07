//--------------------------------------------------------------------------------------
// File: GameState.cpp
//
//--------------------------------------------------------------------------------------
#include "..\..\Global\Global.h"
#include "..\FadeState\FadeState.h"
#include "..\GameState\GameState.h"
#include "..\ResultState\ResultState.h"

#include "..\..\NeuralNetwork\MyNeuralNetwork.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

int GetRandomNumber(int min, int max, bool seed);

//	�j���[�����l�b�g���[�N---------------------
NeuralNetwork	TheBrain;

//	�w�K����f�[�^�s��(�c�̐�)
const int DataNumber = 56;

//	TrainingSet[DataNumber][���ڐ�(���̐�)] =
double TrainingSet[DataNumber][18] =
{
	//	�o����΋Z�d�������擾�ł�����d���ɋZ���~�邱�Ƃ��ł���悤�ɂȂ�B
	//	�������͍U���Ȃ̂��h��Ȃ̂��̃X�e�[�g��CPU���ǂݍ���ŏ󋵂ɉ����ėL���ȋZ���ӂ�悤��

	//	�����@0.0�͓���   0.1��5A or ����   0.2��5B or 2A or 2C   0.3��5C  0.4�����������̋����@0.5���ːi�Z��W�����v���d�|���鋗���@����ȍ~�͐ڋ߂��鋗���Ŕ�ѓ�����ȊO�͒P���ɋ�_�Ȃ�_�b�V��
	//	���  0.5�͗����A0.2�͂��Ⴊ��ł�����
	//	�Q�[�W ���̂Ƃ���R�}���h���͂�CPU�ɂł��ĂȂ��̂Ŏg��������

//	����, �̗�, ���, ����, (4��)	  5A,�@ 2A, �@5B, �@6B,   5C, �R��, �O��, ���, �_�b, �o�b, �K�[, ���W, �O�W, ��W, (14��)

	//�@�v���C���[�̗̑͂������A�^�C������7�b�ȏ�(7�s)
	0.05f, 0.5f, 0.5f, 0.7f,		0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.5f, 0.7f,			0.3f, 0.1f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.2f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.5f, 0.7f,			0.2f, 0.2f, 0.3f, 0.0f, 0.0f, 0.1f, 0.1f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.5f, 0.7f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//�@�v���C���[�̗̑͂������A�^�C������7�b�ȉ�(7�s)
	0.05f, 0.5f, 0.5f, 0.1f,		0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.5f, 0.1f,			0.3f, 0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.1f, 0.2f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.5f, 0.1f,			0.2f, 0.3f, 0.3f, 0.0f, 0.1f, 0.1f, 0.2f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.5f, 0.1f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 


	//	�v���C���[�̗̑͂��~���A�^�C������7�b�ȏ�(7�s)
	0.05f, 0.1f, 0.5f, 0.7f,		0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.5f, 0.7f,			0.3f, 0.1f, 0.0f, 0.2f, 0.0f, 0.1f, 0.0f, 0.1f, 0.1f, 0.1f, 0.3f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.1f, 0.5f, 0.7f,			0.2f, 0.2f, 0.3f, 0.0f, 0.1f, 0.2f, 0.2f, 0.1f, 0.1f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.5f, 0.7f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.1f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//	�v���C���[�̗̑͂��~���A�^�C������7�b�ȉ�(7�s)
	0.05f, 0.1f, 0.5f, 0.1f,		0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.5f, 0.1f,			0.2f, 0.1f, 0.0f, 0.2f, 0.0f, 0.4f, 0.0f, 0.1f, 0.0f, 0.0f, 0.3f, 0.2f, 0.0f, 0.1f, 
	0.2f, 0.1f, 0.5f, 0.1f,			0.2f, 0.2f, 0.3f, 0.0f, 0.1f, 0.3f, 0.2f, 0.0f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.5f, 0.1f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.2f, 0.1f, 0.2f, 0.0f, 0.2f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.3f, 
	1.0f, 0.1f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.7f, 


	//	------------------------------------------------ �������@�����Ⴊ��

	//	�����@0.0�͓���   0.1��5A or ����   0.2��5B or 2A or 2C   0.3��5C  0.4�����������̋����@0.5���ːi�Z��W�����v���d�|���鋗���@����ȍ~�͐ڋ߂��鋗���Ŕ�ѓ�����ȊO�͒P���ɋ�_�Ȃ�_�b�V��


//	����, �̗�, ���, ����, (4��)	  5A,�@ 2A, �@5B, �@6B,   5C, �R��, �O��, ���, �_�b, �o�b, �K�[, ���W, �O�W, ��W, (14��)

	//	�v���C���[�����Ⴊ��ő̗͂������A�^�C������7�b�ȏ�(7�s)
	0.05f, 0.5f, 0.2f, 0.7f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.2f, 0.7f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.1f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.2f, 0.7f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.2f, 0.7f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//	�v���C���[�����Ⴊ��ő̗͂������A�^�C������7�b�ȉ�(7�s)
	0.05f, 0.5f, 0.2f, 0.1f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.2f, 0.1f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.1f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.2f, 0.1f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.2f, 0.1f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//�@�v���C���[�����Ⴊ��ő̗̓~���A�^�C������7�b�ȏ�(7�s)
	0.05f, 0.1f, 0.2f, 0.7f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.2f, 0.7f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.1f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.1f, 0.2f, 0.7f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.2f, 0.0f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.2f, 0.7f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.1f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//�@�v���C���[�����Ⴊ��ő̗̓~���A�^�C������7�b�ȉ�(7�s)
	0.05f, 0.1f, 0.2f, 0.1f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.2f, 0.1f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.1f, 0.2f, 0.1f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.2f, 0.1f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.2f, 
	1.0f, 0.1f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.7f, 

//	����, �̗�, ���, ����, (4��)	  5A,�@ 2A, �@5B, �@6B,   5C, �R��, �O��, ���, �_�b, �o�b, �K�[, ���W, �O�W, ��W, (14��)

	//	�����@0.0�`0.1�܂ł͓����@0.1��5A�@0.2��5B�@0.3��5C���͂������@0.4�����������̋����@0.5���ːi�Z��W�����v���d�|���鋗���@����ȍ~�͐ڋ߂��鋗���Ŕ�ѓ�����ȊO�͒P���ɋ�_�Ȃ�_�b�V��
};

int GetRandomNumber(int min, int max, bool seed)
{
	int	number = (((abs(rand()) % (max - min + 1)) + min));
	if (number > max)
		number = max;
	if (number < min)
		number = min;
	return number;
}

void TrainTheBrain(void)
{
	double	error = 1.0;
	int	count = 0;

	//TheBrain.DumpData("PreTraining.txt");

	//	�j���[�����l�b�g���[�N�̏�����

	//	�덷�A���͒l�Ɨ��z�l�̍������������A���x�������؋�

	//	�w�K�̉񐔁@��
	//	1�����		�e�X�g����s�p
	//	4�����		���Ɨ��z�Ȓl�ɂ����ē����Ă����B�ΐ킷��Ȃ炱�ꂪ��ԗ��K�ɂ��Ȃ邵�ΐ�ɂȂ邩��(�@�����l�@)
	//	8�����		�v���O���������ŋ��̒l�Ȃ炱�ꂪ��ԋ����Ȃ邪���[�h(�ǂݍ��ݎ���)�����ɒ����B���Ɗi�Q�[�͖��m�Ȉ�̓����͒��X���݂��Ȃ����ߌ��܂��������Ɋ����ɋ߂Â���K�v�����������Ȃ��B�œK�����ł��ʔ�����ł͂Ȃ��B
	while (error > 0.0001 && count < 40000)
	{
		error = 0.0;
		count++;
		for (int i = 0; i < DataNumber + 1; i++)
		{
			//	���̓f�[�^
			TheBrain.SetInput(0, TrainingSet[i][0]);
			TheBrain.SetInput(1, TrainingSet[i][1]);
			TheBrain.SetInput(2, TrainingSet[i][2]);
			TheBrain.SetInput(3, TrainingSet[i][3]);
			

			//	�o�̓f�[�^
			TheBrain.SetDesiredOutput(0, TrainingSet[i][4]);
			TheBrain.SetDesiredOutput(1, TrainingSet[i][5]);
			TheBrain.SetDesiredOutput(2, TrainingSet[i][6]);
			TheBrain.SetDesiredOutput(3, TrainingSet[i][7]);
			TheBrain.SetDesiredOutput(4, TrainingSet[i][8]);
			TheBrain.SetDesiredOutput(5, TrainingSet[i][9]);
			TheBrain.SetDesiredOutput(6, TrainingSet[i][10]);
			TheBrain.SetDesiredOutput(7, TrainingSet[i][11]);
			TheBrain.SetDesiredOutput(8, TrainingSet[i][12]);
			TheBrain.SetDesiredOutput(9, TrainingSet[i][13]);
			TheBrain.SetDesiredOutput(10, TrainingSet[i][14]);
			TheBrain.SetDesiredOutput(11, TrainingSet[i][15]);
			TheBrain.SetDesiredOutput(12, TrainingSet[i][16]);
			TheBrain.SetDesiredOutput(13, TrainingSet[i][17]);

			TheBrain.FeedForward();
			error += TheBrain.CalculateError();
			TheBrain.BackPropagate();
		}
		error = error / DataNumber;
	}

}

//--------------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------------
GameState::GameState()
	: State(STATE_GAME)		// �X�e�[�g�ԍ�
	, time(GAME_TIME)
	, timespeed(GAME_TIME_SPEED)
	, timecount(0)

	, stop_cnt(0)
	, stop_cnt_max(0)
	, stop_flg(false)

	, game_round(1)
	, round_faze(0)

	, upd_cnt_max(UPDATE_TIME)
	, upd_cnt(0)

	, game_finish_flg(false)
{
	// �G�̃��[�h----------------------------

	// �^�C��
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/GameTime.png", nullptr, &p_time);

	// �̗̓Q�[�W
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/lifegauge.png", nullptr, &p_gauge_life);

	// �p���[�Q�[�W
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/powergauge.png", nullptr, &p_gauge_power);

	// �p���[�Q�[�W���l
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/power_number.png", nullptr, &p_gauge_power_num);

	// CB�A�C�R��
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/CBicon.png", nullptr, &p_cb_icon);

	// �p���[�Q�[�W��n
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_02.png", nullptr, &p_gauge_upper);
	// �p���[�Q�[�W����
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_03.png", nullptr, &p_gauge_under);

	// ���C�t�Q�[�W��n
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_01.png", nullptr, &p_life_gauge_upper);
	// ���C�t�Q�[�W����
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_00.png", nullptr, &p_life_gauge_under);

	// �R���{����
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/combo_number.png", nullptr, &p_combo_num);
	// �R���{���C�h
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/raid.png", nullptr, &p_combo_raid);
	// �R���{�o�[
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/combo_bar.png", nullptr, &p_combo_bar);
	// �R���{�_���[�W
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/damage.png", nullptr, &p_combo_damage);
	// �R���{�_���[�W�i���o�[
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/damage_number.png", nullptr, &p_combo_damage_num);

	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/RoundCall/Gamecall//string_debug.png", nullptr, &p_ko_debug);



	// �^�X�N�o�^
	p_camera = new Camera();

	p_battlecall = new BattleCall( AIN_CLOMA , AIN_CLOMA );

	p_roundcall = new RoundCall( 1 );

	p_winnercall = new WinnerCall();

	p_effectsystem = new EffectSystem();

	p_cutin = new CutInSystem();

	// �w�i�̃G���g���[
	p_background = new BackGround( AIN_CLOMA , p_camera );

	// BGM���[�h
	bgm.Load(L"Data/Sound/Character/Ain_Cloma/mo.mp3");
	//bgm.Play();

	// �L�����N�^�[�̓o�^
	p_Player1P = new Ain_Cloma_Base( p_gauge_life , p_gauge_power , p_gauge_power_num , p_cb_icon , p_gauge_upper , p_gauge_under , p_combo_num , p_combo_raid , p_combo_bar , p_combo_damage , p_combo_damage_num , PLAYER_1P , CHARACTER_COLOR_1 , p_camera , p_background , p_effectsystem, p_cutin, p_winnercall );
	p_Player2P = new Ain_Cloma_Base( p_gauge_life , p_gauge_power , p_gauge_power_num, p_cb_icon, p_gauge_upper, p_gauge_under, p_combo_num, p_combo_raid, p_combo_bar, p_combo_damage, p_combo_damage_num, PLAYER_2P, CHARACTER_COLOR_2, p_camera, p_background , p_effectsystem, p_cutin, p_winnercall);

	p_Player1P->EnemyPointerGetter( p_Player2P );
	p_Player2P->EnemyPointerGetter( p_Player1P );

	// �L�����N�^�[�̃��E���h�R�[���p�^�X�N��o�^
	p_Player1P_rw = new Ain_RW( CHARACTER_COLOR_1 , PLAYER_1P , RIGHT , p_Player1P->PositionGetterX() , p_Player1P->PositionGetterY() , 1 , 2 );
	p_Player2P_rw = new Ain_RW( CHARACTER_COLOR_1 , PLAYER_2P , LEFT , p_Player2P->PositionGetterX() , p_Player2P->PositionGetterY(), 1 , 2 );
	
}


//--------------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------------
GameState::~GameState()
{
	if (p_time) p_time->Release();

	if (p_gauge_life) p_gauge_life->Release();
	if (p_gauge_power) p_gauge_power->Release();
	if (p_gauge_power_num) p_gauge_power_num->Release();
	if (p_cb_icon) p_cb_icon->Release();

	if (p_gauge_upper) p_gauge_upper->Release();
	if (p_gauge_under) p_gauge_under->Release();

	if (p_life_gauge_upper) p_life_gauge_upper->Release();
	if (p_life_gauge_under) p_life_gauge_under->Release();

	if (p_combo_bar) p_combo_bar->Release();
	if (p_combo_damage) p_combo_damage->Release();
	if (p_combo_damage_num) p_combo_damage_num->Release();
	if (p_combo_num) p_combo_num->Release();
	if (p_combo_raid) p_combo_raid->Release();
	
	if (p_ko_debug) p_ko_debug->Release();


	delete p_Player1P_rw;
	delete p_Player2P_rw;

	delete p_cutin;

	delete p_camera;
	delete p_background;
	delete p_effectsystem;

	delete p_battlecall;
	delete p_roundcall;
	delete p_winnercall;

	delete p_Player1P;
	delete p_Player2P;

	bgm.Unload();
}


//--------------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------------
void GameState::Initialize()
{
	//	�j���[�����l�b�g�̊w�K����֐�
	TheBrain.Initialize(4, 14, 14);		// ���͐�, ������, �o�͐�
	TheBrain.SetLearningRate(0.9);		// �w�K�� 0�`1
	TheBrain.SetMomentum(true, 0.9);	// 0�`1;
	TrainTheBrain();
}


//--------------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------------
void GameState::Update()
{
	// ���E���h�t�F�C�Y0 ( �L�����N�^�[�A�s�[�� ) ---------------
	if (round_faze == 0){
		// ���E���h�R�[�����̃L�����N�^�[�X�V
		p_Player1P_rw->Update();
		p_Player2P_rw->Update();
			
		// 1P2P ���A�s�[�����I�������� 
		if ( p_Player1P_rw->NextFlg() == true && p_Player2P_rw->NextFlg() == true )
		round_faze = 1;

		// �{�^�������͂��ꂽ��A�������΂�
		if (g_InputManager->IsKeyDown('D') || g_InputManager->IsButtonDown(XINPUT_GAMEPAD_X))
		{
			// �I�����
			round_faze = 1;
		}
	}

	// ���E���h�t�F�C�Y1 ( �o�g���R�[�� ) -----------------------
	else if (round_faze == 1){
		//�@�L�����N�^�[�X�V
		p_Player1P_rw->Update();
		p_Player2P_rw->Update();

		// �o�g���R�[���̍X�V
		p_battlecall->Update();

		// �o�g���R�[�����I�����Ă���Ύ���
		if (p_battlecall->NextFlg() == true)
		{
			round_faze = 2;
		}

		// �{�^�������͂��ꂽ��A�������΂�
		if (g_InputManager->IsKeyDown('D') || g_InputManager->IsButtonDown(XINPUT_GAMEPAD_X))
		{
			// �I�����
			round_faze = 2;
		}
	}

	// ���E���h�t�F�C�Y2 ����� 3 ---------------------
	else if (round_faze >= 2 && round_faze <= 4 ){

		// ���E���h�t�F�C�Y2 ( ���E���h�R�[�� ) ---------------------
		if (round_faze == 2){

			// ���E���h�R�[���̍X�V
			p_roundcall->Update();

			// ���E���h�R�[�����I�����Ă���Ύ���
			if (p_roundcall->NextFlg() == true)
			{
				round_faze = 3;
			}
		}

		// ���E���h�t�F�C�Y3 ( �ΐ� ) -------------------------------
		else if (round_faze >= 3 && round_faze <= 4){

			// �L�����N�^�[�̍X�V
			CharacterUpdate();

			// �^�C���̍X�V
			TimeUpDate();

			// �ǂ��炩�̗̑͂�0�ɂȂ��Ă�����
			if (p_Player1P->LifeGetter() <= 0 || p_Player2P->LifeGetter() <= 0)
			{
				// �t�F�C�Y��3�Ȃ��
				if (round_faze == 3){
					// �q�b�g�X�g�b�v������
					stop_flg = true;
					stop_cnt = 0;
					stop_cnt_max = GAME_STOP_CNT_MAX;
					// �w�i��U��������
					p_background->BackVibration(50, 10);
					round_faze = 4;
				}

				// �t�F�C�Y��4�Ȃ��
				else if (round_faze == 4) {

					if (stop_cnt == 0){
						// ���K�E�Z���o�I��
						p_background->EndSuperMove();

						// �L�����N�^�[�����ꂼ�ꌈ����Ԃɂ���
						// �_�u���_�E��
						if (p_Player1P->LifeGetter() <= 0 && p_Player2P->LifeGetter() <= 0){
							p_Player1P->StateChanger(AIN_GAME_LOSE);
							p_Player2P->StateChanger(AIN_GAME_LOSE);

							// �h���[�̎��̓Q�[�������ɂȂ�Ȃ��悤�ɃE�B���J�E���g�����Z����
							if (p_Player1P->GetWinCount() <= GAME_ROUND_NUM - 2){
								p_Player1P->SetWinCount();
							}
							if (p_Player2P->GetWinCount() <= GAME_ROUND_NUM - 2){
								p_Player2P->SetWinCount();
							}
						}
						else if (p_Player1P->LifeGetter() <= 0 && p_Player2P->LifeGetter() >= 1){
							p_Player1P->StateChanger(AIN_GAME_LOSE);
							p_Player2P->StateChanger(AIN_GAME_WIN);
						}
						else if (p_Player2P->LifeGetter() <= 0 && p_Player1P->LifeGetter() >= 1){
							p_Player2P->StateChanger(AIN_GAME_LOSE);
							p_Player1P->StateChanger(AIN_GAME_WIN);
						}
						// �t�F�C�Y��i�߂�
						round_faze = 5;
					}
				}
			}
		}
	}
	// ���E���h�t�F�C�Y4 ( ���� ) -----------------------
	else if (round_faze == 5){
		// �E�C�i�[�R�[���X�V
		p_winnercall->UpDate();

		// �L�����N�^�[�̍X�V
		CharacterUpdate();

		// �^�C���̍X�V
		TimeUpDate();
		// �E�C�i�[�R�[�����I����Ă���Ύ��̃��E���h��
		if ((p_winnercall->GetNextFlg() == true)
			|| (p_Player1P->GetRoundDownFlg() == true && p_Player2P->GetRoundDownFlg() == true)){

			// �ǂ��炩����s��悵�Ă�����
			if (p_Player1P->GetWinCount() >= GAME_ROUND_NUM || p_Player2P->GetWinCount() >= GAME_ROUND_NUM){
				round_faze = 6;
			}
			else{
				//�L�����N�^�[�̃X�e�[�^�X������
				p_Player1P->RoundInitialize();
				p_Player2P->RoundInitialize();

				// �����E���h��
				round_faze = 2;

				// �w�i�̏�����
				p_background->Initialize();

				// �t�@�C�i�����E���h�ɂȂ邩�ǂ����̔��f
				if ((p_Player1P->GetWinCount() >= GAME_ROUND_NUM - 1 && p_Player2P->GetWinCount() >= GAME_ROUND_NUM - 1)
					|| (game_round == GAME_ROUND_NUM + 1)){
					game_round = 4;
				}
				game_round++;
				// ���E���h�R�[��������
				p_roundcall->Initialize(game_round);
			}
		}
	}
	// ���E���h�t�F�C�Y4 ( �Q�[������ ) -----------------------
	else if (round_faze == 6){
		// ���U���g�֑J��
		g_pStateManager->PushState(new FadeInState(new ResultState()));
	}

	//�J�����X�V
	p_camera->Update();
	
	// �G�t�F�N�g�X�V
	p_effectsystem->Update();

	// �w�i�X�V
	p_background->Update();
	p_background->BVUpdate();


	//	�j���[�����l�b�g���[�N//////////////////////////////////////////////////////////////////////////////////////////////

	//	���ۂɏo�Ă���f�[�^
	cout << fixed << setprecision(3) << TheBrain.GetOutput(0) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(1) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(2) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(3) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(4) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(5) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(6) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(7) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(8) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(9) << std::endl;

	cout << fixed << setprecision(3) << TheBrain.GetOutput(10) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(11) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(12) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(13) << std::endl;


	//	---------------------------------------------------------	��	AI�̍s�������𔻒f����ׂɊe�l���擾���Ă��܂��B

	//	���͒l�������؂��鎞�ɂ悭�g���Ă�
	/*if (time <= 73)
	{
	time--;
	}*/


	//	decision �́@�������ĈӖ��ł��B

	//	�v���C���[1��2�̋���
	float distance_decision;
	//	1P�̈ʒu����2P�̈ʒu�������āA�������o���B
	distance_decision = fabs(p_Player1P->PositionGetterX() - p_Player2P->PositionGetterX());
	//	���f���ׂ���������ׂɁA����(���l)������ȏ㗣��Ă���Ƃ��͐��l��������B700�h�b�g�ȏ�̊ԍ����͑S�ē������f������悤��
	if (distance_decision >= 700)
	{
		distance_decision = 1000.0f;
	}
	distance_decision = distance_decision / 1000.0f;		//	1000�Ŋ�����0�`1�܂ł̒l������o���B
	//	������0.7���z�����狗����1.0����Ă��鎞�Ɠ�������������悤��
	if (distance_decision >= 0.6)
	{
		distance_decision = 1.0f;
	}


	//	�v���C���[�̃��C�t���擾
	float life_decision;
	life_decision = p_Player1P->LifeGetter();				//	���l���擾
	life_decision = life_decision / 600.0f;					//	�̗�MAX400 ����400�Ŋ����Ă����A���x�����߂�ׂɏ��������ύX�B
	if (life_decision >= 0.4)
	{
		life_decision = 1.0f;
	}


	//	�v���C���[�̏�Ԃ̎擾(�����A���Ⴊ�ݓ�)			stand�Ȃ�0.5�Ȃǂɂ���K�v������	�s����ԁ@MOVE_STAND Common.h
	float playerstate_decision;
	//playerstate_decision = p_Player1P->StateGetter();		//	��Ԃ̎擾
	//	���Ⴊ�ݏ��
	if (p_Player1P->StateGetter() == MOVE_SQUAT || p_Player1P->StateGetter() == MOVE_ATTACK_LOW)
	{
		//float data_a = p_Player1P->StateGetter();
		playerstate_decision = 0.2f;
	}
	//	�������
	if (p_Player1P->StateGetter() == MOVE_STAND)
	{
		playerstate_decision = 0.5f;
	}
	//	�W�����v��
	if (p_Player1P->StateGetter() == MOVE_AIR)
	{
		playerstate_decision = 0.8f;
	}
	//	����ȊO�̎�
	else
	{
		playerstate_decision = 0.5f;
	}


	//	�Q�[�����̎��Ԃ̎擾
	float gametimecount_decision;
	gametimecount_decision = time;							//	�^�C���̐��l���擾
	gametimecount_decision = gametimecount_decision / 77;	//	? 10�Ŋ��������������̂��ȁH	77�b���}�b�N�X

	//	�Q�[�W�̗ʂ̎擾
	//float gaugeamount_decision;
	//gaugeamount_decision = p_Player1P->GetPlayerGauge();	//	�擾���Ă܂��I
	//gaugeamount_decision = gaugeamount_decision / 100;		//	�Q�[�W�̐��l���j���[�����l�b�g���[�N�Ŏg��0.1���0.5�Ƃ��̐��l�Ƃ��Ďg����悤�Ɋ����Ă܂��B
	//if (gaugeamount_decision > 0.5)
	//{
	//	gaugeamount_decision = 1.0;
	//}



	//	�d�����̎��Ԃ��擾 ����������


	//	�s�����j���[�����l�b�g���[�N�ɓ���	(���͔ԍ�,���ׂ����l)
	TheBrain.SetInput(0, distance_decision);				//	��������
	TheBrain.SetInput(1, life_decision);					//	�̗͓���
	TheBrain.SetInput(2, playerstate_decision);				//	1P�̃L������Ԃ����
	TheBrain.SetInput(3, gametimecount_decision);			//	�������̃^�C�������
	//TheBrain.SetInput(4, gaugeamount_decision);			//	�Q�[�W�̓���

	//	FeedForward()�����s���ACPU�̏�Ԃ����肷��
	TheBrain.FeedForward();									//	�w�K�������l


	//��������������������������������������������������������������������������������������������������������
	// AI_�̍s����������
	//��������������������������������������������������������������������������������������������������������
	//	�ړ���U���̕���@//
	//	�������̏o�͂̒l�ň�ԑ傫���l���A�����̏�Ԃ�I��
	switch (TheBrain.GetMaxOutputID())
	{

		//	�E�E�EA�Z�E�E�E  //
	case 0:	//	5A
		// �L�[���͏�Ԃ�"A"�ɂ���
		p_Player2P->SetAiKeyInputNum(KEY_A);
		// �L�[�̓��͏�Ԃ��A�N�e�B�u�ɂ���
		p_Player2P->SetAiKeyInputActive();
		break;

	case 1:	//	2A
		//	������
		p_Player2P->SetAiLeverInputNum(AI_LEVER_DOWN);
		// �L�[���͏�Ԃ�"A"�ɂ���
		p_Player2P->SetAiKeyInputNum(KEY_A);
		// �L�[�̓��͏�Ԃ��A�N�e�B�u�ɂ���
		p_Player2P->SetAiKeyInputActive();
		break;


		//	�E�E�EB�Z�E�E�E  //
	case 2:	//	5B
		// �L�[���͏�Ԃ�"B"�ɂ���
		p_Player2P->SetAiKeyInputNum(KEY_B);
		// �L�[�̓��͏�Ԃ��A�N�e�B�u�ɂ���
		p_Player2P->SetAiKeyInputActive();
		break;

	case 3:	//	6B
		//	�E�����̎�
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}

		//	�������̎�
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}
		// �L�[���͏�Ԃ�"B"�ɂ���
		p_Player2P->SetAiKeyInputNum(KEY_B);
		// �L�[�̓��͏�Ԃ��A�N�e�B�u�ɂ���
		p_Player2P->SetAiKeyInputActive();
		break;



		//	�E�E�EC�Z�E�E�E  //
	case 4:	//	5C
		// �L�[���͏�Ԃ�"C"�ɂ���
		p_Player2P->SetAiKeyInputNum(KEY_C);
		// �L�[�̓��͏�Ԃ��A�N�e�B�u�ɂ���
		p_Player2P->SetAiKeyInputActive();
		break;

		//	�E�E�E�R�}���h�Z�E�E�E  //
	case 5:	//	�R���΂� ����Ȃ��ł�
		// �L�[���͏�Ԃ�"C"�ɂ���
		p_Player2P->SetAiKeyInputNum(KEY_C);
		// �L�[�̓��͏�Ԃ��A�N�e�B�u�ɂ���
		p_Player2P->SetAiKeyInputActive();
		break;

		//	�E�E�E�s���E�E�E  //
	case 6:	//	�O����
		//	�E�����̎�
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}

		//	�������̎�
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}
		break;


	case 7:	//	������
		//	�E�����̎�
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}

		//	�������̎�
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}
		break;


	case 8:	//	�_�b�V��
		p_Player2P->SetAiLeverInputNum(AI_LEVER_DASH);
		break;


	case 9:	//	�o�b�N�X�e�b�v
		p_Player2P->SetAiLeverInputNum(AI_LEVER_BACKSTEP);
		break;

	case 10:	//	�K�[�h
		//p_Player2P->SetAiLeverInputNum(AI_LEVER_GUARD);

		//	�E�����̎�
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}

		//	�������̎�
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}
		break;

	case 11:	//	�����W�����v
		p_Player2P->SetAiLeverInputNum(AI_LEVER_UP);
		break;

	case 12:	//	�O�W�����v
		//	�E�����̎�
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPRIGHT);
		}
		//	�������̎�
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPLEFT);
		}
		break;

	case 13:	//	���W�����v
		//	�E�����̎�
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPLEFT);
		}
		//	�������̎�
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPRIGHT);
		}
		break;

	default:
		break;
	}
}


//--------------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------------
void GameState::Draw()
{

	// ���E���h�t�F�C�Y0 ( �L�����N�^�[�A�s�[�� ) ---------------
	if (round_faze == 0){
		// ���E���h�R�[�����̃L�����N�^�[�\��
		p_Player1P_rw->Draw();
		p_Player2P_rw->Draw();
	}

	// ���E���h�t�F�C�Y1 ( �o�g���R�[�� ) -----------------------
	else if (round_faze == 1){
		// ���E���h�R�[�����̃L�����N�^�[�\��
		p_Player1P_rw->Draw();
		p_Player2P_rw->Draw();

		// �o�g���R�[���`��
		p_battlecall->Draw();
	}

	// ���E���h�t�F�C�Y2 ( ���E���h�R�[�� ) ---------------------
	else if (round_faze == 2){
		// ���E���h�R�[�����̃L�����N�^�[�\��
		p_Player1P_rw->Draw();
		p_Player2P_rw->Draw();

		// ���E���h�R�[���`��
		p_roundcall->Draw();
	}

	// ���E���h�t�F�C�Y3 ( �ΐ� ) -------------------------------
	else if (round_faze >= 3 && round_faze <= 4){

		// �\��
		p_Player1P->Draw();
		p_Player2P->Draw();

		// �^�C���̕\��
		TimeDraw();
	}
	// ���E���h�t�F�C�Y4 ( KO ) -----------------------
	else if (round_faze == 5){
		// �E�C�i�[�R�[���`��
		p_winnercall->Draw();

		// �\��
		p_Player1P->Draw();
		p_Player2P->Draw();

		// �^�C���̕\��
		TimeDraw();

		//// ���N�g�^�X�V(�؂�o���傫����ʒu)
		//RECT rect = { (LONG)(0), (LONG)(0), (LONG)(1120), (LONG)(174) };

		//// �\��
		//g_Sprites->Draw(
		//	p_ko_debug,														// �e�N�X�`���n���h��
		//	Vector2(0, 228.0f),					// �\�����W
		//	&rect,																// �؂�o����`
		//	Vector4(1.0f, 1.0f, 1.0f, 1.0f),														// ���_�J���[
		//	0.0f,																// ��]�p(���W�A��)
		//	Vector2(0.0f, 0.0f),								// �G�̒��_
		//	1.0f,																// �X�P�[��
		//	SpriteEffects_None,													// �t���b�v(���]����)
		//	0.33f);														// �[�x( 0.0f �` 1.0f )
	}
	

	// �G�t�F�N�g�\��
	p_effectsystem->Draw();

	// �J�b�g�C���`��
	if (p_cutin->GetLifeFlg() == true){
		p_cutin->Draw();
	}

	// �w�i�\��
	p_background->Draw();

	
}

//--------------------------------------------------------------------------------------
// �X�e�[�g���؂�ւ�鎞�ɌĂяo�����֐�
//--------------------------------------------------------------------------------------
void GameState::StateChanged( StateManager *stateManager )
{
	// ���ݎ��s���̃X�e�[�g�Ȃ�X�V�ƕ`�������
	if( stateManager->GetCurrentStateNo() == stateNo )
	{
		SetAttr( TASK_ATTR_UPDATE | TASK_ATTR_DRAW, 0 );
	}
	else {
		SetAttr( 0, TASK_ATTR_UPDATE );
	}
}

//--------------------------------------------------------------------------------------
// �^�C���A�b�v�f�[�g
//--------------------------------------------------------------------------------------
void GameState::TimeUpDate()
{
	
	timecount++;
	if (timecount >= timespeed)
	{
		time--;
		timecount = 0;
	}

	if (time < 0)
	{
		time = GAME_TIME;
	}

}

//--------------------------------------------------------------------------------------
// �^�C���h���[
//--------------------------------------------------------------------------------------
void GameState::TimeDraw()
{
	int tmp1, tmp2;
	tmp1 = time / 10;
	tmp2 = time - (tmp1 * 10);

	g_AnimationSystem->Animation(p_time, 533.0f, 25.0f, (float)(28), (float)(40), (float)(tmp1), 0.0f, 1.0f, 0.04f, true);
	g_AnimationSystem->Animation(p_time, 559.0f, 25.0f, (float)(28), (float)(40), (float)(tmp2), 0.0f, 1.0f, 0.04f, true);

	g_AnimationSystem->Animation(p_life_gauge_under, 0.0f, 0.0f, 1120.0f, 173.0f, 0.0f, 0.0f, 1.0f, 0.01f, true);
	g_AnimationSystem->Animation(p_life_gauge_upper, 0.0f, 0.0f, 1120.0f, 170.0f, 0.0f, 0.0f, 1.0f, 0.02f, true);

}

//--------------------------------------------------------------------------------------
// �v���C���[�̍X�V
//--------------------------------------------------------------------------------------
void GameState::CharacterUpdate()
{
	
	// �v���C���[�̓��͏������󂯕t����B
	p_Player1P->KeyInputUpdate();
	p_Player2P->KeyInputUpdate();

	// �v���C���[�̃��o�[�̓��͏����̍X�V
	p_Player1P->LeverInputUpdate();
	p_Player2P->LeverInputUpdate();

	

	// �X�V��x������
	if (upd_cnt < upd_cnt_max){
		upd_cnt++;
	}
	else{
		// �f�o�b�O�ϐ�
		upd_cnt = 0;

		// �q�b�g�X�g�b�v���N���Ă��Ȃ���΍X�V���s���B
		if (stop_cnt == 0){

			// �v���C���[�̍X�V
			p_Player1P->Update();
			p_Player2P->Update();

			

			// �U�����������Ă��邩�ǂ����̔��f
			if (stop_flg == false){
				p_Player1P->HitGetUpdate();
				stop_flg = p_Player1P->HitStopFlgGetter();
				if (stop_flg == true){ stop_cnt_max += p_Player1P->HitStopCntGetter(); }
			}
			if (stop_flg == false){
				p_Player2P->HitGetUpdate();
				stop_flg = p_Player2P->HitStopFlgGetter();
				if (stop_flg == true){ stop_cnt_max += p_Player2P->HitStopCntGetter(); }
			}

			// �U�����������Ă�����A�H�炢�������s��
			p_Player1P->HitSetUpdate();
			p_Player2P->HitSetUpdate();

		}

	}

	// �Q�[�W�̍X�V
	p_Player1P->GaugeUpdate();
	p_Player2P->GaugeUpdate();

	// �J�b�g�C���̍X�V
	if (p_cutin->GetLifeFlg() == true){
		stop_cnt_max = 10;
		stop_cnt = 0;
		stop_flg = true;
		p_cutin->UpDate();
	}

	// �q�b�g�X�g�b�v�̍X�V���s��
	if (stop_flg == true)
	{
		stop_cnt++;
		if (stop_cnt >= stop_cnt_max)
		{
			stop_flg = false;
			stop_cnt_max = 0;
			stop_cnt = 0;
		}
	}



}
