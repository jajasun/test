using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElementLeaderManager : MonoBehaviour {

    // AIのIDを取得する
    private int _id;
    public int ID
    {
        get { return _id; }
        set { _id = value; }
    }

    ElementLeader[] _leader;

	// Use this for initialization
	void Start () {
        _leader = new ElementLeader[_id];	
	}
	
	// Update is called once per frame
	void Update () {
		foreach(var leader in _leader)
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


}
