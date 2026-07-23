# Lösung perf01 — Speedup, Effizienz, Overhead

## 1. Speedup und Effizienz

Formeln (Vorlesung 3): **S(p) = T_seq / T_par(p)**,
**E(p) = S(p) / p = T_seq / (p · T_par(p))**, mit 0 < E(p) ≤ 1.

| p  | T_par | S(p) = 120/T_par | E(p) = S(p)/p |
|----|-------|------------------|----------------|
| 1  | 126   | 0.95             | 0.95           |
| 2  | 66    | 1.82             | 0.91           |
| 4  | 36    | 3.33             | 0.83           |
| 8  | 21    | 5.71             | 0.71           |
| 16 | 14    | 8.57             | 0.54           |

## 2. Warum T_par(1) > T_seq?

Das parallele Programm enthält auch bei einem Thread **Overhead**:
Thread-Management, Synchronisation, ggf. andere Code-Struktur.
Die Vorlesungsdefinition ist eindeutig: T_seq ist die Zeit des
**schnellsten sequenziellen Programms** — nicht die des parallelen
Programms mit p=1. Deshalb steht 120 (nicht 126) im Zähler, und
S(1) < 1 ist völlig normal.

## 3. Overhead

**O(p) = p · T_par(p) − T_seq** (gesamte von allen Prozessoren
aufgewendete Mehrarbeit gegenüber sequenziell):

| p  | p·T_par | O(p) |
|----|---------|------|
| 1  | 126     | 6    |
| 2  | 132     | 12   |
| 4  | 144     | 24   |
| 8  | 168     | 48   |
| 16 | 224     | 104  |

Der Overhead **verdoppelt sich etwa mit jeder Verdopplung von p** — er
wächst ungefähr linear in p (jeder zusätzliche Thread bringt ~6 s an
Synchronisations-/Verwaltungs-/Wartekosten mit). Genau deshalb sinkt die
Effizienz kontinuierlich: Die zusätzlich eingesetzten Prozessoren
verbringen einen wachsenden Anteil ihrer Zeit nicht mit Nutzarbeit
(Kommunikation, Warten/Idle, Verwaltung).

## 4. Bewertung

- Bis p = 8 ist die Effizienz ≥ 0.71 — vertretbar.
- Bei p = 16 arbeitet das System nur noch zu 54 % effizient: Fast die
  Hälfte der Rechenkapazität wird durch Overhead aufgezehrt. Der Schritt
  8 → 16 bringt nur noch Faktor 1.5 (21 s → 14 s) statt idealer 2.
- Ob sich 16 Threads „lohnen", hängt vom Ziel ab: minimale
  **Time-to-Solution** ja (14 s ist das Minimum), effiziente
  **Ressourcennutzung** eher nein. Faustformel fürs Gespräch: Effizienz
  deutlich unter ~0.5 heißt schlechte Skalierbarkeit für diese
  Problemgröße (Stichwort strong scaling — bei größerer Probleminstanz
  sähe es besser aus, Gustafson).
