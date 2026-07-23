# Lösung omp01.c — Vektorsumme mit Reduction

**Status: KORREKT und effizient.** (Schwierigkeit: leicht)

## Was berechnet das Programm?

Es summiert die Elemente eines Arrays mit `n` Elementen (Werte 0.5 bis 9.5,
zyklisch) und misst die Kernel-Zeit mit `omp_get_wtime()` (Wall-Clock-Zeit).

## Wie funktioniert die Parallelisierung?

- `#pragma omp parallel for` kombiniert das Erzeugen eines Thread-Teams
  (Fork-Join-Modell) mit der Verteilung der Schleifeniterationen (Worksharing).
- `schedule(static)`: Die Iterationen werden zur Compile-/Startzeit in etwa
  gleich große zusammenhängende Blöcke aufgeteilt — ideal hier, weil jede
  Iteration gleich viel Arbeit kostet (regelmäßige Last) und die blockweise
  Zuteilung die räumliche Lokalität (Cache-Lines) ausnutzt.
- `reduction(+:sum)`: Jeder Thread bekommt eine **private Kopie** von `sum`
  (initialisiert mit dem neutralen Element 0). Am Ende der Schleife kombiniert
  die Runtime alle Teilsummen zu einem Ergebnis. Dadurch **kein Data Race**
  und **kein Synchronisations-Bottleneck**.
- Die Schleifenvariable `i` ist automatisch `private`.

## Warum ist das korrekt und schnell?

Ohne `reduction` wäre `sum += a[i]` ein klassisches Data Race (mehrere Threads
schreiben unsynchronisiert dieselbe Variable). Die Reduction vermeidet das ohne
teure Synchronisation in jeder Iteration — nur ein Kombinieren pro Thread am
Ende. Erwartung: nahezu linearer Speedup, bis die **Speicherbandbreite**
limitiert (die Summe macht nur eine Addition pro geladenem Element — das
Programm ist memory-bound, nicht compute-bound).

## Mögliche Prüferfragen

- *Warum reduction statt `#pragma omp critical` im Schleifenkörper?*
  Critical würde jede Iteration serialisieren — langsamer als sequenziell.
- *Was muss für den Reduction-Operator gelten?* Kommutativ und assoziativ.
  Bei Gleitkommazahlen kann es durch andere Additionsreihenfolge minimale
  Rundungsunterschiede geben.
- *Was passiert ohne `schedule(static)`?* `static` ohne Blockgröße ist bei den
  meisten Implementierungen ohnehin der Default; explizit angeben ist guter Stil.
- *Warum skaliert das nicht perfekt auf 64 Threads?* Memory-bound: Die
  Speicherbandbreite des Knotens ist die gemeinsame Ressource (vgl. Übung 6
  zu matmul ikj).
