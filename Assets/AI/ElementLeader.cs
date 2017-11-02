using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElementLeader : MonoBehaviour
{

    // 指示先（トランスフォーム）
    private Transform _targetPoint;

    // 所属チーム
    public Team Team { get; set; }


    // 目標達成
    private bool _success;

    // マネージャー
    ElementLeaderManager _elementManager;

    // LeaderのAI
    public int ID { get; set; }


    /// <summary>
    /// 初期化
    /// </summary>
    void Start()
    {

    }

    /// <summary>
    /// 更新
    /// </summary>
    void Update()
    {

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
            
            // 優先度を調査
            
            // 戦況（どこが自陣で敵陣か）を判断



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
