Serwer serii danych [Projekt ZPR]

Autor:
Grzegorz Wojciechowski

Projekt zarządzany jest przez system kontroli wersji Git i przechowywany jest w repozytorium github:
https://github.com/grliszas14/DataTrace

*** Do uruchomienia wymagane są: ***
Qt w wersji 5
Python2.7
MariaDB (or MySQL)
(sudo apt install mariadb-client-core-10.1)
MySQLdb - python-mysqldb

*** Uruchomienie programu przeglądającego: ***
Linux:

cd DataTrace
make
./DataTrace

Windows:
//TODO

*** Uruchomienie i przetestowanie serwera: ***

python sciezka_do_projektu/Server/server.py

(przykładowe demony zbierają dane o temperaturze w dużych polskich miastach i wysyłają je do serwera, serwer zapisuje te dane do bazy danych)

*** Uruchomienie testów: ***

cd DataTrace
python sciezka_do_projektu/Unit_tests/main.py
