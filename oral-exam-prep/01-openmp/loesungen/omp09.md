# Lösung omp09.c — Präfixsumme mit Schleifenabhängigkeit

**Status: FEHLERHAFT — schleifengetragene Abhängigkeit, nicht so parallelisierbar.**
(Schwierigkeit: mittel–schwer)

## Was berechnet das Programm?

Eine **Präfixsumme** (in-place): `a[i] = a[i] + a[i-1]`. Sequenziell entsteht
aus lauter Einsen die Folge 1, 2, 3, …, n — deshalb wäre `a[n-1] = n`.

## Der Bug: Loop-Carried Dependency

Iteration `i` **liest** `a[i-1]`, das Iteration `i-1` erst **schreibt** —
eine **echte Abhängigkeit** (true dependence / Read-after-Write) **zwischen
den Iterationen**. `#pragma omp parallel for` verteilt die Iterationen aber
auf Threads, die unabhängig und in beliebiger Reihenfolge laufen:

- Thread 1 beginnt z. B. bei i = n/2 und liest `a[n/2 - 1]`, **bevor**
  Thread 0 dort die aufsummierte Präfixsumme hineingeschrieben hat.
- Ergebnis: Jeder Thread berechnet nur die Präfixsumme **seines Blocks**
  relativ zum alten Arrayinhalt → `a[n-1]` ist viel zu klein, Ergebnis
  variiert je nach Timing und Threadanzahl.

Wichtig für die Prüfung: **Nicht jede Schleife ist parallelisierbar.**
Voraussetzung für `parallel for` ist, dass die Iterationen **unabhängig**
sind. OpenMP prüft das nicht — die Direktive ist ein Versprechen des
Programmierers.

## Abhängigkeitsarten (Vorlesung 3) benennen können

- **True dependence** (Write → Read) — liegt hier vor.
- **Anti dependence** (Read → Write).
- **Output dependence** (Write → Write).

## Fix

1. **Sequenziell lassen** — bei dieser einfachen Operation oft die richtige
   Antwort (memory-bound, kaum Rechenarbeit).
2. **Paralleler Scan-Algorithmus** (zweiphasig): Phase 1: jeder Thread
   berechnet lokale Präfixsumme + Blocksumme; danach Präfixsumme über die
   Blocksummen; Phase 2: jeder Thread addiert den Offset auf seinen Block.
   Kostet zwei Durchläufe — lohnt erst bei großem n und vielen Threads.

In der Prüfung genügt: „Abhängigkeit erkennen, erklären, sagen dass ein
umstrukturierter Zweiphasen-Algorithmus nötig wäre."

## Mögliche Prüferfragen

- *Warum kompiliert das ohne Warnung und liefert manchmal fast richtige
  Werte?* OpenMP macht keine Abhängigkeitsanalyse; bei wenigen Threads oder
  glücklichem Timing fällt der Fehler klein aus — typisch nichtdeterministisch.
- *Was sagt der Parallelitätsgrad dazu?* Abhängigkeitsgraph ist hier eine
  Kette: T∞ = n → P = T_seq/T_∞ = 1 für den naiven Algorithmus — erst der
  Scan-Algorithmus schafft Parallelität (P ≈ n/log n).
- *Wäre `ordered` eine Lösung?* Es erzwingt sequenzielle Reihenfolge des
  markierten Blocks → korrekt, aber vollständig serialisiert + Overhead:
  sinnlos.
