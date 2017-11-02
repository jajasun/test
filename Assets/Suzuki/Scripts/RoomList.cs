using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoomList : Photon.PunBehaviour {
  [SerializeField]
  private RectTransform content = null;
  [SerializeField]
  private GameObject roomInfoPrefab = null;

  public override void OnReceivedRoomListUpdate() {
    for (int i = 0; i < content.childCount; ++i) {
      Destroy(content.GetChild(i).gameObject);
    }
    var roomList = PhotonNetwork.GetRoomList();
    if (roomList == null || roomList.Length == 0) return;

    foreach (var roomInfo in roomList) {
      Debug.LogFormat("<color=red>RoomName: {0}, Playres: {1} / {2}</color>", roomInfo.Name, roomInfo.PlayerCount, roomInfo.MaxPlayers);
      var roomListItem = Instantiate(roomInfoPrefab);
      roomListItem.transform.SetParent(content);
      roomListItem.GetComponent<RoomListItem>().SetRoomInfo(roomInfo);
    }
  }

}
