# Spezifikationen des BG Technik Carrera Projektes 2012-2013 #

## 1. Links ##

[Quellcode der Rennsoftware](https://github.com/schattenan/EschwegeCarreraProjectInC)

[Alte Dokumentation (2012)](http://swege.github.io/Carrera/index.html)

[Neue Dokumentation (2013)]()


## 2. Spezifikation der UE9-Ports ##

![](http://swege.github.io/Carrera/_images/UE9_Ports.png)

| Funktion | Portnummer (physikalisch)| Portnummer Software| 
| :-----:  | :-------:	| :---: | 
| *Ampel*              |||  
| Rot 1 | FIO1          | 0 | 
| Rot 2 | FIO2          | 1 |
| Rot 3 | FIO3          | 2 |
| Grün  | FIO4          | 3 |  
| ___________________________________________________________|||
| *Lichtschranken*   |||
| Track 1 | EIO1 | 8 |
| Track 2 | EIO2 | 9 |
| Track 3 | EIO3 | 10 |
| Track 4 | EIO4 | 11 |  
| ___________________________________________________________|||
| *Streckenfreigabe* |||  
| Track 1 | EIO4 | 12 |
| Track 2 | EIO5 | 13 |
| Track 3 | EIO6 | 14 |
| Track 4 | EIO7 | 15 |


## 3. Spezifikationen des UE9-Boards ##

* IP-Adresse: 192.168.1.207
* Port : 52360

## 4. Spezifikation der CSV-Datei ##

| Zeile | Allgemeiner Aufbau ||||  
| :---: | :------:	| :------:	| :------:	| :------:	|  
| _ | _ | _ | _ | _ |
| 1 |string_Gamemode | | | |  
| 2 |int_NumberOfPlayers | | | | |  
| 3|int_NumberOfRounds | | | | 
| 4|string_NameOfPlayer1 | string_NameOfPlayer2 | string_NameOfPlayer3 | string_NameOfPlayer4 |  
| 1+4|string_Round_1_Player_1 | string_Round_1_Player_2 | string_Round_1_Player_3 | string_Round_1_Player_4 |  
| 2+4|string_Round_2_Player_1 | string_Round_2_Player_2 | string_Round_2_Player_3 | string_Round_2_Player_4 |  
| 3+4|string_Round_3_Player_1 | string_Round_3_Player_2 |   string_Round_3_Player_3 | string_Round_3_Player_4 |  
| | …|…|…|…| 
| N+4|string_Round_N_Player_1 | string_Round_N_Player_2 | string_Round_N_Player_3 | string_Round_N_Player_4 | 
| N+5|string_RankOfPlayer1 |  string_RankOfPlayer2 |  string_RankOfPlayer3 |  string_RankOfPlayer4 |  
| N+6|int_BestRoundOfPlayer1 | int_BestRoundOfPlayer2 | int_BestRoundOfPlayer3 | int_BestRoundOfPlayer4 |  
| N+7|string_TotalTimeOfPlayer1 | string_TotalTimeOfPlayer2 | string_TotalTimeOfPlayer3 | string_TotalTimeOfPlayer4 |  
| N+8|"End" | | | |  

* N = Number of Rounds
* "End" = End Sequence

## 5. Verwendete Software & Tools ##

### 5.1 Entwicklung ###

* Visual Studio C++ 2010
* Arduino IDE
* Editra
* Notepad++

### 5.2 Backup & Software Versioning ###
* Dropbox 
* Github

### 5.3 Runtime Software ###

* Windows XP
* XAMP (für die Urkunde)
* Opera (für den Druck der Urkunde)

### 5.4 Erstellung der Doku ###

* LibreOffice / OpenOffice.org
* Microsoft Office
* Evernote
* MultiMarkdown Composer
* Struktogrammeditor

## 6. Softwareentwickler ##

* Nils Winkelbach 
	* Arduino
* Jan Abratis 
 	* Grafik der Urkunde 
* Björn Wittmann 
	* Rennprogramm
	* Logik der Urkunde
	* Dokumentation der Spezifikation
