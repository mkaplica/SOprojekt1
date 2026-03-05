#include "FileHandler.h"
#include <fstream>
#include <iostream>

void FileHandler::writeToFile(const std::string& file_name, int size, int density, int number_of_threads, int time_one_thread, int time_many_threads) {
    std::ofstream plik(file_name, std::ios::app);
    if (!plik) {
        std::cout << "Nie mozna otworzyc pliku\n";
    }
    plik << size << "\t" << density << "\t" << number_of_threads << "\t" << time_one_thread << "\t" <<time_many_threads << "\t" << std::endl;
    plik.close();
}