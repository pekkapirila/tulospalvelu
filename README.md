# Pekka Pirilän tulospalveluohjelma (sports time keeping program)

## Briefly in Finnish / Lyhyesti suomeksi
Tässä ovat Pekka Pirilän (1945-2015) tulospalveluohjelmien lähdekoodit. Mukana 
ovat kaikki muunnelmat: teksti- ja Windows-versio sekä henkilökohtaisesta että 
viestiohjelmasta. Pekan perhe julkaisi lähdekoodin avoimen lähdekoodin
GPLv3-lisenssin alaiseksi. Lisenssi tarkoittaa karkeasti sitä, että kuka vain 
voi muuttaa ja käyttää ohjelmaa vapaasti kaikissa tilanteissa. Ohjelman
käyttöön liittyviä palveluita ja jopa kopioita ohjelmasta saa myydä, mutta 
muutetun version levittäjä sitoutuu julkaisemaan versiostaan myös lähdekoodin
ja ostaja saa jälleen tehdä kopiollaan mitä haluaa.

Valmiiksi käännetyt ja paketoidut versiot sekä ohjeet löytyvät edelleen
osoitteesta http://www.pirila.fi/ohj/index.html.

Pekan perheen toiveena on, että ohjelmasta olisi hyötyä urheiluyhteisölle vielä
vuosien ajan.

## Description
This is the source code of a suite of sports time keeping programs Pekka Pirilä
(1945-2015) started developing in around 1986. The program originally
specialized in orienteering, but was later amended to support additional
sports. The user interface is in Finnish and there are Finnish language
variables and comments throughout the source code. The source code is released 
under GPLv3. More information in Finnish at http://www.pirila.fi/ohj/index.html.

## Console programs

### Required tools
The console program's project files are for Visual Studio. They were
successfully compiled with Visual Studio Express 2013 for Windows Desktop,
but Visual Studio 2010 and anything newer can probably be made to work. The 
language is probably compliant with C++03.

### Compile
1. Open TPsource\V52\VS\Libs\tputilv2.sln
2. Compile by pressing F7
	1. Two new folders will be created next to TPsource: vc10 and TPexe
3. Open TPsource\V52\VS\Hk\HkMaali520.sln
4. Compile by pressing F7
	1. A stand-alone executable TPexe\Hk\V521\HkMaali.exe is built and ready
	to use
5. To build the relay version, repeat with TPsource\V52\VS\V\JukMaali520.sln

## Windows programs

### Required tools
The Windows programs are made with
[Embarcadero C++ Builder](https://www.embarcadero.com/app-development-tools-store/cbuilder).
Version 10.1 Berlin was successfully used to compile and run the program.

[SecureBridge 7.1 for RAD Studio 10.1 Berlin](https://www.devart.com/sbridge/download.html)
is an add-on that is required to compile and run the program. NOTE: At least 
in Windows 10 you must edit one of SecureBridge's header files to be able to 
compile this program. Open 
"Program Files (x86)\Devart\SecureBridge for RAD Studio 10\Include\Win32\ScSSHSocket.hpp"
and change Winapi::Winsock::PSockAddrIn to Winapi::Winsock2::PSockAddrIn.

### Compile
1. Open TPsource\V52\RADStudio10\DBboxm-XE.cbproj
2. Right-click on DBboxm-XE.lib in Project Manager and select Make
3. Open TPsource\V52\RADStudio10\Tputil-XE.cbproj
4. Right-click on Tputil-XE.lib in Project Manager and select Make
5. Open TPsource\V52\RADStudio10\HkKisaWin.cbproj and Run to start the
program for individual competitions
6. Open TPsource\V52\RADStudio10\ViestiWin.cbproj and Run to start the 
relay program

#### Heap errors during compilation
If you get heap errors with linking, you can try these things

1. http://stackoverflow.com/questions/28929516/c-builder-xe7-lme288-error
	1. Run command prompt as Administrator.
	2. Type (without quotes) "bcdedit /set IncreaseUserVa 3072"
	3. Reboot computer.
2. Empty %TEMP%, reboot, try again, repeat
3. Run C++ Builder as admin
