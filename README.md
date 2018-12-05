Serwer serii danych [Projekt ZPR]

Autorzy:
Grzegorz Wojciechowski
Michał Skarzyński

*** Uruchomienie programu przeglądającego: ***
Linux:

cd DataTrace
make
./DataTrace

Windows:


*** Uruchomienie i przetestowanie serwera: ***

python sciezka_do_projektu/Server/server.py

w osobnym terminalu
python sciezka_do_projektu/Daemons/get_temperature.py

(przykładowy demon zbiera dane o temperaturze w Warszawie i wysyła do serwera, serwer w konsoli wyświetla odebraną daną (timestamp, temperatura))

*** Uruchomienie testów: ***

cd DataTrace
python sciezka_do_projektu/Unit_tests/main.py
