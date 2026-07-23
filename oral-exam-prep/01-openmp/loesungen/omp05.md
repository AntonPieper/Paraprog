# Lösung omp05.c — Pi mit critical in jeder Iteration

**Status: KORREKT, aber massives PERFORMANCE-PROBLEM.** (Schwierigkeit: mittel)

## Was berechnet das Programm?

Pi durch numerische Integration (Mittelpunktsregel), wie omp04 — aber hier
ist `x` korrekt im Schleifenkörper deklariert (private) und `sum` wird durch
`#pragma omp critical` geschützt. **Das Ergebnis ist korrekt.**

## Das Performance-Problem

`#pragma omp critical` in **jeder Iteration** bedeutet: In jedem der
`num_steps` Durchläufe darf immer nur **ein** Thread die Addition ausführen —
die Schleife wird praktisch **serialisiert**:

- Der eigentliche Arbeitsanteil (eine Division) ist winzig im Vergleich zu den
  Kosten des Locks (critical = intern ein globaler Lock).
- Mit mehr Threads wird es **schlechter**, nicht besser: mehr **Lock-Contention**,
  Cache-Line von `sum` wandert ständig zwischen den Kernen
  (Kohärenzverkehr). Erwartung: deutlich langsamer als die sequenzielle
  Version, Laufzeit steigt mit der Threadanzahl (vgl. Übung 5, Variante A
  vs. B; und count3s-Variante mit atomic).

Fachbegriffe: **Synchronisations-Overhead**, **Serialisierung**,
**Bottleneck**, **Lock-Contention**.

## Fix

Teilsummen privat akkumulieren, nur **einmal pro Thread** kombinieren:

```c
#pragma omp parallel for reduction(+:sum)
for (long i = 0; i < num_steps; i++) {
    double x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
}
```

Oder manuell (Übung 5, Variante B): lokale Variable pro Thread, am Ende ein
einziges `critical` pro Thread. Reduction ist die idiomatische Lösung.

## Mögliche Prüferfragen

- *Ist das Programm korrekt?* Ja — das ist die Fangfrage. Erst Korrektheit
  („kein Data Race, critical schützt sum"), dann Performance bewerten.
- *Wie würde sich die Laufzeit mit 1, 2, 8, 64 Threads entwickeln?*
  1 Thread ≈ sequenziell + Lock-Overhead; mehr Threads → mehr Contention →
  langsamer (negativer Speedup gegenüber sequenziell).
- *Vorlesungs-Faustregel für gegenseitigen Ausschluss?*
  atomic → critical → Locks (in dieser Reihenfolge bevorzugen). Hier wäre
  atomic zwar etwas schneller als critical, das eigentliche Problem
  (Synchronisation pro Iteration) bliebe aber bestehen.
- *Wie oft synchronisiert die Reduction-Version?* Einmal pro Thread am Ende
  (statt `num_steps`-mal).
