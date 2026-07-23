# Lösung mpi12.c — Blockverteilung mit verlorenem Rest

**Status: FEHLERHAFT — Rest-Elemente werden nicht verarbeitet.**
(Schwierigkeit: schwer, weil unauffällig)

## Was soll das Programm tun?

Die Summe 1 + 2 + … + n verteilt berechnen: Blockverteilung, jeder Rank
summiert seinen Bereich, `MPI_Reduce` kombiniert; Rank 0 vergleicht mit
der Gauß-Formel n(n+1)/2.

## Der Bug: Ganzzahldivision verliert den Rest

```c
long chunk = n / size;          /* Ganzzahldivision rundet AB */
long start = rank * chunk + 1;
long end   = start + chunk;
```

Ist n **nicht** durch size teilbar, gehen die letzten `n % size` Zahlen
verloren: Mit n = 1.000.000 und size = 7 ist chunk = 142.857, der letzte
Rank endet bei 999.999 — die 1.000.000 selbst summiert **niemand**.

Das Fiese (deshalb „schwer"): Mit size ∈ {1, 2, 4, 5, 8, 10, …} teilt
size die Million glatt und das Ergebnis **stimmt** — der Bug erscheint
nur bei bestimmten Prozessanzahlen. Klassischer **latenter Bug**: Tests
mit 2 oder 4 Prozessen im Lab laufen korrekt, der Cluster-Job mit 7
Ranks liefert stillschweigend ein falsches Ergebnis (kein Absturz, kein
Hänger!).

## Fixes (einen sauber vorrechnen)

**Variante A — Rest auf die ersten Ranks verteilen** (wie Übung 10,
uneven distribution: die ersten n mod size Ranks bekommen ein Element
mehr):

```c
long base = n / size, rest = n % size;
long mycount = base + (rank < rest ? 1 : 0);
long start = rank * base + (rank < rest ? rank : rest) + 1;
long end = start + mycount;
```

**Variante B — letzter Rank nimmt den Rest** (einfacher, minimal
unbalanciert):

```c
long end = (rank == size - 1) ? n + 1 : start + chunk;
```

**Variante C — zyklische Verteilung** (eliminiert das Problem elegant,
vgl. mpi05):

```c
for (long i = rank + 1; i <= n; i += size) local_sum += i;
```

## Mögliche Prüferfragen

- *Wie hätte man den Bug gefunden?* Ergebnis gegen bekannte Formel/
  sequenzielle Referenz prüfen (macht das Programm ja — Ausgabe lesen!)
  und mit **verschiedenen** Prozessanzahlen testen, auch „krummen".
- *Ist der Reduce-Teil korrekt?* Ja — alle rufen ihn auf, MPI_SUM auf
  MPI_LONG, Root 0. Der Fehler liegt allein in der Bereichsberechnung.
- *Zusammenhang zu MPI_Scatter?* MPI_Scatter setzt Teilbarkeit voraus
  (n/p gleich große Stücke); für ungleiche Anteile gibt es MPI_Scatterv
  mit sendcounts[]/displs[] — dieselbe Denkfigur wie Variante A.
- *Overflow-Gefahr?* n(n+1)/2 für n = 10⁶ ist ~5·10¹¹ — passt in long
  (64 Bit), aber bei int (32 Bit) würde es überlaufen — Datentyp
  `MPI_LONG` passt zu `long`.
