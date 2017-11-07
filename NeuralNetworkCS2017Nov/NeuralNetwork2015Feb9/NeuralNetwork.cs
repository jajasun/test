using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NN
{

    public class Brain
    {
        // トレーニングデータ
        /*
        static float[,] TrainingSet = new float[,]  {
            {0.0f,	1.0f,	0.0f,	0.2f,	0.9f,	0.1f,	0.1f},
            {0.0f,	1.0f,	1.0f,	0.2f,	0.9f,	0.1f,	0.1f},
            {0.0f,	1.0f,	0.0f,	0.8f,	0.1f,	0.1f,	0.1f},
            {0.1f,	0.5f,	0.0f,	0.2f,	0.9f,	0.1f,	0.1f},
            {0.0f,	0.25f,	1.0f,	0.5f,	0.1f,	0.9f,	0.1f},
            {0.0f,	0.2f,	1.0f,	0.2f,	0.1f,	0.1f,	0.9f},
            {0.3f,	0.2f,	0.0f,	0.2f,	0.9f,	0.1f,	0.1f},
            {0.0f,	0.2f,	0.0f,	0.3f,	0.1f,	0.9f,	0.1f},
            {0.0f,	1.0f,	0.0f,	0.2f,	0.1f,	0.9f,	0.1f},
            {0.0f,	1.0f,	1.0f,	0.6f,	0.1f,	0.1f,	0.1f},
            {0.0f,	1.0f,	0.0f,	0.8f,	0.1f,	0.9f,	0.1f},
            {0.1f,	0.2f,	0.0f,	0.2f,	0.1f,	0.1f,	0.9f},
            {0.0f,	0.25f,	1.0f,	0.5f,	0.1f,	0.1f,	0.9f},
            {0.0f,	0.6f,	0.0f,	0.2f,	0.1f,	0.1f,	0.9f},
        };
        */
        
        static float[,] TrainingSet = new float[,]  {
        // degree radian  sin
        { 0.0f,  0.000f,  0.000f},		// 0 degree
        { 5.0f,  0.087f,  0.087f},
        {10.0f,  0.175f,  0.174f},
        {15.0f,  0.262f,  0.259f},
        {20.0f,  0.349f,  0.342f},
        {25.0f,  0.436f,  0.432f},
        {30.0f,  0.524f,  0.500f},
        {35.0f,  0.611f,  0.574f},
        {40.0f,  0.698f,  0.643f},
        {45.0f,  0.785f,  0.707f},		// 45 degree
        {50.0f,  0.873f,  0.766f},
        {55.0f,  0.960f,  0.819f},
        {60.0f,  1.047f,  0.866f},
        {65.0f,  1.134f,  0.906f},
        {70.0f,  1.222f,  0.940f},
        {75.0f,  1.309f,  0.966f},
        {80.0f,  1.399f,  0.985f},
        {85.0f,  1.484f,  0.996f},
        {90.0f,  1.571f,  1.000f},		// 90 degree
        {95.0f,  1.658f,  0.996f},
        {100.0f, 1.745f,  0.985f},
        {105.0f, 1.833f,  0.966f},
        {110.0f, 1.920f,  0.940f},
        {115.0f, 2.007f,  0.906f},
        {120.0f, 2.094f,  0.866f},
        {125.0f, 2.182f,  0.819f},
        {130.0f, 2.269f,  0.766f},
        {135.0f, 2.356f,  0.707f},		// 135 degree
        {140.0f, 2.443f,  0.643f},
        {145.0f, 2.531f,  0.574f},
        {150.0f, 2.618f,  0.500f},
        {155.0f, 2.705f,  0.423f},
        {160.0f, 2.793f,  0.342f},
        {165.0f, 2.880f,  0.259f},
        {170.0f, 2.967f,  0.174f},
        {175.0f, 3.054f,  0.087f},
        {180.0f, 3.142f,  0.000f},		// 180 degree
        {185.0f, 3.246f, -0.087f}, 
        {190.0f, 3.316f, -0.174f},
        {195.0f, 3.403f, -0.259f},
        {200.0f, 3.491f, -0.342f}, 
        {205.0f, 3.578f, -0.423f},
        {210.0f, 3.665f, -0.500f},
        {215.0f, 3.752f, -0.574f},
        {220.0f, 3.840f, -0.643f},
        {225.0f, 3.927f, -0.707f},		// 225 degree
        {230.0f, 4.014f, -0.766f},
        {235.0f, 4.102f, -0.819f},
        {240.0f, 4.189f, -0.866f},
        {245.0f, 4.276f, -0.906f},
        {250.0f, 4.363f, -0.940f},
        {255.0f, 4.451f, -0.966f},
        {260.0f, 4.538f, -0.985f},
        {265.0f, 4.625f, -0.996f},
        {270.0f, 4.712f, -1.000f},		// 270 degree
        {275.0f, 4.800f, -0.996f},
        {280.0f, 4.887f, -0.985f},
        {285.0f, 4.974f, -0.966f},
        {290.0f, 5.061f, -0.940f},
        {295.0f, 5.149f, -0.906f},
        {300.0f, 5.236f, -0.866f},
        {305.0f, 5.323f, -0.819f},
        {310.0f, 5.411f, -0.766f},
        {315.0f, 5.498f, -0.707f},		// 315 degree
        {320.0f, 5.585f, -0.643f},
        {325.0f, 5.672f, -0.574f},
        {330.0f, 5.760f, -0.500f},
        {335.0f, 5.847f, -0.423f},
        {340.0f, 5.934f, -0.342f},
        {345.0f, 6.021f, -0.259f},
        {350.0f, 6.109f, -0.174f},
        {355.0f, 6.196f, -0.087f},
        {360.0f, 6.283f,  0.000f}		// 360 degree
    };


        NeuralNetwork Neuron;
        public const int NumberOfData = 73;
        public const float LearningRate = 0.2f;
        public const float Moment = 0.99f;

        public int NumberOfInputNodes = 1;
        public int NumberOfHiddenNodes = NumberOfData;
        public int NumberOfOutputNodes = 1;



        // コンストラクタ
        public Brain()
        {
            // ニューラルネットワークオブジェクトを生成する
            Neuron = new NeuralNetwork();
        }

        // ニューラルネットワークを初期化する
        public void Initialize()
        {
            // ニューラルネットワークを生成する
            Neuron.Initialize(NumberOfInputNodes, NumberOfHiddenNodes, NumberOfOutputNodes);
            // 学習率を設定する
            Neuron.SetLearningRate(LearningRate);
            // モメンタムを設定する
            Neuron.SetMomentum(true, Moment);
        }

        // ニューラルネットワークをトレーニングする
        public void Training()
        {
            float error = 1.0f;
            int count = 0; 

            Neuron.DumpData("PreTraining.txt");
            while ((error > 0.0001) && (count < 80000))
            {
                error = 0;
                count++;
                for (int i = 0; i < NumberOfData; i++)
                {
                    // ニューラルネットワークに値を入力する
                    Neuron.SetInput(0, TrainingSet[i, 0] / 360.0f);
                    // ニューラルネットワークに理想値を入力する
                    Neuron.SetDesiredOutput(0, ((TrainingSet[i, 2] + 1.0f) / 2.0f));

                    // 前方伝播する
                    Neuron.FeedForward();
                    // 誤差を計算する
                    error += Neuron.CalculateError();
                    // 誤差逆伝播する
                    Neuron.BackPropagate();
                }
                error = error / NumberOfData;
            }

            Neuron.DumpData("PostTraining.txt");
            Console.WriteLine("Error: {0}", error);
            Console.WriteLine("Count: {0}", count);

            int degree = 0;
	        for(;;)
	        {
		        Neuron.SetInput(0, degree / 360.0f);
		        Neuron.FeedForward();
		        Console.Write("sin({0,3}) ", degree );
		        Console.WriteLine("{0,6:f3}",Neuron.GetOutput(0) * 2.0f - 1.0f);
		        degree += 45;
		        if(degree > 360)
			        break;
	        }
        }
    }

    // ニューラルネットワーク層クラス
    public class NeuralNetworkLayer
    {
        public int NumberOfNodes;
        public int NumberOfChildNodes;
        public int NumberOfParentNodes;
        public float[,] Weights;
        public float[,] WeightChanges;
        public float[] NeuronValues;
        public float[] DesiredValues;
        public float[] Errors;
        public float[] BiasWeights;
        public float[] BiasValues;
        public float LearningRate;

        public bool LinearOutput;
        public bool UseMomentum;
        public float MomentumFactor;

        public NeuralNetworkLayer ParentLayer;
        public NeuralNetworkLayer ChildLayer;
       
        private Random random = new Random();

        // コンストラクタ
        public NeuralNetworkLayer()
        {
            LinearOutput = false;
            UseMomentum = true;
            MomentumFactor = 0.9f;
        }

        // 初期化する
        public void Initialize(int NumNodes, NeuralNetworkLayer parent,  NeuralNetworkLayer child)
        {
            NeuronValues = new float[NumberOfNodes];
            DesiredValues = new float[NumberOfNodes];
            Errors = new float[NumberOfNodes];

            if (parent != null)
                ParentLayer = parent;
            if (child != null)
            {
                ChildLayer = child;
                Weights = new float[NumberOfNodes, NumberOfChildNodes];
                WeightChanges = new float[NumberOfNodes, NumberOfChildNodes];

                BiasValues = new float[NumberOfChildNodes];
                BiasWeights = new float[NumberOfChildNodes];
            }
            // ゼロで初期化する
            for (int i = 0; i < NumberOfNodes; i++)
            {
                NeuronValues[i] = 0.0f;
                DesiredValues[i] = 0.0f;
                Errors[i] = 0.0f;

                if (ChildLayer != null)
                {
                    for (int j = 0; j < NumberOfChildNodes; j++)
                    {
                        Weights[i, j] = 0.0f;
                        WeightChanges[i, j] = 0.0f;
                    }
                }
            }
            if (ChildLayer != null)
            {
                for (int j = 0; j < NumberOfChildNodes; j++)
                {
                    BiasValues[j] = -1.0f;
                    BiasWeights[j] = 0.0f;
                }
            }
        }

        // オブジェクトを破棄する
        public void CleanUp()
        {
        }

        // 重みを乱数化する
        public void RandomizeWeights()
        {
            int min = 0;
            int max = 200;

            for (int i = 0; i < NumberOfNodes; i++)
            {
                for (int j = 0; j < NumberOfChildNodes; j++)
                {
                    Weights[i, j] = random.Next(min, max) / 100.0f - 1;
                }
            }

            for (int j = 0; j < NumberOfChildNodes; j++)
            {
                BiasWeights[j] = random.Next(min, max) / 100.0f - 1;
            }
        }

        // 誤差を計算する
        public void CalculateErrors()
        {
            float sum = 0.0f;

            if (ChildLayer == null) 
            { // 出力層
                for (int i = 0; i < NumberOfNodes; i++)
                {
                    Errors[i] = (DesiredValues[i] - NeuronValues[i]) * NeuronValues[i] * (1.0f - NeuronValues[i]);
                }
            }
            else if (ParentLayer == null)
            { // 入力層
                for (int i = 0; i < NumberOfNodes; i++)
                {
                    Errors[i] = 0.0f;
                }
            }
            else
            { // 隠れ層
                for (int i = 0; i < NumberOfNodes; i++)
                {
                    sum = 0.0f;
                    for (int j = 0; j < NumberOfChildNodes; j++)
                    {
                        sum += ChildLayer.Errors[j] * Weights[i, j];
                    }
                    Errors[i] = sum * NeuronValues[i] * (1.0f - NeuronValues[i]);
                }
            }
        }

        // 重みを調整する
        public void AdjustWeights()
        {
            float dw = 0.0f;

            if (ChildLayer != null)
            {
                for (int i = 0; i < NumberOfNodes; i++)
                {
                    for (int j = 0; j < NumberOfChildNodes; j++)
                    {
                        dw = LearningRate * ChildLayer.Errors[j] * NeuronValues[i];
                        Weights[i, j] += dw + MomentumFactor * WeightChanges[i, j];
                        WeightChanges[i, j] = dw;
                    }
                }

                for (int j = 0; j < NumberOfChildNodes; j++)
                {
                    BiasWeights[j] += LearningRate * ChildLayer.Errors[j] * BiasValues[j];
                }
            }
        }

        // ニューロン値を計算する
        public void CalculateNeuronValues()
        {
            float x = 0.0f;

            if (ParentLayer != null)
            {
                for (int j = 0; j < NumberOfNodes; j++)
                {
                    x = 0.0f;
                    for (int i = 0; i < NumberOfParentNodes; i++)
                    {
                        x += ParentLayer.NeuronValues[i] * ParentLayer.Weights[i, j];
                    }
                    x += ParentLayer.BiasValues[j] * ParentLayer.BiasWeights[j];

                    if (ChildLayer == null && LinearOutput)
                        NeuronValues[j] = x;
                    else
                        NeuronValues[j] = 1.0f / (1.0f + (float)Math.Exp(-x));
                }
            }
        }
    }

    // ニューラルネットワーククラス
    public class NeuralNetwork
    {
        public NeuralNetworkLayer InputLayer;
        public NeuralNetworkLayer HiddenLayer;
        public NeuralNetworkLayer OutputLayer;

        // コンストラクタ
        public NeuralNetwork()
        {
            InputLayer = new NeuralNetworkLayer();
            HiddenLayer = new NeuralNetworkLayer();
            OutputLayer = new NeuralNetworkLayer();
        }

        // 初期化する
        public void Initialize(int nNodesInput,int nNodesHidden,int nNodesOutput)
        {
            InputLayer.NumberOfNodes = nNodesInput;
            InputLayer.NumberOfChildNodes = nNodesHidden;
            InputLayer.NumberOfParentNodes = 0;
            InputLayer.Initialize(nNodesInput, null, HiddenLayer);
            InputLayer.RandomizeWeights();

            HiddenLayer.NumberOfNodes = nNodesHidden;
            HiddenLayer.NumberOfChildNodes = nNodesOutput;
            HiddenLayer.NumberOfParentNodes = nNodesInput;
            HiddenLayer.Initialize(nNodesHidden, InputLayer, OutputLayer);
            HiddenLayer.RandomizeWeights();

            OutputLayer.NumberOfNodes = nNodesOutput;
            OutputLayer.NumberOfChildNodes = 0;
            OutputLayer.NumberOfParentNodes = nNodesHidden;
            OutputLayer.Initialize(nNodesOutput, HiddenLayer, null);
        }

        public void CleanUp()
        {
        }

        // パラメータを設定する
        public void SetInput(int index, float value)
        {
            if (index >= 0 && index < InputLayer.NumberOfNodes)
            {
                InputLayer.NeuronValues[index] = value;
            }
        }

        // 出力値を取得する
        public float GetOutput(int index)
        {
            if (index >= 0 && index < OutputLayer.NumberOfNodes)
            {
                return OutputLayer.NeuronValues[index];
            }
            return float.MaxValue;
        }

        // 期待される出力値を設定する
        public void SetDesiredOutput(int index, float value)
        {
            if (index >= 0 && index < OutputLayer.NumberOfNodes)
            {
                OutputLayer.DesiredValues[index] = value;
            }
        }

        // フィードフォワードをおこなう
        public void FeedForward()
        {
            InputLayer.CalculateNeuronValues();
            HiddenLayer.CalculateNeuronValues();
            OutputLayer.CalculateNeuronValues();
        }

        // バックプロパゲーションをおこなう
        public void BackPropagate()
        {
            OutputLayer.CalculateErrors();
            HiddenLayer.CalculateErrors();

            HiddenLayer.AdjustWeights();
            InputLayer.AdjustWeights();
        }

        // 出力値IDを取得する
        public int GetMaxOutputID()
        {
            float maxval = OutputLayer.NeuronValues[0];
            int id = 0;

            for (int i = 1; i < OutputLayer.NumberOfNodes; i++)
            {
                if (OutputLayer.NeuronValues[i] > maxval)
                {
                    maxval = OutputLayer.NeuronValues[i];
                    id = i;
                }
            }
            return id;
        }

        // 誤差を計算する
        public float CalculateError()
        {
            // ２乗誤差を計算する
            float error = 0.0f;
            for (int i = 0; i < OutputLayer.NumberOfNodes; i++)
            {
                error += (float)Math.Pow((float)(OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i]), 2.0f);
            }
            return error / OutputLayer.NumberOfNodes;
        }

        // 学習率を設定する
        public void SetLearningRate(float rate)
        {
            InputLayer.LearningRate = rate;
            HiddenLayer.LearningRate = rate;
            OutputLayer.LearningRate = rate;
        }

        // 線形出力を設定する
        public void SetLinearOutput(bool useLinear)
        {
            InputLayer.LinearOutput = useLinear;
            HiddenLayer.LinearOutput = useLinear;
            OutputLayer.LinearOutput = useLinear;
        }

        // モメンタムを設定する
        public void SetMomentum(bool useMomentum, float factor)
        {
            InputLayer.UseMomentum = useMomentum;
            HiddenLayer.UseMomentum = useMomentum;
            OutputLayer.UseMomentum = useMomentum;

            InputLayer.MomentumFactor = factor;
            HiddenLayer.MomentumFactor = factor;
            OutputLayer.MomentumFactor = factor;
        }

        // データをダンプする
        public void DumpData(string filename)
        {
            using (StreamWriter w = new StreamWriter(filename))
            {
                w.WriteLine("--------------------------------------------------------");
                w.WriteLine("Input Layer");
                w.WriteLine("--------------------------------------------------------");
                w.WriteLine("Node Values:");
                for (int i = 0; i < InputLayer.NumberOfNodes; i++)
                    w.WriteLine("{0} = {1}", i, InputLayer.NeuronValues[i]);
                w.WriteLine();
                w.WriteLine("Weights:");
                for (int i = 0; i < InputLayer.NumberOfNodes; i++)
                    for (int j = 0; j < InputLayer.NumberOfChildNodes; j++)
                        w.WriteLine("({0}, {1}) = {2}", i, j, InputLayer.Weights[i, j]);
                w.WriteLine();
                w.WriteLine("Bias Weights:");
                for (int j = 0; j < InputLayer.NumberOfChildNodes; j++)
                    w.WriteLine("({0}) = {1}", j, InputLayer.BiasWeights[j]);

                w.WriteLine();
                w.WriteLine();

                w.WriteLine("--------------------------------------------------------");
                w.WriteLine("Hidden Layer");
                w.WriteLine("--------------------------------------------------------");
                w.WriteLine("Weights:");
                for (int i = 0; i < HiddenLayer.NumberOfNodes; i++)
                    for (int j = 0; j < HiddenLayer.NumberOfChildNodes; j++)
                        w.WriteLine("({0}, {1}) = {2}", i, j, HiddenLayer.Weights[i, j]);
                w.WriteLine();
                w.WriteLine("Bias Weights:");
                for (int j = 0; j < HiddenLayer.NumberOfChildNodes; j++)
                    w.WriteLine("({0}) = {1}", j, HiddenLayer.BiasWeights[j]);

                w.WriteLine();
                w.WriteLine();

                w.WriteLine("--------------------------------------------------------");
                w.WriteLine("Output Layer");
                w.WriteLine("--------------------------------------------------------");
                w.WriteLine("Node Values:");
                for (int i = 0; i < OutputLayer.NumberOfNodes; i++)
                    w.WriteLine("({0}) = {1}", i, OutputLayer.NeuronValues[i]);
                w.WriteLine();
                w.Close();
            }
        }
    }
}