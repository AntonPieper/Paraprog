# Lösung job04.sh — Oversubscription: 64 Threads auf 1 Kern

**Status: FEHLERHAFT — Threadanzahl passt nicht zur Reservierung.**
(Schwierigkeit: mittel)

## Das Problem

Der Job reserviert `--cpus-per-task=1` — **einen einzigen Kern** — setzt
aber `OMP_NUM_THREADS=64`. Das OpenMP-Programm startet 64 Threads, die
sich alle einen Kern teilen (SLURM bindet den Task an die reservierten
Ressourcen; die Threads laufen per Zeitscheiben abwechselnd):

- **Keine echte Parallelität** — es ist Konkurrenz statt Parallelität
  (concurrent, nicht parallel).
- Kontextwechsel-Overhead + Synchronisationskosten von 64 Threads →
  **langsamer als sequenziell**.
- Gemessene „Speedups" sind Unsinn; das Ergebnis bleibt korrekt — es ist
  ein reines Ressourcen-/Performance-Problem.

Merksatz: `OMP_NUM_THREADS` sollte **nie** größer sein als
`--cpus-per-task`. Idiomatisch koppelt man beides:
`export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK`.

## Fix

```bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./pi_par_loop 1000000000
```

(64 = Vollausbau eines Fulda-Knotens; für Skalierungsmessungen P ∈
{1, 2, 4, 8, 16, 32, 64} sweept man cpus-per-task bzw. übergibt die
Threadanzahl als Programmargument — Übungs-Workflow `run_all.sh` mit
Schleifen und einer CSV-Zeile pro Lauf.)

## Mögliche Prüferfragen

- *Läuft der Job überhaupt los?* Ja — SLURM prüft nur die Reservierung,
  nicht wie viele Threads das Programm startet. Genau deshalb fällt der
  Fehler nur über die Laufzeit auf.
- *Priorität der Threadanzahl-Quellen in OpenMP?* `num_threads`-Klausel >
  `omp_set_num_threads()` > `OMP_NUM_THREADS` > Default. Das Skript wirkt
  also nur, wenn das Programm nichts anderes erzwingt.
- *Umgekehrter Fall: cpus-per-task=64, OMP_NUM_THREADS=8?* Kein Fehler,
  aber Verschwendung: 56 reservierte Kerne bleiben ungenutzt.
- *Was wäre bei einem MPI-Programm das Analogon?* `mpirun -np 64` bei
  16 reservierten Slots — Oversubscription der Ranks (vgl. job02.sh).
