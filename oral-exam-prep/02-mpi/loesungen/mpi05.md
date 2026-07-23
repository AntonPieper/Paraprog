# Lösung mpi05.c — Pi mit MPI_Reduce

**Status: KORREKT und effizient.** (Schwierigkeit: mittel)

## Was berechnet das Programm?

Pi durch numerische Integration von 4/(1+x²) über [0,1] — dieselbe
Mathematik wie in den OpenMP-Übungen, aber mit MPI über Prozesse verteilt.

## Wie funktioniert die Parallelisierung?

- **Zyklische Verteilung** (round-robin): Rank r bearbeitet die Iterationen
  r, r+size, r+2·size, … — `for (i = rank; i < num_steps; i += size)`.
  Jeder Prozess erhält so fast exakt gleich viele Iterationen — auch wenn
  `num_steps` nicht durch `size` teilbar ist (**kein Rest-Bug**, vgl.
  mpi12!). Da alle Iterationen gleich teuer sind, herrscht **Lastbalance**.
- Jeder Prozess akkumuliert **lokal** in `local_sum` — keinerlei
  Kommunikation während der Rechenphase.
- `MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, ...)`:
  **Kollektivoperation** — alle Prozesse rufen sie auf, die Teilsummen
  werden (typischerweise baumbasiert, O(log p)) zur Gesamtsumme am
  **Root-Prozess 0** kombiniert.
- Nur Rank 0 gibt das Ergebnis aus — bei allen anderen ist `global_sum`
  nach dem Reduce **nicht definiert** (bleibt hier 0.0; nur Root erhält
  das Ergebnis).

Das ist das MPI-Pendant zur OpenMP-`reduction(+:sum)`.

## Warum ist das effizient?

- Kommunikation: **genau eine** Reduktion von je einem double am Ende —
  minimaler Kommunikationsanteil, Rechenanteil dominiert →
  gute **Skalierbarkeit** (strong scaling nahezu linear, bis num_steps/p
  zu klein wird).
- Kollektiv statt p−1 Einzelnachrichten an Rank 0: kürzer und schneller
  (baumbasierte Implementierung durch die MPI-Bibliothek).

## Mögliche Prüferfragen

- *Zyklische vs. Blockverteilung?* Block: Rank r bekommt zusammenhängenden
  Bereich [r·n/p, (r+1)·n/p) — Vorsicht bei Rest (mpi12-Bug); zyklisch:
  automatisch ausgeglichen. Hier egal fürs Ergebnis (Addition kommutativ),
  beide korrekt umsetzbar.
- *Was, wenn alle Prozesse das Ergebnis brauchen?* `MPI_Allreduce`
  (entspricht Reduce + Bcast).
- *Müssen alle Prozesse MPI_Reduce aufrufen?* Ja — Kollektivoperationen
  müssen von **allen** Prozessen des Kommunikators aufgerufen werden,
  sonst Hänger (vgl. mpi06).
- *Ist das Ergebnis bitidentisch mit der sequenziellen Summe?* Nicht
  zwingend — andere Additionsreihenfolge → Gleitkomma-Rundungsunterschiede
  (Reduktionsoperator muss assoziativ/kommutativ sein).
- *Wie sähe schwaches Skalieren hier aus?* num_steps proportional zu p
  erhöhen — Laufzeit sollte konstant bleiben (Gustafson).
