#include <iostream>
// Основной код находится в заголовочном файле functions.h
#include "functions.h"

int main()
{
	// Устанавливаем возможность обрабатывать русский язык для консоли
	setlocale(LC_ALL, "Russian");

	// Считываем выражение со строки консоли
	std::string stroke;
	std::cin >> stroke;

	// Считаем и выводим результат
	std::cout << "Result: " << func::calculate(stroke) << '\n';
}