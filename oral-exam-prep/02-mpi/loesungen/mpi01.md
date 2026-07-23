# Lösung mpi01.c — MPI Hello World

**Status: KORREKT (Verständnisaufgabe).** (Schwierigkeit: leicht)

## Was berechnet das Programm?

Jeder MPI-Prozess gibt seinen **Rank** und die Gesamtzahl der Prozesse aus.
Das ist das Einstiegsbeispiel aus Vorlesung 8 — hier muss das
**Ausführungsmodell** sauber erklärt werden.

## Die Konzepte, die der Prüfer hören will

- **SPMD** (Single Program, Multiple Data): **Ein** Programm wird als N
  unabhängige **Prozesse** gestartet (`mpirun -np 4 ./a.out` bzw. `srun`).
  Jeder Prozess hat **eigenen privaten Speicher** — kein Shared Memory,
  Kommunikation nur per Message Passing.
- `MPI_Init(&argc, &argv)`: initialisiert MPI; davor darf keine andere
  MPI-Funktion aufgerufen werden.
- `MPI_Comm_rank(MPI_COMM_WORLD, &rank)`: eigene Prozess-ID, 0 … N−1.
- `MPI_Comm_size(MPI_COMM_WORLD, &size)`: Anzahl Prozesse im Kommunikator.
- **Kommunikator** `MPI_COMM_WORLD`: verwaltete Prozessgruppe, umfasst alle
  gestarteten Prozesse.
- `MPI_Finalize()`: meldet den Prozess ab; danach keine MPI-Aufrufe mehr;
  beendet **nicht** das Programm.

## Was gibt das Programm aus?

Mit `mpirun -np 4`: vier Zeilen `Hello from process X of 4` (X = 0…3) in
**nichtdeterministischer Reihenfolge** — die Prozesse laufen unabhängig,
niemand koordiniert die Ausgabe.

## Übersetzen und Starten (Prüfungsklassiker)

```
mpicc -O2 -Wall -o hello mpi01.c
mpirun -np 4 ./hello        # lokal / Lab
srun ./hello                # auf dem Cluster via SLURM
```

`mpicc` ist ein Wrapper um den C-Compiler, der MPI-Header und -Bibliothek
einbindet. Prozessanzahl steht beim **Start** fest.

## Mögliche Prüferfragen

- *Unterschied Prozess (MPI) vs. Thread (OpenMP)?* Prozess: eigener
  Adressraum, explizite Kommunikation; Thread: gemeinsamer Adressraum
  (gemeinsames Heap/globale Daten, eigener Stack), implizite Kommunikation
  über gemeinsame Variablen.
- *Warum MPI statt OpenMP?* Distributed Memory — skaliert über
  Knotengrenzen (Fulda-Cluster: 13 Knoten × 64 Kerne = max. 832 Prozesse);
  OpenMP nur innerhalb eines Knotens.
- *Was ist MPI eigentlich?* Ein **Standard** (aktuell MPI 5.0), keine
  Implementierung — Implementierungen: Open MPI, MPICH, MVAPICH.
- *Wie macht man aus SPMD unterschiedliche Rollen?* Verzweigung über den
  Rank, z. B. `if (rank == 0) { master } else { worker }`.
