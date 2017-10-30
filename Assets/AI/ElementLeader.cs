using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElementLeader : MonoBehaviour {

    // 指示先（トランスフォーム）
    private Transform _targetPoint;

    // ゾーン(優先度、占領チームをここから取得する)
    [SerializeField]
    private Zone[] _zone;

    // 所属チーム
    private Team _team;


    // 目標達成
    private bool _success;


    /// <summary>
    /// 初期化
    /// </summary>
	void Start ()
    {
		
	}
	
	/// <summary>
    /// 更新
    /// </summary>
	void Update ()
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
        switch (_team)
        {
            case Team.Blue:
                break;
            case Team.Red:
                break;
        }

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
    /// Managerに走らせるためのやーつ
    /// </summary>
    public void Run()
    {
        // ターゲット指定されていて,目標達成した場合
        if (_targetPoint && _success)
        {
            _targetPoint = null;
        }
        else
        {
            // 戦況（どこが自陣で敵陣か）を判断
            // 優先度を調査
            // 上記２つを加味してターゲットにする陣地を決める

            // ターゲット先が自陣（防衛）
            // どのくらい敵が入ってきているか（危険度 = 敵の数 - 味方）
            // 危険度が低い場合ほかの陣営を調査
            // 危険度が高い場合ターゲット指定

            // ターゲット先が敵陣（攻撃）
            // どのくらい敵が入っているか（危険度 = 敵の数 - 味方）
            // 危険度が低い場合ターゲット指定
            // 危険度が高い場合ほかの陣営を調査
        }
    }



}
