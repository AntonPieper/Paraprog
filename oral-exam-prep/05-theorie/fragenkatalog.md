# Fragenkatalog mit Antworten — Theorieteil (und Zwischenfragen)

Fragen wie sie im 5-minütigen Theorieteil (und als Nachfragen zu den
Programmen) vorkommen können. Erst selbst laut beantworten, dann Antwort
aufklappen bzw. lesen. Gruppiert nach Themen, innerhalb grob nach
Schwierigkeit.

---

## A. Grundlagen und Architekturen (Vorlesung 1–3)

**A1. Warum überhaupt parallel programmieren?**
Physikalische Grenzen sequenzieller Architekturen (Taktfrequenz stagniert:
Wärmeabfuhr, Lichtgeschwindigkeit/Strukturgrößen), Speicherbandbreiten-
Lücke, und Anwendungsbedarf (größere/präzisere Simulationen, kürzere
Time-to-Solution). Moore's Law liefert weiter Transistoren, aber seit
~2005 keine höheren Takte — die Transistoren gehen in mehr Kerne.

**A2. Was besagt Moore's Law wirklich?**
Nicht „Prozessorleistung verdoppelt sich alle zwei Jahre", sondern:
Transistordichte/Transistoren pro Chip (bzw. pro Dollar) wachsen
exponentiell (Verdopplung ~18–24 Monate); Stagnation seit ~2016.

**A3. Unterschied Shared Memory vs. Distributed Memory?**
Shared: alle Kerne sehen einen gemeinsamen Adressraum (SMP),
Kommunikation implizit über gemeinsame Daten, explizite Synchronisation
nötig; Programmiermodell OpenMP; relativ leicht zu programmieren.
Distributed: jeder Knoten hat lokalen Speicher, Kommunikation explizit
per Message Passing (MPI), Datenverteilung explizit; sehr gut skalierbar.
Hybrid: Cluster aus SMP-Knoten (NUMA), MPI + OpenMP kombiniert.

**A4. Flynns Klassifikation?**
Nach Befehls- und Datenströmen: SISD (von Neumann), SIMD
(Vektor-/Array-Prozessoren, GPUs), MISD (praktisch irrelevant), MIMD
(heutige Parallelrechner). MIMD unterteilt in Multiprozessoren
(gemeinsamer Adressraum: SMP/NUMA) und Multicomputer (NORMA/Cluster).

**A5. SMP vs. NUMA?**
SMP: Speicherzugriffszeit für alle Prozessoren identisch. NUMA: jeder
Prozessor hat schnellen lokalen und langsameren entfernten Speicher —
Datenplatzierung/Lokalität wird performancerelevant.

**A6. Prozess vs. Thread?**
Prozess: vom OS bereitgestellte Instanz eines Programms, eigener
Adressraum. Thread: leichtgewichtige Ausführungseinheit im Prozess —
eigener Stack und Registersatz, teilt Heap und globale Daten mit den
anderen Threads; kleinste schedulbare Einheit.

**A7. Was bedeutet SPMD?**
Single Program, Multiple Data: ein Programm läuft auf allen
Prozessen/Threads; Verhalten differenziert über die ID
(`if (rank == 0) ...`). Master ist üblicherweise Rank 0. Fundamentales
Paradigma von MPI (und sinngemäß OpenMP).

**A8. Domain vs. Functional Decomposition?**
Domain: Datenraum in (gleich große) Regionen zerlegen, alle mit demselben
Code parallel bearbeitet — Standard im HPC. Functional: verschiedene
Programmteile/Module laufen parallel — komplexer.

**A9. Cache-Kohärenz und False Sharing?**
Kohärenz: Bei parallelen Caches mit Schreibzugriff müssen Kopien
konsistent gehalten werden (z. B. Bus Snooping + Invalidierung der
Cache-Line). False Sharing: mehrere Prozessoren schreiben verschiedene
Daten **derselben Cache-Line** — die Line wandert ständig
(Invalidierungs-Ping-Pong), unnötiger Kohärenzverkehr, Performance
bricht ein. Fix: Padding auf Cache-Line-Größe (64 Byte) oder private
Kopien/Reduction.

**A10. Temporale vs. räumliche Lokalität?**
Temporal: kürzlich benutzte Daten werden bald wieder benutzt (Cache
lohnt). Räumlich: benachbarte Daten werden zeitnah benutzt (Cache-Line
lohnt; C ist row-major → zeilenweise Traversierung ist schnell,
spaltenweise verursacht pro Zugriff einen neuen Line-Load — Übung 3:
Faktor wächst mit N).

**A11. Was ist die TOP500?**
Liste der 500 schnellsten Supercomputer, LINPACK-Benchmark, Metrik
FLOPS, seit 1993 zweimal jährlich (ISC/SC). Aktuell vorn: El Capitan
(LLNL, ~1.7–1.8 ExaFLOPS, >11 Mio. Kerne). Alternativen: Green500
(Energieeffizienz), IO500, AI500.

**A12. Kenndaten des Fulda HPC-Clusters?**
13 Knoten × 64 Kerne (4× AMD Opteron mit je 16 Kernen), kein
Hyperthreading → max. 832 MPI-Prozesse; 256 GB RAM/Knoten; Batch-System
SLURM; Zugriff per SSH; jede Ausführung über SLURM (nie auf dem
Login-Knoten); OpenMP-Programme: genau ein Knoten.

---

## B. Performance-Begriffe und Gesetze (Vorlesung 3)

**B1. Definition Speedup und Effizienz?**
S(p) = T_seq / T_par(p), wobei T_seq das **schnellste sequenzielle**
Programm ist. E(p) = S(p)/p = T_seq/(p·T_par(p)), 0 < E ≤ 1.
Ideal: S(p) = p, E = 1.

**B2. Definition totaler paralleler Overhead?**
O(p) = p·T_par(p) − T_seq — die insgesamt von allen Prozessoren
zusätzlich aufgewendete Zeit (Kommunikation, Warten, Verwaltung,
Zusatzrechnung).

**B3. Amdahls Gesetz — Formel, Annahme, Konsequenz?**
S_max(p) = 1/(f + (1−f)/p) bei festem sequenziellen Anteil f und
konstanter Problemgröße; für p → ∞: S ≤ 1/f. Schon 5 % sequenzieller
Anteil begrenzen auf Speedup 20.

**B4. Gustafsons Gesetz — Idee?**
Bei weak scaling (Problem wächst mit p) bleibt der sequenzielle Anteil
absolut etwa konstant, sein relativer Anteil sinkt → skalierter Speedup
S_scaled(p) = p − f·(p−1); hinreichend große Probleme lassen sich
effizient parallelisieren. Optimistischer als Amdahl, weil andere Frage.

**B5. Strong vs. Weak Scaling?**
Strong: p steigt, Problemgröße fest (Metrik: Speedup/Effizienz).
Weak: p steigt, Problemgröße proportional mit (Metrik: T(p) ≈ T(1),
Weak-Scaling-Effizienz T(1)/T(p)).

**B6. Warum gibt es (fast) nie idealen Speedup?**
1) Nicht parallelisierbare Anteile/Abhängigkeiten (Amdahl), 2)
Management-Overhead und Lastungleichgewicht, 3) Synchronisation und
Kommunikation.

**B7. Wie kann Speedup superlinear sein?**
Cache-Effekte (aggregierter Cache wächst mit p — Arbeitsset passt
plötzlich hinein) oder veränderter Berechnungspfad bei Suchalgorithmen
(paralleles Suchen findet früher).

**B8. Was ist der Parallelitätsgrad?**
Abhängigkeitsgraph (DAG) der Operationen; T_∞ = längster Pfad;
P = T_seq/T_∞ = maximal erreichbarer Speedup bei beliebig vielen
Prozessoren.

**B9. Welche Datenabhängigkeiten gibt es?**
True dependence (Write→Read), Anti dependence (Read→Write), Output
dependence (Write→Write); dazu Kontrollabhängigkeiten. Nur unabhängige
Iterationen dürfen parallelisiert werden.

**B10. Data Race vs. Race Condition vs. Deadlock?**
Data Race: unsynchronisierter gleichzeitiger Zugriff auf dieselbe
Speicherstelle, mindestens ein Schreiber — Ergebnis laut Standard
unspezifiziert. Race Condition: Ergebnis hängt von der
Ausführungsreihenfolge ab. Deadlock: Prozesse warten zirkulär auf
Ergebnisse/Ressourcen, die nur die Gruppe selbst liefern könnte — kein
Fortschritt.

**B11. Wie misst man parallele Programme korrekt?**
Wall-Clock-Zeit (nicht CPU-Zeit!): `omp_get_wtime()`/`MPI_Wtime()`, nur
den Kernel messen, Problemgröße groß genug (≥ ~30 s), ≥ 3 Wiederholungen
(Mittel oder Minimum, Methode dokumentieren), Ergebnis ausgeben gegen
Dead-Code-Elimination. `time`-Kommando: real = Wall-Clock, user/sys =
CPU-Zeit in User-/Kernel-Modus.

---

## C. OpenMP (Vorlesung 4–7)

**C1. Was ist OpenMP?**
Standardisierte API für Multi-Threading im Shared Memory:
Compiler-Direktiven (#pragma omp), Runtime-Bibliothek,
Umgebungsvariablen; C/C++/Fortran; Fork-Join-Modell; inkrementelle
Parallelisierung; keine Laufzeitprüfung der Direktiven-Korrektheit.

**C2. Fork-Join-Modell?**
Master-Thread läuft sequenziell; am parallel-Konstrukt wird ein Team
erzeugt (Fork), am Ende implizite Barriere, danach läuft nur der Master
weiter (Join). Verschachtelte Regionen möglich (Default: deaktiviert).

**C3. Default-Scoping-Regeln?**
Alles shared — außer: Schleifenvariablen paralleler Schleifen und
innerhalb der Region deklarierte Variablen sind private.
Klauseln: private, firstprivate (initialisiert von außen), lastprivate
(letzter Wert nach außen), shared, default(none), reduction.

**C4. Wie wird die Threadanzahl bestimmt (Priorität)?**
num_threads-Klausel > omp_set_num_threads() > OMP_NUM_THREADS >
Implementierungs-Default. omp_get_num_threads() außerhalb einer
parallelen Region liefert 1.

**C5. Scheduling-Varianten der parallelen Schleife?**
static (Blöcke gleich groß, deterministisch, beste Lokalität; mit
Chunk: zyklisch), dynamic (Blöcke zur Laufzeit geholt — beste Balance
bei irregulärer Last, Overhead), guided (erst große, dann kleinere
Blöcke), auto (Compiler/Runtime), runtime (aus OMP_SCHEDULE). Chunk =
Trade-off Lastbalance vs. Lokalität/Overhead.

**C6. Wann welches Schedule?**
Gleichmäßige Last → static. Bekannte, strukturierte Ungleichlast →
static mit kleinem Chunk (zyklisch). Unvorhersehbare Last (Mandelbrot,
Collatz) → dynamic (oder guided). Messen statt raten: schedule(runtime).

**C7. reduction — Semantik und Anforderungen?**
Private Kopie pro Thread (neutrales Element), am Ende Kombination.
Operator muss kommutativ und assoziativ sein; Gleitkomma: minimale
Rundungsunterschiede möglich. Eigene Reduktionen: declare reduction
(z. B. argmax-Struct in Übung 6).

**C8. critical vs. atomic vs. Locks — Rangfolge?**
Faustregel: atomic → critical → Locks. atomic: einzelne einfache
Operation (x++, x+=y), meist Hardware-Instruktion. critical: beliebiger
Block; unbenannte criticals teilen **einen globalen Lock**; Namen
statisch. Locks (omp_lock_t, init/set/unset/test/destroy): flexibel,
Anzahl zur Laufzeit (z. B. ein Lock pro Bin), omp_test_lock erlaubt
Arbeit während des Wartens. Achtung: critical und atomic auf derselben
Variable mischen → Konflikte.

**C9. Wo gibt es implizite Barrieren?**
Am Ende von parallel, for, sections, single — jeweils entfernbar mit
nowait (außer parallel). master hat **keine** Barriere (und keine
Klauseln). Explizit: #pragma omp barrier — muss von allen oder keinem
Thread erreicht werden (nie in einseitigem if ohne Block!).

**C10. single vs. master?**
single: genau ein (beliebiger) Thread, implizite Barriere am Ende
(außer nowait). master: nur Thread 0, keine Barriere.

**C11. OpenMP-Tasks — wozu und wie?**
Für irreguläre Parallelität: Listen, Rekursion, Divide & Conquer.
Muster: parallel + single erzeugt Tasks (#pragma omp task), Team
arbeitet sie ab; taskwait wartet auf direkte Kind-Tasks; Datenumgebung:
private Variablen des Erzeugers werden default-firstprivate im Task,
shared bleibt shared. Cutoff gegen Task-Overhead. taskloop teilt
Schleife in Tasks (grainsize/num_tasks). Tied (Default) vs. untied.

**C12. Wann Tasks, wann parallel for?**
Reguläre Schleifen → parallel for (weniger Overhead). Tasks nur, wenn
die Struktur es erfordert (while über Liste, Rekursion) — Programmierer
kontrolliert Anzahl/Granularität.

**C13. OpenMP-Cancellation?**
Kooperativ: #pragma omp cancel for/parallel/... signalisiert Abbruch;
Threads prüfen an Cancellation Points (#pragma omp cancellation point);
muss mit OMP_CANCELLATION=true aktiviert werden; Aufräumen ist
Programmierersache. (Übung 7: parallele Suche.)

**C14. First-Touch und NUMA?**
malloc reserviert nur virtuell; die physische Seite wird beim **ersten
Zugriff** platziert — beim Thread, der zuerst anfasst. Initialisiert der
Master alles, liegen alle Daten an seinem Speichercontroller → entfernte
Zugriffe für andere Threads. Daher: Daten von den Threads initialisieren
lassen, die sie später benutzen.

**C15. Effizienz-Checkliste OpenMP?**
Lange/häufige criticals vermeiden; reduction statt Handakkumulation;
parallele Regionen groß machen (eine Region, mehrere for — nicht
parallel in Schleifen); thread-lokal akkumulieren; atomic→critical→locks;
unnötige Barrieren vermeiden (nowait wo sicher).

**C16. Was macht flush / das OpenMP-Speichermodell? (Kurzversion)**
Relaxed Consistency: Threads dürfen Werte in einer temporären Sicht
(Cache/Register) halten; flush synchronisiert mit dem Hauptspeicher.
Implizit bei Barrieren, critical, Lock-Operationen (atomic nur für die
betroffene Variable). Explizites flush ist fehleranfällig — Zugriffe
stattdessen mit critical/atomic/locks schützen; volatile ist **kein**
Ersatz.

**C17. Nested Parallelism?**
Default deaktiviert (innere Regionen laufen sequenziell); aktivierbar
via OMP_NESTED/omp_set_nested bzw. OMP_MAX_ACTIVE_LEVELS;
Threadanzahlen pro Ebene (OMP_NUM_THREADS=2,3).

---

## D. MPI (Vorlesung 8–10)

**D1. Was ist MPI?**
Ein **Standard** (nicht Implementierung) für Message Passing:
portabel, C/Fortran-Bindings, aktuell MPI 5.0; Implementierungen:
Open MPI, MPICH, MVAPICH. Prozessanzahl steht beim Start fest; jeder
Prozess hat privaten Speicher; SPMD mit Ranks; Start über mpirun/srun.

**D2. Grundgerüst jedes MPI-Programms?**
MPI_Init (vorher keine MPI-Aufrufe) → MPI_Comm_rank/MPI_Comm_size auf
MPI_COMM_WORLD → Arbeit/Kommunikation → MPI_Finalize (danach keine
MPI-Aufrufe; beendet das Programm nicht). Kompilieren mit mpicc.

**D3. Was ist ein Kommunikator?**
Verwaltete Prozessgruppe mit eigener Nummerierung (Ranks 0…size−1);
MPI_COMM_WORLD enthält alle gestarteten Prozesse. Send/Recv matchen nur
innerhalb desselben Kommunikators.

**D4. Parameter und Matching von Send/Recv?**
buf/count/datatype, dest bzw. source, tag, comm (+status bei Recv).
Matching: Kommunikator identisch, Ziel/Quelle passend (Recv darf
MPI_ANY_SOURCE), Tag passend (MPI_ANY_TAG möglich), Empfangspuffer ≥
Sendegröße. Nachrichten desselben Sender-Empfänger-Paars überholen sich
nicht; Fairness ist nicht garantiert. Tatsächliche Länge: MPI_Get_count.

**D5. Warum MPI-Datentypen (MPI_INT statt int)?**
Interoperabilität in heterogenen Umgebungen — die Bibliothek kennt
Darstellung/Größe und konvertiert korrekt.

**D6. Semantik von blockierendem Send/Recv?**
MPI_Send kehrt zurück, wenn der Sendepuffer wiederverwendbar ist —
Nachricht zugestellt **oder** systemgepuffert (nicht unterscheidbar!).
MPI_Recv kehrt zurück, wenn die Daten im Puffer liegen.

**D7. Die vier Kommunikationsmodi?**
Standard (Send — puffert oder synchron, entscheidet Implementierung),
Buffered (Bsend + MPI_Buffer_attach: kehrt immer zurück; Kopierkosten),
Synchronous (Ssend: Rendezvous — kehrt erst zurück, wenn Recv
gestartet), Ready (Rsend: Recv muss schon gepostet sein — praktisch
nie benutzen). Immediate-Varianten mit I-Präfix sind nichtblockierend.

**D8. Eager vs. Rendezvous?**
Kleine Nachrichten: eager — sofort in Systempuffer kopiert, Send kehrt
früh zurück. Große: rendezvous — Send wartet auf das Recv. Konsequenz:
Send-Send-Deadlocks bleiben bei kleinen Nachrichten **latent** und
schlagen erst bei großen zu. MPI_Ssend als Debug-Werkzeug macht sie
deterministisch sichtbar. „Safe" Programm: Korrektheit unabhängig von
Systempuffern.

**D9. Nichtblockierende Kommunikation — Konzept?**
Zwei Phasen: Initiierung (MPI_Isend/Irecv — lokal, kehrt sofort zurück,
Request-Handle) und Abschluss (MPI_Wait blockierend / MPI_Test mit
Flag; MPI_Waitall für Arrays). Sendepuffer bis zum Abschluss nicht
ändern; Empfangspuffer bis zum Abschluss weder lesen noch schreiben.
Nutzen: Überlappung Kommunikation/Berechnung, Deadlock- und
Serialisierungs-Vermeidung. Isend+sofortiges Wait ≡ Send.

**D10. MPI_Sendrecv?**
Senden und Empfangen in einem Aufruf, intern deadlock-frei geordnet —
ideal für paarweisen Austausch/Ringe/Halos (_replace-Variante: ein
Puffer).

**D11. Kollektivoperationen — Kategorien und Regeln?**
One-to-All (Bcast, Scatter), All-to-One (Gather, Reduce), All-to-All
(Allgather, Alltoall, Allreduce), Synchronisation (Barrier). Regeln:
**alle** Prozesse des Kommunikators rufen auf, gleiche Reihenfolge,
**keine Tags**, Puffergrößen exakt; blockierend oder nichtblockierend
(Ibcast, Ireduce — nicht mit blockierenden mischen). Vorteil: optimierte
Algorithmen (baumbasiert O(log p)), Performance-Portabilität.

**D12. Reduce vs. Allreduce vs. Scan?**
Reduce: elementweise Kombination (MPI_SUM, MPI_MAX, MPI_MAXLOC, …) zum
Root. Allreduce: Ergebnis an alle. Reduce_scatter: reduzieren +
Stücke verteilen. Scan/Exscan: Präfix-Reduktion (Prozess i erhält
Reduktion über 0…i). Eigene Operatoren: MPI_Op_create (assoziativ!).

**D13. Scatter/Gather-Familie?**
Scatter: Root verteilt gleich große Stücke (n durch p teilbar!);
Scatterv: counts[]+displs[] für ungleiche/nicht zusammenhängende Teile.
Gather: sammelt in Rank-Reihenfolge am Root (Puffergröße =
Programmiererpflicht; recvcount = Anteil EINES Prozesses!); Gatherv
analog; Allgather = Gather + Bcast; Alltoall = kompletter Austausch
(Block j von Prozess i landet als Block i bei Prozess j).

**D14. Typische MPI-Deadlock-Muster?**
1) Beide Recv zuerst (garantiert), 2) beide Send zuerst (latent — je
nach Pufferung; mit Ssend garantiert), 3) zyklischer Ring aus
blockierenden Sends, 4) Kollektiv+P2P verschränkt in unterschiedlicher
Reihenfolge (Bcast vs. Recv), 5) Kollektive in unterschiedlicher
Reihenfolge auf verschiedenen Ranks. Fixes: Umordnen, Sendrecv,
nichtblockierend (+Wait), Ibcast.

**D15. Warum Kollektive statt P2P-Schleifen?**
Kürzer, schneller (Baum statt linear), performance-portabel — die
Bibliothek wählt den Algorithmus passend zur Architektur (vgl. 9
Bcast-Algorithmen in EduMPI von basic_linear bis binomial).

---

## E. SLURM und Cluster-Praxis (Übungen 4–11)

**E1. Wichtigste sbatch-Direktiven?**
--job-name, --nodes, --ntasks (Prozesse/Ranks), --ntasks-per-node,
--cpus-per-task (Kerne pro Task → OpenMP-Threads), --time (Limit),
--output (%j = Job-ID). Einreichen: sbatch skript.sh; Status: squeue.

**E2. OpenMP-Job vs. MPI-Job — Grundmuster?**
OpenMP: --nodes=1 --ntasks=1 --cpus-per-task=N;
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK; Programm direkt starten.
MPI: --ntasks=N (ggf. --nodes/--ntasks-per-node); Start mit srun
(nimmt Allokation) oder mpirun -np $SLURM_NTASKS.
Hybrid: --ntasks × --cpus-per-task; srun + OMP_NUM_THREADS.

**E3. Typische Mismatches in Batch-Skripten?**
mpirun -np ≠ --ntasks (Verschwendung oder Oversubscription); OpenMP-
Programm mit srun/--ntasks>1 (N unabhängige Kopien!); MPI-Programm ohne
Launcher (läuft als 1 Rank); OMP_NUM_THREADS > --cpus-per-task
(Oversubscription); OpenMP über mehrere Knoten (unmöglich).

**E4. Warum Messungen auf dem Cluster, nie auf dem Login-Knoten?**
Reproduzierbarkeit (exklusive, definierte Ressourcen), Login-Knoten
teilen sich alle; Übungs-Workflow: ein Binary, Parameter als Argumente,
eine CSV-Zeile pro Lauf, run_all.sh mit Schleifen, ≥ 3 Wiederholungen,
P ∈ {1,2,4,8,16,32,64}.

---

## F. Performance-Analyse und Debugging (Vorlesung 11)

**F1. Profiling vs. Tracing?**
Profiling: aggregierte Statistik (Zeit/Aufrufe pro Funktion; event- oder
sampling-basiert) — wenig Overhead, keine zeitliche Ordnung. Tracing:
Ereignisse mit Zeitstempel — erhält Ordnung/Kausalität, erlaubt
Ursachenanalyse, aber hoher Speicher-/Laufzeit-Overhead.

**F2. Instrumentierung?**
Code zum Datensammeln einfügen: manuell, durch Compiler, oder
vorinstrumentierte Bibliotheken (MPI-Wrapper). Statisch (Compile-Zeit)
vs. dynamisch (am Binary, zur Laufzeit).

**F3. Late Sender / Late Receiver?**
Late Sender: Empfänger blockiert im Recv, weil das Send noch nicht
abgesetzt ist → Idle beim Empfänger. Late Receiver: Sender blockiert
(synchron/Rendezvous), weil kein Recv gepostet ist. Beides via
Trace-Analyse (Scalasca, EduMPI) erkennbar und quantifizierbar.

**F4. Werkzeuge zuordnen?**
Score-P: Instrumentierung/Messung (CUBE4-Profile, OTF2-Traces).
Cube: Profil-GUI (Metrik/Programm/System). Vampir: Trace-Timeline
(kommerziell). Scalasca: automatische Mustererkennung (Late Sender …).
TAU: All-in-One. Debugger: DDT, TotalView (kommerziell); MUST
(MPI-Korrektheit), STAT, ISP. EduMPI: Kurs-Tool mit 2D/3D-Ansicht,
Kommunikationsmatrix, Bcast-Algorithmen-Visualisierung.

**F5. Typische Fehlerklassen in parallelen Programmen?**
Races (Nichtdeterminismus), Deadlocks, MPI-spezifisch: unpassende
Send/Recv-Parameter (Größe, Typ, Tag, Kommunikator), Pufferprobleme.
Performance: Load Imbalance, Serialisierung, False Sharing,
Kommunikations-Overhead, Oversubscription.

**F6. Vorgehen Performance-Optimierung?**
Iterativ: messen → verstehen (Profil/Trace, Muster) → optimieren →
erneut messen. Erst Korrektheit, dann Performance.
