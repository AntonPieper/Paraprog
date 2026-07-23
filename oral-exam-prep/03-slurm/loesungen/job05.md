# Lösung job05.sh — MPI-Job über 2 Knoten

**Status: KORREKT.** (Schwierigkeit: mittel)

## Was fordert der Job an, wie startet er das Programm?

- `--nodes=2` + `--ntasks-per-node=64`: **128 MPI-Ranks**, exakt 64 pro
  Knoten — der Vollausbau zweier Fulda-Knoten (64 physische Kerne pro
  Knoten, kein Hyperthreading). Ein Rank pro Kern: die Standardbelegung
  für reine MPI-Programme.
- `srun ./halo_exchange`: startet die 128 Prozesse als MPI-Ranks über
  beide Knoten; srun übernimmt Prozessanzahl und Platzierung direkt aus
  der Allokation — kein `-np` nötig, keine Diskrepanzgefahr.

Warum kein `OMP_NUM_THREADS`? Reines MPI-Programm — es gibt keine
Threads; jeder Rank ist einkernig. (Kein Fehler, dass es fehlt!)

## Diskussionspunkte, die Eindruck machen

- **Intra- vs. Inter-Knoten-Kommunikation**: Ranks auf demselben Knoten
  kommunizieren über Shared Memory (schnell), Ranks auf verschiedenen
  Knoten über das Netzwerk (langsamer, höhere Latenz). Beim
  Halo-Austausch mit Ring-Topologie überqueren nur wenige
  Nachbar-Paare die Knotengrenze — gutes Kommunikations-Layout.
- `--ntasks-per-node=64` (statt nur `--ntasks=128`) fixiert die
  **gleichmäßige Platzierung** — wichtig für reproduzierbare Messungen.
- Alternative äquivalente Angabe: `--nodes=2 --ntasks=128`.

## Mögliche Prüferfragen

- *Maximale Prozesszahl auf dem Fulda-Cluster?* 13 Knoten × 64 Kerne =
  **832**.
- *Was ändert sich mit `mpirun` statt `srun`?* Funktional hier nichts —
  `mpirun -np $SLURM_NTASKS` wäre äquivalent; srun ist die
  SLURM-integrierte Variante.
- *Warum kann so ein Job trotz freier Kerne warten?* Der Scheduler muss
  **2 komplette Knoten** frei haben; außerdem Fairness/Prioritäten in
  der Queue.
- *Wie würde man Strong Scaling messen?* Gleiche Problemgröße, Ranks
  sweepen (z. B. 1…832) — mehrere Repetitionen, Wall-Clock-Zeit des
  Kernels in eine CSV, Speedup S(p) = T(1)/T(p) plotten.
- *Woran erkennt man, dass es KEIN OpenMP-Job ist?* Mehrere Tasks, kein
  OMP_NUM_THREADS, Start über srun — und OpenMP dürfte nie über
  `--nodes=2` gehen (kein gemeinsamer Speicher über Knoten).
