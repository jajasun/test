using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class Ticket : NetworkBehaviour {
  public const int MaxTicket = 1000;
  [SyncVar(hook = "OnTicketChanged")] private int currentTicket = 0;
  [SerializeField] private RectTransform ticketBar;

  [Server]
  public void AddTicket(int value) {
    currentTicket += value;
    if (currentTicket >= MaxTicket) {
      currentTicket = MaxTicket;
    }
  }

  private void OnTicketChanged(int value) {
    var width = 300 * (value / (float)MaxTicket);
    ticketBar.sizeDelta = new Vector2(width, ticketBar.sizeDelta.y);
  }

  [ClientCallback]
  private void Start() {
    var width = 300 * (currentTicket / (float)MaxTicket);
    ticketBar.sizeDelta = new Vector2(width, ticketBar.sizeDelta.y);
  }
}
