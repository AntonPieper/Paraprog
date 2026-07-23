# Lösung omp08.c — Dreiecksschleife mit schedule(static)

**Status: KORREKT, aber PERFORMANCE-PROBLEM: Load Imbalance.** (Schwierigkeit: mittel)

## Was berechnet das Programm?

Eine Doppelsumme über das „obere Dreieck": für jedes `i` läuft `j` von `i`
bis `n-1`. Die Arbeit pro äußerer Iteration ist also `n - i` innere
Iterationen — **sie nimmt mit i linear ab**. `reduction(+:total)` macht die
Summation korrekt (kein Data Race).

## Das Performance-Problem: Lastungleichgewicht

`schedule(static)` (ohne Chunk-Größe) teilt die i-Iterationen in **gleich
große zusammenhängende Blöcke**:

- Thread 0 bekommt i = 0 … n/T−1 → die **teuersten** Iterationen
  (innere Schleife fast n lang).
- Der letzte Thread bekommt die billigsten (innere Schleife fast leer).

Bei 4 Threads erledigt Thread 0 ca. 7/16 der Gesamtarbeit, Thread 3 nur
1/16. Alle anderen warten an der **impliziten Barriere** am Ende der
Schleife auf Thread 0 → **Load Imbalance**, Speedup weit unter linear
(dasselbe Muster wie Mandelbrot in Übung 4/5: teure Pixel in der Bildmitte).

## Fix

Mehrere Möglichkeiten, in der Prüfung benennen und abwägen:

```c
#pragma omp parallel for schedule(static, 16) reduction(+:total)   /* zyklisch */
#pragma omp parallel for schedule(dynamic, 16) reduction(+:total)
#pragma omp parallel for schedule(guided) reduction(+:total)
```

- **`static` mit kleiner Chunk-Größe** (zyklische Verteilung): Jeder Thread
  bekommt abwechselnd teure und billige Blöcke — gleicht strukturiert
  fallende Kosten gut aus, ohne Laufzeit-Overhead.
- **`dynamic`**: Threads holen sich Blöcke zur Laufzeit — beste Balance bei
  unvorhersehbarer Last, aber Scheduling-Overhead pro Block; Chunk-Größe
  = Trade-off Balance vs. Overhead/Lokalität.
- **`guided`**: erst große, dann kleinere Blöcke — Kompromiss mit adaptivem
  Overhead.

Hier reicht `schedule(static, 16)`, weil die Kostenstruktur **bekannt und
regelmäßig** ist.

## Mögliche Prüferfragen

- *Woran erkennt man Load Imbalance im Experiment?* Speedup stagniert
  (z. B. 2× bei 2 Threads, kaum mehr bei 4+); per-Thread-Zeiten ungleich.
- *Wie verteilt `schedule(static, 4)` 16 Iterationen auf 3 Threads?*
  T0: 0–3, T1: 4–7, T2: 8–11, T0: 12–15 (zyklisch, deterministisch).
- *Warum nicht immer `dynamic`?* Overhead pro Chunk-Vergabe und schlechtere
  Lokalität; bei regelmäßiger Last ist static besser.
- *Was macht `schedule(runtime)`?* Policy kommt aus `OMP_SCHEDULE`
  (z. B. `"dynamic,4"`) — Tuning ohne Neukompilieren (Übung 5, Mandelbrot).
