# Lösung omp03.c — count3s mit Data Race

**Status: FEHLERHAFT — Data Race auf `count`.** (Schwierigkeit: leicht)

## Was berechnet das Programm?

Das klassische **count3s**-Beispiel aus der Vorlesung: Es zählt, wie oft die
Zahl 3 in einem Array mit Zufallszahlen (0–9, Seed 42) vorkommt.

## Wie ist es parallelisiert?

`#pragma omp parallel for` verteilt die Iterationen auf das Thread-Team.
`count` ist **außerhalb** der Region deklariert und damit per Default
**shared**; `i` ist als Schleifenvariable automatisch private.

## Der Bug: Data Race

`count++` ist **nicht atomar** — es besteht aus Laden, Inkrementieren,
Zurückschreiben (read-modify-write). Führen zwei Threads das gleichzeitig aus,
geht ein Inkrement verloren (**lost update**):

```
Thread 0: lädt count = 100        Thread 1: lädt count = 100
Thread 0: schreibt 101            Thread 1: schreibt 101   ← ein ++ verloren
```

Fachbegriffe: **Data Race** = unsynchronisierter gleichzeitiger Zugriff
mehrerer Threads auf dieselbe Speicherstelle, mindestens ein Schreibzugriff.
**Race Condition** = das Ergebnis hängt von der Ausführungsreihenfolge ab.
Laut OpenMP-Standard ist das Programmergebnis dann **unspezifiziert**.
Das Ergebnis ist typischerweise **zu klein** und variiert von Lauf zu Lauf.

## Fix (in der Reihenfolge der Vorlesung diskutieren)

**Beste Lösung — Reduction:**

```c
#pragma omp parallel for reduction(+:count)
for (int i = 0; i < n; i++)
    if (a[i] == 3)
        count++;
```

Alternativen und warum sie schlechter sind (count3s-Varianten der Vorlesung):

1. `#pragma omp atomic` vor `count++`: korrekt, aber **Bottleneck** — alle
   Threads serialisieren am gemeinsamen Zähler; langsamer als sequenziell.
2. Private Zählerarray `private_count[tid]`: korrekt, aber **False Sharing**
   (Elemente teilen sich eine Cache-Line, 64 Byte).
3. Gepaddete Structs (60 Byte Padding auf Cache-Line-Größe): korrekt und
   schnell, aber umständlich — genau das erledigt `reduction` automatisch.

## Mögliche Prüferfragen

- *Woran erkennt man das Race im Experiment?* Unterschiedliche, zu kleine
  Ergebnisse bei wiederholten Läufen mit mehreren Threads.
- *Warum meldet der Compiler nichts?* OpenMP macht **keine** Korrektheitsprüfung
  der Direktiven zur Laufzeit oder Compile-Zeit.
- *Unterschied atomic vs. critical?* atomic: einzelne einfache Speicheroperation,
  meist Hardware-Instruktion, schneller. critical: beliebiger Codeblock,
  ein globaler Lock (alle unbenannten criticals teilen sich einen).
