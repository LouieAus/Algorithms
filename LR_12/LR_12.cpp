#include <iostream>
#include <algorithm>
#include <cstdio>
#include "func.h"

// Для работы программы необходим файл хранящий список чисел array_in.txt,
// который находится в одной папке вместе с LR_12.cpp (main) и func.h

int main()
{
    setlocale(LC_ALL, "Rus");

    int S = 3;                                              // Размер отрезков (чанков)

    std::string input   = "array_in.txt";                   // Название файла с данными
    std::string A       = "A.txt";                          // Название для файла-помощника A
    std::string B       = "B.txt";                          // Название для файла-помощника B
    std::string C       = "C.txt";                          // Название для файла-помощника C
    std::string D       = "D.txt";                          // Название для файла-помощника D
    file::clearFile(A);                                     // Очищаем файл А
    file::clearFile(B);                                     // Очищаем файл B
    file::clearFile(C);                                     // Очищаем файл C
    file::clearFile(D);                                     // Очищаем файл D
    file::clearFile("merge.txt");                           // Очищаем временный файл, в который записываются слитые отрезки

    std::string curr_filename = A;                          // Назначаем временному файлу-помощнику имя файла А
    unsigned int lines_amount = file::countLines(input);    // Количество строк в файле с данными
    unsigned int curr_index = 0;                          // Индекс, отвечащий за начало каждого отрезка

    if (lines_amount != 0) {    // Если файл с данными не пустой
        do {
            std::vector<float> numsVec = file::readChunk(input, curr_index, S); // Считываем отрезок (чанк)
            std::sort(numsVec.begin(), numsVec.end());                          // Сортируем отрезок
            file::writeChunk(curr_filename, numsVec);                           // Записываем в временный файл-помощник
            if (curr_filename == A)                                             // Меняем временный файл-помощник на другой
                curr_filename = B;
            else
                curr_filename = A;
            curr_index += S;                                                    // Сдвигаем индекс начала на следующий отрезок
        } while ((lines_amount - 1) >= curr_index);     // Повторяем, пока не выйдем за пределы количества строк в файле с данными
    }

    int size                = S;    // Копируем переменную размера отрезков
    std::string input_1     = A;    // Название для файла-помощника input_1
    std::string input_2     = B;    // Название для файла-помощника input_2
    std::string curr_output = C;    // Название для временного файла-помощника

    while (true) {
        if (file::countLines(input_2) == 0) { // Если файл input_2 пустой (т.е. сортировка окончена),
            break;                            // то выходим из цикла
        }
        while (true) {
            std::string mergedFile = file::cutMergeVec(input_1, input_2, size);         // Сливаем два вектора (отрезка) в один файл
            file::writeChunk(curr_output, mergedFile);                                  // В временный файл записываем данных слитых отрезков

            if (curr_output == A)                                                       // Меняем временный файл
                curr_output = B;
            else if (curr_output == B)
                curr_output = A;
            else if (curr_output == C)
                curr_output = D;
            else if (curr_output == D)
                curr_output = C;

            if (size > file::countLines(input_1))                               // Если вышли за пределы количества строк в input_1,
                break;                                                          // то выходим из цикла
        }
        std::string mergedFile = file::cutMergeVec(input_1, input_2, size);
        file::writeChunk(curr_output, mergedFile);

        size = size * 2;            // Увеличиваем размер отрезков (т.е. количество чанков) вдвое
        if (input_1 == A) {         // Меняем input_1, input_2, curr_output
            input_1     = C;
            input_2     = D;
            curr_output = A;
        }
        else {
            input_1     = A;
            input_2     = B;
            curr_output = C;
        }

    }

    // Удаляем файлы-помощники и файл, в котором хранится результат переименовываем в result.txt
    remove("result.txt");
    remove(B.c_str());
    remove(D.c_str());
    remove("merge.txt");
    if (input_1 == A) {
        if (rename(A.c_str(), "result.txt"))
            std::cout << "Ошибка переименовывания файла\nРезультат хранится в result.txt" << '\n';
        remove(C.c_str());
    }
    else {
        if (rename(C.c_str(), "result.txt"))
            std::cout << "Ошибка переименовывания файла. Результат хранится в result.txt" << '\n';
        remove(A.c_str());
    }

    // Результат сортировки хранится в result.txt
}