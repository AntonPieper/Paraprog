# Lösung omp13.c — Histogramm mit critical

**Status: KORREKT, aber PERFORMANCE-PROBLEM: globale Serialisierung.**
(Schwierigkeit: schwer)

## Was berechnet das Programm?

Ein Histogramm (Übung 6, Task 4): Jedes `i` wird über eine deterministische
Hash-Funktion einem von 256 Bins zugeordnet; `hist[b]++` wird mit
`#pragma omp critical` geschützt. Korrektheitscheck: Summe aller Bins = n. ✓

## Das Performance-Problem

`#pragma omp critical` (unbenannt) ist intern **ein einziger globaler
Lock** — alle unbenannten critical-Bereiche im Programm teilen ihn sich.
Obwohl 256 verschiedene Bins existieren und Threads meist **verschiedene**
Bins treffen, darf immer nur **ein** Thread irgendein Bin inkrementieren:

- Die Schleife ist praktisch vollständig serialisiert (die Hash-Berechnung
  ist zu billig, um das zu verstecken).
- Mit mehr Threads: mehr **Lock-Contention** → Laufzeit **steigt**.
- Erwartetes Messbild (Übung 6): critical und ein grober Lock brechen
  katastrophal ein; atomic, feingranulare Locks und private Histogramme
  skalieren.

## Fixes — als Rangliste präsentieren (so war es in der Übung)

1. **Privatisierung (am besten):** Jeder Thread füllt ein lokales Histogramm,
   am Ende einmalig zusammenführen — die „Reduction-Idee":

   ```c
   #pragma omp parallel
   {
       long local[BINS] = {0};
       #pragma omp for schedule(static)
       for (long i = 0; i < n; i++)
           local[(i * 2654435761UL + 12345UL) % BINS]++;
       #pragma omp critical
       for (int b = 0; b < BINS; b++) hist[b] += local[b];
   }
   ```

   (Ab OpenMP 4.5 auch direkt: `reduction(+:hist[:BINS])` — Array-Section.)

2. **`#pragma omp atomic`** statt critical: Hardware-Atomic pro Bin-Update,
   Konflikte nur noch bei **gleichem** Bin — deutlich besser, aber bei 64
   Threads auf 256 Bins immer noch Contention + Cache-Line-Ping-Pong.

3. **Feingranulare Locks** (ein `omp_lock_t` **pro Bin**): skaliert, aber
   mehr Code; Faustregel der Vorlesung: atomic → critical → Locks.

## Mögliche Prüferfragen

- *Warum verhalten sich unbenanntes critical und ein einzelner grober Lock
  identisch?* Unbenanntes critical **ist** genau das: ein globaler Lock.
- *Was würde ein benanntes critical bringen?* Namen müssen statisch im Code
  stehen — für 256 Bins unpraktikabel; genau dafür gibt es Lock-Variablen
  (Anzahl erst zur Laufzeit bekannt).
- *Warum erodiert der Vorteil privater Histogramme bei sehr vielen Threads?*
  Merge-Aufwand wächst mit T×BINS und Speicherverkehr steigt.
- *Was passiert bei nur 16 Bins?* Mehr Konflikte auf gleichen Bins → atomic
  und feine Locks werden schlechter; Privatisierung bleibt gut.
