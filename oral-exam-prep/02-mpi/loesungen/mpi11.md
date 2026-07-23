# Lösung mpi11.c — Master-Worker mit serialisierter Verteilung

**Status: FUNKTIONAL KORREKT (bei teilbarem n), aber PERFORMANCE-/SKALIERBARKEITS-PROBLEM.**
(Schwierigkeit: schwer)

## Was berechnet das Programm?

Eine Summe über n Elemente im **Master-Worker-Muster** (wie die
Matrixmultiplikation aus Übung 11/EduMPI): Rank 0 initialisiert alle Daten,
schickt jedem Worker per `MPI_Send` einen Block, rechnet seinen eigenen
Block, sammelt die Teilergebnisse per `MPI_Recv` ein.

## Die Performance-Probleme (zentralisierte Kommunikation)

1. **Serialisierte Verteilung:** Rank 0 sendet die Blöcke **nacheinander**
   — Worker size−1 wartet, bis alle anderen beliefert sind
   (**Late Sender** aus Sicht der Worker: sie blockieren im Recv).
   Verteilzeit wächst **linear mit p** statt logarithmisch.
2. **Zentraler Flaschenhals:** Die gesamte Kommunikation läuft über
   Rank 0 (Stern-Topologie) — dessen Netzwerkanbindung limitiert;
   die EduMPI-Analyse aus Übung 11 zeigt genau dieses Muster:
   Kommunikation zentralisiert → schlechte Skalierbarkeit.
3. **Einsammeln in fester Reihenfolge:** `MPI_Recv` in der Schleife wartet
   erst auf Worker 1, dann 2, … — ist Worker 1 langsam, warten fertige
   Ergebnisse anderer Worker (unnötiges Idling; `MPI_ANY_SOURCE` oder
   besser eine Kollektivoperation vermeidet das).
4. Außerdem hält Rank 0 das **gesamte** Array im Speicher — bei großem n
   ein Speicherproblem (kein verteilter Speichervorteil).

## Fix: Kollektivoperationen

```c
MPI_Scatter(data, chunk, MPI_DOUBLE,
            mydata, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
/* lokale Teilsumme berechnen */
MPI_Reduce(&partial, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
```

- `MPI_Scatter` verteilt baumbasiert (O(log p) statt O(p)), ein Aufruf
  statt p−1 Sends.
- `MPI_Reduce` kombiniert die Teilsummen ebenfalls baumbasiert und in
  beliebiger Ankunftsreihenfolge.

## Sekundärer Befund (Bonuspunkt im Gespräch)

`chunk = n / size` ignoriert den **Rest**: Ist n nicht durch size teilbar,
werden die letzten `n % size` Elemente **nie verarbeitet** → leicht
falsches Ergebnis (mit Default n=10⁸ und z. B. 3 Prozessen). Fix:
`MPI_Scatterv` mit counts/displs (Übung 10) oder Restbehandlung beim
letzten Rank. Vergleiche mpi12 — dort ist genau das der Hauptfehler.

## Mögliche Prüferfragen

- *Wie würde EduMPI/eine Trace-Analyse das zeigen?* Kommunikationsmatrix:
  alle Kanten zu/von Rank 0; Timeline: Worker mit langen Wartezeiten im
  Recv (Late Sender); MPI_Send/Recv dominieren die Kommunikationszeit.
- *Wann ist Master-Worker trotzdem sinnvoll?* Bei **dynamischer**
  Lastverteilung ungleicher Tasks (Worker melden sich fertig, Master
  schickt nächstes Arbeitspaket — wie die zeilenweise Matmul in Übung 11
  mit Tag 0 als Terminierungssignal).
- *Was begrenzt die Skalierbarkeit hier grundsätzlich?* Der serielle
  Anteil (Initialisierung + Verteilung durch einen Prozess) — Amdahl.
