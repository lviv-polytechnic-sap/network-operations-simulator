# Сервіс «Реєстр пристроїв» (services/device-registry) — Команда 1

Навчальний консольний сервіс C++17 для обліку, валідації, пошуку та збору статистики мережевих пристроїв.

## Інструкція зі збірки та запуску
```bash
cd services/device-registry
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app
./app
