using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerUnitAnimationController : MonoBehaviour
{
	[SerializeField]
	Animator _animator;

	public class Flags
	{
		public Animator _animator;
		/// <summary>しゃがみフラグ</summary>
		public bool isCrouch
		{
			get { return _animator.GetBool("IsCrouch"); }
			set { _animator.SetBool("IsCrouch", value); }
		}

		/// <summary>スプリントフラグ</summary>
		public bool isSprint
		{
			get { return _animator.GetBool("IsSprint"); }
			set { _animator.SetBool("IsSprint", value); }
		}

		/// <summary>後退フラグ</summary>
		public bool isBack
		{
			get { return _animator.GetBool("IsBack"); }
			set { _animator.SetBool("IsBack", value); }
		}

		/// <summary>横歩きフラグ</summary>
		public bool isStrafe
		{
			get { return _animator.GetBool("IsStrafe"); }
			set { _animator.SetBool("IsStrafe", value); }
		}

		/// <summary>前進フラグ</summary>
		public bool isForward
		{
			get { return _animator.GetBool("IsForward"); }
			set { _animator.SetBool("IsForward", value); }
		}

		/// <summary>待機フラグ</summary>
		public bool isIdle
		{
			get { return _animator.GetBool("IsIdle"); }
			set { _animator.SetBool("IsIdle", value); }
		}

		/// <summary>反転フラグ</summary>
		public bool isMirror
		{
			get { return _animator.GetBool("IsMirror"); }
			set { _animator.SetBool("IsMirror", value); }
		}
	}

	Flags _flags = new Flags();

	/// <summary>アニメーションフラグ</summary>
	public Flags flags
	{
		get { return _flags; }
	}

	private void Awake()
	{
		if (_animator == null)
			_animator = GetComponentInChildren<Animator>();
		_flags._animator = _animator;
	}

	private void Update()
	{
		if (Input.GetKeyDown(KeyCode.R))
			_animator.SetTrigger("ReloadTrigger");

		_animator.SetBool("IsRecoil", Input.GetMouseButton(0));
	}
}
