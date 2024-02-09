# CPP_ToDo

Das ist eine einfache GUI-Anwendung, die in C++ mit `wxWidgets` erstellt wurde. Es ist ein einfacher ToDo-Manager, der es Ihnen ermöglicht, Aufgaben hinzuzufügen, zu bearbeiten und zu löschen.

---

## Struktur

Das Projekt ist wie folgt strukturiert:

- `src/`: Dieses Verzeichnis enthält den Quellcode des Projekts.
    - `App.cpp`: Die Hauptanwendungsklasse.
    - `Task.cpp`: Die Klasse, die eine einzelne Aufgabe repräsentiert.
    - `main.cpp`: Der Einstiegspunkt des Programms.
- `header/`: Dieses Verzeichnis enthält die Header-Dateien für die Klassen.
    - `App.h`: Der Header für die App-Klasse.
    - `Task.h`: Der Header für die Task-Klasse.
    - `main.h`: Der Haupt-Header.

---

## Kompilieren und Ausführen
Um das Projekt zu kompilieren und auszuführen, benötigen Sie `wxWidgets` und einen C++-Compiler. Sie können `wxWidgets` von der [offiziellen Website](https://www.wxwidgets.org/) herunterladen. Nachdem Sie `wxWidgets` installiert haben, muss es noch in Visual Studio eingebunden werden. Dazu müssen Sie die Umgebungsvariable `WXWIN` auf das Verzeichnis setzen, in dem `wxWidgets` installiert ist. Anschließend können Sie das Projekt in Visual Studio öffnen und ausführen.

oder sie können das Projekt auch direkt ausführen, indem sie die [CPP_ToDo.exe](Debug/CPP_ToDo.exe) ausführen.
