using UnityEngine;
using UnityEngine.Networking;

public class DisableRemotePlayerBehaviours : NetworkBehaviour {
  [SerializeField] private Behaviour[] behaviours;

  void Start() {
    if (!isLocalPlayer) {
      foreach (var behaviour in behaviours) {
        behaviour.enabled = false;
      }
    }
  }
}