#include "Marquesina.h"
#include <windows.h>
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <string>
#include <windows.h>
#include <thread>
#include <mutex>
#include <algorithm>

std::atomic<bool> ejecutando(true);
std::mutex mtx;

void gotoxy(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void imprimir_en_linea(int y, const std::string& texto) {
    std::lock_guard<std::mutex> lock(mtx);
    gotoxy(0, y);
    std::cout << std::string(80, ' ') << std::flush;
    gotoxy(0, y);
    std::cout << texto << std::flush;
}

void mostrar_marquesina() {
    std::string mensaje = "  Bienvenido al sistema de gestion de turnos medicos  ";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (ejecutando) {
        std::lock_guard<std::mutex> lock(mtx);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        COORD cursorActual = csbi.dwCursorPosition;

        COORD coord = {0, 0};
        SetConsoleCursorPosition(hConsole, coord);
        DWORD charsWritten;
        FillConsoleOutputCharacter(hConsole, ' ', 80, coord, &charsWritten);
        SetConsoleCursorPosition(hConsole, coord);
        std::cout << mensaje << std::flush;

        SetConsoleCursorPosition(hConsole, cursorActual);

        std::rotate(mensaje.begin(), mensaje.begin() + 1, mensaje.end());
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}