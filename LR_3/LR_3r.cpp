#include <iostream>
#include <cmath>

// Функция поиска максимальной степени для числа x
int findMaxDegree(int x, int num) {
    int curr_num = 1;
    int count = 0;
    while (curr_num <= x) {
        curr_num *= num;
        count++;
    }
    return (count - 1);
}

int main()
{
    const int x = 789855;

    int max_3_deg = findMaxDegree(x, 3);  // Максимально возможная степень 3
    int max_5_deg = findMaxDegree(x, 5);  // Максимально возможная степень 5
    int max_7_deg = findMaxDegree(x, 7);  // Максимально возможная степень 7

    for (int deg_3 = 0; deg_3 <= max_3_deg; deg_3++) {
        for (int deg_5 = 0; deg_5 <= max_5_deg; deg_5++) {
            for (int deg_7 = 0; deg_7 <= max_7_deg; deg_7++) {

                int num_3 = pow(3, deg_3);                  // Возведение в текущую степень 3
                int num_5 = pow(5, deg_5);                  // Возведение в текущую степень 5
                int num_7 = pow(7, deg_7);                  // Возведение в текущую степень 7
                unsigned int res = num_3 * num_5 * num_7;   // Вычисление числа на проверку

                if (res <= x)                               // Если оно меньше или равно x,
                    std::cout << res << '\n';               // то выводим его
                else break;                                 // Иначе степени 7 больше текущей не проверяем
                    
            }
        }
    }
}