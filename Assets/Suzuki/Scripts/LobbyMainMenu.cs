using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LobbyMainMenu : MonoBehaviour {
  public void OnRoomCreateButtonClicked() {
    byte maxPlayersPerRoom = 4;
    Debug.LogFormat("<color=green>MainMenu.OnRoomCreateButtonClicked() was called.\nCalling PhotonNetwork.CreateRoom(null, new RoomOptions() {{ maxPlayers = {0} }}, null);</color>", maxPlayersPerRoom);
    PhotonNetwork.CreateRoom(null, new RoomOptions() { MaxPlayers = maxPlayersPerRoom }, null);
  }

}
