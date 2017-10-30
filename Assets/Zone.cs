using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class Zone : NetworkBehaviour {
  private int[] unitCount = new int[2];
  private int dominationCounter = 0;
  private Team dominatedTeam = Team.Neutral;
  private State state = State.Neutral;
  [SerializeField] private TicketManager ticketManager;
  public enum State {
    Dominated,
    Dominating,
    Neutral,
  }

  [ServerCallback]
  private void OnTriggerEnter(Collider other) {
    if (other.tag != "Player") return;
    var player = other.GetComponent<Player>();
    if (player == null) return;
    ++unitCount[(int)player.Team];
  }

  [ServerCallback]
  private void OnTriggerExit(Collider other) {
    if (other.tag != "Player") return;
    var player = other.GetComponent<Player>();
    if (player == null) return;
    --unitCount[(int)player.Team];
  }

  [ServerCallback]
  private void Update() {
    switch (state) {
      case State.Dominated:
        ticketManager.AddTicket(dominatedTeam);
        if (unitCount[(int)Team.Blue] == 0 && unitCount[(int)Team.Red] == 0) {
          dominationCounter += System.Math.Sign(dominationCounter);
        }
        else {
          dominationCounter += (unitCount[(int)Team.Blue] - unitCount[(int)Team.Red]);
        }
        dominationCounter = Mathf.Clamp(dominationCounter, -100, 100);
        if (dominatedTeam == Team.Blue) {
          if (dominationCounter < 0) {
            dominatedTeam = Team.Neutral;
            state = State.Neutral;
          }
        }
        else {
          if (dominationCounter > 0) {
            dominatedTeam = Team.Neutral;
            state = State.Neutral;
          }
        }
        break;

      case State.Dominating:
        if (unitCount[(int)Team.Blue] == 0 && unitCount[(int)Team.Red] == 0) {
          state = State.Neutral;
        }
        dominationCounter += (unitCount[(int)Team.Blue] - unitCount[(int)Team.Red]);

        if (System.Math.Abs(dominationCounter) >= 100) {
          if (dominationCounter > 0) {
            dominatedTeam = Team.Blue;
          }
          else {
            dominatedTeam = Team.Red;
          }
          state = State.Dominated;
        }
        break;

      case State.Neutral:
        if (unitCount[(int)Team.Blue] - unitCount[(int)Team.Red] != 0) {
          state = State.Dominating;
          break;
        }
        if (System.Math.Abs(dominationCounter) > 0) {
          dominationCounter += System.Math.Sign(-dominationCounter);
        }
        break;

      default:
        break;
    }
  }

}
