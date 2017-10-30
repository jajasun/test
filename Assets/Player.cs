using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public enum Team {
  Blue = 0,
  Red = 1,
  Neutral = -1,
}

public class Player : NetworkBehaviour {
  [SerializeField] private MeshRenderer meshRenderer;
  public Team Team { get; set; }

  private void Start() {
    Team = Team.Blue;
  }

  private void Update() {
    if (!isLocalPlayer) return;
    if (Input.GetKeyDown(KeyCode.Alpha1)) {
      CmdChangeTeam(Team.Blue);
    }
    if (Input.GetKeyDown(KeyCode.Alpha2)) {
      CmdChangeTeam(Team.Red);
    }
  }

  public override void OnStartLocalPlayer() {
    if (meshRenderer == null) return;
    meshRenderer.material.color = Color.red;
  }

  [Command]
  private void CmdChangeTeam(Team team) {
    Team = team;
  }
}
