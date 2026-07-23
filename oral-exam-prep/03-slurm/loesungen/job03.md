# Lösung job03.sh — OpenMP-Programm mit srun als „MPI-Job" gestartet

**Status: FEHLERHAFT — falscher Launcher für ein OpenMP-Programm.**
(Schwierigkeit: mittel)

## Was passiert hier wirklich?

`matmul` ist ein **OpenMP**-Programm (Shared Memory, ein Prozess).
Das Skript reserviert aber `--ntasks=8` und startet mit `srun ./matmul`:

- `srun` startet das Programm **einmal pro Task** → **8 unabhängige
  Kopien** von matmul, jede ohne Wissen von den anderen (es sind keine
  MPI-Ranks — das Programm ruft ja kein MPI_Init auf, die Kopien
  kooperieren nicht).
- Jede Kopie liest `OMP_NUM_THREADS=8` → **8 Prozesse × 8 Threads = 64
  Threads**, denen je nach Slot-Zuteilung nur 8 Kerne (1 pro Task)
  zustehen → massive **Oversubscription**.
- Ausgabe: 8-fach dupliziert (8 Checksummen, 8 Zeiten) — die Rechnung
  wird 8-mal komplett ausgeführt, **nicht** 8-fach beschleunigt.

Erkennungsmerkmal in der Prüfung: **Passt der Programmtyp zum
Startmechanismus?** OpenMP → direkt starten, 1 Task; MPI → srun/mpirun,
n Tasks.

## Fix

```bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./matmul 4000
```

## Mögliche Prüferfragen

- *Woran erkennt man im Output, dass etwas schiefging?* Mehrfache
  identische Ausgabezeilen; Laufzeit pro Kopie eher schlechter als der
  1-Kopie-Fall.
- *Und wenn man ein MPI-Programm OHNE srun/mpirun startet?* Der
  Spiegelfehler: `./ring` direkt → MPI_Init ohne Launcher-Umgebung →
  läuft als **1 einzelner Rank** (size = 1) — Programm rechnet allein
  oder bricht ab (z. B. „requires exactly 2 processes").
- *Warum ist Oversubscription schlecht für Messungen?* Threads/Prozesse
  konkurrieren um Kerne, ständige Kontextwechsel → Laufzeiten instabil
  und nicht interpretierbar (weder Speedup noch Effizienz aussagekräftig).
- *Wann wären mehrere Tasks + OMP_NUM_THREADS zusammen richtig?*
  **Hybrid** MPI+OpenMP-Programme (vgl. job06.sh) — aber dann mit
  `--cpus-per-task` für die Threads pro Rank.
