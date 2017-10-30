using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class WaitingRoomPlayerListIcon : NetworkBehaviour {
  [SerializeField]
  private GameObject kickButton;
  [SerializeField]
  private GameObject roomMasterIcon;

  private void Start() {
    if (!isLocalPlayer) {
      kickButton.SetActive(false);
      roomMasterIcon.SetActive(false);
    }
  }
}