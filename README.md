# Ferngesteuerter Quick Release Haken

## Mein Video
**Mein Video zum Haken: https://youtu.be/QvPBMdb_oJw**

[![Ferngesteuerter Haken für den Kran im Kliemannsland](https://img.youtube.com/vi/QvPBMdb_oJw/0.jpg)](https://youtu.be/QvPBMdb_oJw)

## Einleitung
Dies ist die Dokumentation, die Fertigungsdateien und der Programmcode zur zum ferngesteuerten Quick Release Haken, den ich hauptsächlich für den Kran im Kliemannsland entwickelt habe. Reinschauen lohnt sich also ;)

Mein Video: https://youtu.be/QvPBMdb_oJw

Falls es noch Fragen gibt, gerne hier oder per Instagram ( https://www.instagram.com/shocker0815/ ) nachfragen.

Kliemannslandvideo: https://youtu.be/sF-ZqUxCdno

## BoM
Folgende Bauteile wurden verwendet:
* ESP32
* Schalter
* Batteriehalter (mit Vorsicht zu genießen)
* Servo

Eine Liste mit Links ist in der Beschreibung vom Video: https://youtu.be/QvPBMdb_oJw

## Entwicklungsumgebung
Als Entwicklungsumgebung wird platformio genutzt, da die Entwicklung deutlich komfortabler ist als mit der Arduino Entwicklungsumgebung. Außerdem ist das Teilen von Projekten deutlich einfacher, da alle Abhängigkeiten sauber angegeben werden können. Ein guter Einstieg ist folgender Artikel: https://www.heise.de/ct/artikel/Mikrocontroller-bequem-programmieren-mit-PlatformIO-4403209.html

## Konfiguration
Die wichtigsten Konfigurationseinstellungen, wie z.B. die Zugangsdaten fürs Wlan, befinden sich in der Datei SystemConfig.h. Da diese bei jedem unterschiedlich sind, gibt es eine Beispieldatei namens SystemConfig.h.example. Diese muss kopiert und dann angepasst werden.

Der ESP erstellt ein Wlan, mti dem man sich verbinden muss. Anschließend ruft man http://192.168.4.1 auf um die Weboberfläche zu öffnen.

Auch hier sei noch einmal gesagt, dass gerne nachgefragt werden kann, falls etwas unklar sein sollte.

## Hinweis
Ich übernehme keinerlei Verantwortung für Schäden, die im Zusammenhang mit diesem Projekt entstehen. Die Benutzung geschieht auf eigene Gefahr.

## Entwicklung
### Befehl zum Konvertieren der HTML Seite
```bash
echo "#include <Arduino.h>\nString mainPage = \"$(minify --html-keep-document-tags page.html | sed 's/\"/\\\"/g' | sed '$!s/$/\\/')\";" > ./src/page.html.h