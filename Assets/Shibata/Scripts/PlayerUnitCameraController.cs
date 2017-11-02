using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class PlayerUnitCameraController : MonoBehaviour
{
	Camera _camera;

	[SerializeField]
	GameObject _parent;
	[SerializeField]
	GameObject _spine;

	[SerializeField]
	float _distance;
	[SerializeField]
	float _horizontalPos;
	[SerializeField]
	float _height;

	[SerializeField]
	float _mouseSensi = 300f;

	[SerializeField]
	float _elevationLimit = 60f;
	[SerializeField]
	float _depressionLimit = 60f;

	float _angleH = 0f;
	float _angleV = 0f;

	void Start()
	{
		_camera = Camera.main;
	}

	private void LateUpdate()
	{
		_spine.transform.rotation = Quaternion.Euler(
			_spine.transform.eulerAngles.x + _camera.transform.eulerAngles.x,
			_spine.transform.eulerAngles.y,
			_spine.transform.eulerAngles.z);
	}

	void Update()
	{

		Vector2 delta = new Vector2();
		delta.x = Input.GetAxis("Mouse X") * Time.deltaTime * _mouseSensi;
		delta.y = Input.GetAxis("Mouse Y") * Time.deltaTime * _mouseSensi;

		_angleH += delta.x;
		_angleV -= delta.y;

		_angleV = Mathf.Clamp(_angleV, -_depressionLimit, _elevationLimit);

		Quaternion rotH = Quaternion.AngleAxis(_angleH, Vector3.up);
		Quaternion rotV = Quaternion.AngleAxis(_angleV, Vector3.right);
		_camera.transform.rotation = rotH * rotV;

		Vector3 manip = Vector3.zero;
		manip -= _camera.transform.forward * _distance;
		manip.y += _height;
		manip += _camera.transform.right * _horizontalPos;

		_camera.transform.position = _parent.transform.position;
		_camera.transform.position += manip;

		transform.eulerAngles = new Vector3(transform.eulerAngles.x, _camera.transform.eulerAngles.y, transform.eulerAngles.z);
	}
}