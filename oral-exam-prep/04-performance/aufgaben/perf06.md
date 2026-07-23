# Aufgabe perf06 — Rechenzeit vs. Kommunikationszeit (MPI)

Für ein MPI-Programm auf dem Fulda-Cluster wurden Rechen- und
Kommunikationsanteil getrennt gemessen (Problemgröße fest):

| p  | T_comp in s | T_comm in s | T_total in s |
|----|-------------|-------------|--------------|
| 1  | 800         | 0           | 800          |
| 2  | 400         | 12          | 412          |
| 4  | 200         | 18          | 218          |
| 8  | 100         | 27          | 127          |
| 16 | 50          | 40          | 90           |
| 32 | 25          | 60          | 85           |
| 64 | 12.5        | 90          | 102.5        |

**Aufgaben:**

1. Berechne Speedup und Effizienz (bezogen auf T_total) für alle p.
   Bei welchem p ist die Laufzeit minimal?
2. Beschreibe die Entwicklung der beiden Anteile. Warum wird das
   Programm ab p = 64 wieder langsamer, obwohl die Rechenzeit perfekt
   skaliert?
3. Ab welchem p dominiert die Kommunikation die Laufzeit?
4. Welche Maßnahmen könnten die Skalierbarkeit verbessern? Nenne
   mindestens drei mit kurzer Begründung.
5. Mit welchen Werkzeugen aus Vorlesung/Übung würdest du die
   Kommunikationsanteile im Detail untersuchen?
