#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace func {
	// Функция проверяет, является ли скобка открывающей (выводит 1), закрывающей (выводит 2) или не является скобкой (выводит 0)
	int is_skobe(char symbol) {
		if (symbol == '{' || symbol == '(' || symbol == '[')		return 1;
		else if (symbol == '}' || symbol == ')' || symbol == ']')	return 2;
		else														return 0;
	}

	// Функция проверяет, соответствует ли выражение правилу постановки скобок (Лабораторная работа 1)
	bool check_braces(std::string& expression) {
		std::vector <char> stack;

		int i = 0;
		while (i != expression.length()) {
			char s = expression[i];
			if (is_skobe(s) == 1) stack.push_back(s);
			else if (is_skobe(s) == 2) {
				if (stack.size() != 0) {
					if ((s == '}' && stack[stack.size() - 1] == '{') ||
						(s == ')' && stack[stack.size() - 1] == '(') ||
						(s == ']' && stack[stack.size() - 1] == '[')) {
						stack.pop_back();
					}
					else break;
				}
				else {
					stack[0] = '1';
					break;
				}
			}
			i++;
		}

		if (stack.size() != 0)
			return false;
		else return true;
	}
	
	// Структура для хранения индекса в выражении и скобки
	struct brace {
		unsigned int index;
		char brace;
	};

	// Структура для хранения индекса и символа
	struct symbol {
		unsigned int index;
		std::string symb;
	};

	// Функция берет подстроку из строки
	std::string take_string(std::string &str, unsigned int begin_index, unsigned int end_index) {
		std::string new_str = "";
		for (unsigned int i = begin_index; i <= end_index; i++)
			new_str += str[i];
		return new_str;
	}

	// Функия проверяет, может ли символ быть частью числа
	bool check_number(char symbol) {
		if (symbol == '0' ||
			symbol == '1' ||
			symbol == '2' ||
			symbol == '3' ||
			symbol == '4' ||
			symbol == '5' ||
			symbol == '6' ||
			symbol == '7' ||
			symbol == '8' ||
			symbol == '9' ||
			symbol == '.' ||
			symbol == 'm')
			return 1;
		else return 0;
	}

	// Функция меняет знак "-" в числе на символ "m" и обратно
	// (необходимо, если одно из чисел при подсчете простого выражения отрицательное)
	std::string change_minus(std::string number) {
		if (number[0] == 'm') {
			int minus_count = 0;
			for (int j = 0; j < number.size(); j++) {
				if (number[j] == 'm')
					minus_count++;
			}
			if (minus_count % 2 == 0) {
				number.erase(0, minus_count);
			}
			else {
				number.erase(0, minus_count);
				number.insert(0, "-");
			}
		}
		else if (number[0] == '-')
			number[0] = 'm';
		return number;
	}

	// Функция ищет число до (location = false) или после (location = true) оператора
	symbol find_number(std::string& expr, unsigned int index, bool location) {
		if (!location) {
			unsigned int begin_index = index;
			while (check_number(expr[begin_index - 1])) {
				begin_index--;
			}
			std::string num_in_str = take_string(expr, begin_index, index - 1);
			symbol number = { begin_index, change_minus(num_in_str) };
			return number;
		}
		else {
			unsigned int end_index = index;
			while (check_number(expr[end_index + 1])) {
				end_index++;
			}
			std::string num_in_str = take_string(expr, index + 1, end_index);
			symbol number = { end_index, change_minus(num_in_str) };
			return number;
		}
	}

	// Функция вычисляет простое выражение, состоящее из двух чисел и оператора
	void exec_operation(std::string& expr, unsigned int operator_index) {
		unsigned int i = operator_index;
		symbol a = find_number(expr, i, 0);		// Ищем число до оператора
		symbol b = find_number(expr, i, 1);		// Ищем число после оператора
		// Переводим числа a и b из строки в float
		float number_a;
		std::istringstream(a.symb) >> number_a; 
		float number_b;
		std::istringstream(b.symb) >> number_b;

		float number = 0;
		// Вычисляем значение согласно оператору
		if (expr[i] == '*') {
			number = number_a * number_b;
		}
		else if (expr[i] == '/') {
			if (number_b == 0) {
				std::cout << "Деление на 0 невозможно!" << '\n';
				expr = "error";
				return;
			}
			number = number_a / number_b;
		}
		else if (expr[i] == '+') {
			number = number_a + number_b;
		}

		// Переводим получившийся результат из float в строку
		std::stringstream stream;
		stream.precision(3);
		stream << std::fixed;
		stream << number;
		std::string str_number = stream.str();

		// Заменяем в подвыражении простое выражение на результат (число)
		expr.erase(a.index, (b.index - a.index + 1));
		expr.insert(a.index, change_minus(str_number));
	}

	// Функция отвечает за нахождение операторов *, /, -  по правилу последовательности вычислений
	void solve_expression(std::string& expr) {
		int i = 0;
		while (true) {
			if (expr[i] == '*' || expr[i] == '/') {
				// Если нашли * или /, то отправляем в функцию exec_operation()
				exec_operation(expr, i);
				i = 0;
			}
			i++;
			if (i >= (expr.size() - 1))
				break;
		}
		i = 0;
		while (true) {
			if (expr[i] == '+') {
				// Если нашли -, то отправляем в функцию exec_operation()
				exec_operation(expr, i);
				i = 0;
			}
			i++;
			if (i >= (expr.size() - 1))
				break;
		}
	}

	// Функция удаляет лишние пробелы в выражении
	void remove_spaces(std::string& expr) {
		int i = 0;
		while (true) {
			if (i == expr.size())
				break;
			if (expr[i] == ' ') {
				expr.erase(i, 1);
			}
			else i++;
		}
	}

	// Основная функция для вычисления результата выражения
	float calculate(std::string& expression) {
		// Убираем все лишние пробелы в выражении
		remove_spaces(expression);

		// Проверяем на правильность расстановки скобок
		if (!func::check_braces(expression)) {
			std::cout << "Неправильно расставлены скобки!" << '\n';
			return NULL;
		}

		// Вставляем скобки по бокам и убираем = (необходимо для дальнейшей работы программы)
		expression.insert(0, "(");
		expression[expression.size() - 1] = ')';

		// Заменяем минусы в программе на + или знак m, обозначающий отрицательное число
		int number_count = 0;
		for (int k = 0; k < expression.size(); k++) {
			if (check_number(expression[k]))
				number_count++;
			if (expression[k] == '-') {
				if (k > 0) {
					if (check_number(expression[k - 1]) || is_skobe(expression[k - 1]) == 2) {
						expression.erase(k, 1);
						expression.insert(k, "+m");
					}
					else {
						expression.erase(k, 1);
						expression.insert(k, "m");
					}
				}
				else {
					expression.erase(k, 1);
					expression.insert(k, "m");
				}
			}
		}

		// Проверяем на наличие чисел в программе
		if (number_count == 0) {
			std::cout << "Отсутствуют числа!" << '\n';
			return NULL;
		}
		
		std::vector<brace> stack;	// Вектор для стека скобок
		int i = 0;					// Счетчик текущего индекса (символа) в выражении
		while(true) {
			// Проверяем, достиг ли счетчик конца выражения
			if (i == expression.size())
				break;
			// Если нашли открывающуюся скобку, то закидываем в стек
			if (is_skobe(expression[i]) == 1) {
				brace brace_in_stack = { i, expression[i] };
				stack.push_back(brace_in_stack);
			}
			// Если нашли закрывающую скобку, то обрабатываем выражение
			else if (is_skobe(expression[i]) == 2) {
				// Берём подвыражение, которое содержится в паре скобок
				std::string subexpression = take_string(expression, stack[stack.size() - 1].index, i);
				// Решаем его с помощью функции solve_expression
				solve_expression(subexpression);
				// Если возвращается ошибка, то завершаем вызов функции calculate()
				if (subexpression == "error")
					return NULL;

				// Удаляем получивишеся скобки в результате подсчета выражения
				subexpression.erase(0, 1);
				subexpression.erase(subexpression.size() - 1, 1);

				// После подсчета заменяем подвыражение в выражении на получившийся результат (число)
				expression.erase(stack[stack.size() - 1].index, (i - stack[stack.size() - 1].index + 1));
				expression.insert(stack[stack.size() - 1].index, subexpression);
				// Очищаем стек
				stack.clear();
				// Обрабатываем получившееся выражение заново, с нулевого индекса
				i = -1;
			}
			i++;
		}
		// Если число отрицательное, то меняем "m" на "-"
		expression = change_minus(expression);
		// Переводим число из строковой величины в float
		float calc_number;
		std::istringstream(expression) >> calc_number;
		// Возвращаем получившееся число
		return calc_number;
	}
}