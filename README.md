# ParaProg – Parallele Programmierung (ST26)

Vorbereitung auf die mündliche Prüfung in Parallele Programmierung (Hochschule Fulda, Sommersemester 2026, Prüfung 27.–29.07.2026).

## Inhalt

- [`oral-exam-prep/`](oral-exam-prep/) – Übungspaket für die mündliche Prüfung (OpenMP, MPI, SLURM, Performance, Theorie, Lernmethoden). Details im dortigen [README](oral-exam-prep/README.md).
- `course-materials/` – Vorlesungs- und Übungsfolien, Prüfungsablauf (`ParaProg_ST26_OralProcedure.pdf`), Code-Beispiele. Nicht im Repo enthalten (Urheberrecht liegt bei der Hochschule), siehe unten.
- `.claude/skills/pruefung/` – Claude-Code-Skill für eine interaktive Prüfungssimulation (`/pruefung`).

## Kursmaterialien herunterladen

Die Folien lassen sich mit einem fd-Account direkt aus dem Moodle-Kurs laden (Python ≥ 3.9, keine Abhängigkeiten):

```bash
python3 download-course-materials.py
```

Das Skript fragt Benutzername und Passwort ab (alternativ Umgebungsvariablen `MOODLE_USERNAME` / `MOODLE_PASSWORD`) und legt alle Datei-Ressourcen des Kurses unter `course-materials/` ab. Bereits vorhandene Dateien werden übersprungen.
