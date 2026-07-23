# Lösung perf06 — Rechenzeit vs. Kommunikationszeit

## 1. Speedup und Effizienz

S(p) = 800/T_total(p), E(p) = S(p)/p:

| p  | T_total | S(p)  | E(p) |
|----|---------|-------|------|
| 1  | 800     | 1.00  | 1.00 |
| 2  | 412     | 1.94  | 0.97 |
| 4  | 218     | 3.67  | 0.92 |
| 8  | 127     | 6.30  | 0.79 |
| 16 | 90      | 8.89  | 0.56 |
| 32 | 85      | 9.41  | 0.29 |
| 64 | 102.5   | 7.80  | 0.12 |

**Minimum bei p = 32** (85 s) — der „Sweet Spot" dieser Problemgröße.

## 2. Entwicklung der Anteile

- **T_comp halbiert sich mit jeder Verdopplung** (perfekte Aufteilung
  der Rechenarbeit — ideal parallelisierbar).
- **T_comm wächst monoton** (12 → 90 s, grob Faktor 1.5 pro
  Verdopplung): mehr Prozesse = mehr Nachrichten/Partner, kleinere
  Nachrichten (Latenz dominiert), ab Knotengrenze (> 64 Kerne? hier:
  mehr Knoten) langsameres Netzwerk statt Shared Memory.
- Ab p = 64 frisst der Kommunikationszuwachs (+30 s von p=32) die
  Rechenersparnis (−12.5 s) mehr als auf → **T_total steigt wieder**.
  Die Kommunikation wirkt wie ein wachsender nicht-parallelisierbarer
  Anteil — deshalb bricht die Effizienz ein (Amdahl-artiges Verhalten
  durch Overhead statt durch sequenziellen Code).

## 3. Wann dominiert Kommunikation?

Bei p = 64: T_comm (90) > T_comp (12.5). Schon bei p = 32 ist das
Verhältnis 60:25 — Kommunikation ist dort bereits der größere Posten
(> 70 % der Laufzeit).

## 4. Maßnahmen (mindestens drei)

1. **Überlappung von Kommunikation und Berechnung**: nichtblockierende
   Operationen (MPI_Isend/Irecv, nichtblockierende Kollektive) — Teile
   von T_comm hinter T_comp verstecken.
2. **Weniger/größere Nachrichten**: Daten aggregieren statt viele kleine
   Sends (Latenzanteil senken); Kollektivoperationen statt
   P2P-Schleifen (baumbasiert O(log p)).
3. **Weak Scaling / größere Probleminstanz**: mehr Rechenarbeit pro
   Prozess verschiebt das Verhältnis T_comp/T_comm — für große Probleme
   lohnt auch p = 64 wieder (Gustafson).
4. **Hybrid MPI+OpenMP**: weniger Ranks (einer pro Knoten/Socket),
   Threads intern — reduziert Nachrichtenzahl und Puffer.
5. **Kommunikationsmuster verbessern**: Nachbarschaftskommunikation
   statt zentralem Master (kein Stern über Rank 0), gute Prozess-
   Platzierung (Nachbarn auf denselben Knoten).

## 5. Werkzeuge

- **Score-P** (Instrumentierung) + **Cube** (Profil: Zeit pro
  MPI-Funktion) — wo geht die Kommunikationszeit hin?
- **Scalasca** (automatische Muster-Erkennung: Late Sender /
  Late Receiver, Wartezeiten quantifiziert) bzw. **Vampir** (Timeline).
- **EduMPI** (Übung 11): 2D/3D-Ansicht, Kommunikationsmatrix,
  Send-/Recv-Raten, Late-Sender-/Late-Receiver-Ansicht — z. B. erkennen,
  ob die Kommunikation zentralisiert (alles über Rank 0) oder verteilt
  ist und welche MPI-Funktion am meisten Zeit/Datenvolumen kostet.
