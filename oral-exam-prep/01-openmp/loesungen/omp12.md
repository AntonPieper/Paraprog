# Lösung omp12.c — nowait mit Abhängigkeit zwischen Schleifen

**Status: FEHLERHAFT — Race durch entfernte implizite Barriere.**
(Schwierigkeit: schwer)

## Was berechnet das Programm?

Zwei Worksharing-Schleifen in **einer** parallelen Region (guter Stil —
Region nicht ständig neu öffnen):

1. `b[i] = 2 * a[i]`
2. `c[i] = b[i] + b[n-1-i]` — liest b **an zwei Stellen**: vorne und hinten.

## Der Bug

`#pragma omp for` hat am **Ende eine implizite Barriere** — `nowait`
entfernt sie. Damit darf ein Thread, der mit Schleife 1 fertig ist, sofort
mit seinem Teil von Schleife 2 beginnen, **während andere Threads noch
Schleife 1 schreiben**.

Konkret: Thread 0 berechnet `c[0] = b[0] + b[n-1]`. `b[n-1]` schreibt aber
der **letzte** Thread (statische Blockverteilung!). Ist der noch nicht
fertig, liest Thread 0 einen **uninitialisierten Wert** → Data Race,
Ergebnis nichtdeterministisch falsch.

## Wann wäre nowait erlaubt? (Das will der Prüfer hören)

Die Vorlesungsregel: nowait ist korrekt, wenn die zweite Schleife nur Daten
liest, die **derselbe Thread** in der ersten Schleife geschrieben hat —
garantiert bei **identischen Schleifenköpfen mit `schedule(static)`**
(gleiche Iteration → gleicher Thread). Beispiel: wäre Schleife 2 nur
`c[i] = b[i] + 1`, wäre nowait **korrekt und sinnvoll** (spart eine
Barriere). Der Zugriff `b[n-1-i]` ist **cross-thread** — genau deshalb
bricht es hier.

## Fix

`nowait` entfernen — die implizite Barriere am Ende von Schleife 1 stellt
sicher, dass ganz b geschrieben ist, bevor irgendein Thread Schleife 2
beginnt:

```c
#pragma omp for schedule(static)
for (int i = 0; i < n; i++)
    b[i] = 2.0 * a[i];

#pragma omp for schedule(static)
for (int i = 0; i < n; i++)
    c[i] = b[i] + b[n - 1 - i];
```

(Ein explizites `#pragma omp barrier` zwischen den Schleifen wäre redundant
zur impliziten.)

## Mögliche Prüferfragen

- *Wo gibt es überall implizite Barrieren?* Am Ende von `parallel`, `for`,
  `sections`, `single` (jeweils ohne nowait). **Nicht** bei `master`.
- *Warum überhaupt nowait?* Barrieren kosten Wartezeit; unnötige Barrieren
  vermeiden ist ein Punkt der Effizienz-Checkliste. Nur eben nicht hier.
- *Warum eine große parallele Region mit zwei `for` statt zweimal
  `parallel for`?* Thread-Team nur einmal erzeugen — weniger
  Fork-Join-Overhead (Effizienz-Checkliste der Vorlesung).
- *Wie äußert sich der Fehler?* Meist korrekt (Timing!), gelegentlich
  falsche Werte — typisch für Races: selten, nichtdeterministisch, schwer
  zu debuggen.
