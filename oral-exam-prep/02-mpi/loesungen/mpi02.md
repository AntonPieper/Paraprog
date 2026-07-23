# Lösung mpi02.c — Ping-Pong

**Status: KORREKT.** (Schwierigkeit: leicht)

## Was berechnet das Programm?

Das Ping-Pong-Beispiel aus Übung 8: Zwei Prozesse schicken sich einen
Zähler hin und her, bis `LIMIT` erreicht ist. Wer sendet, entscheidet
`count % 2 == rank`: Rank 0 sendet die geraden Stände (0→1: 1, 2→3, …),
Rank 1 die ungeraden — sie wechseln sich also ab.

## Warum ist das korrekt (deadlock-frei)?

In jeder Runde ist **genau einer** Sender und **genau einer** Empfänger —
`MPI_Send` und `MPI_Recv` finden immer ein Gegenstück (Matching über
Rank + Tag 0 + Kommunikator). Es gibt nie den Fall, dass beide gleichzeitig
blockierend senden oder empfangen.

Außerdem sauber: Der Guard `size != 2` mit `MPI_Abort` — das Programm ist
nur für exakt 2 Prozesse definiert.

## Semantik, die man erklären können muss

- **Blockierend**: `MPI_Send` kehrt zurück, wenn der Sendepuffer
  wiederverwendbar ist — entweder wurde die Nachricht zugestellt **oder vom
  System zwischengepuffert** (Standard Mode; welche Variante, entscheidet
  die Implementierung). `MPI_Recv` kehrt zurück, wenn die Daten im
  Empfangspuffer liegen.
- **Matching-Regeln**: Kommunikator identisch, Quelle/Ziel passend, Tag
  passend, Empfangspuffer ≥ Sendegröße.
- Beide Prozesse führen **denselben Code** aus (SPMD); die Rollen ergeben
  sich aus dem Rank.

## Performance-Einordnung (Vorlesung 9/11)

Ping-Pong ist inhärent **sequenziell** — es gibt keinerlei Parallelität,
das Programm misst Kommunikations-**Latenz**. Mit Rechenarbeit zwischen den
Nachrichten würde die blockierende Variante Wartezeiten erzeugen
(**Late Sender**-Muster); mit `MPI_Isend`/`MPI_Irecv` + `MPI_Wait` ließe
sich Kommunikation mit Rechnung **überlappen** (Vorlesungsexperiment:
Speedup 2 bei 10 ms Rechenzeit pro Runde).

## Mögliche Prüferfragen

- *Was passiert mit 4 Prozessen?* Der Guard bricht mit `MPI_Abort` ab.
  Ohne Guard würden die Ranks 2 und 3 auf Nachrichten warten, die nie
  gesendet werden → das Programm hängt.
- *Was macht MPI_STATUS_IGNORE?* Statusinformationen (Quelle, Tag,
  tatsächliche Länge) werden nicht benötigt und verworfen.
- *Woher weiß der Empfänger die Nachrichtengröße?* Hier statisch (1 int);
  allgemein: `count` im Recv ist eine **Obergrenze**, tatsächliche Länge via
  `MPI_Get_count` aus dem Status; unbekannte Größen vorab mit `MPI_Probe`.
- *Ist die Ausgabe geordnet?* Innerhalb eines Prozesses ja, global gemischt
  (stdout der Prozesse wird vom Launcher zusammengeführt).
