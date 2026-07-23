# Lösung mpi03.c — Beide empfangen zuerst

**Status: FEHLERHAFT — garantierter Deadlock.** (Schwierigkeit: leicht–mittel)

## Was soll das Programm tun?

Zwei Prozesse sollen je einen int austauschen (Rank 0 schickt 0, Rank 1
schickt 100).

## Der Bug: Zirkuläres Warten

**Beide** Prozesse rufen zuerst `MPI_Recv` auf:

```
Rank 0: MPI_Recv(von 1)  → blockiert, wartet auf Nachricht von 1
Rank 1: MPI_Recv(von 0)  → blockiert, wartet auf Nachricht von 0
```

`MPI_Recv` ist **blockierend** und kehrt erst zurück, wenn eine passende
Nachricht angekommen ist. Keiner der beiden erreicht jemals sein
`MPI_Send` → **Deadlock** (Definition Vorlesung 3: Prozesse warten auf ein
Ergebnis, das nur ein Prozess der Gruppe selbst liefern könnte —
zirkuläre Wartebedingung).

Wichtig: Anders als beim Send-Send-Fall (siehe mpi07) ist dieser Deadlock
**garantiert und unabhängig von der Nachrichtengröße** — ein Recv kann
niemals „gepuffert" abgeschlossen werden.

## Fixes (alle drei nennen können, Übung 9 Task 4)

1. **Reihenfolge nach Rank** — einer sendet zuerst, der andere empfängt
   zuerst:

   ```c
   if (rank == 0) {
       MPI_Send(&mine, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
       MPI_Recv(&theirs, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   } else {
       MPI_Recv(&theirs, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       MPI_Send(&mine, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
   }
   ```

2. **`MPI_Sendrecv`** — die eleganteste Lösung: Senden und Empfangen in
   einem Aufruf, MPI ordnet intern deadlock-frei an:

   ```c
   MPI_Sendrecv(&mine,   1, MPI_INT, partner, 0,
                &theirs, 1, MPI_INT, partner, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   ```

3. **Nichtblockierend** — `MPI_Irecv` starten, dann senden, dann
   `MPI_Wait`:

   ```c
   MPI_Request req;
   MPI_Irecv(&theirs, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &req);
   MPI_Send(&mine, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
   MPI_Wait(&req, MPI_STATUS_IGNORE);
   ```

## Mögliche Prüferfragen

- *Woran erkennt man den Deadlock im Betrieb?* Programm hängt ohne Ausgabe
  und ohne CPU-Fortschritt; auf dem Cluster läuft der Job ins Zeitlimit.
- *Was ist ein „sicheres" (safe) MPI-Programm?* Eines, dessen Korrektheit
  nicht von Existenz oder Größe der MPI-Systempuffer abhängt — dieses
  Programm ist maximal unsicher.
- *Warum ist Sendrecv besser als manuelles Ordnen?* Kürzer, symmetrischer
  Code (kein if/else), funktioniert generisch in Ringen/Ketten; das System
  kann Senden und Empfangen effizient kombinieren.
