# Aufgabe perf05 — Merkwürdige Speedup-Kurve

Ein OpenMP-Programm (Suche in einer großen Datenstruktur, Knoten mit
64 Kernen) liefert folgende Messwerte (Problemgröße fest):

| p (Threads) | Zeit in s | 
|-------------|-----------|
| 1           | 240       |
| 2           | 118       |
| 4           | 58        |
| 8           | 31        |
| 16          | 18        |
| 32          | 14        |
| 64          | 15        |

**Aufgaben:**

1. Berechne den Speedup für alle p. Was fällt bei p = 2 und p = 4 auf?
2. Wie ist dieses Phänomen möglich, obwohl es „eigentlich" nicht sein
   dürfte? Nenne die beiden klassischen Erklärungen aus der Vorlesung.
3. Erkläre den Verlauf bei p = 32 und p = 64. Warum wird das Programm am
   Ende wieder langsamer?
4. Welche zusätzlichen Messungen/Angaben bräuchtest du, um deine
   Erklärungen zu belegen?
