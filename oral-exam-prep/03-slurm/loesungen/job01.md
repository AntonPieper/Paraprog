# Lösung job01.sh — OpenMP-Job

**Status: KORREKT.** (Schwierigkeit: leicht)

## Was fordert der Job an, wie startet er das Programm?

Zeile für Zeile erklären können:

| Direktive | Bedeutung |
|-----------|-----------|
| `--job-name=matmul` | Anzeigename in der Queue (`squeue`) |
| `--nodes=1` | genau 1 Knoten — richtig: OpenMP läuft nur im Shared Memory **eines** Knotens |
| `--ntasks=1` | 1 Task = 1 Prozess — richtig: ein OpenMP-Programm ist **ein** Prozess mit vielen Threads |
| `--cpus-per-task=16` | 16 CPU-Kerne für diesen einen Task reserviert |
| `--time=00:15:00` | Zeitlimit (Job wird danach abgebrochen) |
| `--output=matmul_%j.out` | stdout/stderr-Datei, `%j` = Job-ID |

- `export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK`: koppelt die Threadanzahl
  an die Reservierung — **die** idiomatische Zeile. Ändert man
  `--cpus-per-task`, passt die Threadanzahl automatisch — keine
  Über-/Unterbelegung.
- `./matmul 4000`: direkter Start reicht — es ist **kein** MPI-Programm,
  also kein `srun`/`mpirun` nötig (auf dem reservierten Rechenknoten
  ausgeführt, nicht auf dem Login-Knoten).

Eingereicht wird mit `sbatch job01.sh`; das Batch-System plant den Job auf
einen freien Knoten ein (Fulda-Cluster: 13 Knoten × 64 Kerne).

## Merksatz OpenMP-Jobs

**1 Knoten, 1 Task, Kerne über `--cpus-per-task`, Threads =
`$SLURM_CPUS_PER_TASK`.** (Vorlesungsregel: OpenMP-Programme immer genau
ein Knoten; jede Ausführung über SLURM, nie auf dem Login-Knoten.)

## Mögliche Prüferfragen

- *Warum nicht `--ntasks=16`?* ntasks steuert **Prozesse** (MPI-Ranks) —
  16 Tasks hieße 16 separate Programminstanzen (vgl. job03.sh).
- *Was passiert ohne das export?* OMP_NUM_THREADS unset → Runtime-Default,
  meist alle sichtbaren Kerne; je nach Konfiguration mehr Threads als
  reservierte Kerne → Oversubscription (vgl. job04.sh).
- *Könnte man 128 Threads anfordern?* `--cpus-per-task=128` scheitert:
  ein Knoten hat nur 64 Kerne — OpenMP skaliert nicht über Knoten hinweg.
- *Wozu das Zeitlimit?* Scheduling-Planbarkeit; zu klein → Job wird vor
  Ende abgebrochen; hängende Jobs (Deadlocks!) laufen bis ins Limit.
