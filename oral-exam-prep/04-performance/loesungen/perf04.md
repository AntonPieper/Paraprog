# Lösung perf04 — Zwei Experimente interpretieren

## 1. Zuordnung

- **Experiment A = Strong Scaling**: Problemgröße fest (8192²), p steigt
  → Zeit muss idealerweise wie 1/p fallen.
- **Experiment B = Weak Scaling**: Arbeit **pro Prozess** konstant
  (2048² je Prozess), Gesamtproblem wächst mit p → Zeit sollte
  idealerweise **konstant** bleiben.

## 2. Experiment A: Speedup und Effizienz

S(p) = T(1)/T(p), E(p) = S(p)/p:

| p  | Zeit | S(p)  | E(p) |
|----|------|-------|------|
| 1  | 400  | 1.00  | 1.00 |
| 2  | 210  | 1.90  | 0.95 |
| 4  | 115  | 3.48  | 0.87 |
| 8  | 65   | 6.15  | 0.77 |
| 16 | 42   | 9.52  | 0.60 |

(Streng genommen gehört das schnellste **sequenzielle** Programm in den
Zähler; hier dient T(1) des parallelen Programms als Näherung — in der
Prüfung ansprechen!)

## 3. Experiment B: Weak-Scaling-Effizienz

Kennzahl: **Weak-Scaling-Effizienz E_w(p) = T(1)/T(p)** — „wie nah bleibt
die Zeit an der Ein-Prozess-Zeit, obwohl das Problem p-mal größer ist".

| p  | Zeit | E_w(p) |
|----|------|--------|
| 1  | 100  | 1.00   |
| 2  | 104  | 0.96   |
| 4  | 110  | 0.91   |
| 8  | 119  | 0.84   |
| 16 | 133  | 0.75   |

## 4. Bewertung und Ursachen

- **A**: Bis p=8 ordentlich (E ≥ 0.77), bei p=16 nur noch 60 % — typisch
  strong scaling: Der Rechenanteil pro Prozess schrumpft (8192²/16), der
  Kommunikations-/Synchronisationsanteil bleibt oder wächst → sein
  relativer Anteil steigt (Amdahl-Effekt durch Overhead).
- **B**: Zeit steigt langsam (100 → 133 s). Da die Arbeit pro Prozess
  konstant ist, kommt der Anstieg vom **wachsenden
  Kommunikations-Overhead** mit mehr Prozessen (mehr Nachbarn/Nachrichten,
  ab Knotengrenze Netzwerk statt Shared Memory) — bei perfektem weak
  scaling (Gustafson-Annahme) bliebe die Kurve flach.
- **Überprüfen**: Kommunikations- und Rechenzeit getrennt messen
  (MPI_Wtime um die Kommunikationsphasen; Profiling/Tracing mit
  Score-P/Scalasca oder EduMPI — Anteil der MPI-Zeit, Late-Sender-Muster,
  Kommunikationsmatrix).
