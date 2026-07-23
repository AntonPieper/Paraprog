# Lösung job02.sh — MPI-Job mit Rank-Mismatch

**Status: FEHLERHAFT/UNSTIMMIG — Allokation passt nicht zum Launcher-Aufruf.**
(Schwierigkeit: mittel)

## Was fordert der Job an?

- `--nodes=2`, `--ntasks=16`: SLURM reserviert **16 Task-Slots verteilt
  über 2 Knoten** (z. B. 8+8) für ein MPI-Programm.

## Der Mismatch

```bash
mpirun -np 4 ./ring
```

Gestartet werden aber nur **4 Ranks** — 12 der 16 reservierten Slots (und
damit bezahlte/blockierte Kerne) bleiben **ungenutzt**. Der Job „läuft"
zwar (kein Absturz — das macht den Fehler tückisch), aber:

- **Ressourcenverschwendung**: 75 % der Allokation idle; andere Nutzer
  warten derweil in der Queue.
- Messungen sind irreführend: Man glaubt mit 16 Prozessen zu messen,
  misst aber 4.

Der umgekehrte Fall (`-np 32` bei 16 Slots) wäre **Oversubscription**:
mehr Ranks als Kerne → Zeitscheiben-Wechsel, unbrauchbare Messwerte,
bzw. Abbruch je nach Konfiguration.

## Fix

Prozesszahl an die Allokation koppeln — nie hart codieren:

```bash
srun ./ring                      # srun übernimmt --ntasks automatisch
# oder
mpirun -np $SLURM_NTASKS ./ring
```

`srun` ist auf dem Cluster der bevorzugte Launcher: Er liest die
Allokation (ntasks, Knotenliste) direkt aus der Job-Umgebung.

## Mögliche Prüferfragen

- *Woher weiß srun die Prozessanzahl?* Aus den SLURM-Umgebungsvariablen
  der Allokation (`SLURM_NTASKS` etc.) — deshalb keine Diskrepanz möglich.
- *Wie viele Knoten braucht man für 128 Ranks?* Fulda: 64 Kerne/Knoten →
  `--nodes=2` bzw. `--ntasks=128` (SLURM wählt Knoten); Maximum des
  Clusters: 13 × 64 = **832** Prozesse.
- *Ist `--nodes=2` mit nur 4 Ranks sinnvoll?* Nein — 4 Ranks passen auf
  einen Knoten; 2 Knoten erzwingen unnötige Inter-Knoten-Kommunikation
  (Netzwerk statt Shared Memory) und verschwenden einen Knoten.
- *Unterschied `--ntasks` vs. `--cpus-per-task`?* ntasks = Anzahl
  Prozesse (MPI-Ranks); cpus-per-task = Kerne **pro** Prozess (für
  OpenMP-Threads, hybrid).
