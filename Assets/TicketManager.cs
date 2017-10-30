using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class TicketManager : NetworkBehaviour {
  [SerializeField] private Ticket[] ticket;

  [Server]
  public void AddTicket(Team team) {
    ticket[(int)team].AddTicket(1);
  }

}
