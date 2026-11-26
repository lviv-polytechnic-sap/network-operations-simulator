// Signal Analyzer — Команда 2
// Класифікує навчальні вимірювання якості зв'язку як GOOD, DEGRADED або CRITICAL.
// Збірка: g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// ---------- Ліміти та порогові значення ----------

const int MAX_MEASUREMENTS = 100;

const int MIN_DEVICE_ID = 1;
const int MAX_DEVICE_ID = 9999;
const int MIN_SIGNAL = -150;
const int MAX_SIGNAL = 0;
const int MIN_LATENCY = 0;
const int MAX_LATENCY = 5000;
const double MIN_LOSS = 0.0;
const double MAX_LOSS = 100.0;

const int CRITICAL_SIGNAL = -100;
const int CRITICAL_LATENCY = 300;
const double CRITICAL_LOSS = 20.0;
const int DEGRADED_SIGNAL = -85;
const int DEGRADED_LATENCY = 150;
const double DEGRADED_LOSS = 5.0;

const int MENU_EXIT = 0;
const int MENU_MAX = 5;
const int INVALID_CHOICE = -1;
const int NOT_FOUND = -1;

// ---------- Модель даних ----------

struct SignalMeasurement {
    int deviceId;
    int signalLevel;
    int latencyMs;
    double packetLossPercent;
};

struct SignalAverages {
    double avgSignalLevel;
    double avgLatencyMs;
    double avgPacketLossPercent;
};

enum class SignalClass { GOOD, DEGRADED, CRITICAL };

// ---------- Input ----------

void printMenu() {
    std::cout << "\n=== Signal Analyzer ===\n"
              << "1. Додати вимірювання\n"
              << "2. Показати всі вимірювання з класом\n"
              << "3. Середні значення\n"
              << "4. Найгірше вимірювання\n"
              << "5. Повний звіт\n"
              << "0. Вихід\n"
              << "Ваш вибір: ";
}

// Повертає номер пункту меню або INVALID_CHOICE, якщо введено не одне ціле число.
// Кінець введення (EOF) трактується як вихід, щоб програма не зациклилась.
int readMenuChoice() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return MENU_EXIT;
    }
    std::istringstream input(line);
    int choice = 0;
    std::string extra;
    if (!(input >> choice) || (input >> extra)) {
        return INVALID_CHOICE;
    }
    return choice;
}

// Зчитує рядок із чотирма числами. Повертає false, якщо чисел не чотири
// або є зайві символи; у цьому випадку стан програми не змінюється.
bool readMeasurement(SignalMeasurement& measurement) {
    std::cout << "Введіть deviceId signalLevel latencyMs packetLossPercent: ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        return false;
    }
    std::istringstream input(line);
    std::string extra;
    if (!(input >> measurement.deviceId >> measurement.signalLevel
                >> measurement.latencyMs >> measurement.packetLossPercent)
        || (input >> extra)) {
        return false;
    }
    return true;
}

// ---------- Validation ----------

// Формує текст помилки з констант, щоб він залишався правильним після зміни меж.
std::string rangeMessage(const std::string& fieldName, double minValue, double maxValue) {
    std::ostringstream message;
    message << fieldName << " має бути від " << minValue << " до " << maxValue;
    return message.str();
}

// Перевіряє діапазони всіх полів. Якщо поле некоректне, записує пояснення в errorMessage.
bool validateMeasurement(const SignalMeasurement& measurement, std::string& errorMessage) {
    if (measurement.deviceId < MIN_DEVICE_ID || measurement.deviceId > MAX_DEVICE_ID) {
        errorMessage = rangeMessage("deviceId", MIN_DEVICE_ID, MAX_DEVICE_ID);
        return false;
    }
    if (measurement.signalLevel < MIN_SIGNAL || measurement.signalLevel > MAX_SIGNAL) {
        errorMessage = rangeMessage("signalLevel", MIN_SIGNAL, MAX_SIGNAL);
        return false;
    }
    if (measurement.latencyMs < MIN_LATENCY || measurement.latencyMs > MAX_LATENCY) {
        errorMessage = rangeMessage("latencyMs", MIN_LATENCY, MAX_LATENCY);
        return false;
    }
    if (measurement.packetLossPercent < MIN_LOSS || measurement.packetLossPercent > MAX_LOSS) {
        errorMessage = rangeMessage("packetLossPercent", MIN_LOSS, MAX_LOSS);
        return false;
    }
    return true;
}

// ---------- State ----------

// Додає вимірювання, лише якщо в масиві є місце. Індекс завжди < MAX_MEASUREMENTS.
bool addMeasurement(SignalMeasurement measurements[], int& measurementCount,
                    const SignalMeasurement& measurement) {
    if (measurementCount >= MAX_MEASUREMENTS) {
        return false;
    }
    measurements[measurementCount] = measurement;
    measurementCount++;
    return true;
}

// ---------- Processing ----------

// Правила перевіряються саме в такому порядку: спочатку CRITICAL, потім DEGRADED.
SignalClass classifySignal(const SignalMeasurement& measurement) {
    if (measurement.signalLevel < CRITICAL_SIGNAL || measurement.latencyMs > CRITICAL_LATENCY
        || measurement.packetLossPercent > CRITICAL_LOSS) {
        return SignalClass::CRITICAL;
    }
    if (measurement.signalLevel < DEGRADED_SIGNAL || measurement.latencyMs > DEGRADED_LATENCY
        || measurement.packetLossPercent > DEGRADED_LOSS) {
        return SignalClass::DEGRADED;
    }
    return SignalClass::GOOD;
}

std::string classToText(SignalClass signalClass) {
    switch (signalClass) {
        case SignalClass::CRITICAL:
            return "CRITICAL";
        case SignalClass::DEGRADED:
            return "DEGRADED";
        case SignalClass::GOOD:
            return "GOOD";
    }
    return "UNKNOWN";
}

// Чим більше число, тим гірший клас: GOOD = 0, DEGRADED = 1, CRITICAL = 2.
int severityRank(SignalClass signalClass) {
    switch (signalClass) {
        case SignalClass::CRITICAL:
            return 2;
        case SignalClass::DEGRADED:
            return 1;
        case SignalClass::GOOD:
            return 0;
    }
    return 0;
}

// Викликається лише при measurementCount > 0, тому ділення на нуль неможливе.
SignalAverages calculateAverages(const SignalMeasurement measurements[], int measurementCount) {
    SignalAverages averages = {0.0, 0.0, 0.0};
    if (measurementCount <= 0) {
        return averages;
    }
    for (int i = 0; i < measurementCount; i++) {
        averages.avgSignalLevel += measurements[i].signalLevel;
        averages.avgLatencyMs += measurements[i].latencyMs;
        averages.avgPacketLossPercent += measurements[i].packetLossPercent;
    }
    averages.avgSignalLevel /= measurementCount;
    averages.avgLatencyMs /= measurementCount;
    averages.avgPacketLossPercent /= measurementCount;
    return averages;
}

// Повертає індекс найгіршого вимірювання або NOT_FOUND для порожнього масиву.
// Гірший клас важливіший; за однакового класу гірше те, де більші втрати;
// за повної рівності залишається вимірювання, введене раніше.
int findWorst(const SignalMeasurement measurements[], int measurementCount) {
    if (measurementCount <= 0) {
        return NOT_FOUND;
    }
    int worstIndex = 0;
    for (int i = 1; i < measurementCount; i++) {
        int currentRank = severityRank(classifySignal(measurements[i]));
        int worstRank = severityRank(classifySignal(measurements[worstIndex]));
        bool worseClass = currentRank > worstRank;
        bool sameClassMoreLoss = currentRank == worstRank
            && measurements[i].packetLossPercent > measurements[worstIndex].packetLossPercent;
        if (worseClass || sameClassMoreLoss) {
            worstIndex = i;
        }
    }
    return worstIndex;
}

// Причина — перше правило, яке спрацювало для класу вимірювання,
// у порядку перевірки: рівень сигналу, затримка, втрати.
std::string explainReason(const SignalMeasurement& measurement) {
    std::ostringstream reason;
    SignalClass signalClass = classifySignal(measurement);
    int signalLimit = DEGRADED_SIGNAL;
    int latencyLimit = DEGRADED_LATENCY;
    double lossLimit = DEGRADED_LOSS;
    if (signalClass == SignalClass::CRITICAL) {
        signalLimit = CRITICAL_SIGNAL;
        latencyLimit = CRITICAL_LATENCY;
        lossLimit = CRITICAL_LOSS;
    } else if (signalClass == SignalClass::GOOD) {
        return "усі показники в межах норми";
    }

    if (measurement.signalLevel < signalLimit) {
        reason << "рівень сигналу " << measurement.signalLevel << " < " << signalLimit;
    } else if (measurement.latencyMs > latencyLimit) {
        reason << "затримка " << measurement.latencyMs << " мс > " << latencyLimit;
    } else {
        reason << "втрати " << measurement.packetLossPercent << "% > " << lossLimit;
    }
    return reason.str();
}

// ---------- Report ----------

void printMeasurement(const SignalMeasurement& measurement) {
    std::cout << "device " << measurement.deviceId
              << " | signal " << measurement.signalLevel << " dBm"
              << " | latency " << measurement.latencyMs << " ms"
              << " | loss " << measurement.packetLossPercent << "%"
              << " | " << classToText(classifySignal(measurement)) << "\n";
}

void printMeasurements(const SignalMeasurement measurements[], int measurementCount) {
    if (measurementCount == 0) {
        std::cout << "Немає даних.\n";
        return;
    }
    for (int i = 0; i < measurementCount; i++) {
        std::cout << i + 1 << ". ";
        printMeasurement(measurements[i]);
    }
}

void printAverages(const SignalAverages& averages) {
    std::cout << std::fixed << std::setprecision(1)
              << "Середній signalLevel: " << averages.avgSignalLevel << " dBm\n"
              << "Середній latencyMs: " << averages.avgLatencyMs << " ms\n"
              << "Середній packetLossPercent: " << averages.avgPacketLossPercent << "%\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout << std::setprecision(6);
}

void printWorst(const SignalMeasurement& measurement, const std::string& reason) {
    std::cout << "Найгірше вимірювання: ";
    printMeasurement(measurement);
    std::cout << "Причина: " << reason << "\n";
}

void printReport(const SignalMeasurement measurements[], int measurementCount) {
    std::cout << "=== Звіт ===\n"
              << "Кількість вимірювань: " << measurementCount << " з " << MAX_MEASUREMENTS << "\n";
    if (measurementCount == 0) {
        std::cout << "Немає даних для аналізу.\n";
        return;
    }

    int goodCount = 0;
    int degradedCount = 0;
    int criticalCount = 0;
    for (int i = 0; i < measurementCount; i++) {
        switch (classifySignal(measurements[i])) {
            case SignalClass::GOOD:
                goodCount++;
                break;
            case SignalClass::DEGRADED:
                degradedCount++;
                break;
            case SignalClass::CRITICAL:
                criticalCount++;
                break;
        }
    }
    std::cout << "GOOD: " << goodCount << ", DEGRADED: " << degradedCount
              << ", CRITICAL: " << criticalCount << "\n";

    printAverages(calculateAverages(measurements, measurementCount));
    int worst = findWorst(measurements, measurementCount);
    printWorst(measurements[worst], explainReason(measurements[worst]));
}

// ---------- Main ----------

int main() {
    SignalMeasurement measurements[MAX_MEASUREMENTS];
    int measurementCount = 0;
    int choice = INVALID_CHOICE;

    do {
        printMenu();
        choice = readMenuChoice();
        if (choice < MENU_EXIT || choice > MENU_MAX) {
            std::cout << "Невідомий пункт меню. Введіть число від " << MENU_EXIT
                      << " до " << MENU_MAX << ".\n";
            continue;
        }

        switch (choice) {
            case 1: {
                if (measurementCount >= MAX_MEASUREMENTS) {
                    std::cout << "Сховище заповнене: максимум " << MAX_MEASUREMENTS << " вимірювань.\n";
                    break;
                }
                SignalMeasurement measurement;
                std::string errorMessage;
                if (!readMeasurement(measurement)) {
                    std::cout << "Некоректне введення: потрібно чотири числа через пробіл.\n";
                } else if (!validateMeasurement(measurement, errorMessage)) {
                    std::cout << "Помилка: " << errorMessage << ". Вимірювання не додано.\n";
                } else if (addMeasurement(measurements, measurementCount, measurement)) {
                    std::cout << "Додано. Клас: " << classToText(classifySignal(measurement)) << "\n";
                }
                break;
            }
            case 2:
                printMeasurements(measurements, measurementCount);
                break;
            case 3:
                if (measurementCount == 0) {
                    std::cout << "Немає даних для обчислення.\n";
                } else {
                    printAverages(calculateAverages(measurements, measurementCount));
                }
                break;
            case 4: {
                int worst = findWorst(measurements, measurementCount);
                if (worst == NOT_FOUND) {
                    std::cout << "Немає даних.\n";
                } else {
                    printWorst(measurements[worst], explainReason(measurements[worst]));
                }
                break;
            }
            case 5:
                printReport(measurements, measurementCount);
                break;
        }
    } while (choice != MENU_EXIT);

    std::cout << "Завершення роботи.\n";
    return 0;
}
