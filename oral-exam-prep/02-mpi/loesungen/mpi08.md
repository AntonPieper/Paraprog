# Lösung mpi08.c — Sendepuffer vor MPI_Wait überschrieben

**Status: FEHLERHAFT — Puffer-Wiederverwendung vor Abschluss des Isend.**
(Schwierigkeit: mittel–schwer)

## Was soll das Programm tun?

Rank 0 schickt ein Array (0…N−1) nichtblockierend an Rank 1 und will die
Wartezeit nutzen, um „die nächste Iteration vorzubereiten". Rank 1 prüft
die Daten.

## Der Bug

Die Grundregel nichtblockierender Kommunikation (Vorlesung 9):

> Zwischen **Initiierung** (`MPI_Isend`) und **Abschluss** (`MPI_Wait` /
> erfolgreicher `MPI_Test`) darf der **Sendepuffer nicht verändert**
> werden. (Beim `MPI_Irecv` darf der Empfangspuffer weder gelesen noch
> geschrieben werden.)

Hier wird `buf` **zwischen** `MPI_Isend` und `MPI_Wait` mit −1
überschrieben. `MPI_Isend` kehrt sofort zurück — die eigentliche
Übertragung läuft **im Hintergrund** und liest den Puffer erst später aus.
Was Rank 1 empfängt, ist **undefiniert**: alte Werte, −1 oder eine
Mischung — je nach Protokoll (eager kopiert früh, rendezvous liest spät),
Nachrichtengröße und Timing. Klassischer **nichtdeterministischer** Fehler:
klein getestet korrekt, groß im Cluster falsch.

## Fix

Erst abschließen, dann wiederverwenden:

```c
MPI_Isend(buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
/* hier nur Arbeit, die buf NICHT anfasst */
MPI_Wait(&request, MPI_STATUS_IGNORE);
for (int i = 0; i < N; i++)   /* jetzt ist buf wieder frei */
    buf[i] = -1;
```

Wenn man wirklich währenddessen neue Daten aufbauen will: **zweiten
Puffer** verwenden (Double Buffering) — genau dafür ist nichtblockierende
Kommunikation gedacht: **Überlappung von Kommunikation und Berechnung**,
nur eben mit disjunkten Daten.

## Konzepte zum Erklären

- Zwei Phasen: **Initiierung** (lokal, kehrt sofort zurück) und
  **Abschluss** (`MPI_Wait` blockierend / `MPI_Test` nichtblockierend mit
  Flag). `request` ist das Handle auf den Kommunikationszustand.
- `MPI_Isend` + sofortiges `MPI_Wait` ≡ blockierendes `MPI_Send`.
- Jede nichtblockierende Operation **muss** abgeschlossen werden
  (sonst Ressourcen-Leck); `MPI_Waitall` für mehrere Requests.

## Mögliche Prüferfragen

- *Warum „repariert" MPI_Wait das nicht?* Wait erzwingt nur den Abschluss;
  die Verfälschung ist dann längst passiert. Wait/Test beschleunigen die
  Kommunikation auch nicht — sie stellen nur Abschluss fest.
- *Wie ist es beim Irecv?* Empfangspuffer zwischen Irecv und Wait weder
  lesen noch schreiben — Inhalt erst nach Wait gültig.
- *Wozu ist das Muster gut, wenn man es richtig macht?* Kommunikation mit
  Berechnung überlappen (Ping-Pong-Experiment der Vorlesung: Speedup 2),
  Deadlocks vermeiden, Serialisierung vermeiden.
