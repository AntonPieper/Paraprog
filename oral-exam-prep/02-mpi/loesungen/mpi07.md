# Lösung mpi07.c — Beide senden zuerst (Ssend)

**Status: FEHLERHAFT — garantierter Deadlock durch MPI_Ssend.**
(Schwierigkeit: mittel)

## Was soll das Programm tun?

Zwei Prozesse tauschen je ein Array mit 1.000.000 ints aus
(bidirektionaler Austausch).

## Der Bug

**Beide** Prozesse rufen zuerst `MPI_Ssend` auf:

- `MPI_Ssend` = **synchroner Modus**: kehrt erst zurück, wenn der
  Empfänger das passende **Recv gestartet** hat (Rendezvous, keine
  Pufferung).
- Beide warten also darauf, dass der **jeweils andere** sein `MPI_Recv`
  erreicht — das steht aber erst **nach** dem Ssend → zirkuläres Warten →
  **Deadlock, bei jedem Lauf, unabhängig von Puffergrößen**.

## Die Prüfungs-Kernfrage: Warum „funktioniert" dasselbe mit MPI_Send manchmal?

Standard-`MPI_Send` darf zurückkehren, wenn die Nachricht **vom System
zwischengepuffert** wurde:

- **Kleine Nachrichten** → **Eager-Protokoll**: Nachricht wird sofort in
  einen Systempuffer kopiert, Send kehrt zurück → der latente Bug bleibt
  **versteckt**, das Programm läuft.
- **Große Nachrichten** (wie hier 4 MB) → **Rendezvous-Protokoll**: Send
  verhält sich wie Ssend und blockiert → Deadlock.

Ein Programm, dessen Korrektheit von Systempuffern abhängt, ist **nicht
„safe"**. Deshalb ist `MPI_Ssend` ein **Debug-Werkzeug**: Es deckt solche
latenten Deadlocks deterministisch auf („läuft klein im Test, hängt im
Cluster bei großen Daten").

## Fixes (Übung 9, Task 4 — alle drei nennen)

1. **Nichtblockierend:** `MPI_Isend` + `MPI_Recv` + `MPI_Wait`
   — das Isend kehrt sofort zurück (Initiierung ist lokal), dann kann
   jeder empfangen.
2. **`MPI_Sendrecv`** — kombiniert beides deadlock-frei, eleganteste
   Lösung für paarweisen Austausch.
3. **Rank-basierte Umordnung** — Rank 0: Send→Recv, Rank 1: Recv→Send.

## Mögliche Prüferfragen

- *Die vier Kommunikationsmodi?* Standard (Send: puffert oder synchron —
  Implementierungsentscheidung), Buffered (Bsend: Nutzerpuffer via
  `MPI_Buffer_attach`, kehrt immer zurück), Synchronous (Ssend:
  Rendezvous), Ready (Rsend: Recv muss schon gepostet sein — praktisch nie
  verwenden).
- *Trade-offs?* Bsend: geringe Latenz, Kopier-Overhead, schlechte
  Bandbreite; Ssend: beste Bandbreite (kein Kopieren), Latenz/Wartezeiten,
  Deadlock-Risiko.
- *Nach MPI_Send zurückgekehrt — ist die Nachricht angekommen?* Nein!
  Garantiert ist nur: der Sendepuffer ist wiederverwendbar. Zustellung ↔
  Pufferung ist nicht unterscheidbar.
- *Wie diagnostiziert man das auf dem Cluster?* Job hängt bis zum
  Zeitlimit; Trace-Analyse (Scalasca) zeigt beide Prozesse wartend im
  Ssend; MUST kann solche Fehler statisch/dynamisch erkennen.
