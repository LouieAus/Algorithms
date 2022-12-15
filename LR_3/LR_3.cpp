#include <iostream>

// Функция проверяет, подходит ли число
// Число состоит из чисел 3^a, 5^b, 7^c
// Значит, необходимо проверить тот факт, что число состоит только из произведений чисел 3, 5, 7
bool check_divisibility(int number) {
	bool divide = true;
	while (divide) {
		divide = false;
		// Максимально возможно делим число на 3
		if (number % 3 == 0) {
			number /= 3;
			divide = true;
		}
		// Максимально возможно делим число на 5
		if (number % 5 == 0) {
			number /= 5;
			divide = true;
		}
		// Максимально возможно делим число на 7
		if (number % 7 == 0) {
			number /= 7;
			divide = true;
		}
	}
	// Если в конце осталась только 1, значит число состоит из произведений чисел 3, 5, 7
	if (number == 1)
		return true;
	else return false;
}

int main() {
	unsigned int x;
	// Вводим число x
	std::cin >> x;
	std::cout << "Numbers: " << '\n';

	// Проверяем числа от 1 до X
	for (int curr_x = 1; curr_x < x; curr_x++) {
		if (check_divisibility(curr_x))
			std::cout << curr_x << '\n';
	}
}