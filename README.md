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

w osobnym terminalu
python sciezka_do_projektu/Daemons/get_temperature.py

(przykładowy demon zbiera dane o temperaturze w Warszawie i wysyła do serwera, serwer w konsoli wyświetla odebraną daną (timestamp, temperatura))

*** Uruchomienie testów: ***

cd DataTrace
python sciezka_do_projektu/Unit_tests/main.py
