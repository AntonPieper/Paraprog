# Lösung mpi06.c — Bcast nur beim Root

**Status: FEHLERHAFT — Kollektivoperation nicht von allen aufgerufen → Hänger.**
(Schwierigkeit: mittel)

## Was soll das Programm tun?

Rank 0 soll zwei Parameter (Problemgröße, Seed) an alle Prozesse verteilen.

## Der Bug

`MPI_Bcast` steht **nur im if-Zweig von Rank 0**; die anderen Prozesse
versuchen, die Daten mit `MPI_Recv` zu empfangen. Das verletzt die
Grundregel für Kollektivoperationen (Vorlesung 10):

> **Alle Prozesse des Kommunikators müssen die Kollektivoperation
> aufrufen** — und zwar alle dieselbe, in derselben Reihenfolge.

Ein Broadcast ist **kein** einfaches Send an alle: Die Bibliothek verwendet
intern z. B. baumbasierte Algorithmen, bei denen auch Nicht-Root-Prozesse
weiterleiten. Kollektivnachrichten matchen grundsätzlich **nicht** mit
Punkt-zu-Punkt-`MPI_Recv` (Kollektive haben nicht einmal Tags).

**Folge:** Rank 0 blockiert im Bcast und wartet darauf, dass die anderen
in den Broadcast eintreten; die anderen blockieren im `MPI_Recv` und warten
auf eine P2P-Nachricht, die nie kommt → **Deadlock/Hänger**.

## Fix

Alle Prozesse rufen denselben Bcast auf (SPMD — der Aufruf steht
**außerhalb** jeder Rank-Verzweigung; nur das Befüllen ist Root-Sache):

```c
if (rank == 0) {
    params[0] = 1000;
    params[1] = 42;
}
MPI_Bcast(params, 2, MPI_INT, 0, MPI_COMM_WORLD);
```

Beim Root ist `params` Eingabe, bei allen anderen Ausgabe — dasselbe
`buffer`-Argument für beide Rollen, `root = 0` sagt, wessen Daten verteilt
werden.

## Mögliche Prüferfragen

- *Eigenschaften von Kollektivoperationen?* Über einen Kommunikator; alle
  müssen aufrufen; **keine Tags**; blockierend oder nichtblockierend
  (MPI ≥ 3.0, z. B. `MPI_Ibcast`); Puffergrößen müssen exakt passen.
- *Warum Bcast statt einer Schleife mit p−1 Sends?* Kürzer und schneller:
  baumbasiert O(log p) statt O(p) sequenzieller Sends; zudem
  Performance-Portabilität (Bibliothek wählt den Algorithmus passend zur
  Architektur — vgl. die 9 Bcast-Algorithmen in EduMPI).
- *Kategorien der Kollektive?* One-to-All (Bcast, Scatter), All-to-One
  (Gather, Reduce), All-to-All (Allgather, Alltoall), Synchronisation
  (Barrier).
- *Verwandter Deadlock aus der Vorlesung?* Kollektiv + P2P verschränkt:
  Rank 0: Bcast, dann Send; Rank 1: Recv, dann Bcast → Deadlock. Fix:
  `MPI_Ibcast` + `MPI_Wait` oder Reihenfolge angleichen.
