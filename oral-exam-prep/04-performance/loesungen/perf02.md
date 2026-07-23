# Lösung perf02 — Amdahls Gesetz

## 1. Formel und Werte

**Amdahl (1967):** S_max(p) = 1 / (f + (1−f)/p) — f = sequenzieller
Anteil, (1−f) = parallelisierbarer Anteil, Problemgröße konstant.

- S(4) = 1 / (0.1 + 0.9/4) = 1 / 0.325 ≈ **3.08**
- S(16) = 1 / (0.1 + 0.9/16) = 1 / 0.15625 = **6.4**

## 2. Grenzwert p → ∞

S_max ≤ **1/f = 10**. Der parallele Anteil schrumpft gegen 0, die
sequenziellen 10 % bleiben — mehr als Faktor 10 ist unmöglich, egal wie
viele Prozessoren.

## 3. f aus der Messung schätzen

Amdahl nach f umstellen:

f = (1/S − 1/p) / (1 − 1/p) = (1/6.4 − 1/16) / (1 − 1/16)
  = (0.15625 − 0.0625) / 0.9375 = 0.09375 / 0.9375 = **0.10** ✓

Die Messung ist konsistent mit einem sequenziellen Anteil von 10 %.
(So wurde in Übung 3 aus T_par(16) der Anteil f geschätzt.)

## 4. Stellungnahme „64 Kerne = 4× schneller"

S(64) = 1 / (0.1 + 0.9/64) = 1 / 0.11406 ≈ **8.8**.

Von S(16) = 6.4 auf S(64) = 8.8 ist nur **Faktor 1.37** — nicht 4.
Die 4-fachen Ressourcen bringen 37 % Beschleunigung, die Effizienz fällt
von 6.4/16 = 0.40 auf 8.8/64 = **0.14**. Klare Empfehlung dagegen —
das Geld wäre besser investiert, den sequenziellen Anteil zu verkleinern
(Algorithmus, I/O, Initialisierung parallelisieren), denn f dominiert.

## 5. Warum oft zu pessimistisch?

**Gustafsons Gesetz (1988):** In der Praxis nutzt man mehr Prozessoren,
um **größere Probleminstanzen** in gleicher Zeit zu lösen (weak scaling).
T_seq wächst mit der Problemgröße, während der sequenzielle Anteil
konstant bleibt oder langsamer wächst — f **schrumpft relativ**, und ein
hinreichend großes Problem lässt sich effizient parallelisieren
(skalierter Speedup: S_scaled(p) = p − f·(p−1)). Amdahl gilt für **feste**
Problemgröße (strong scaling) — das ist die Annahme, die man nennen muss.
