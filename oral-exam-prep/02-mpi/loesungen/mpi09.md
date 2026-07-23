# Lösung mpi09.c — Halo-Austausch mit MPI_Sendrecv

**Status: KORREKT.** (Schwierigkeit: mittel–schwer)

## Was berechnet das Programm?

Den 1D-**Halo-Austausch** (Ghost Cells) aus Übung 9: Jeder Prozess hält ein
lokales Array (hier: alle Elemente = eigener Rank) und benötigt je eine
**Randzelle des Nachbarn** — typisches Muster für **Stencil-Berechnungen**
(z. B. Wärmeleitung), bei denen ein verteiltes Gitter an den Rändern die
Nachbarwerte braucht.

- Erster `MPI_Sendrecv`: letztes eigenes Element → rechter Nachbar;
  gleichzeitig `left_halo` ← linker Nachbar.
- Zweiter `MPI_Sendrecv`: erstes eigenes Element → linker Nachbar;
  gleichzeitig `right_halo` ← rechter Nachbar.
- **Zyklische Ränder**: `(rank±1+size) % size` schließt den Ring
  (`+size` verhindert negatives Modulo in C bei rank 0).

Erwartete Ausgabe: `left_halo` = Rank des linken, `right_halo` = Rank des
rechten Nachbarn.

## Warum ist das deadlock-frei?

`MPI_Sendrecv` führt Senden **und** Empfangen in **einem** Aufruf aus —
die MPI-Bibliothek ordnet beides intern so an, dass kein zirkuläres Warten
entstehen kann. Die naiven Alternativen aus der Vorlesung:

- Alle `MPI_Send(rechts)` dann `MPI_Recv(links)`: **Deadlock-Risiko** im
  Ring (bei synchronem Protokoll / großen Nachrichten).
- Geordnete Ketten oder Even/Odd-Alternation: korrekt, aber
  **Serialisierung** (Wartezeiten pflanzen sich fort).
- `Isend`/`Irecv` + `Waitall`: ebenfalls korrekt und parallel — die
  Bonus-Variante der Übung; Sendrecv ist die kompakteste Form.

Die Tags (0 für die Rechts-Richtung, 1 für die Links-Richtung) trennen die
beiden Austauschrichtungen sauber — Verwechslungen beim Matching sind
ausgeschlossen.

## Mögliche Prüferfragen

- *Was ist ein Halo/Ghost Cell?* Kopie der Randdaten des Nachbarn im
  eigenen Speicher, damit der lokale Stencil ohne Kommunikation im
  Innenbereich rechnen kann; wird pro Zeitschritt aktualisiert.
- *Warum zwei Sendrecv-Aufrufe?* Zwei Richtungen; jeder Aufruf bedient eine
  Richtung für alle Prozesse gleichzeitig (voll parallel).
- *Ginge es mit einem Tag?* Ja, hier schon (unterschiedliche Quellen
  matchen), aber getrennte Tags sind robuster/lesbarer.
- *Nicht-zyklische Variante?* Rand-Prozesse verwenden `MPI_PROC_NULL` als
  Nachbar (Send/Recv daran sind No-Ops) — oder if-Guards.
- *Wie oft passiert das in echter Anwendung?* In jedem Iterationsschritt
  (z. B. Jacobi/Wärmeleitung) — deshalb lohnt ggf. Überlappung: Irecv/Isend
  starten, **Innenbereich** rechnen, Waitall, dann Ränder rechnen.
