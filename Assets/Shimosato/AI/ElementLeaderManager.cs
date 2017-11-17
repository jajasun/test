using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElementLeaderManager : MonoBehaviour
{

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
    void Start()
    {

        // 各チームのAIの数を取得
        _red_AI = 16;
        _blue_AI = 16;
        _total_AI = _red_AI + _blue_AI;


        CreateLeader();

        brain = new Brain();
        brain.Initialize();
        brain.Training();


    }

    // Update is called once per frame
    void Update()
    {
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
        // 他タゲ（タゲ無 = 0.0 , チームの半分タゲ = 0.5 , タゲ全員 = 1.0）

        // 距離A、距離B、距離C、距離D、距離E、A自敵割り、B自敵割り、C自敵割り、D自敵割り、E自敵割り、A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ   A、B、C、D、E

        // Aが近いとき、自敵割りに影響
        // A:近、B:中、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.2f , 0.1f , 0.1f , 0.1f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A敵多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     1.0f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.4f , 0.1f , 0.1f , 0.1f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A自多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.0f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.0f , 0.6f , 0.2f , 0.2f , 0.2f},

        // A近いとき、自敵割りに影響、他タゲに影響される
        // A:近、B:中、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.2f , 0.1f , 0.1f , 0.1f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A敵多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     1.0f,  0.5f,  0.5f,  0.5f,  0.5f,      0.5f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.4f , 0.6f , 0.1f , 0.1f , 0.1f},
        // A:近、B:中、C:遠、D:遠:、E:遠、   A自多、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.2f, 0.5f, 0.7f, 0.7f, 0.7f,     0.0f,  0.5f,  0.5f,  0.5f,  0.5f,      1.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.0f , 0.8f , 0.2f , 0.2f , 0.2f},
        

        // Bが近いとき、自敵割りに影響
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.8f , 0.1f , 0.1f , 0.1f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B敵多、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  1.0f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.4f , 0.8f , 0.1f , 0.1f , 0.1f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B自多、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  0.0f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.6f , 0.0f , 0.2f , 0.2f , 0.2f},

        // B近いとき、自敵割りに影響、他タゲに影響される
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.8f , 0.1f , 0.1f , 0.1f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B敵多、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  1.0f,  0.5f,  0.5f,  0.5f,      0.0f,    0.5f,    0.0f,    0.0f,    0.0f,                    0.8f , 0.4f , 0.1f , 0.1f , 0.1f},
        // A:中、B:近、C:遠、D:遠:、E:遠、   A均衡、B自多、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.2f, 0.7f, 0.7f, 0.7f,     0.5f,  0.0f,  0.5f,  0.5f,  0.5f,      0.0f,    1.0f,    0.0f,    0.0f,    0.0f,                    0.6f , 0.0f , 0.2f , 0.2f , 0.2f},


        // Cが近いとき、自敵割りに影響
        // A:遠、B:中、C:近、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.2f , 0.8f , 0.1f , 0.1f},
        // A:遠、B:中、C:近、D:遠:、E:遠、   A均衡、B均衡、C敵多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,  0.5f,  1.0f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.6f , 0.4f , 0.1f , 0.1f},
        // A:遠、B:中、C:近、D:遠:、E:遠、   A均衡、B均衡、C自多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,  0.5f,  0.0f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.6f , 0.0f , 0.2f , 0.2f},

        // C近いとき、自敵割りに影響、他タゲに影響される
        // A:遠、B:中、C:近、D:遠:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.8f , 0.1f , 0.1f},
        // A:遠、B:中、C:近、D:遠:、E:遠、   A均衡、B均衡、C敵多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,  0.5f,  1.0f,  0.5f,  0.5f,      0.0f,    0.0f,    0.5f,    0.0f,    0.0f,                    0.1f , 0.4f , 0.6f , 0.1f , 0.1f},
        // A:遠、B:中、C:近、D:遠:、E:遠、   A均衡、B均衡、C自多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.5f, 0.5f, 0.2f, 0.7f, 0.7f,     0.5f,  0.5f,  0.0f,  0.5f,  0.5f,      0.0f,    0.0f,    1.0f,    0.0f,    0.0f,                    0.2f , 0.6f , 0.0f , 0.2f , 0.2f},




        // Dが近いとき、自敵割りに影響
        // A:遠、B:遠、C:中、D:近:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.5f, 0.2f, 0.2f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.1f , 0.1f , 0.8f , 0.2f},
        // A:遠、B:遠、C:中、D:近:、E:遠、   A均衡、B均衡、C均衡、D敵多、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.5f, 0.2f, 0.2f, 0.7f,     0.5f,  0.5f,  0.5f,  1.0f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.1f , 0.4f , 0.8f , 0.6f},
        // A:遠、B:遠、C:中、D:近:、E:遠、   A均衡、B均衡、C均衡、D自多、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.5f, 0.2f, 0.2f, 0.7f,     0.5f,  0.5f,  0.5f,  0.0f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.2f , 0.6f , 0.0f , 0.1f},

        // D近いとき、自敵割りに影響、他タゲに影響される
        // A:遠、B:遠、C:中、D:近:、E:遠、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.5f, 0.2f, 0.7f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.1f , 0.1f , 0.2f , 0.8f , 0.1f},
        // A:遠、B:遠、C:中、D:近:、E:遠、   A均衡、B均衡、C均衡、D敵多、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.5f, 0.2f, 0.7f,     0.5f,  0.5f,  0.5f,  1.0f,  0.5f,      0.0f,    0.0f,    0.0f,    0.5f,    0.0f,                    0.1f , 0.1f , 0.2f , 0.6f , 0.1f},
        // A:遠、B:遠、C:中、D:近:、E:遠、   A均衡、B均衡、C均衡、D自多、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.5f, 0.2f, 0.7f,     0.5f,  0.5f,  0.5f,  0.0f,  0.5f,      0.0f,    0.0f,    0.0f,    1.0f,    0.0f,                    0.2f , 0.5f , 0.6f , 0.0f , 0.1f},

        
        // Eが近いとき、自敵割りに影響
        // A:遠、B:遠、C:遠、D:中:、E:近、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.4f , 0.8f},
        // A:遠、B:遠、C:遠、D:中:、E:近、   A均衡、B均衡、C敵多、D均衡、E敵多、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,  0.5f,  0.5f,  0.5f,  1.0f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.4f , 0.8f},
        // A:遠、B:遠、C:遠、D:中:、E:近、   A均衡、B均衡、C自多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,  0.5f,  0.5f,  0.5f,  0.0f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.2f , 0.6f , 0.0f},

        // Eが近いとき、自敵割りに影響
        // A:遠、B:遠、C:遠、D:中:、E:近、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.4f , 0.8f},
        // A:遠、B:遠、C:遠、D:中:、E:近、   A均衡、B均衡、C敵多、D均衡、E敵多、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,  0.5f,  0.5f,  0.5f,  1.0f,      0.0f,    0.0f,    0.0f,    0.0f,    0.5f,                    0.1f , 0.1f , 0.1f , 0.6f , 0.4f},
        // A:遠、B:遠、C:遠、D:中:、E:近、   A均衡、B均衡、C自多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.5f, 0.2f,     0.5f,  0.5f,  0.5f,  0.5f,  0.0f,      0.0f,    0.0f,    0.0f,    0.0f,    1.0f,                    0.1f , 0.1f , 0.1f , 0.4f , 0.4f},


        // D,Eが近いとき、自敵割りに影響
        // A:遠、B:遠、C:遠、D:近:、E:中、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.8f , 0.4f},
        // A:遠、B:遠、C:遠、D:近:、E:中、   A均衡、B均衡、C敵多、D敵多、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,  0.5f,  0.5f,  1.0f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.8f , 0.4f},
        // A:遠、B:遠、C:遠、D:近:、E:中、   A均衡、B均衡、C自多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,  0.5f,  0.5f,  0.0f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.2f , 0.0f , 0.6f},

        // D,Eが近いとき、自敵割りに影響
        // A:遠、B:遠、C:遠、D:近:、E:中、   A均衡、B均衡、C均衡、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,      0.0f,    0.0f,    0.0f,    0.0f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.8f , 0.4f},
        // A:遠、B:遠、C:遠、D:近:、E:中、   A均衡、B均衡、C敵多、D敵多、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,  0.5f,  0.5f,  1.0f,  0.5f,      0.0f,    0.0f,    0.0f,    0.5f,    0.0f,                    0.2f , 0.2f , 0.1f , 0.8f , 0.4f},
        // A:遠、B:遠、C:遠、D:近:、E:中、   A均衡、B均衡、C自多、D均衡、E均衡、    A他タゲ、B他タゲ、C他タゲ、D他タゲ、E他タゲ                  A,     B,     C,    D,      E
        {  0.7f, 0.7f, 0.7f, 0.2f, 0.5f,     0.5f,  0.5f,  0.5f,  0.0f,  0.5f,      0.0f,    0.0f,    0.0f,    1.0f,    0.0f,                    0.2f , 0.2f , 0.2f , 0.0f , 0.6f},
    };

    NeuralNetwork Neuron;
    public const int NumberOfData = 36;
    public const float LearningRate = 0.2f;
    public const float Moment = 0.99f;

    public int NumberOfInputNodes = 15;
    public int NumberOfHiddenNodes = NumberOfData;
    public int NumberOfOutputNodes = 5;



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
        while ((error > 0.0001) && (count < 4000))
        {
            error = 0;
            count++;
            for (int i = 0; i < NumberOfData; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    // ニューラルネットワークに値を入力する
                    Neuron.SetInput(j, TrainingData[i, j]);
                }

                // ニューラルネットワークに理想値を入力する
                Neuron.SetDesiredOutput(0, TrainingData[i, 15]);
                Neuron.SetDesiredOutput(1, TrainingData[i, 16]);
                Neuron.SetDesiredOutput(2, TrainingData[i, 17]);
                Neuron.SetDesiredOutput(3, TrainingData[i, 18]);
                Neuron.SetDesiredOutput(4, TrainingData[i, 19]);


                // 前方伝播する
                Neuron.FeedForward();
                // 誤差を計算する
                error += Neuron.CalculateError();
                // 誤差逆伝播する
                Neuron.BackPropagate();
            }
            error = error / NumberOfData;
        }



        testA();
        Neuron.DumpData("PostTraining.txt");
        Debug.Log("Error: {0}" + error);
        Debug.Log("Count: {0}" + count);

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
    float GetRate(float num1, float num2, int v)
    {
        float n = num1 - num2;
        n /= v;
        float result = (n + 1.0f) / 2.0f;

        return result;

    }


    void testA()
    {
        // 距離
        float[] distance =
        {
            1000f,
            1000f,
            1000f,
            1000f,
            1000f
        };
        for (int i = 0; i < 5; i++)
        {
            distance[i] /= 1000;
        }
        Neuron.SetInput(0, distance[0]);
        Neuron.SetInput(1, distance[1]);
        Neuron.SetInput(2, distance[2]);
        Neuron.SetInput(3, distance[3]);
        Neuron.SetInput(4, distance[4]);

        // 自と敵
        float[] wariai = {
            GetRate(0, 0, 16),
            GetRate(0, 0, 16),
            GetRate(0, 0, 16),
            GetRate(0, 0, 16),
            GetRate(0, 0, 16),
        };
        Neuron.SetInput(5, wariai[0]);
        Neuron.SetInput(6, wariai[1]);
        Neuron.SetInput(7, wariai[2]);
        Neuron.SetInput(8, wariai[3]);
        Neuron.SetInput(9, wariai[4]);

        // 他ターゲと
        int[] targetPointNum = {
            0,
            0,
            0,
            0,
            0,
        };
        float[] t = new float[5];

        for (int i = 0; i < 5; i++)
        {
            float target = targetPointNum[i];
            target /= 16;
            t[i] += target;
        }
        Neuron.SetInput(10, t[0]);
        Neuron.SetInput(11, t[1]);
        Neuron.SetInput(12, t[2]);
        Neuron.SetInput(13, t[3]);
        Neuron.SetInput(14, t[4]);

        Neuron.FeedForward();



        switch(Neuron.GetMaxOutputID())
        {
            case 0:
                // A地点
                Debug.Log("Target : A");
                break;
            case 1:
                // B地点
                Debug.Log("Target : B");
                break;
            case 2:
                // C地点
                Debug.Log("Target : C");
                break;
            case 3:
                // D地点
                Debug.Log("Target : D");
                break;
            case 4:
                // E地点
                Debug.Log("Target : E");
                break;
        }





    }

    void testE()
    {
        // 距離
        float[] distance =
        {
            500f,
            1500f,
            1200f,
            5000f,
            450f
        };
        for (int i = 0; i < 5; i++)
        {
            if (distance[i] >= 1000)
            {
                distance[i] = 1000;
            }
            distance[i] /= 1000;
        }
        Neuron.SetInput(0, distance[0]);
        Neuron.SetInput(1, distance[1]);
        Neuron.SetInput(2, distance[2]);
        Neuron.SetInput(3, distance[3]);
        Neuron.SetInput(4, distance[4]);

        // 自と敵
        float[] wariai = {
            GetRate(2, 0, 16),
            GetRate(2, 1, 16),
            GetRate(5, 0, 16),
            GetRate(4, 5, 16),
            GetRate(0, 0, 16),
        };
        Neuron.SetInput(5, wariai[0]);
        Neuron.SetInput(6, wariai[1]);
        Neuron.SetInput(7, wariai[2]);
        Neuron.SetInput(8, wariai[3]);
        Neuron.SetInput(9, wariai[4]);

        // 他ターゲと
        int[] targetPointNum = {
            1,
            5,
            4,
            2,
            0,
        };
        float[] t = new float[5];

        for (int i = 0; i < 5; i++)
        {
            float target = targetPointNum[i];
            target /= 16;
            t[i] = target;
        }
        Neuron.SetInput(10, t[0]);
        Neuron.SetInput(11, t[1]);
        Neuron.SetInput(12, t[2]);
        Neuron.SetInput(13, t[3]);
        Neuron.SetInput(14, t[4]);

        Neuron.FeedForward();




        switch (Neuron.GetMaxOutputID())
        {
            case 0:
                // A地点
                Debug.Log("Target : A");
                break;
            case 1:
                // B地点
                Debug.Log("Target : B");
                break;
            case 2:
                // C地点
                Debug.Log("Target : C");
                break;
            case 3:
                // D地点
                Debug.Log("Target : D");
                break;
            case 4:
                // E地点
                Debug.Log("Target : E");
                break;
        }

    }




}