# Lösung mpi04.c — Token-Ring

**Status: KORREKT.** (Schwierigkeit: mittel)

## Was berechnet das Programm?

Der Token-Ring aus Übung 8: Ein int-Token läuft einmal durch den Ring
0 → 1 → 2 → … → size−1 → 0. Jeder Rank ≥ 1 addiert seinen Rank; am Ende
prüft Rank 0: Token = 1 + 2 + … + (size−1) = size·(size−1)/2 (Gauß).

## Wie funktioniert die Kommunikation — und warum kein Deadlock?

Die entscheidende Asymmetrie (in der Prüfung explizit benennen):

- **Rank 0 sendet zuerst** und empfängt zuletzt.
- **Alle anderen empfangen zuerst**, verarbeiten, senden dann weiter.

Dadurch existiert zu jedem Zeitpunkt genau **eine** wandernde Nachricht,
und für jedes blockierende `MPI_Recv` ist das passende `MPI_Send` schon
unterwegs bzw. wird garantiert erreicht. Würden **alle** Ranks zuerst
senden (mit großen Nachrichten/synchronem Protokoll) oder alle zuerst
empfangen, gäbe es zirkuläres Warten → Deadlock (vgl. mpi03/mpi07).

Nebenbemerkung: `(rank + 1) % size` — der Modulo schließt den Ring
(size−1 sendet an 0).

## Ablauf mit 4 Prozessen

```
Rank 0: sendet 0 an Rank 1
Rank 1: empfängt 0, +1 → sendet 1 an Rank 2
Rank 2: empfängt 1, +2 → sendet 3 an Rank 3
Rank 3: empfängt 3, +3 → sendet 6 an Rank 0
Rank 0: empfängt 6 = 4·3/2 ✓
```

## Performance-Einordnung

Der Ring ist **vollständig seriell** — jeder wartet auf seinen Vorgänger;
Laufzeit wächst linear mit size, Parallelität = 1. Als Berechnungsmuster
für eine Summe wäre das schlecht: `MPI_Reduce` erledigt dieselbe Reduktion
baumbasiert in O(log size) und ist dafür das richtige Werkzeug. Der Ring
ist ein Lehrbeispiel für Punkt-zu-Punkt-Kommunikation, kein effizientes
Reduktionsmuster.

## Mögliche Prüferfragen

- *Was passiert mit nur 1 Prozess?* Guard bricht ab (size < 2). Ohne Guard:
  Rank 0 würde an den nicht existierenden Rank 1 senden → Fehler/Absturz.
- *Warum blockiert `MPI_Send` von Rank 0 nicht ewig?* Kleine Nachricht →
  wird typischerweise vom System gepuffert (Eager-Protokoll); und selbst
  synchron würde Rank 1 ja sofort sein Recv erreichen. Das Programm ist
  auch mit `MPI_Ssend` korrekt — es ist ein „sicheres" Programm.
- *Wie würde man alle Ranks gleichzeitig ihren Rank an den Nachbarn
  schicken lassen (jeder sendet, jeder empfängt)?* `MPI_Sendrecv` oder
  Isend/Recv/Wait — siehe Halo-Austausch (mpi09, mpi13).
- *Alternative Kollektivoperation für „Summe aller Ranks bei Rank 0"?*
  `MPI_Reduce(&rank, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD)`.
