# Lösung job06.sh — Hybrid-Job (MPI + OpenMP)

**Status: KORREKT (Verständnisaufgabe Hybrid).** (Schwierigkeit: schwer)

## Wie viele Prozesse, wie viele Threads, wie viele Kerne?

Die Rechnung explizit vorführen:

- `--nodes=2` × `--ntasks-per-node=2` = **4 MPI-Ranks** insgesamt.
- `--cpus-per-task=32`: jeder Rank bekommt 32 Kerne.
- `OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK`: jeder Rank startet **32
  OpenMP-Threads**.
- Gesamt: 4 × 32 = **128 Threads auf 128 Kernen** (2 volle Knoten à 64
  Kerne: pro Knoten 2 Ranks × 32 Threads). Reservierung und Nutzung
  decken sich exakt — keine Oversubscription, keine Verschwendung.

`srun` startet die 4 Ranks (2 pro Knoten); innerhalb jedes Ranks öffnet
OpenMP seine parallelen Regionen.

## Warum hybrid? (Das Konzept hinter dem Skript)

Das Muster passt zur **Hybrid-Architektur** des Clusters (Vorlesung 1:
Cluster aus SMP-Knoten):

- **Zwischen** Knoten gibt es keinen gemeinsamen Speicher → MPI
  (Message Passing) notwendig.
- **Innerhalb** eines Knotens ist Shared Memory schneller und
  speichersparender als viele MPI-Ranks: weniger Ranks → weniger
  Halo-/Puffer-Kopien, weniger Nachrichten, implizite Kommunikation über
  gemeinsame Daten.
- Typisch: Grobzerlegung der Domäne über MPI (z. B. Gitterblöcke des
  Wärmeleitungsproblems), Feinparallelisierung der Schleifen pro Block
  mit OpenMP.

Trade-off nennen können: Hybrid ist komplexer (zwei Modelle, zwei
Fehlerklassen); Alternative wäre reines MPI mit 128 Ranks — auf 2 Knoten
oft ähnlich schnell, aber mehr Kommunikations-/Speicher-Overhead.

## Mögliche Prüferfragen

- *Was passiert, wenn man OMP_NUM_THREADS=64 setzt?* 4 × 64 = 256
  Threads auf 128 Kernen → Oversubscription; pro Knoten kämpfen 128
  Threads um 64 Kerne.
- *Warum nicht `--ntasks-per-node=1` mit 64 Threads?* Ginge auch
  (1 Rank/Knoten). Mehr Ranks pro Knoten können NUMA-freundlicher sein
  (kleinere Speicherdomänen pro Rank); 2×32 ist ein üblicher Kompromiss.
- *Wie viele Knoten für 832 Ranks à 1 Thread?* 13 — der ganze Cluster,
  reines MPI.
- *Welche OpenMP-Regel gilt trotzdem?* Threads eines Ranks bleiben
  innerhalb ihres Knotens — OpenMP niemals über Knotengrenzen.
