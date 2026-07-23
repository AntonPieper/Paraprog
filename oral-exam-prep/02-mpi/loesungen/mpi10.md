# Lösung mpi10.c — Gather mit falschem recvcount

**Status: FEHLERHAFT — recvcount muss die Anzahl **pro Prozess** sein.**
(Schwierigkeit: schwer)

## Was soll das Programm tun?

Jeder Prozess hält N=4 lokale Werte (`rank·N + i`); Rank 0 soll alle
mit `MPI_Gather` in Rank-Reihenfolge einsammeln: 0,1,2,3, 4,5,6,7, …

## Der Bug

```c
MPI_Gather(local, N, MPI_INT,
           all,   N * size, MPI_INT,   /* FALSCH */
           0, MPI_COMM_WORLD);
```

Die Signatur-Falle: **`recvcount` ist die Elementanzahl, die der Root von
JEDEM einzelnen Prozess empfängt** — nicht die Gesamtgröße des
Empfangspuffers. Richtig ist `recvcount = N`; die Bibliothek legt die
Beiträge dann selbst hintereinander (Gesamtgröße size·N ergibt sich).

Mit `N * size` erwartet der Root von jedem Prozess size·N Elemente,
gesendet werden aber nur N → **Message-Truncation-Fehler** (Empfangszusage
und Sendegröße passen nicht zusammen); Open MPI bricht typischerweise mit
einer Fehlermeldung ab, andere Implementierungen liefern undefiniertes
Verhalten/Pufferüberläufe. Zusätzlich würde der Root über das Ende von
`all` hinaus schreiben wollen — der Puffer ist nur size·N groß.

## Fix

```c
MPI_Gather(local, N, MPI_INT,
           all,   N, MPI_INT,
           0, MPI_COMM_WORLD);
```

Merkregel: Bei Scatter/Gather beziehen sich **sendcount und recvcount
beide auf den Anteil eines einzelnen Prozesses** (bei Scatter: sendcount =
Elemente pro Empfänger). Die Puffergröße beim Root ist Sache des
Programmierers (Vorlesung: „receive buffer of sufficient size is the
programmer's responsibility").

## Randbedingungen, die man nennen kann

- Nur der **Root** braucht einen gültigen `all`-Puffer (bei den anderen
  darf das Argument NULL sein — wie hier).
- Gather sammelt in **Rank-Reihenfolge** — deterministisches Layout.
- Ungleiche Anteile (n nicht durch p teilbar)? → `MPI_Gatherv` mit
  `recvcounts[]` und `displs[]` (Übung 10: 22 Elemente auf 4 Prozesse als
  6/6/5/5).

## Mögliche Prüferfragen

- *Gegenstück zum Verteilen?* `MPI_Scatter` — Voraussetzung: n durch p
  teilbar, sonst `MPI_Scatterv`.
- *Alle sollen das Gesamtergebnis haben?* `MPI_Allgather`
  (≡ Gather + Bcast, aber effizienter implementiert).
- *Unterschied Gather vs. Reduce?* Gather sammelt Rohdaten (Konkatenation),
  Reduce kombiniert sie elementweise mit einem Operator (z. B. MPI_SUM).
- *Warum überhaupt Kollektive statt p Sends an Root?* O(log p)-Algorithmen,
  weniger Code, Performance-Portabilität.
