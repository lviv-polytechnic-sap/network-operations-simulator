#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

const int MAX_DEVICES = 100;

struct Device {
    int id;
    std::string name;
    std::string type;
    bool online;
};

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int findDeviceById(const Device devices[], int count, int id) {
    for (int i = 0; i < count; ++i) {
        if (devices[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool isValidType(const std::string& type) {
    return (type == "ROUTER" || type == "SERVER" || type == "SENSOR" || type == "TERMINAL");
}

bool validateDevice(const Device& dev, const Device devices[], int count, std::string& errorMsg) {
    if (dev.id <= 0) {
        errorMsg = "ID пристрою повинен бути додатним числом (> 0).";
        return false;
    }
    if (findDeviceById(devices, count, dev.id) != -1) {
        errorMsg = "Пристрій з ID " + std::to_string(dev.id) + " вже існує (дублікат).";
        return false;
    }
    if (dev.name.empty()) {
        errorMsg = "Назва пристрою не може бути порожньою.";
        return false;
    }
    if (!isValidType(dev.type)) {
        errorMsg = "Некоректний тип пристрою. Дозволені: ROUTER, SERVER, SENSOR, TERMINAL.";
        return false;
    }
    return true;
}

bool addDevice(Device devices[], int& count, const Device& dev) {
    if (count >= MAX_DEVICES) {
        std::cout << "[Помилка] Реєстр заповнений (максимум " << MAX_DEVICES << " пристроїв).\n";
        return false;
    }
    devices[count] = dev;
    count++;
    return true;
}

bool changeStatus(Device devices[], int count, int id, bool newStatus) {
    int index = findDeviceById(devices, count, id);
    if (index == -1) {
        return false;
    }
    devices[index].online = newStatus;
    return true;
}

void printDevices(const Device devices[], int count) {
    if (count == 0) {
        std::cout << "\n[Реєстр порожній]\n";
        return;
    }

    std::cout << "\n=====================================================================\n";
    std::cout << std::left 
              << std::setw(8)  << "ID" 
              << std::setw(25) << "Name" 
              << std::setw(15) << "Type" 
              << std::setw(10) << "Status" << "\n";
    std::cout << "---------------------------------------------------------------------\n";

    for (int i = 0; i < count; ++i) {
        std::cout << std::left 
                  << std::setw(8)  << devices[i].id 
                  << std::setw(25) << devices[i].name 
                  << std::setw(15) << devices[i].type 
                  << std::setw(10) << (devices[i].online ? "ONLINE" : "OFFLINE") << "\n";
    }
    std::cout << "=====================================================================\n";
}

void printStatistics(const Device devices[], int count) {
    int onlineCount = 0;
    int offlineCount = 0;
    int routerCount = 0, serverCount = 0, sensorCount = 0, terminalCount = 0;

    for (int i = 0; i < count; ++i) {
        if (devices[i].online) {
            onlineCount++;
        } else {
            offlineCount++;
        }

        if (devices[i].type == "ROUTER") routerCount++;
        else if (devices[i].type == "SERVER") serverCount++;
        else if (devices[i].type == "SENSOR") sensorCount++;
        else if (devices[i].type == "TERMINAL") terminalCount++;
    }

    std::cout << "\n=== СТАТИСТИКА РЕЄСТРУ ПРИСТРОЇВ ===\n";
    std::cout << "Всього пристроїв: " << count << " / " << MAX_DEVICES << "\n";
    std::cout << " - ONLINE:  " << onlineCount << "\n";
    std::cout << " - OFFLINE: " << offlineCount << "\n";
    std::cout << "За типами:\n";
    std::cout << " - ROUTER:   " << routerCount << "\n";
    std::cout << " - SERVER:   " << serverCount << "\n";
    std::cout << " - SENSOR:   " << sensorCount << "\n";
    std::cout << " - TERMINAL: " << terminalCount << "\n";
    std::cout << "====================================\n";
}

Device readDevice(const Device devices[], int count, bool& cancel) {
    Device dev;
    cancel = false;

    std::cout << "Введіть ID пристрою: ";
    if (!(std::cin >> dev.id)) {
        clearInputBuffer();
        std::cout << "[Помилка] ID повинен бути цілим числом.\n";
        cancel = true;
        return dev;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Введіть назву пристрою: ";
    std::getline(std::cin, dev.name);

    std::cout << "Введіть тип (ROUTER, SERVER, SENSOR, TERMINAL): ";
    std::cin >> dev.type;

    int statusInput;
    std::cout << "Введіть стан (1 - ONLINE, 0 - OFFLINE): ";
    if (!(std::cin >> statusInput) || (statusInput != 0 && statusInput != 1)) {
        clearInputBuffer();
        std::cout << "[Помилка] Стан повинен бути 1 або 0.\n";
        cancel = true;
        return dev;
    }
    dev.online = (statusInput == 1);

    std::string errorMsg;
    if (!validateDevice(dev, devices, count, errorMsg)) {
        std::cout << "[Помилка валідації] " << errorMsg << "\n";
        cancel = true;
    }

    return dev;
}

int main() {
    Device devices[MAX_DEVICES];
    int deviceCount = 0;
    int choice = 0;

    std::cout << "=========================================\n";
    std::cout << "  Network Operations Simulator: Team 1   \n";
    std::cout << "       Сервіс 'Реєстр пристроїв'         \n";
    std::cout << "=========================================\n";

    while (true) {
        std::cout << "\n--- ГОЛОВНЕ МЕНЮ ---\n";
        std::cout << "1. Додати пристрій\n";
        std::cout << "2. Знайти пристрій за ID\n";
        std::cout << "3. Змінити стан пристрою (ONLINE/OFFLINE)\n";
        std::cout << "4. Вивести всі пристрої\n";
        std::cout << "5. Вивести статистику\n";
        std::cout << "0. Вихід\n";
        std::cout << "Виберіть опцію: ";

        if (!(std::cin >> choice)) {
            clearInputBuffer();
            std::cout << "[Помилка] Введіть число від 0 до 5.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Завершення роботи програми.\n";
            break;
        }

        switch (choice) {
            case 1: {
                bool cancel = false;
                Device newDev = readDevice(devices, deviceCount, cancel);
                if (!cancel) {
                    if (addDevice(devices, deviceCount, newDev)) {
                        std::cout << "[Успіх] Пристрій успішно додано!\n";
                    }
                }
                break;
            }
            case 2: {
                int searchId;
                std::cout << "Введіть ID пристрою для пошуку: ";
                if (!(std::cin >> searchId)) {
                    clearInputBuffer();
                    std::cout << "[Помилка] Некоректний ID.\n";
                    break;
                }
                int index = findDeviceById(devices, deviceCount, searchId);
                if (index != -1) {
                    std::cout << "\n[Знайдено] ID: " << devices[index].id 
                              << " | Назва: " << devices[index].name 
                              << " | Тип: " << devices[index].type 
                              << " | Стан: " << (devices[index].online ? "ONLINE" : "OFFLINE") << "\n";
                } else {
                    std::cout << "[Інфо] Пристрій з ID " << searchId << " не знайдено.\n";
                }
                break;
            }
            case 3: {
                int targetId, newStatusInt;
                std::cout << "Введіть ID пристрою для зміни стану: ";
                if (!(std::cin >> targetId)) {
                    clearInputBuffer();
                    std::cout << "[Помилка] Некоректний ID.\n";
                    break;
                }
                if (findDeviceById(devices, deviceCount, targetId) == -1) {
                    std::cout << "[Помилка] Пристрій з ID " << targetId << " відсутній у реєстрі.\n";
                    break;
                }
                std::cout << "Введіть новий стан (1 - ONLINE, 0 - OFFLINE): ";
                if (!(std::cin >> newStatusInt) || (newStatusInt != 0 && newStatusInt != 1)) {
                    clearInputBuffer();
                    std::cout << "[Помилка] Некоректне значення стану (введіть 1 або 0).\n";
                    break;
                }
                changeStatus(devices, deviceCount, targetId, (newStatusInt == 1));
                std::cout << "[Успіх] Стан пристрою оновлено.\n";
                break;
            }
            case 4:
                printDevices(devices, deviceCount);
                break;
            case 5:
                printStatistics(devices, deviceCount);
                break;
            default:
                std::cout << "[Помилка] Некоректний вибір меню (0-5).\n";
                break;
        }
    }

    return 0;
}
