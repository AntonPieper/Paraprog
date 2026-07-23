# Lösung omp11.c — Fibonacci mit Tasks und Cutoff

**Status: KORREKT und effizient (Lehrbuch-Beispiel).** (Schwierigkeit: schwer)

## Was berechnet das Programm?

Fibonacci rekursiv per **Divide & Conquer mit OpenMP-Tasks**
(Vorlesung 7 / Übung 7): `fib(n-1)` und `fib(n-2)` werden als Tasks
parallel berechnet, `taskwait` wartet auf beide, dann `x + y`.

## Wie funktioniert die Parallelisierung? (alle Bausteine erklären)

- `#pragma omp parallel` + `#pragma omp single`: Team erzeugen, aber nur
  **ein** Thread startet `fib(n)` — sonst würde jeder Thread den kompletten
  Baum rechnen. Die anderen Threads „stehlen" die erzeugten Tasks.
- `#pragma omp task shared(x)`: `x` ist im umgebenden Kontext lokal
  (→ wäre im Task default-firstprivate als **Kopie**) — dann käme das
  Ergebnis nie beim Aufrufer an. `shared(x)` sorgt dafür, dass der Task in
  das `x` des Aufrufers schreibt.
- `firstprivate(n)`: friert den Wert von `n` beim Erzeugen ein.
- `#pragma omp taskwait`: wartet auf die **direkten Kind-Tasks** — ohne
  taskwait würde `x + y` gelesen, bevor die Tasks fertig sind (Race!).
- **Cutoff** (`n < CUTOFF`): unterhalb der Schwelle normal sequenziell
  rekursieren. Ohne Cutoff entstehen exponentiell viele winzige Tasks —
  der **Task-Overhead** (Erzeugen, Verwalten, Scheduling) übersteigt die
  Arbeit; mit Cutoff gibt es genug grobe Tasks für Parallelität, aber
  wenig Overhead. (Übung 7: Cutoff-Tuning.)

## Warum kein Race auf x und y?

Jede Rekursionsebene hat **eigene** `x`/`y` auf ihrem Stack; `shared`
bezieht sich nur auf das Eltern-`x` der jeweiligen Task-Erzeugung, und
`taskwait` ordnet Schreiben (Task) vor Lesen (Eltern) — korrekt
synchronisiert.

## Mögliche Prüferfragen

- *Was passiert, wenn man `taskwait` weglässt?* `return x + y` liest
  Müll/alte Werte — Race zwischen Kind-Task und Eltern. Nichtdeterministisch
  falsch.
- *Was passiert ohne `shared(x)`?* `x` wäre firstprivate im Task — der Task
  schreibt in seine Kopie, das Eltern-`x` bleibt uninitialisiert.
- *Was passiert ohne `single`?* Jeder Thread des Teams führt `fib(n)`
  komplett aus → T-fache Arbeit (und T-fach Tasks), Ergebnis zwar je Thread
  korrekt, aber sinnlos verschwendete Arbeit.
- *Warum ist dieser Algorithmus trotzdem ineffizient?* Naive
  Fibonacci-Rekursion rechnet exponentiell viel doppelt — Parallelisierung
  beschleunigt einen schlechten Algorithmus. (Fair im Prüfungsgespräch zu
  erwähnen: hier geht es um das Task-Muster, nicht um Fibonacci.)
- *Wie würde man den Cutoff wählen?* Experimentell (Übung 7): groß genug,
  dass Tasks „genug Arbeit" enthalten; klein genug für Lastbalance —
  typisch ~20.
