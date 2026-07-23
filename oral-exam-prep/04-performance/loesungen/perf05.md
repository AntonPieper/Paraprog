# Lösung perf05 — Merkwürdige Speedup-Kurve

## 1. Speedup

| p  | Zeit | S(p) = 240/T | E(p) |
|----|------|--------------|------|
| 1  | 240  | 1.00         | 1.00 |
| 2  | 118  | **2.03**     | 1.02 |
| 4  | 58   | **4.14**     | 1.03 |
| 8  | 31   | 7.74         | 0.97 |
| 16 | 18   | 13.33        | 0.83 |
| 32 | 14   | 17.14        | 0.54 |
| 64 | 15   | 16.00        | 0.25 |

Bei p = 2 und p = 4 ist der Speedup **superlinear** (S(p) > p,
Effizienz > 1).

## 2. Superlinearer Speedup — die zwei Vorlesungs-Erklärungen

1. **Cache-Effekte**: Mit p Threads steht das p-fache an
   Cache-Kapazität zur Verfügung (jeder Kern bringt eigene L1/L2 mit).
   Passt das Arbeitsset pro Thread plötzlich in den Cache, sinken die
   Speicherzugriffszeiten drastisch — der Gewinn kommt also nicht nur
   von der Arbeitsteilung, sondern von der besseren Speicherhierarchie.
2. **Veränderter Berechnungspfad bei Suchalgorithmen**: Parallele Suche
   durchsucht den Raum in anderer Reihenfolge; ein Thread findet das
   Ergebnis früher, die Gesamtarbeit sinkt (hier plausibel: „Suche in
   großer Datenstruktur").

Beides zusammen erklärt Effizienz > 1, ohne die Theorie zu verletzen —
der Vergleichsmaßstab (1 Thread) leidet unter Cache-Misses bzw.
ungünstigem Suchpfad.

## 3. Sättigung und Rückgang (p = 32, 64)

- Der pro Thread verbleibende Arbeitsanteil wird klein; **Overhead**
  (Thread-Verwaltung, Synchronisation, implizite Barrieren) wird relativ
  groß — klassischer Amdahl-/Overhead-Effekt.
- Gemeinsame Ressourcen limitieren: **Speicherbandbreite** des Knotens,
  ggf. NUMA-Effekte (Daten liegen bei einem Speichercontroller,
  First-Touch durch den Master-Thread).
- Bei 64 Threads schließlich **negativer Grenznutzen**: mehr
  Synchronisations-/Verwaltungskosten als zusätzliche Rechenleistung —
  die Kurve kippt (14 s → 15 s).

## 4. Was würde man zusätzlich messen?

- **Arbeitssetgröße vs. Cache-Größen** (L1/L2/L3 des Knotens) — passt
  das Set ab p = 2/4 in den aggregierten Cache?
- Cache-Miss-Raten (Hardware-Counter) pro Threadanzahl.
- Bei Suche: **gefundene Position / abgearbeitete Elemente** pro Lauf —
  sinkt die Gesamtarbeit bei paralleler Suche?
- Per-Thread-Zeiten (Lastbalance) und Zeit in Barrieren/kritischen
  Abschnitten (Profiler) für die Overhead-These.
- Mehrere Wiederholungen (≥ 3) und Streuung — bei 14 vs. 15 s kann auch
  Messrauschen mitspielen; Wall-Clock-Zeiten unter ~30 s sind auf dem
  Cluster mit Vorsicht zu interpretieren (Übungs-Faustregel).
