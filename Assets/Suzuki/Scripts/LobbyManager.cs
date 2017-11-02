using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyManager : Photon.PunBehaviour {
  [SerializeField]
  private string gameVersion = "1.0.0";
  [SerializeField]
  private PhotonLogLevel logLevel = PhotonLogLevel.Informational;

  private void Awake() {
    PhotonNetwork.autoJoinLobby = false;
    PhotonNetwork.automaticallySyncScene = true;
    PhotonNetwork.logLevel = logLevel;
  }

  private void Start() {
    PhotonNetwork.ConnectUsingSettings(gameVersion);
  }

  public override void OnConnectedToMaster() {
    Debug.Log("<color=green>Launcher.OnConnectedToMaster() was called by PUN</color>");
    PhotonNetwork.JoinLobby();
  }

  public override void OnDisconnectedFromPhoton() {
    Debug.LogWarning("<color=green>Launcher.OnDisconnectedFromPhoton() was called by PUN</color>");
  }

  public override void OnJoinedRoom() {
    Debug.Log("<color=green>Launcher.OnJoinedRoom() was called by PUN. Now this client is in a room.</color>");
    //if (PhotonNetwork.room.PlayerCount == 1) {
    //  Debug.Log("We load the Room");
    //  PhotonNetwork.LoadLevel("Room");
    //}
  }

}
