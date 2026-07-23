# Vorbereitung auf die mündliche Prüfung — Parallel Programming (ST26)

Übungspaket für die mündliche Prüfung bei Prof. Posner (35 Minuten, LinuxLab 46.29).
Alle Aufgaben sind dem echten Prüfungsformat nachempfunden: **kurze C-Programme lesen,
erklären, Fehler/Performance-Probleme finden** — nicht selbst Code schreiben.

## Prüfungsformat (aus `course-materials/ParaProg_ST26_OralProcedure.pdf`)

| Teil | Zeit | Inhalt |
|------|------|--------|
| OpenMP | ~12 Min. | 1 kurzes C-Programm: erklären, ggf. Bug/Performance-Problem finden + Fix |
| MPI | ~12 Min. | 1 kurzes C-Programm: erklären, ggf. Bug/Performance-Problem finden + Fix |
| SLURM / Performance | ~6 Min. | Batch-Skript lesen ODER Messwerte interpretieren (Speedup, Effizienz, Amdahl/Gustafson) |
| Theorie | ~5 Min. | Allgemeine Fragen zu Vorlesung und Übung |

Wichtige Regeln:

- Pro Programm: **bis zu 3 Minuten Sichtung**, dann behalten oder **einmalig** tauschen.
- Manche Programme sind **korrekt**, andere enthalten **Bugs und/oder Performance-Probleme**.
  Dateinamen verraten nichts — genau wie hier in diesem Paket.
- Erwartet wird **präzise Terminologie**: data race, reduction, false sharing, load imbalance,
  collective, blocking vs. nonblocking, deadlock, overhead, scalability.
- Keine Notizen, kein Internet. Sprache: Deutsch oder Englisch.

## Aufbau dieses Pakets

```
01-openmp/       C-Programme (programs/) + Lösungen auf Deutsch (loesungen/)
02-mpi/          C-Programme (programs/) + Lösungen auf Deutsch (loesungen/)
03-slurm/        Batch-Skripte (scripts/) + Lösungen (loesungen/)
04-performance/  Analyse-Aufgaben mit Messwerttabellen (aufgaben/) + Lösungen (loesungen/)
05-theorie/      Fragenkatalog mit Antworten (nach Themen sortiert)
06-lernmethoden/ Prüfungssimulation, Anki-Karten, Spickzettel, Lernplan, interaktives Quiz
```

Die Dateinamen (`omp01.c`, `mpi03.c`, …) sind **neutral** — ob ein Programm korrekt,
fehlerhaft oder langsam ist, steht nur in der Lösung. Die Nummern sind grob nach
Schwierigkeit sortiert (01 = leicht).

## So übst du richtig

1. **Programm öffnen, Timer auf 3 Minuten** — nur sichten, wie in der Prüfung.
2. **Laut erklären** (wirklich laut!): Was berechnet es? Wie ist es parallelisiert?
   Ist es korrekt? Ist es schnell?
3. **Erst danach** die Lösung im `loesungen/`-Ordner lesen und ehrlich vergleichen.
4. Fachbegriffe, die dir nicht sofort eingefallen sind, in Anki wiederholen.

Komplette Prüfungssimulation mit Timern:

```bash
./06-lernmethoden/pruefungssimulation.sh
```

## Empfohlene Reihenfolge

Siehe [Lernplan](06-lernmethoden/lernplan.md). Kurzfassung:
Erst Theorie auffrischen (Spickzettel), dann täglich 2-3 Programme pro Kategorie
laut erklären, ab Tag 3 komplette Simulationen, am Vortag nur noch Wiederholung.
