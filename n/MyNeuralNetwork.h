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

// ニューラルネットワーク層を宣言する
class NeuralNetworkLayer
{
public:
	int			NumberOfNodes;			// ニューロンの数
	int			NumberOfChildNodes;		// 子層のニューロンの数
	int			NumberOfParentNodes;	// 親層のニューロンの数
	double**	Weights;				// 親層と子層を接続するノードの重み
	double**	WeightChanges;			// 重み値の調整に使われる値が格納される
	double*		NeuronValues;			// 層のニューロンに対して計算された値がされる
	double*		DesiredValues;			// 層のニューロンの望ましい値が格納される
	double*		Errors;					// 層の各ニューロンに関連するエラーが格納される
	double*		BiasWeights;			// 層の各ニューロンに接続するバイアスの重みが格納される
	double*		BiasValues;				// 層の各ニューロンに接続するバイアス値が格納される
	double		LearningRate;			// 重みの調整を計算する学習率を保持する

	bool		LinearOutput;			// 層のニューロンに対して線形活性化関数を使用する
	bool		UseMomentum;			// 重みを調整する際に、モーメンタムを使用するかどうかを示す
	double		MomentumFactor;			// モーメンタム因数を保持する

	NeuralNetworkLayer*	ParentLayer;	// 特定の層に接続する親層
	NeuralNetworkLayer*	ChildLayer;		// 特定の層に接続する子層

	NeuralNetworkLayer();

	// 層間の親子の接続を処理する（接続の重みを初期化する）
	void	Initialize(int NumNodes, NeuralNetworkLayer* parent, NeuralNetworkLayer* child);
	// 層オブジェクトの後処理をおこなう
	void	CleanUp(void);
	// 重みをランダム値を設定する
	void	RandomizeWeights(void);
	// 出力セットに関連するエラーを計算する
	void	CalculateErrors(void);
	// 重みを調整する
	void	AdjustWeights(void);
	// ニューロン値を計算する
	void	CalculateNeuronValues(void);
};

// 入力層、隠れ層、出力層をもった3階層ニューラルネットワークを実装する
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
