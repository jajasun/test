using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElementLeaderManager : MonoBehaviour {

    // ゾーン(優先度、占領チームをここから取得する)
    [SerializeField]
    public Zone[] _zone;

    
    // リーダー
    ElementLeader[] _leader;
    [SerializeField]
    private ElementLeader _leaderPrefab;

    // 各チームのAIの数を取得
    int _red_AI;
    int _blue_AI;
    int _total_AI;

    // 各チームのリーダー数
    int _red_leader;
    int _blue_leader;
    int _total_leader;


    // 小隊の人数
    [SerializeField]
    private int party_num = 4;

    Brain brain;

    // Use this for initialization
    void Start () {

        // 各チームのAIの数を取得
        _red_AI = 15;
        _blue_AI = 15;
        _total_AI = _red_AI + _blue_AI;


        CreateLeader();


        brain = new Brain();
        brain.Initialize();
        brain.Training();


    }
	
	// Update is called once per frame
	void Update () {
        foreach (var leader in _leader)
        {
            leader.Run();
        }
    }

    /// <summary>
    /// 指定したidの小隊長を取得
    /// </summary>
    /// <param name="id"></param>
    /// <returns>指定したidの小隊長</returns>
    ElementLeader GetLeader(int id)
    {
        return _leader[id];
    }

    /// <summary>
    /// 拠点に近い小隊を探す
    /// </summary>
    private void SerchClosePlatoon()
    {
        
    }

    /// <summary>
    /// 優先度調査
    /// </summary>
    private void CheckPrioritylevels()
    {
        // もうセットしてあるつもり
    }

    /// <summary>
    /// 拠点に入っている敵の数をチェック
    /// </summary>
    private void CheckEnemyNum()
    {

    }

    /// <summary>
    /// 指示をする
    /// </summary>
    /// <returns>ターゲット</returns>
    public Transform Instruct()
    {
        // 自陣敵陣の調査
        // _zone[i].GetDominatedTeam();
        // 優先度
        int priority = 1; // _zone[i].GetPriority(this.Team);




        return new GameObject().transform;
    }



    /// <summary>
    /// グループ分けの通りに生成する
    /// </summary>
    void CreateLeader()
    {
        // リーダーの生成
        _red_leader = _red_AI / party_num;
        if (_red_AI % party_num > 0)
            _red_leader++;
        _blue_leader = _blue_AI / party_num;
        if (_blue_AI % party_num > 0)
            _blue_leader++;

        _total_leader = _red_leader + _blue_leader;
        _leader = new ElementLeader[_total_leader];

        // 実体化
        ElementLeader prefab = _leaderPrefab;
        ElementLeader el;
        for (var i = 0; i < _leader.Length; i++)
        {
            el = Instantiate(prefab);
            _leader[i] = el.GetComponent<ElementLeader>();
        }

        // 生成後、チームとIDを割り当てる
        for (var i = 0; i < _red_leader; i++)
        {
            _leader[i].Team = Team.Red;
            _leader[i].ID = i + 1;
            
        }
        for (var i = _red_leader; i < _total_leader; i++)
        {
            _leader[i].Team = Team.Blue;
            _leader[i].ID = i - _red_leader + 1;
        }

        // デバッグ表示
        Debug.Log("RED_TEAM: " + _red_AI + "," + "BLUE_TEAM: " + _blue_AI);
        for (var i = 0; i < _leader.Length; i++)
        {
            Debug.Log("Team:" + _leader[i].Team + ": ID:" + _leader[i].ID);
        }


    }
}
public class Brain
{

    static float[,] TrainingData = new float[,]
    {
        // データの作り方
        // 【入力】拠点までの距離、自と敵の割合、他の味方がどこに向かおうとしているか
        // 【出力】拠点 A,B,C,D,E（理想）

        // 自と敵の割合:(自多い = 0.0 , 均衡 = 0.5 , 敵多い = 1.0)

        // 距離A、距離B、距離C、距離D、距離E、A自敵割り、B自敵割り、C自敵割り、D自敵割り、E自敵割り、A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                                                                             A、B、C、D、E

        // Aが近いとき
        // A:近、B:中、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.2f , 0.0f , 0.0f , 0.0f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A敵多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     1.0f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.4f , 0.0f , 0.0f , 0.0f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A自多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.0f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.0f , 0.8f , 0.2f , 0.2f , 0.2f},

        // A近くて他タゲに影響される
        // A:近、B:中、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.2f , 0.0f , 0.0f , 0.0f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A敵多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     1.0f,  0.5f,  0.5f,  0.5f,  0.5f,      0.5f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.6f , 0.0f , 0.0f , 0.0f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A自多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.0f,  0.5f,  0.5f,  0.5f,  0.5f,      1.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.0f , 0.8f , 0.2f , 0.2f , 0.2f},
        





        // Bが近いとき
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.5f,    0.5f,    0.5f,    0.5f,    0.5f,                    0.2f , 0.8f , 0.0f , 0.0f , 0.0f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B敵多、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  1.0f,  0.5f,  0.5f,  0.5f,      0.5f,    0.5f,    0.5f,    0.5f,    0.5f,                    0.2f , 0.8f , 0.0f , 0.0f , 0.0f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B自多、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  0.0f,  0.5f,  0.5f,  0.5f,      0.5f,    0.5f,    0.5f,    0.5f,    0.5f,                    0.2f , 0.8f , 0.0f , 0.0f , 0.0f},






        // A:中、B:近、C:遠、D:遠:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.2f , 0.8f , 0.0f , 0.0f , 0.0f},
        // A:遠、B:中、C:近、D:遠:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.2f , 0.8f , 0.0f , 0.0f},
        // A:遠、B:遠、C:中、D:近:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.7f, 0.5f, 0.2f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.0f , 0.2f , 0.8f , 0.0f},
        // A:遠、B:遠、C:遠、D:中:、E:近、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.0f , 0.0f , 0.2f , 0.8f},
        // A:遠、B:遠、C:遠、D:近:、E:中、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.0f , 0.0f , 0.8f , 0.2f},

        // 【敵多、他タゲ少ない】
        // A:近、B:中、C:遠、D:遠:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.8f , 0.2f , 0.0f , 0.0f , 0.0f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.2f , 0.8f , 0.0f , 0.0f , 0.0f},
        // A:遠、B:中、C:近、D:遠:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.2f , 0.8f , 0.0f , 0.0f},
        // A:遠、B:遠、C:中、D:近:、E:遠、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.7f, 0.5f, 0.2f, 0.7f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.0f , 0.2f , 0.8f , 0.0f},
        // A:遠、B:遠、C:遠、D:中:、E:近、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.0f , 0.0f , 0.2f , 0.8f},
        // A:遠、B:遠、C:遠、D:近:、E:中、   A自敵:均衡、B自敵:均衡、C自敵:均衡、D自敵:均衡、E自敵:均衡 ,A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,0.5f,0.5f,0.5f,0.5f,                                  0.5f,0.5f,0.5f,0.5f,0.5f                                    , 0.0f , 0.0f , 0.0f , 0.8f , 0.2f},



    };

    
    
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
        Debug.Log("Error: {0}" + error);
        Debug.Log("Count: {0}" + count);

        int degree = 0;
        for (;;)
        {
            Neuron.SetInput(0, degree / 360.0f);
            Neuron.FeedForward();
            Debug.Log("sin({0,3}) " +  degree);
            Debug.Log("{0,6:f3}" + (Neuron.GetOutput(0) * 2.0f - 1.0f));
            degree += 45;
            if (degree > 360)
                break;
        }
    }


    // ・ほかの味方達が、どこの拠点をターゲットにしているか
    // 出力結果（例：A地点）を配列にストックしていく
    // float[] targetPoint;
    // AI1のターゲットがAに決まる
    // targetPoint[A] += 1.0f;
    // 0.0 ~ 1.0の範囲にするために 16（人数で割る）
    // targetPoint[A] /= 16f;
    // 出た結果を入力に使用する
    void AddTargetValue()
    {
        int[] targetPointNum = new int[5];

        // 出力結果を取得
        int id = 1;// A
        targetPointNum[id] += 1;

        float target = targetPointNum[id];
        target /= 16f;
    }


    // 割合を求める()
    float GetRate(float num1,float num2,int v)
    {
        float n = num1 - num2;
        n /= v;
        float result = (n + 1.0f) / 2.0f;

        return result;

    }

}



