# Aufgabe perf01 — Speedup, Effizienz, Overhead

Ein Programm wurde auf dem Fulda-Cluster gemessen. Das **schnellste
sequenzielle Programm** benötigt **T_seq = 120 s**. Die parallele Version
wurde mit p Threads gemessen:

| p (Threads) | T_par(p) in s |
|-------------|---------------|
| 1           | 126           |
| 2           | 66            |
| 4           | 36            |
| 8           | 21            |
| 16          | 14            |

**Aufgaben (laut beantworten, Formeln nennen):**

1. Berechne Speedup S(p) und Effizienz E(p) für alle p (2 Nachkommastellen).
2. Warum ist T_par(1) größer als T_seq — und welche der beiden Zeiten
   gehört in den Nenner der Speedup-Formel?
3. Berechne den totalen parallelen Overhead O(p). Wie entwickelt er sich
   mit p und was bedeutet das?
4. Bewerte: Lohnt sich der Einsatz von 16 Threads? Ab wann würdest du von
   guter bzw. schlechter Skalierbarkeit sprechen?
