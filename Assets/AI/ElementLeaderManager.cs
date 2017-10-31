using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElementLeaderManager : MonoBehaviour {



    // ゾーン(優先度、占領チームをここから取得する)
    [SerializeField]
    private Zone[] _zone;

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

    // Use this for initialization
    void Start () {

        // 各チームのAIの数を取得
        _red_AI = 15;
        _blue_AI = 16;
        _total_AI = _red_AI + _blue_AI;


        CreateLeader();


    }
	
	// Update is called once per frame
	void Update () {
		//foreach(var leader in _leader)
  //      {
  //          leader.Run();
  //      }
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
        return new GameObject().transform;
    }



    /// <summary>
    /// グループ分けの通りに生成する
    /// </summary>
    void CreateLeader()
    {
        // リーダーは４人グループに１人存在する
        _total_leader = _total_AI / 4;
        if (_total_AI % 4 > 0)
            _total_leader++;
        _leader = new ElementLeader[_total_leader];

        // 実体化
        ElementLeader prefab = _leaderPrefab;
        ElementLeader el;
        for(var i = 0; i<_leader.Length;i++)
        {
            el = Instantiate(prefab);
            _leader[i] = el.GetComponent<ElementLeader>();
        }

        // テスト
        _red_leader = _red_AI / 4;
        if (_red_AI % 4 > 0)
            _red_leader++;
        _blue_leader = _blue_AI / 4;
        if (_blue_AI % 4 > 0)
            _blue_leader++;

        _red_leader = _red_AI / 4;

        Debug.Log(_red_leader);

        for (var i = 0; i < _red_leader; i++)
        {
            _leader[i].Team = Team.Red;
        }
        for (var i = _red_leader; i < _total_leader; i++)
        {
            _leader[i].Team = Team.Blue;
        }

        for (var i = 0; i < _leader.Length; i++)
        {
            Debug.Log(_leader[i].Team);
        }
        

    }




}
