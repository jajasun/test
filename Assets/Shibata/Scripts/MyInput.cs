using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class MyInput
{

	public static bool w
	{
		get { return Input.GetKey(KeyCode.W); }
	}
	public static bool a
	{
		get { return Input.GetKey(KeyCode.A); }
	}
	public static bool s
	{
		get { return Input.GetKey(KeyCode.S); }
	}
	public static bool d
	{
		get { return Input.GetKey(KeyCode.D); }
	}
	public static bool leftCtrl
	{
		get { return Input.GetKey(KeyCode.LeftControl); }
	}
	public static bool leftShift
	{
		get { return Input.GetKey(KeyCode.LeftShift); }
	}
}
