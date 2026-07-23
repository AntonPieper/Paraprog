#!/usr/bin/env bash
# Prüfungssimulation für die mündliche Prüfung Parallel Programming (ST26)
#
# Simuliert den echten Ablauf:
#   1. Zufälliges OpenMP-Programm  (3 Min. Sichtung, dann behalten oder 1x tauschen, ~12 Min. erklären)
#   2. Zufälliges MPI-Programm     (gleicher Ablauf, ~12 Min.)
#   3. Zufällig: SLURM-Skript ODER Performance-Aufgabe (~6 Min.)
#   4. Erinnerung an Theoriefragen (~5 Min.)
#
# Aufruf:  ./pruefungssimulation.sh
# Tipp: Laut sprechen, als säße der Prüfer daneben. Erst NACH der Simulation
#       die Lösung im jeweiligen loesungen/-Ordner lesen.

set -euo pipefail
BASE="$(cd "$(dirname "$0")/.." && pwd)"

pick_random() {
  # $1 = Verzeichnis, $2 = Glob
  find "$1" -name "$2" | sort -R | head -n 1
}

countdown() {
  # $1 = Sekunden, $2 = Beschriftung
  local secs=$1
  while [ "$secs" -gt 0 ]; do
    printf "\r%s: %02d:%02d verbleibend " "$2" $((secs/60)) $((secs%60))
    sleep 1
    secs=$((secs-1))
  done
  printf "\r%s: Zeit abgelaufen!            \n" "$2"
  # akustisches Signal, falls Terminal es unterstützt
  printf "\a"
}

open_file() {
  echo ""
  echo "==> Datei: $1"
  echo "    (In der Prüfung öffnest du sie im Editor. Hier: selbst öffnen, z.B. 'less' in zweitem Terminal.)"
  echo ""
}

phase() {
  local title="$1" dir="$2" glob="$3" minutes="$4"
  echo "============================================================"
  echo " $title  (Zeitbudget: ca. $minutes Minuten)"
  echo "============================================================"
  local file replacement
  file=$(pick_random "$dir" "$glob")
  open_file "$file"
  echo "Du hast jetzt 3 Minuten zum Sichten. Danach entscheidest du: behalten oder tauschen (nur 1x!)."
  read -r -p "[Enter] startet die 3-Minuten-Sichtung..."
  countdown 180 "Sichtung"
  read -r -p "Behalten (b) oder tauschen (t)? " choice
  if [ "$choice" = "t" ]; then
    replacement=$(pick_random "$dir" "$glob")
    while [ "$replacement" = "$file" ]; do replacement=$(pick_random "$dir" "$glob"); done
    file="$replacement"
    open_file "$file"
    echo "Das ist jetzt dein Programm — kein weiterer Tausch möglich."
    read -r -p "[Enter] startet erneut 3 Minuten Sichtung..."
    countdown 180 "Sichtung"
  fi
  echo ""
  echo "Erkläre jetzt LAUT:"
  echo "  1. Was berechnet das Programm?"
  echo "  2. Wie funktioniert die Parallelisierung?"
  echo "  3. Ist es korrekt? Wenn nicht: Fehler benennen, erklären, Fix vorschlagen."
  echo "  4. Ist es effizient? Performance-Probleme benennen."
  read -r -p "[Enter] startet den ${minutes}-Minuten-Timer..."
  countdown $((minutes*60)) "Erklärung"
  echo ""
  echo "Danach: Lösung vergleichen -> ${file%.*}"
  echo "        (passende .md im loesungen/-Ordner daneben)"
  echo ""
}

echo ""
echo "############################################################"
echo "#   PRÜFUNGSSIMULATION — Mündliche Prüfung ParaProg ST26   #"
echo "#   Gesamt: 35 Minuten                                     #"
echo "############################################################"
echo ""

phase "TEIL 1: OpenMP" "$BASE/01-openmp/programs" "*.c" 12
phase "TEIL 2: MPI"    "$BASE/02-mpi/programs"    "*.c" 12

echo "============================================================"
echo " TEIL 3: SLURM oder Performance-Analyse (ca. 6 Minuten)"
echo "============================================================"
if [ $((RANDOM % 2)) -eq 0 ]; then
  phase "TEIL 3: SLURM-Skript" "$BASE/03-slurm/scripts" "*.sh" 6
else
  file=$(pick_random "$BASE/04-performance/aufgaben" "*.md")
  open_file "$file"
  echo "Interpretiere die Zahlen: Speedup, Effizienz, Amdahl/Gustafson, strong vs. weak scaling."
  read -r -p "[Enter] startet den 6-Minuten-Timer..."
  countdown 360 "Performance-Analyse"
  echo "Lösung: $BASE/04-performance/loesungen/$(basename "$file")"
fi

echo ""
echo "============================================================"
echo " TEIL 4: Theoriefragen (ca. 5 Minuten)"
echo "============================================================"
echo "Lass dir von jemandem (oder per Zufall) 4-5 Fragen aus"
echo "05-theorie/fragenkatalog.md stellen und beantworte sie laut."
echo ""
echo "Simulation beendet. Ehrliche Selbstbewertung nicht vergessen:"
echo "  - Was konnte ich ohne Zögern erklären?"
echo "  - Wo fehlten mir die Fachbegriffe (data race, false sharing, ...)?"
echo "  - Notiere Lücken und wiederhole sie morgen zuerst."
