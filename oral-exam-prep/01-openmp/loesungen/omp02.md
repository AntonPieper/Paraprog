# Lösung omp02.c — Parallel-Region ohne Klammern

**Status: KORREKT (Verständnisaufgabe — keine Fehler, aber typische Stolperfalle).**
(Schwierigkeit: leicht)

## Was berechnet das Programm?

Es gibt zwei Textzeilen mit Thread-ID und Threadanzahl aus. Kernpunkt:
**Eine Direktive ohne `{ }` gilt nur für die unmittelbar folgende Anweisung.**

- Zeile A (`printf("A: ...")`): steht direkt nach `#pragma omp parallel` und
  wird deshalb **von jedem Thread des Teams einmal** ausgeführt — bei z. B.
  4 Threads also 4-mal, in **nichtdeterministischer Reihenfolge**, mit
  IDs 0–3 und Threadanzahl 4.
- Zeile B: liegt **außerhalb** der parallelen Region und wird nur **einmal**
  vom Master-Thread (Thread 0) ausgeführt. `omp_get_num_threads()` liefert
  außerhalb einer parallelen Region **1**, `omp_get_thread_num()` liefert 0.

Erwartete Ausgabe mit 4 Threads (Reihenfolge der A-Zeilen variiert):

```
A: thread 2 of 4
A: thread 0 of 4
A: thread 3 of 4
A: thread 1 of 4
B: thread 0 of 1
```

## Wichtige Konzepte zum Erklären

- **Fork-Join-Modell**: Am `parallel`-Konstrukt wird ein Team erzeugt (Fork),
  am Ende der Region gibt es eine **implizite Barriere**, danach läuft nur der
  Master weiter (Join).
- Wie viele Threads? Priorität: `num_threads`-Klausel > `omp_set_num_threads()`
  > Umgebungsvariable `OMP_NUM_THREADS` > Default (meist Anzahl der Kerne).
- Direktiven-Syntax: `#pragma omp` + Direktive + optionale Klauseln + Newline;
  gilt für die folgende Anweisung **oder** den folgenden `{ }`-Block.

## Mögliche Prüferfragen

- *Wie bekommt man beide printf parallel?* Beide in einen Block fassen:
  `#pragma omp parallel { printf(A); printf(B); }` — öffnende Klammer muss
  auf einer **neuen Zeile** stehen, nicht auf der Pragma-Zeile.
- *Warum ist die Ausgabereihenfolge nichtdeterministisch?* Threads laufen
  gleichzeitig; das OS/Runtime-Scheduling bestimmt die Reihenfolge (kein
  Data Race — printf auf stdout ist zeilenweise ok, aber Reihenfolge undefiniert).
- *Was gibt `omp_get_num_threads()` außerhalb einer parallelen Region zurück?* 1.
- *Was passiert, wenn ohne `-fopenmp` kompiliert wird?* Pragmas werden
  ignoriert; das Programm läuft sequenziell (A und B je einmal).
