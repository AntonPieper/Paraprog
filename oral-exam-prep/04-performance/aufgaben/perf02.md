# Aufgabe perf02 — Amdahls Gesetz

Ein Programm besteht zu **f = 10 %** aus nicht parallelisierbarem
(sequenziellem) Anteil; der Rest ist perfekt parallelisierbar
(Overhead wird vernachlässigt).

**Aufgaben:**

1. Nenne Amdahls Gesetz als Formel und berechne den maximalen Speedup
   für p = 4 und p = 16.
2. Welcher Speedup ist mit beliebig vielen Prozessoren (p → ∞) maximal
   erreichbar?
3. Eine Messung mit p = 16 ergibt S(16) = 6.4. Zeige, dass das zu
   f = 10 % passt (schätze f aus der Messung).
4. Der Chef will 64 statt 16 Kerne kaufen, „dann wird es 4× schneller".
   Nimm begründet Stellung.
5. Warum ist Amdahls Gesetz oft zu pessimistisch für die Praxis?
   (Stichwort: Gustafson)
