using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
public class NeuralNetwork : MonoBehaviour {

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
    public void Initialize(int nNodesInput, int nNodesHidden, int nNodesOutput)
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
            error += (float)Mathf.Pow((float)(OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i]), 2.0f);
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
    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
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


    // コンストラクタ
    public NeuralNetworkLayer()
    {
        LinearOutput = false;
        UseMomentum = true;
        MomentumFactor = 0.9f;
    }

    // 初期化する
    public void Initialize(int NumNodes, NeuralNetworkLayer parent, NeuralNetworkLayer child)
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
                Weights[i, j] = Random.Range(min, max + 1) / 100.0f - 1;
            }
        }

        for (int j = 0; j < NumberOfChildNodes; j++)
        {
            BiasWeights[j] = Random.Range(min, max + 1) / 100.0f - 1;
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
                    NeuronValues[j] = 1.0f / (1.0f + (float)Mathf.Exp(-x));
            }
        }
    }
}