# Aufgabe perf04 — Zwei Experimente interpretieren

Für dasselbe MPI-Programm liegen zwei Messreihen vom Fulda-Cluster vor.

**Experiment A** (Gittergröße fest: 8192 × 8192):

| p  | Zeit in s |
|----|-----------|
| 1  | 400       |
| 2  | 210       |
| 4  | 115       |
| 8  | 65        |
| 16 | 42        |

**Experiment B** (Gittergröße wächst mit p: pro Prozess konstant
2048 × 2048):

| p  | Zeit in s |
|----|-----------|
| 1  | 100       |
| 2  | 104       |
| 4  | 110       |
| 8  | 119       |
| 16 | 133       |

**Aufgaben:**

1. Welches Experiment ist strong scaling, welches weak scaling? Woran
   erkennst du das?
2. Experiment A: Berechne Speedup und Effizienz für alle p.
3. Experiment B: Welche Kennzahl verwendet man hier statt des klassischen
   Speedups? Berechne sie für alle p.
4. Wie gut skaliert das Programm jeweils? Was wäre eine plausible Ursache
   für die Verluste — und wie würdest du das überprüfen?
