#include <iostream>
#include <climits>
#include <chrono>
#include <vector>
#include <mutex>
#include <thread>

#include "FileHandler.h"
#include "GenerateGraph.h"

using namespace std;

// === Deklaracje funkcji pomocniczych ===
void print_generated_graph(int **graph, int edges); // Wyświetlanie wygenerowanego grafu
void print_neighbors_list(int ***neighbor_list, const int *neighbor_counts, int size); // Wyświetlanie listy sąsiedztwa
void classical_dijkstra(int start, int size, const int* neighbor_counts, int*** neighbor_list); // Algorytm Dijkstry (wersja sekwencyjna)
void print_path_from_start_to(int start, int to, int** delta, int size); // Wyświetlenie ścieżki od wierzchołka startowego do wskazanego
void help(unsigned int number_of_available_threads);

int main(const int argc, char *argv[]) {
    // argv[1] - ilość wierzchołków grafu
    // argv[2] - gęstość grafu
    // argv[3] - ilość wątków

    const unsigned int number_of_available_threads = std::thread::hardware_concurrency();

    int number_of_vertices = 0;
    int graph_density = 0;
    int number_of_threads = 0;

    if (std::string(argv[1]) == "--help" ) {
    help(number_of_available_threads);
        return 0;
    }
    if (argc != 4) {
        cerr << "Niepoprawna liczba argumentow!\n\n";
        help(number_of_available_threads);
        return 1;
    }

    stringstream ss;
    ss << argv[1];
    ss >> number_of_vertices;

    ss.clear();

    ss << argv[2];
    ss >> graph_density;

    ss.clear();

    ss << argv[3];
    ss >> number_of_threads;

    if (number_of_vertices <= 0 || graph_density <= 0 || number_of_threads <= 0) {
        throw std::invalid_argument("Number of vertices, density and threads must be greater than zero\n");
    }
    if (number_of_threads > number_of_available_threads) {
        throw std::invalid_argument("Number of threads exceeds available threads\n");
    }

    //cout << "Number of availible threads: " << number_of_available_threads << endl;

    // === Generowanie losowego grafu ===
    // Funkcja generateGraph(liczba_wierzcholkow, liczba_krawedzi, czy_skierowany)
    int **graph = GenerateGraph::generateGraph(number_of_vertices, graph_density, true);

    int edges = graph[0][0]; // liczba krawędzi
    int size = graph[0][1];  // liczba wierzchołków

    // Wyświetlenie listy krawędzi
    //print_generated_graph(graph, edges);

    // === Tworzenie listy sąsiedztwa ===
    // Struktura: neighbor_list[i][j][0] -> indeks sąsiada
    //            neighbor_list[i][j][1] -> waga krawędzi
    int ***neighbor_list = new int **[size];
    for (int i = 0; i < size; i++) {
        neighbor_list[i] = new int *[size];
        for (int j = 0; j < size; j++) {
            neighbor_list[i][j] = new int[2]; // (sąsiad, waga)
        }
    }

    // Tablica z liczbą sąsiadów dla każdego wierzchołka
    int *neighbor_counts = new int[size];
    for (int i = 0; i < size; i++)
        neighbor_counts[i] = 0;

    // Wypełnianie listy sąsiedztwa na podstawie listy krawędzi
    for (int i = 0; i < edges; i++) {
        int from = graph[i + 1][0];
        int to = graph[i + 1][1];
        int weight = graph[i + 1][2];

        int pos = neighbor_counts[from]; // pozycja nowego sąsiada
        neighbor_list[from][pos][0] = to;
        neighbor_list[from][pos][1] = weight;
        neighbor_counts[from]++;
    }

    // Wyświetlenie listy sąsiedztwa
    //print_neighbors_list(neighbor_list, neighbor_counts, size);

    // Pomiar czasu działania algorytmu dla trybu sekwencyjnego
    auto start_one_thread = chrono::high_resolution_clock::now();

    // === Uruchomienie algorytmu Dijkstry sekwencyjnie dla każdego wierzchołka ===
    for (int i = 0; i <= size - 1; i++)
        classical_dijkstra(i, size, neighbor_counts, neighbor_list);

    auto stop_one_thread = chrono::high_resolution_clock::now();

    //Całkowity czas trwania algorytmu
    auto duration_one_thread = chrono::duration_cast<chrono::microseconds>(stop_one_thread - start_one_thread);

    cout <<"Czas dzialania dla 1 watku: " << duration_one_thread.count() << " microseconds\n" << endl;


    //Pomiar czasu dla trybu wielowątkowego
    std::vector<std::thread> threads;

    auto start_multi_thread = chrono::high_resolution_clock::now();

    auto worker = [&](int thread_id) {
        int start_node = (size * thread_id) / number_of_threads;
        int end_node = (size * (thread_id + 1)) / number_of_threads;

        for (int i = start_node; i < end_node; i++) {
            classical_dijkstra(i, size, neighbor_counts, neighbor_list);
        }
    };

    for (int i = 0; i < number_of_threads; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        if (t.joinable()) t.join();
    }

    auto stop_multi_thread = chrono::high_resolution_clock::now();
    auto duration_multi_thread = chrono::duration_cast<chrono::microseconds>(stop_multi_thread - start_multi_thread);

    cout <<"Czas dzialania dla " << number_of_threads << " watkow: " << duration_multi_thread.count() << " microseconds\n" << endl;

    FileHandler::writeToFile("results.txt", size, graph_density, number_of_threads, duration_one_thread.count(), duration_multi_thread.count());

    // === Zwolnienie pamięci ===
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            delete[] neighbor_list[i][j];
        }
        delete[] neighbor_list[i];
    }
    delete[] neighbor_list;
    delete[] neighbor_counts;

    return 0;
}

void help(const unsigned int number_of_available_threads) {
    cout << "Projekt z przedmiotu Systemy Operacyjne 1. Prowadzacy dr inz Mariusz Makuchowski\n";
    cout << "Authors: Michal Kapica, Krzysztof Wilnicki, Mateusz Wasilewki\n";
    cout << "To run the program type: ./SOprojekt1 [number of vertices] [density of the graph] [number of threads]\n";
    cout << "Type --help to see help method\n";
    cout << "Number of threads available of this device: " << number_of_available_threads << " Do not exceed this number while testing!\n\n";
}

// === Implementacja algorytmu Dijkstry ===
void classical_dijkstra(int start, int size, int const *neighbor_counts, int*** neighbor_list) {
    // Tablica delta: [odległość, poprzednik]
    int **delta = new int *[size];
    for (int i = 0; i < size; i++) {
        delta[i] = new int[2];
        delta[i][0] = INT_MAX; // początkowo odległość = nieskończoność
        delta[i][1] = -1;      // brak poprzednika
    }

    // Tablica odwiedzonych wierzchołków
    bool *visited = new bool[size];
    for (int i = 0; i < size; i++)
        visited[i] = false;

    delta[start][0] = 0; // odległość od wierzchołka startowego = 0

    // Pętla główna — powtarzamy dla każdego wierzchołka
    for (int count = 0; count < size - 1; count++) {
        // Szukanie nieodwiedzonego wierzchołka o najmniejszej odległości
        int minDist = INT_MAX, minIndex = -1;
        for (int v = 0; v < size; v++) {
            if (!visited[v] && delta[v][0] <= minDist) {
                minDist = delta[v][0];
                minIndex = v;
            }
        }

        // Jeśli nie znaleziono kolejnego wierzchołka (graf niespójny)
        if (minIndex == -1) break;

        visited[minIndex] = true; // oznacz jako odwiedzony

        // Aktualizacja odległości do sąsiadów wierzchołka minIndex
        for (int i = 0; i < neighbor_counts[minIndex]; i++) {
            int neighbor = neighbor_list[minIndex][i][0];
            int weight = neighbor_list[minIndex][i][1];

            // Sprawdzenie czy nowa ścieżka jest krótsza
            if (!visited[neighbor] &&
                delta[minIndex][0] != INT_MAX &&
                delta[minIndex][0] + weight < delta[neighbor][0]) {

                delta[neighbor][0] = delta[minIndex][0] + weight;
                delta[neighbor][1] = minIndex; // zapamiętaj poprzednika
            }
        }
    }

    // === Wyświetlenie wyników ===
    /*cout << "\n=== Wyniki Dijkstry ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << "Wierzcholek " << i << ": dystans = " << delta[i][0]
             << ", poprzednik = " << delta[i][1] << endl;
    }*/

    // Wyświetlenie ścieżki z wierzchołka 0 do ostatniego
    //print_path_from_start_to(start, size-1, delta, size);

    // Zwolnienie pamięci
    for (int i = 0; i < size; i++) {
        delete[] delta[i];
    }
    delete[] delta;
    delete[] visited;
}


// === Wyświetlenie krawędzi wygenerowanego grafu ===
void print_generated_graph(int **graph, int edges) {
    cout << "\n=== Krawedzie grafu ===" << endl;
    for (int i = 1; i <= edges; i++) {
        cout << graph[i][0] << " " << graph[i][1] << " " << graph[i][2] << endl;
    }
}

// === Wyświetlenie listy sąsiedztwa ===
void print_neighbors_list(int ***neighbor_list, const int *neighbor_counts, int size) {
    cout << "\n=== Lista sasiedztwa ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << i;
        for (int j = 0; j < neighbor_counts[i]; j++) {
            cout << " -> " << neighbor_list[i][j][0]
                 << "(" << neighbor_list[i][j][1] << ")";
        }
        cout << endl;
    }
}

// === Wyświetlenie ścieżki od wierzchołka startowego do wskazanego ===
void print_path_from_start_to(int start, int to, int** delta, int size) {
    cout << "Sciezka od wierzcholka " << start << " do wierzcholka " << to << ": ";

    int* path = new int[size];
    for (int i = 0; i < size; i++) {
        path[i] = -1;
    }

    // Od końca do początku — cofamy się po poprzednikach
    int v = to, n = 0;
    while (v != -1) {
        path[n++] = v;
        v = delta[v][1];
    }

    // Wypisanie ścieżki w poprawnej kolejności
    for (int i = n - 1; i >= 0; i--) {
        if (i != n - 1)
            cout << " -> ";
        cout << path[i];
    }

    cout << " (" << delta[to][0] << ")" << endl;

    delete [] path;
}