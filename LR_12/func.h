#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//#define DEBUG

namespace file {

#ifdef DEBUG

	// Функция вывода на консоль вектора (для debug):
	// вектор, обозначение в консоли
	void printVec(std::vector<float>& vec, std::string name = "") {
		std::cout << '\n' << "VEC " << name << ": " << '\n';
		for (auto& k : vec) {
			std::cout << k << '\n';
		}
	}

	// Функция вывода на консоль содержимого файла (для debug):
	// имя файла, обозначение в консоли
	void printFile(std::string filename, std::string name = "") {
		std::ifstream file;
		file.open(filename);
		std::string line;
		std::cout << '\n' << "FILE " << name << ": " << '\n';
		if (file.is_open()) {
			while (getline(file, line)) {
				std::cout << line << '\n';
			}
		}
		file.close();
	}
#endif

	// Функция очистики файла
	// Имя файла
	void clearFile(std::string filename) {
		std::ofstream file;
		file.open(filename);
		file.close();
	}

	// Функция подсчёта строк (элементов) в файле:
	// имя файла
	// Возвращает: количество строк
	unsigned int countLines(std::string filename) {
		std::string		line;
		std::ifstream	in(filename);
		int count = 0;
		if (in.is_open()) {
			while (getline(in, line)) {
				count++;
			}
		}
		return count;
		in.close();
	}

	// Функция удаления строк в файле:
	// имя файла, с какой строки удаляем, сколько строк удаляем
	void deleteFileChunk(std::string filename, int begin_index, int elements_amount) {
		std::ifstream file;
		file.open(filename);

		std::string line;

		std::string tmp_name = "tmp_" + filename;
		std::ofstream tmp(tmp_name);
		clearFile(tmp_name);
		unsigned int count = 0;

		if (file.is_open() && tmp.is_open()) {
			while (getline(file, line)) {
				if ((count < begin_index) || (count > (begin_index + elements_amount - 1))) {
					tmp << line << '\n';
				}
				else {
				}
				count++;
			}
		}
		tmp.close();
		file.close();

		std::ofstream file_out;
		file_out.open(filename);
		std::ifstream tmp_in;
		tmp_in.open(tmp_name);

		if (file_out.is_open() && tmp_in.is_open()) {
			while (getline(tmp_in, line)) {
				file_out << line << '\n';
			}
		}
		tmp_in.close();
		file_out.close();

		remove(tmp_name.c_str());
	}
	
	// Функция чтения отрезка (чанка) из файла:
	// имя, с какой строки читаем, сколько строк читаем, надо ли при этом удалить отрезок из файла
	// Возвращает: вектор прочитанного отрезка
	std::vector<float> readChunk(std::string filename, int begin_index, int elements_amount, bool delete_chunk = false) {
		std::string		line;
		std::ifstream	in(filename);
		unsigned int curr_index = 0;
		unsigned int add_index	= 0;
		std::vector<float> readVec;

		if (in.is_open()) {
			while (getline(in, line)) {
				if (add_index >= elements_amount)
					break;
				if (curr_index == (begin_index + add_index)) {
					float f;
					std::istringstream(line) >> f;
					readVec.push_back(f);
					add_index++;
				}
				curr_index++;
			}
		}
		else {
			std::cout << "CAN'T OPEN" << '\n';
		}
		in.close();

		if (delete_chunk) {
			deleteFileChunk(filename, begin_index, elements_amount);
		}

		return readVec;
	}

	// Функция записи элеметов из вектора в файл:
	// имя файла, вектор
	void writeChunk(std::string filename, std::vector<float>& arr) {
		std::ofstream out;
		out.open(filename, std::ios_base::app);
		if (out.is_open()) {
			for (auto& element : arr) {
				out << element << '\n';
			}
		}
		out.close();
	}

	// Функция записи элементов (строк) из одного файла в другой:
	// имя файла куда записываем, имя файла откуда записываем
	void writeChunk(std::string filename, std::string data_filename) {
		std::ofstream out;
		out.open(filename, std::ios_base::app);
		//out.open(filename);
		std::ifstream in;
		in.open(data_filename);
		std::string line;
		if (out.is_open() && in.is_open()) {
			while (getline(in, line)) {
				out << line << '\n';
			}
		}
		out.close();
		in.close();
	}

	// Функция слияния (при этом сортировки) элементов двух векторов:
	// первый вектор, второй вектор
	// Возвращает: имя файла, в который записано слитие векторов
	std::string mergeFiles(std::vector<float>& vec_1, std::vector<float>& vec_2) {
		std::string merge_filename = "merge.txt";

		std::ofstream merge;
		merge.open(merge_filename);
		int count_1 = 0;
		int count_2 = 0;
		while ((count_1 != vec_1.size()) && (count_2 != vec_2.size())) {
			if (vec_1[count_1] < vec_2[count_2]) {
				merge << vec_1[count_1] << '\n';
				count_1++;
			}
			else {
				merge << vec_2[count_2] << '\n';
				count_2++;
			}
		}

		if (count_1 == vec_1.size()) {
			for (int i = count_2; i < vec_2.size(); i++) {
				merge << vec_2[i] << '\n';
			}
		}
		else {
			for (int i = count_1; i < vec_1.size(); i++) {
				merge << vec_1[i] << '\n';
			}
		}
		merge.close();

		return merge_filename;
	}

	// Функция чтения отрезков (чанков) из файла размера size (вместе с их удалением)
	// и слияния в один файл:
	// имя первого файла, имя второго файла, размер отрезков
	// Возвращает: имя файла, в который слиты два отрезка
	std::string cutMergeVec(std::string input_1, std::string input_2, int size) {
		std::vector<float> numsVecI1 = file::readChunk(input_1, 0, size, true);     // Вырезаем первый отрезок (чанк) размером size из input_1
		std::vector<float> numsVecI2 = file::readChunk(input_2, 0, size, true);     // вырезаем второй отрезок (чанк) размером size из input_2
		std::string mergedFile = file::mergeFiles(numsVecI1, numsVecI2);            // Сливаем два вектора (при этом данные записываются в файл)
		return mergedFile;
	}

}
