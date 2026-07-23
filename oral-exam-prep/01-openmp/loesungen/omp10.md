# Lösung omp10.c — Tasks mit geteilter Schleifenvariable

**Status: FEHLERHAFT — Tasks lesen die shared Schleifenvariable `i`.**
(Schwierigkeit: schwer)

## Was soll das Programm tun?

Das Muster „**parallel + single + task**" aus Vorlesung 7: Ein Thread
(im `single`-Block) erzeugt 8 Tasks, das Team arbeitet sie ab. Erwartet:
Jedes `item 0…7` wird genau einmal verarbeitet.

## Der Bug: Datenumgebung der Tasks

Die Regel aus der Vorlesung: Variablen, die im umgebenden Kontext **private**
sind, werden im Task per Default **firstprivate** (Wert wird beim Erzeugen
kopiert). Variablen, die **shared** sind, **bleiben shared**.

`int i` ist hier **vor** der parallelen Region deklariert → **shared**.
Der Task kopiert `i` also **nicht**, sondern liest die gemeinsame Variable
**erst wenn er ausgeführt wird** — und die Erzeugerschleife läuft währenddessen
weiter (`usleep` macht das Timing sichtbar):

- Typische Ausgabe: mehrfach `item 8` (der Endwert), Duplikate, fehlende
  Werte — **Data Race** zwischen dem Inkrement in der Schleife und den
  lesenden Tasks; Ergebnis nichtdeterministisch.

## Fix

```c
for (int i = 0; i < 8; i++) {        /* i im for deklariert -> private   */
    #pragma omp task firstprivate(i) /* Wert bei Task-ERZEUGUNG kopieren */
    process(i);
}
```

Schon `int i` **innerhalb** des single-Blocks (bzw. im for-Kopf) würde
reichen: dann ist `i` private im umgebenden Kontext und damit automatisch
firstprivate im Task. `firstprivate(i)` explizit zu schreiben ist trotzdem
guter Stil (Empfehlung der Vorlesung).

## Konzepte, die man hier erklären können muss

- **Warum single?** Ohne `single` würde **jeder** Thread des Teams die
  Schleife ausführen → 4×8 = 32 Tasks. `single` + implizite Barriere am Ende
  stellt sicher: ein Erzeuger, alle arbeiten mit.
- **Wann laufen die Tasks?** Die Runtime entscheidet (sofort oder verzögert);
  spätestens an der Barriere am Ende der parallelen Region sind alle fertig.
- **Task-Scheduling-Points**: Task-Erzeugung, Task-Ende, taskwait, Barrieren.

## Mögliche Prüferfragen

- *Unterschied `firstprivate` auf einem Task vs. auf einer parallelen Region?*
  Beim Task wird der Wert **zum Erzeugungszeitpunkt** eingefroren — genau
  deshalb ist es hier der Fix.
- *Wozu wäre `taskwait` gut?* Wartet auf alle **Kind-Tasks** des aktuellen
  Tasks — nötig, wenn nach der Schleife mit den Ergebnissen weitergearbeitet
  würde (hier übernimmt die Barriere am Ende von single/parallel das Warten).
- *Wann Tasks statt `parallel for`?* Bei irregulären Strukturen: verkettete
  Listen, Rekursion (Fibonacci, Divide & Conquer). Für reguläre Schleifen ist
  `parallel for` overhead-ärmer.
- *Was ist ein tied vs. untied Task?* tied (Default): wird nach Unterbrechung
  nur vom selben Thread fortgesetzt; untied: von jedem.
