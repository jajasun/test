#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "..\NeuralNetwork\main.h"
#include "..\NeuralNetwork\mymath.h"
#include "..\NeuralNetwork\stdafx.h"
#include "..\NeuralNetwork\targetver.h"



#ifndef _MYNEURALNETWORKHEADER
#define _MYNEURALNETWORKHEADER

// �j���[�����l�b�g���[�N�w��錾����
class NeuralNetworkLayer
{
public:
	int			NumberOfNodes;			// �j���[�����̐�
	int			NumberOfChildNodes;		// �q�w�̃j���[�����̐�
	int			NumberOfParentNodes;	// �e�w�̃j���[�����̐�
	double**	Weights;				// �e�w�Ǝq�w��ڑ�����m�[�h�̏d��
	double**	WeightChanges;			// �d�ݒl�̒����Ɏg����l���i�[�����
	double*		NeuronValues;			// �w�̃j���[�����ɑ΂��Čv�Z���ꂽ�l�������
	double*		DesiredValues;			// �w�̃j���[�����̖]�܂����l���i�[�����
	double*		Errors;					// �w�̊e�j���[�����Ɋ֘A����G���[���i�[�����
	double*		BiasWeights;			// �w�̊e�j���[�����ɐڑ�����o�C�A�X�̏d�݂��i�[�����
	double*		BiasValues;				// �w�̊e�j���[�����ɐڑ�����o�C�A�X�l���i�[�����
	double		LearningRate;			// �d�݂̒������v�Z����w�K����ێ�����

	bool		LinearOutput;			// �w�̃j���[�����ɑ΂��Đ��`�������֐����g�p����
	bool		UseMomentum;			// �d�݂𒲐�����ۂɁA���[�����^�����g�p���邩�ǂ���������
	double		MomentumFactor;			// ���[�����^��������ێ�����

	NeuralNetworkLayer*	ParentLayer;	// ����̑w�ɐڑ�����e�w
	NeuralNetworkLayer*	ChildLayer;		// ����̑w�ɐڑ�����q�w

	NeuralNetworkLayer();

	// �w�Ԃ̐e�q�̐ڑ�����������i�ڑ��̏d�݂�����������j
	void	Initialize(int NumNodes, NeuralNetworkLayer* parent, NeuralNetworkLayer* child);
	// �w�I�u�W�F�N�g�̌㏈���������Ȃ�
	void	CleanUp(void);
	// �d�݂������_���l��ݒ肷��
	void	RandomizeWeights(void);
	// �o�̓Z�b�g�Ɋ֘A����G���[���v�Z����
	void	CalculateErrors(void);
	// �d�݂𒲐�����
	void	AdjustWeights(void);
	// �j���[�����l���v�Z����
	void	CalculateNeuronValues(void);
};

// ���͑w�A�B��w�A�o�͑w��������3�K�w�j���[�����l�b�g���[�N����������
// Implements a 3-Layer neural network with one input layer, one hidden layer, and one output layer
class NeuralNetwork 
{
public:
	NeuralNetworkLayer	InputLayer;
	NeuralNetworkLayer	HiddenLayer;
	NeuralNetworkLayer	OutputLayer;

	void	Initialize(int nNodesInput, int nNodesHidden, int nNodesOutput);
	void	CleanUp();
	void	SetInput(int i, double value);
	double	GetOutput(int i);
	void	SetDesiredOutput(int i, double value);
	void	FeedForward(void);
	void	BackPropagate(void);
	int		GetMaxOutputID(void);
	double	CalculateError(void);
	void	SetLearningRate(double rate);
	void	SetLinearOutput(bool useLinear);
	void	SetMomentum(bool useMomentum, double factor);
	void	DumpData(char* filename);
};

#endif
