#pragma once
#include <string>
#include <atomic>
#include <mutex>

void gotoxy(int x, int y);
void imprimir_en_linea(int y, const std::string& texto);
void mostrar_marquesina();

extern std::atomic<bool> ejecutando;
extern std::mutex mtx;
