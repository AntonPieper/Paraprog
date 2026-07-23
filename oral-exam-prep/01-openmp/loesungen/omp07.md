# Lösung omp07.c — Matrixmultiplikation (i-k-j)

**Status: KORREKT und effizient.** (Schwierigkeit: mittel)

## Was berechnet das Programm?

Matrixmultiplikation C = A·B (n×n, eindimensional als `i*n+j` gespeichert,
Row-Major wie in C üblich) mit **i-k-j-Schleifenreihenfolge** (Übung 6) und
Checksumme über C.

## Wie funktioniert die Parallelisierung?

- `#pragma omp parallel for schedule(static)` auf der **äußersten i-Schleife**:
  Jeder Thread berechnet einen zusammenhängenden Block von **Zeilen von C**.
- **Korrekt, weil die Threads disjunkte Zeilen von C besitzen** — kein
  gemeinsames Schreiben, kein Data Race. `aik`, `j`, `k` sind im Block
  deklariert bzw. Schleifenvariablen → private.
- `schedule(static)` passt: Jede Zeile kostet gleich viel (regelmäßige Last),
  Blöcke maximieren Lokalität.

## Warum i-k-j statt i-j-k? (Lokalität!)

Im innersten Loop (über `j`) sind die Zugriffe:

- `c[i*n + j]` — **stride 1** (zeilenweise, räumliche Lokalität ✓)
- `b[k*n + j]` — **stride 1** ✓
- `a[i*n + k]` — im j-Loop konstant, in Skalar `aik` gezogen ✓

Bei klassischem i-j-k läuft `b[k*n + j]` dagegen **spaltenweise** (stride n)
durch den Speicher → jede Iteration eine neue Cache-Line → viele Cache
Misses. i-k-j nutzt jede geladene 64-Byte-Cache-Line voll aus
(**räumliche Lokalität**).

## Wichtige Nebendetails (Prüfer fragt gern danach)

- **Checksumme**: verhindert, dass der Compiler die gesamte Berechnung als
  toten Code eliminiert (**dead-code elimination**) — ohne Verwendung des
  Ergebnisses wäre die Zeitmessung wertlos.
- `omp_get_wtime()` misst **Wall-Clock-Zeit** — die relevante Metrik für
  parallele Programme (nicht CPU-Zeit).
- Nur der **Kernel** wird gemessen, nicht Initialisierung/Ausgabe.

## Mögliche Prüferfragen

- *Dürfte man auch die k-Schleife parallelisieren?* Nein — dann würden mehrere
  Threads gleichzeitig auf dieselben `c[i*n+j]` addieren → Data Race.
  (j-Schleife ginge, wäre aber feingranularer mit mehr Overhead.)
- *Was bringt `collapse(2)` auf i- und k-Schleife?* Nichts Gutes — k darf
  nicht unabhängig verteilt werden (Race auf C). `collapse(2)` über i und j
  bei i-j-k wäre denkbar (mehr Parallelität bei kleinem n).
- *Warum skaliert die schnelle i-k-j-Version evtl. schlechter als i-j-k?*
  Sie ist bereits memory-effizient und stößt früher an die gemeinsame
  **Speicherbandbreite** (Übung 6, Frage zu Task 2).
- *Wie weiter optimieren?* Tiling/Blocking (B×B-Blöcke) für temporale
  Lokalität in den Caches.
