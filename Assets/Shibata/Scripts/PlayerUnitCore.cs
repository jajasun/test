using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG;

public class PlayerUnitCore : MonoBehaviour
{
	Rigidbody _body;

	[SerializeField]
	float _speed = 10f;

	PlayerUnitAnimationController _animCon;
	PlayerUnitCameraController _camCon;

	private void Awake()
	{
		_animCon = GetComponent<PlayerUnitAnimationController>();
		_camCon = GetComponent<PlayerUnitCameraController>();
	}
	// Use this for initialization
	void Start ()
	{
		//カーソルの無効化
		Cursor.visible = false;
		Cursor.lockState = CursorLockMode.Locked;

		_body = GetComponent<Rigidbody>();

	}

	// Update is called once per frame
	void Update ()
	{
		//アニメーションフラグの設定
		var flags = _animCon.flags;
		flags.isIdle = !(MyInput.w || MyInput.a || MyInput.s || MyInput.d || MyInput.leftCtrl);
		flags.isForward = MyInput.w;
		flags.isBack = MyInput.s;
		flags.isStrafe = MyInput.a || MyInput.d;
		flags.isSprint = MyInput.leftShift;
		flags.isCrouch = MyInput.leftCtrl;

		//移動速度の設定
		Vector3 vel = new Vector3();
		if(MyInput.w)
		{
			vel.z += _speed;
		}
		if(MyInput.a)
		{
			vel.x -= _speed;
		}
		if(MyInput.d)
		{
			vel.x += _speed;
		}
		if(MyInput.s)
		{
			vel.z -= _speed;
		}
		if(MyInput.leftShift&&MyInput.w)
		{
			vel = Vector3.zero;
			vel.z += _speed * 1.5f;
		}
		_body.velocity = transform.TransformDirection(vel);
		
	}
}
