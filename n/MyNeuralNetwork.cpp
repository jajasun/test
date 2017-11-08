#include "MyNeuralNetwork.h"


/*
Book:    AI for Game Developers
Authors: David M. Bourg & Glenn Seemann
Example: Neural Networks, Chapter 14
*/

// コンストラクタ
NeuralNetworkLayer::NeuralNetworkLayer()
{
	ParentLayer = NULL;
	ChildLayer = NULL;
	LinearOutput = false;
	UseMomentum = true;
	MomentumFactor = 0.9;
}

// ニューラルネットワークを初期化する
void NeuralNetworkLayer::Initialize(int NumNodes, NeuralNetworkLayer* parent, NeuralNetworkLayer* child)
{
	// メモリを確保する(Allocate memory)
	NeuronValues = (double*) malloc(sizeof(double) * NumberOfNodes);
	DesiredValues = (double*) malloc(sizeof(double) * NumberOfNodes);
	Errors = (double*) malloc(sizeof(double) * NumberOfNodes);

	if(parent != NULL)
	{		
		ParentLayer = parent;
	}

	if(child != NULL)
	{
		ChildLayer = child;

		Weights = (double**) malloc(sizeof(double*) * NumberOfNodes);
		WeightChanges = (double**) malloc(sizeof(double*) * NumberOfNodes);
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Weights[i] = (double*) malloc(sizeof(double) * NumberOfChildNodes);
			WeightChanges[i] = (double*) malloc(sizeof(double) * NumberOfChildNodes);
		}

		BiasValues = (double*) malloc(sizeof(double) * NumberOfChildNodes);
		BiasWeights = (double*) malloc(sizeof(double) * NumberOfChildNodes);
	} 
	else 
	{
		Weights = NULL;
		BiasValues = NULL;
		BiasWeights = NULL;
	}

	// ゼロで初期化をおこなう(Make sure everything contains zeros)
	for(int i = 0; i < NumberOfNodes; i++)
	{
		NeuronValues[i] = 0;
		DesiredValues[i] = 0;
		Errors[i] = 0;
		
		if(ChildLayer != NULL)
		{
			for(int j = 0; j < NumberOfChildNodes; j++)
			{
				Weights[i][j] = 0;
				WeightChanges[i][j] = 0;
			}
		}
	}

	if(ChildLayer != NULL)
	{
		for(int j = 0; j < NumberOfChildNodes; j++)
		{
			BiasValues[j] = -1.0;
			BiasWeights[j] = 0.0;
		}
	}
}

// ニューラルネットワークを解放する
void NeuralNetworkLayer::CleanUp(void)
{
	free(NeuronValues);
	free(DesiredValues);
	free(Errors);
	
	if(Weights != NULL)
	{
		for(int i = 0; i<NumberOfNodes; i++)
		{
			free(Weights[i]);
			free(WeightChanges[i]);
		}

		free(Weights);
		free(WeightChanges);
	}

	if(BiasValues != NULL) 
		free(BiasValues);
	if(BiasWeights != NULL) 
		free(BiasWeights);
}

// ランダムに重みを設定する
void NeuralNetworkLayer::RandomizeWeights(void)
{
	int	min = 0;
	int	max = 200;
	int	number;

	srand( (unsigned)time( NULL ) );

	for(int i = 0; i < NumberOfNodes; i++)
	{
		for(int j = 0; j < NumberOfChildNodes; j++)
		{	
			number = (((abs(rand()) % (max - min + 1)) + min));    
			if(number>max)
				number = max;
			if(number<min)
    			number = min;
			Weights[i][j] = number / 100.0 - 1;
		}
	}
	
	for(int j = 0; j < NumberOfChildNodes; j++)
	{
		number = (((abs(rand()) % (max - min + 1)) + min));    
    
		if(number > max)
			number = max;

		if(number < min)
    		number = min;		
			
		BiasWeights[j] = number / 100.0 - 1.0;
	}
}

// 誤差を計算する
void NeuralNetworkLayer::CalculateErrors(void)
{
double	sum;
	
	if(ChildLayer == NULL) // 出力層(output layer)
	{
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Errors[i] = (DesiredValues[i] - NeuronValues[i]) * NeuronValues[i] * (1.0f - NeuronValues[i]);
		}
	}
	else if(ParentLayer == NULL) 
	{ // 入力層(input layer)
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Errors[i] = 0.0f;
		}
	}
	else 
	{ // 隠れ層(hidden layer)
		for(int i = 0; i < NumberOfNodes; i++)
		{
			sum = 0;
			for(int j = 0; j < NumberOfChildNodes; j++)
			{
				sum += ChildLayer->Errors[j] * Weights[i][j];	
			}
			Errors[i] = sum * NeuronValues[i] * (1.0f - NeuronValues[i]);
		}
	}
}

// 重みを調整する
void NeuralNetworkLayer::AdjustWeights(void)
{
	if(ChildLayer != NULL)
	{
		for(int i = 0; i < NumberOfNodes; i++)
		{
			for(int j = 0; j < NumberOfChildNodes; j++)
			{
				double dw = LearningRate * ChildLayer->Errors[j] * NeuronValues[i];
				Weights[i][j] += dw + MomentumFactor * WeightChanges[i][j];			
				WeightChanges[i][j] = dw;
			}
		}

		for(int j = 0; j < NumberOfChildNodes; j++)
		{
			BiasWeights[j] += LearningRate * ChildLayer->Errors[j] * BiasValues[j];
		}
	}
}

// ニューロン値を計算する
void NeuralNetworkLayer::CalculateNeuronValues(void)
{
double	x;
	
	if(ParentLayer != NULL)
	{
		for(int j = 0; j < NumberOfNodes; j++)
		{
			x = 0;
			for(int i = 0; i < NumberOfParentNodes; i++)
			{
				x += ParentLayer->NeuronValues[i] * ParentLayer->Weights[i][j];
			}			
			x += ParentLayer->BiasValues[j] * ParentLayer->BiasWeights[j];
			
			if((ChildLayer == NULL) && LinearOutput)
				NeuronValues[j] = x;
			else
				NeuronValues[j] = 1.0f / (1.0 + exp( -x ));				
		}
	}
}



// 初期化する(NeuralNetwork Class)
void NeuralNetwork::Initialize(int nNodesInput, int nNodesHidden, int nNodesOutput)
{
	InputLayer.NumberOfNodes = nNodesInput;
	InputLayer.NumberOfChildNodes = nNodesHidden;
	InputLayer.NumberOfParentNodes = 0;	
	InputLayer.Initialize(nNodesInput, NULL, &HiddenLayer);
	InputLayer.RandomizeWeights();
	
	HiddenLayer.NumberOfNodes = nNodesHidden;
	HiddenLayer.NumberOfChildNodes = nNodesOutput;
	HiddenLayer.NumberOfParentNodes = nNodesInput;		
	HiddenLayer.Initialize(nNodesHidden, &InputLayer, &OutputLayer);
	HiddenLayer.RandomizeWeights();
	
	OutputLayer.NumberOfNodes = nNodesOutput;
	OutputLayer.NumberOfChildNodes = 0;
	OutputLayer.NumberOfParentNodes = nNodesHidden;		
	OutputLayer.Initialize(nNodesOutput, &HiddenLayer, NULL);	
}

// ニューラルネットワークをクリーンナップする
void NeuralNetwork::CleanUp()
{
	InputLayer.CleanUp();
	HiddenLayer.CleanUp();
	OutputLayer.CleanUp();
}

// 入力値を設定する
void NeuralNetwork::SetInput(int n, double value)
{
	if(n >= 0 && n < InputLayer.NumberOfNodes)
	{
		InputLayer.NeuronValues[n] = value;
	}
}

// 出力値を取得する
double NeuralNetwork::GetOutput(int n)
{
	if(n >= 0 && n < OutputLayer.NumberOfNodes)
	{
		return OutputLayer.NeuronValues[n];
	}
	return (double) INT_MAX; // to indicate an error
}

// 期待される出力を設定する
void NeuralNetwork::SetDesiredOutput(int n, double value)
{
	if(n >= 0 && n < OutputLayer.NumberOfNodes)
	{
		OutputLayer.DesiredValues[n] = value;
	}
}

// フィードフォワードをおこなう
void NeuralNetwork::FeedForward(void)
{
	InputLayer.CalculateNeuronValues();
	HiddenLayer.CalculateNeuronValues();
	OutputLayer.CalculateNeuronValues();
}

// バックプロパゲーションをおこなう
void NeuralNetwork::BackPropagate(void)
{
	OutputLayer.CalculateErrors();
	HiddenLayer.CalculateErrors();

	HiddenLayer.AdjustWeights();
	InputLayer.AdjustWeights();
}

// 最大出力IDを取得する
int	NeuralNetwork::GetMaxOutputID(void)
{
double maxval = OutputLayer.NeuronValues[0];
int id = 0;

	for(int i = 1; i < OutputLayer.NumberOfNodes; i++)
	{
		if(OutputLayer.NeuronValues[i] > maxval)
		{
			maxval = OutputLayer.NeuronValues[i];
			id = i;
		}
	}
	return id;
}

// 誤差を計算する
double NeuralNetwork::CalculateError(void)
{
double	error = 0.0;

	for(int i = 0; i < OutputLayer.NumberOfNodes; i++)
	{
		error += pow(OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i], 2);
	}
	return error / OutputLayer.NumberOfNodes;
}

// 学習率を設定する
void NeuralNetwork::SetLearningRate(double rate)
{
	InputLayer.LearningRate = rate;
	HiddenLayer.LearningRate = rate;
	OutputLayer.LearningRate = rate;
} 

// リニアな出力を設定する
void NeuralNetwork::SetLinearOutput(bool useLinear)
{
	InputLayer.LinearOutput = useLinear;
	HiddenLayer.LinearOutput = useLinear;
	OutputLayer.LinearOutput = useLinear;
}

// モメンタムを設定する
void NeuralNetwork::SetMomentum(bool useMomentum, double factor)
{
	InputLayer.UseMomentum = useMomentum;
	HiddenLayer.UseMomentum = useMomentum;
	OutputLayer.UseMomentum = useMomentum;

	InputLayer.MomentumFactor = factor;
	HiddenLayer.MomentumFactor = factor;
	OutputLayer.MomentumFactor = factor;
}

// データをダンプする
void NeuralNetwork::DumpData(char* filename)
{
	errno_t err;

	FILE* f;
	err = fopen_s(&f,filename, "w");
	
	fprintf(f, "--------------------------------------------------------\n");
	fprintf(f, "Input Layer\n");
	fprintf(f, "--------------------------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "Node Values:\n");
	fprintf(f, "\n");
	for(int i = 0; i < InputLayer.NumberOfNodes; i++)		
		fprintf(f, "(%d) = %f\n", i, InputLayer.NeuronValues[i]);
	fprintf(f, "\n");
	fprintf(f, "Weights:\n");
	fprintf(f, "\n");
	for(int i = 0; i < InputLayer.NumberOfNodes; i++)
		for(int j = 0; j < InputLayer.NumberOfChildNodes; j++)
			fprintf(f, "(%d, %d) = %f\n", i, j, InputLayer.Weights[i][j]);
	fprintf(f, "\n");
	fprintf(f, "Bias Weights:\n");
	fprintf(f, "\n");
	for(int j = 0; j < InputLayer.NumberOfChildNodes; j++)
		fprintf(f, "(%d) = %f\n", j, InputLayer.BiasWeights[j]);

	fprintf(f, "\n");
	fprintf(f, "\n");

	fprintf(f, "--------------------------------------------------------\n");
	fprintf(f, "Hidden Layer\n");
	fprintf(f, "--------------------------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "Weights:\n");
	fprintf(f, "\n");
	for(int i = 0; i < HiddenLayer.NumberOfNodes; i++)
		for(int j = 0; j < HiddenLayer.NumberOfChildNodes; j++)
			fprintf(f, "(%d, %d) = %f\n", i, j, HiddenLayer.Weights[i][j]);
	fprintf(f, "\n");
	fprintf(f, "Bias Weights:\n");
	fprintf(f, "\n");
	for(int j = 0; j < HiddenLayer.NumberOfChildNodes; j++)
		fprintf(f, "(%d) = %f\n", j, HiddenLayer.BiasWeights[j]);

	fprintf(f, "\n");
	fprintf(f, "\n");

	fprintf(f, "--------------------------------------------------------\n");
	fprintf(f, "Output Layer\n");
	fprintf(f, "--------------------------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "Node Values:\n");
	fprintf(f, "\n");
	for(int i = 0; i < OutputLayer.NumberOfNodes; i++)		
		fprintf(f, "(%d) = %f\n", i, OutputLayer.NeuronValues[i]);
	fprintf(f, "\n");

	fclose(f);
}
