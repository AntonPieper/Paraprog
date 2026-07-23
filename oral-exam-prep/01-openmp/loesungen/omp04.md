# Lösung omp04.c — Pi-Integration mit zwei Data Races

**Status: FEHLERHAFT — zwei Data Races (`x` und `sum`).** (Schwierigkeit: leicht–mittel)

## Was berechnet das Programm?

Pi durch **numerische Integration**: ∫₀¹ 4/(1+x²) dx = π, angenähert mit der
Mittelpunktsregel über `num_steps` Rechtecke der Breite `step`
(bekannt aus Übung 5).

## Die Bugs

`x` und `sum` sind **vor** der parallelen Region deklariert und damit per
Default **shared**. Nur die Schleifenvariable `i` ist automatisch private.

1. **`x` ist shared** — jeder Thread überschreibt das gemeinsame `x`, während
   andere Threads es gerade lesen. Ein Thread rechnet dann mit dem `x` eines
   anderen Threads → **falsches Ergebnis**, nicht nur Performance.
2. **`sum += ...` ist ein Data Race** — read-modify-write ohne
   Synchronisation, verlorene Updates wie bei count3s.

Ergebnis: Pi ist falsch und ändert sich von Lauf zu Lauf
(**nichtdeterministisch** — typisches Race-Symptom).

## Fix

```c
double sum = 0.0;
#pragma omp parallel for reduction(+:sum)
for (long i = 0; i < num_steps; i++) {
    double x = (i + 0.5) * step;      /* im Block deklariert -> private */
    sum += 4.0 / (1.0 + x * x);
}
```

- `x` **innerhalb** der Schleife deklarieren (Variablen, die in der parallelen
  Region deklariert werden, sind automatisch private) — oder Klausel
  `private(x)` verwenden.
- `reduction(+:sum)` für die Teilsummen.
- `step` darf shared bleiben: **nur Lesezugriff braucht keine Synchronisation**.

## Mögliche Prüferfragen

- *Welche Default-Regeln für Datenumgebung gelten?* Alles shared, außer:
  Schleifenvariablen paralleler Schleifen und innerhalb der Region deklarierte
  Variablen sind private.
- *Was macht `default(none)`?* Zwingt, jede Variable explizit als
  shared/private zu deklarieren — guter Stil, hätte den Bug beim Kompilieren
  aufgedeckt.
- *Unterschied `private` vs. `firstprivate` vs. `lastprivate`?*
  private: uninitialisierte private Kopie; firstprivate: mit Wert von außen
  initialisiert; lastprivate: Wert der letzten Iteration wird nach außen
  kopiert (nur bei for/sections).
- *Wäre `#pragma omp critical` um beide Zeilen korrekt?* Ja, aber es
  serialisiert die komplette Schleife → langsamer als sequenziell
  (siehe omp05).
