# Aufgabe perf03 — Gustafson und Weak Scaling

Eine Simulation hat pro Lauf einen festen sequenziellen Anteil von
**f = 5 %** (Initialisierung, I/O). Das Team plant, mit p Prozessoren
jeweils eine **p-mal größere Probleminstanz** in etwa gleicher Wandzeit
zu rechnen (weak scaling).

**Aufgaben:**

1. Erkläre den Unterschied zwischen strong scaling und weak scaling.
   Welches Gesetz gehört zu welchem Szenario?
2. Berechne den skalierten Speedup nach Gustafson für p = 16 und p = 64.
3. Vergleiche mit dem Speedup nach Amdahl für p = 16 (gleiche f = 5 %).
   Warum sind die Werte so unterschiedlich, obwohl f gleich ist?
4. Unter welcher Annahme gilt Gustafson — und wann wäre sie verletzt?
