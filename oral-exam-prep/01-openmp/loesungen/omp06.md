# Lösung omp06.c — count3s mit privatem Zählerarray (False Sharing)

**Status: KORREKT, aber PERFORMANCE-PROBLEM: False Sharing.** (Schwierigkeit: mittel)

## Was berechnet das Programm?

count3s, **Variante 3 aus der Vorlesung**: Jeder Thread zählt in seinem
eigenen Element eines Arrays `private_count[T]`, am Ende addiert jeder Thread
seinen Wert per `#pragma omp atomic` auf `count`. Kein Data Race:

- Jeder Thread schreibt **nur sein eigenes** Element `private_count[tid]`.
- Die finale Addition ist mit atomic geschützt (und passiert nach der
  impliziten Barriere am Ende von `#pragma omp for`).

**Das Ergebnis ist korrekt.**

## Das Performance-Problem: False Sharing

`private_count` ist ein Array von 8 `int`s = 32 Byte — **alle Elemente liegen
in derselben Cache-Line (64 Byte)**. Obwohl die Threads *logisch* auf
verschiedene Daten zugreifen, teilen sie sich *physisch* eine Cache-Line:

1. Thread 0 schreibt `private_count[0]` → seine Cache-Line-Kopie wird
   modifiziert.
2. Die **Cache-Kohärenz** (z. B. Bus Snooping) **invalidiert** die Kopien
   aller anderen Kerne.
3. Thread 1 will `private_count[1]` schreiben → Cache Miss, Line muss neu
   geholt werden.

Die Cache-Line „ping-pongt" zwischen den Kernen → unnötiger Kohärenzverkehr,
kaum oder negativer Speedup, obwohl keinerlei logische Abhängigkeit besteht.
Genau das ist die Definition von **False Sharing**: mehrere Prozessoren
arbeiten auf verschiedenen Daten **innerhalb derselben Cache-Line**.

## Fix

**Variante 4 der Vorlesung — Padding auf Cache-Line-Größe:**

```c
struct padded_int { int value; char padding[60]; } private_count[T];
```

**Idiomatischer und besser — Reduction (oder lokale Variable):**

```c
int count = 0;
#pragma omp parallel for reduction(+:count)
for (int i = 0; i < n; i++)
    if (a[i] == 3)
        count++;
```

Die Runtime legt die private Kopie auf dem **Thread-Stack** ab (dort ist
False Sharing unwahrscheinlich) und kombiniert am Ende.

## Mögliche Prüferfragen

- *Warum ist das korrekt, obwohl mehrere Threads in dasselbe Array schreiben?*
  Disjunkte Indizes — Korrektheit ok, nur Performance leidet (Vorlesung L7:
  „Schreiben auf disjunkte Indizes: korrekt, aber Risiko False Sharing").
- *Wie groß ist eine Cache-Line?* Typisch 64 Byte.
- *Wie erkennt man False Sharing im Experiment?* Korrekte Ergebnisse, aber
  Laufzeit skaliert nicht (oder verschlechtert sich) mit mehr Threads;
  Padding beseitigt das Problem.
- *Warum funktioniert die Kombination per atomic am Ende ohne Barriere-Bug?*
  `#pragma omp for` hat am **Ende** eine implizite Barriere — alle Zählungen
  sind fertig, bevor kombiniert wird.
