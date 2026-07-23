# Spickzettel — Alles auf einer Seite (zum Einprägen, nicht zum Mitnehmen!)

> In der Prüfung sind keine Notizen erlaubt — dieser Zettel ist die
> Kompaktwiederholung für die letzten Tage.

## Formeln (auswendig + herleiten können)

| Größe | Formel | Merkhilfe |
|-------|--------|-----------|
| Speedup | S(p) = T_seq / T_par(p) | T_seq = **schnellstes sequenzielles** Programm |
| Effizienz | E(p) = S(p)/p | 0 < E ≤ 1; „Auslastung pro Prozessor" |
| Overhead | O(p) = p·T_par(p) − T_seq | „bezahlt vs. gebraucht" |
| Amdahl | S(p) = 1/(f + (1−f)/p), S ≤ 1/f | Problemgröße **fest** (strong scaling) |
| Gustafson | S_scaled(p) = p − f·(p−1) | Problem **wächst** mit p (weak scaling) |
| Parallelitätsgrad | P = T_seq / T_∞ | längster Pfad im DAG |
| f aus Messung | f = (1/S − 1/p)/(1 − 1/p) | Amdahl umgestellt |

## Der 3-Minuten-Blick auf ein Programm (Checkliste)

1. **Was berechnet es?** (Algorithmus erkennen: Pi, count3s, matmul,
   Histogramm, Ring, Halo, Master-Worker …)
2. **Wie parallelisiert?** (Konstrukte + Datenverteilung benennen)
3. **Korrektheit** — OpenMP: Was ist shared/private? Race auf
   Akkumulator? Abhängigkeit zwischen Iterationen? nowait/Barriere?
   Task-Datenumgebung? — MPI: Send/Recv-Reihenfolge (Deadlock?),
   Kollektiv von allen gerufen? counts/Rest? Puffer vor Wait angefasst?
4. **Performance** — critical/atomic im Hot Loop? False Sharing?
   Schedule vs. Laststruktur? Serialisierung/zentraler Master?
   Lokalität (Schleifenreihenfolge)?
5. **Fix vorschlagen** + bessere Alternative benennen (reduction,
   Sendrecv, Kollektiv, anderes Schedule …).

## OpenMP-Kernwissen

- Scoping-Default: **shared**; private: Schleifenvariablen + in Region
  deklarierte. Klauseln: private/firstprivate/lastprivate/default(none).
- Threadanzahl: num_threads > omp_set_num_threads > OMP_NUM_THREADS.
- Schedules: static (Lokalität) | static,c (zyklisch, strukturierte
  Ungleichlast) | dynamic (irregulär, Overhead) | guided | runtime
  (OMP_SCHEDULE).
- Synchronisation: **reduction** > atomic > critical > Locks.
  Unbenanntes critical = EIN globaler Lock.
- Implizite Barrieren: Ende parallel/for/sections/single (nowait);
  master: keine.
- Tasks: parallel+single erzeugen, firstprivate-Default für private
  Außenvariablen, shared(x) fürs Ergebnis, taskwait, Cutoff.
- NUMA: First-Touch — Initialisierung durch den nutzenden Thread.

## MPI-Kernwissen

- Gerüst: Init → Comm_rank/Comm_size → … → Finalize; mpicc; mpirun/srun.
- Send-Semantik: Rückkehr = Puffer frei (zugestellt ODER gepuffert).
  Eager (klein) vs. Rendezvous (groß) → latente Deadlocks!
- Modi: Send | Bsend (+Buffer_attach) | **Ssend** (Rendezvous,
  Debug-Tool) | Rsend (nie).
- Nichtblockierend: Isend/Irecv → Request → Wait/Test/Waitall.
  Sendepuffer bis Wait nicht ändern, Empfangspuffer nicht anfassen.
- Deadlock-Fixes: Umordnen | **Sendrecv** | Isend/Irecv + Wait.
- Kollektive: alle rufen auf, gleiche Reihenfolge, keine Tags.
  Bcast (identisch) / Scatter(v) (Stücke) / Gather(v) (recvcount = pro
  Prozess!) / Reduce / Allreduce / Alltoall / Barrier. Baum: O(log p).

## SLURM-Kernwissen

```bash
# OpenMP:                          # MPI:                    # Hybrid:
--nodes=1                          --nodes=2                 --nodes=2
--ntasks=1                         --ntasks-per-node=64      --ntasks-per-node=2
--cpus-per-task=16                 srun ./prog               --cpus-per-task=32
export OMP_NUM_THREADS=\
  $SLURM_CPUS_PER_TASK                                       export OMP_NUM_THREADS=…
./prog                                                       srun ./prog
```

Fehlerbilder: -np ≠ ntasks | OpenMP mit srun (N Kopien!) | MPI ohne
Launcher (1 Rank) | OMP_NUM_THREADS > cpus-per-task (Oversubscription).
Cluster: 13 × 64 = 832 Kerne; Login-Knoten tabu; OpenMP = 1 Knoten.

## Fachbegriffe (die der Prüfer hören will)

data race · race condition · reduction · false sharing (64-B-Cache-Line)
· load imbalance · collective · blocking vs. nonblocking · deadlock ·
serialization · overhead · scalability · strong/weak scaling · eager vs.
rendezvous · late sender/late receiver · first touch · oversubscription
