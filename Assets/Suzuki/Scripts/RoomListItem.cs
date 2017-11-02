using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RoomListItem : MonoBehaviour {
  [SerializeField]
  private Text roomName = null;
  [SerializeField]
  private Text players = null;

  public void SetRoomInfo(RoomInfo roomInfo) {
    roomName.text = roomInfo.Name;
    players.text = string.Format("{0} / {1}", roomInfo.PlayerCount, roomInfo.MaxPlayers);
  }

  public void OnJoinButtonClicked() {
    PhotonNetwork.JoinRoom(roomName.text);
  }
}
