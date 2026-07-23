# Lösung mpi13.c — Nichtblockierender Ring

**Status: KORREKT.** (Schwierigkeit: schwer — Begründung ist der Kern)

## Was berechnet das Programm?

Der nichtblockierende Ring aus Übung 9: **Jeder** Prozess schickt seinen
Rank gleichzeitig an den rechten Nachbarn und empfängt den Rank des linken
Nachbarn. Muster: `MPI_Isend` (rechts) → blockierendes `MPI_Recv` (links)
→ `MPI_Wait` auf das Isend.

## Warum ist das deadlock-frei? (Die Kernfrage)

- `MPI_Isend` ist eine **Initiierung**: Sie kehrt **sofort** zurück —
  unabhängig davon, was jeder andere Prozess tut (lokale Operation).
  Kein Prozess blockiert also beim Senden.
- Danach stehen **alle** Prozesse gleichzeitig im `MPI_Recv` — und für
  jedes Recv ist das passende Send bereits initiiert → alle Empfänge
  können abgeschlossen werden.
- `MPI_Wait` schließt das Isend ab (gibt den Request frei; erst danach
  dürfte der Sendepuffer wiederverwendet werden — hier wird `rank` nicht
  mehr verändert, also unkritisch, aber Wait ist trotzdem **Pflicht**,
  sonst Ressourcen-Leck).

## Kontrast: Was wäre mit blockierendem MPI_Send? (Vorlesungsfrage)

Alle rufen `MPI_Send(rechts)` auf, **bevor** irgendjemand empfängt:

- **Kleine Nachrichten** (wie hier 1 int): Eager-Pufferung → läuft
  scheinbar problemlos — **latenter Bug**.
- **Große Nachrichten / synchrones Protokoll / MPI_Ssend**: Alle warten
  zirkulär auf das Recv des Nachbarn → **Deadlock**. Das Programm wäre
  nicht „safe".

Auch die „Reparaturversuche" aus der Vorlesung (letzter Rank empfängt
zuerst; Even/Odd-Alternation) vermeiden zwar den Deadlock, erzeugen aber
**Serialisierung** — nur das nichtblockierende Muster (oder
`MPI_Sendrecv`) ist gleichzeitig korrekt **und** voll parallel.

## Mögliche Prüferfragen

- *Alternative mit Irecv?* `MPI_Irecv` (links) → `MPI_Send` (rechts) →
  `MPI_Wait` auf das Irecv — gleichwertig; Irecv zuerst kann sogar
  schneller sein (Empfang schon gepostet → keine Zwischenpufferung).
- *Was gilt zwischen Isend und Wait für den Puffer?* Nicht verändern
  (vgl. mpi08). Bei Irecv: weder lesen noch schreiben bis zum Wait.
- *Was misst die MPI_Wtime-Ausgabe von Rank 0?* Nur die eigene Zeit von
  Rank 0 — für eine saubere Messung aller: `MPI_Barrier` vor Start und
  Maximum über alle Prozesse (vgl. Übung 9, Benchmark-Aufgabe).
- *`MPI_Test` vs. `MPI_Wait`?* Test kehrt sofort mit Flag zurück
  (Polling, erlaubt Arbeit nebenher), Wait blockiert bis zum Abschluss.
- *Wo braucht man dieses Muster praktisch?* Halo-Austausch in
  Stencil-Codes, Nachbarschaftskommunikation allgemein — Überlappung von
  Kommunikation und Berechnung.
