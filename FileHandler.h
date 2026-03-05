#ifndef FILEHANLDER_H
#define FILEHANLDER_H

#include <string>


class FileHandler {
    public:
    //Zapis do pliku
    static void writeToFile(const std::string& file_name, int size, int density, int number_of_threads, int time_one_thread, int time_many_threads);
};


#endif