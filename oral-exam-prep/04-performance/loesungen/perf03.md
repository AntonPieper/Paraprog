# Lösung perf03 — Gustafson und Weak Scaling

## 1. Strong vs. Weak Scaling

- **Strong Scaling**: Problemgröße **konstant**, Prozessorzahl steigt —
  Ziel: dieselbe Aufgabe schneller. Grenze: **Amdahl** (sequenzieller
  Anteil limitiert, S ≤ 1/f).
- **Weak Scaling**: Problemgröße wächst **proportional** zur
  Prozessorzahl — Ziel: größere Aufgabe in gleicher Zeit. Beschreibung:
  **Gustafson**.

## 2. Skalierter Speedup nach Gustafson

S_scaled(p) = p − f·(p−1):

- p = 16: 16 − 0.05·15 = **15.25**
- p = 64: 64 − 0.05·63 = **60.85**

Interpretation: Die große Instanz auf 64 Prozessoren erledigt ~60.85-mal
so viel Arbeit pro Zeit wie ein Prozessor an der kleinen Instanz —
nahezu linear.

## 3. Vergleich mit Amdahl (p = 16, f = 5 %)

Amdahl: S(16) = 1/(0.05 + 0.95/16) = 1/0.109375 ≈ **9.14**
(und maximal 1/f = 20 für p → ∞), Gustafson: 15.25.

Der Unterschied liegt **nicht** in f, sondern in der Fragestellung:

- Amdahl hält die Problemgröße fest — der sequenzielle Anteil bleibt
  absolut gleich groß und dominiert relativ immer mehr, während der
  parallele Teil auf immer mehr Prozessoren schrumpft.
- Gustafson lässt das Problem mitwachsen — der **parallele** Anteil
  wächst mit p, der sequenzielle bleibt (etwa) konstant → sein
  **relativer** Anteil an der Gesamtarbeit sinkt mit p.

Merkformulierung aus der Vorlesung: T_seq wächst mit der Problemgröße,
f bleibt konstant oder wächst langsamer → T_seq/f ist nicht mehr
konstant → ein hinreichend großes Problem kann effizient parallelisiert
werden.

## 4. Annahme — und ihre Verletzung

Annahme: Der sequenzielle Anteil wächst **nicht** (oder deutlich
langsamer als) proportional mit der Problemgröße. Verletzt, wenn z. B.
die sequenzielle Initialisierung oder das Einsammeln der Ergebnisse mit
der Problemgröße mitwächst (etwa Master liest p-mal so viel Daten ein) —
dann bleibt f konstant statt zu schrumpfen und der Gustafson-Vorteil
verpufft. Auch wachsender Kommunikations-Overhead pro Prozessor kann das
Bild kippen (siehe perf06).
