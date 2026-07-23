# Lernplan — 7 Tage bis zur mündlichen Prüfung

Prinzip: **Aktiv üben schlägt passiv lesen.** Die Prüfung verlangt
Erklären unter Zeitdruck — genau das wird trainiert: laut sprechen,
Timer stellen, erst danach die Lösung lesen.

> **Wenig Zeit? (Prüfungen sind am 27.–29. Juli!)** Komprimierte
> 4-Tage-Variante: Tag 1 = Tage 1+2 (Formeln, perf01/02, omp01–05),
> Tag 2 = Tag 3 + halber Tag 4 (omp06–13, mpi01–04), Tag 3 = Rest
> MPI + SLURM (mpi05–13, job01–06), Tag 4 = Generalprobe +
> Wiederholung. Anki läuft täglich nebenher.

## Tag 1 — Fundament Performance & Theorie

- [Spickzettel](spickzettel.md) einmal komplett durcharbeiten.
- Alle Formeln handschriftlich herleiten/aufschreiben (Amdahl auch
  umgestellt nach f).
- [perf01](../04-performance/aufgaben/perf01.md) und
  [perf02](../04-performance/aufgaben/perf02.md) mit Stift und Papier
  rechnen — Taschenrechner gibt es in der Prüfung nicht, Kopfrechnen mit
  runden Zahlen üben.
- Anki-Deck importieren (`anki-karten.txt`, Tab-getrennt) und erste
  Runde lernen.

## Tag 2 — OpenMP Basics

- omp01–omp05 nach Prüfungsprotokoll: 3 Min. Timer sichten → laut
  erklären → Lösung lesen → Lücken notieren.
- Theoriefragen Block C (C1–C8) laut beantworten.
- Anki-Wiederholung (täglich ab jetzt, ~10 Min.).

## Tag 3 — OpenMP Fortgeschritten

- omp06–omp13 im gleichen Protokoll.
- Eigene Fehlersuche vertiefen: Zu jedem Programm die Frage „wäre es mit
  X korrekt/schneller?" durchspielen (X = reduction, dynamic, nowait
  weg, firstprivate …).
- Theoriefragen C9–C17.

## Tag 4 — MPI Basics

- mpi01–mpi07 im Prüfungsprotokoll.
- Die Deadlock-Muster auf ein Blatt zeichnen (wer wartet auf wen?) —
  Send/Send, Recv/Recv, Ring, Kollektiv+P2P.
- Theoriefragen D1–D8.

## Tag 5 — MPI Fortgeschritten + SLURM

- mpi08–mpi13 im Prüfungsprotokoll.
- Alle 6 SLURM-Skripte (job01–job06): je 2 Min. sichten, dann laut:
  „Was wird angefordert, wie gestartet, passt es zusammen?"
- Theoriefragen D9–D15 und E1–E4.

## Tag 6 — Generalprobe

- **Komplette Prüfungssimulation** mit `./pruefungssimulation.sh` —
  ideal mit einer zweiten Person als „Prüfer" (sie darf Nachfragen aus
  den Lösungs-Dateien stellen).
- perf03–perf06 rechnen.
- Theoriefragen Block F + alles, was in den Notizen als Lücke steht.
- Zweite Simulation am Abend, wenn die erste holprig war.

## Tag 7 (Vortag) — Nur noch Wiederholung

- Spickzettel + Anki-Endrunde. Keine neuen Inhalte mehr.
- Die persönliche Lückenliste ein letztes Mal durchgehen.
- Logistik: Zeitslot in Horstl prüfen, Ausweis + Studierendenausweis
  bereitlegen, Weg zum LinuxLab (46.29) klären. Früh schlafen.

## Übe-Techniken (die „anderen Lernwege")

1. **Laut erklären (Feynman-Methode)** — der Kern dieser Prüfung. Wer es
   nicht laut erklären kann, kann es nicht.
2. **Prüfungssimulation mit Timer** (`pruefungssimulation.sh`) —
   trainiert die 3-Minuten-Sichtung und die Skip-Entscheidung.
3. **Spaced Repetition** — Anki-Deck täglich, Begriffe und Formeln.
4. **Interaktives Quiz** (`quiz.html` im Browser öffnen) — schnelle
   Selbsttests zwischendurch, auch am Handy.
5. **Partner-Quiz** — Fragenkatalog an eine zweite Person geben; sie
   fragt kreuz und quer und hakt nach („Warum?", „Und wenn nicht?").
6. **Fehler-Safari** — eigene alte Übungsabgaben öffnen und mit dem
   3-Minuten-Blick (Spickzettel-Checkliste) bewerten, als wären es
   fremde Prüfungsprogramme.
7. **Whiteboard-Zeichnen** — Deadlocks, Fork-Join, Baum-Broadcast,
   Cache-Line/False-Sharing einmal selbst aufmalen; Skizzen helfen auch
   im Prüfungsgespräch.

## Skip-Strategie für die Prüfung

Pro Kategorie darf **einmal** getauscht werden. Faustregel: Tauschen,
wenn nach 2 Minuten weder klar ist, *was* das Programm berechnet, noch
*wo* man suchen müsste. Nicht tauschen, nur weil kein Bug auffällt —
„das Programm ist korrekt, weil …" ist eine vollwertige (und häufige!)
Antwort; manche Programme SIND korrekt.
