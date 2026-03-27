# SOprojekt1

Projekt nr 1 z przedmiotu **Systemy Operacyjne**, ITE SEM 5.
Prowadzący: dr inż. Mariusz Makuchowski

---

## Opis projektu

Program implementuje **wielowątkowy algorytm Dijkstry** służący do wyznaczania najkrótszych ścieżek w losowo generowanym grafie skierowanym z wagami.

Działanie programu obejmuje:
1. Generowanie losowego grafu skierowanego o zadanej liczbie wierzchołków i gęstości.
2. Uruchomienie algorytmu Dijkstry **sekwencyjnie** (1 wątek) dla wszystkich wierzchołków.
3. Uruchomienie algorytmu Dijkstry **równolegle** (N wątków) — każdy wątek przetwarza inny przedział wierzchołków startowych.
4. Porównanie czasów wykonania obu podejść.
5. Zapis wyników do pliku `results.txt`.

---

## Autorzy

- Michał Kapica
- Krzysztof Wiłnicki
- Mateusz Wasilewski

---

## Wymagania

- Kompilator C++ z obsługą standardu **C++20** (np. GCC 10+, Clang 11+)
- **CMake** w wersji 3.30 lub nowszej

---

## Kompilacja

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Po kompilacji plik wykonywalny `SOprojekt1` znajdzie się w katalogu `build/`.

---

## Uruchomienie

```bash
./SOprojekt1 [liczba_wierzchołków] [gęstość_grafu] [liczba_wątków]
```

### Parametry

| Parametr              | Opis                                                                                  |
|-----------------------|---------------------------------------------------------------------------------------|
| `liczba_wierzchołków` | Liczba wierzchołków grafu (całkowita, > 0)                                            |
| `gęstość_grafu`       | Gęstość grafu wyrażona w procentach (1–100); określa, jaka część krawędzi jest obecna |
| `liczba_wątków`       | Liczba wątków użytych przy równoległym wykonaniu (nie może przekraczać liczby dostępnych wątków sprzętowych) |

### Pomoc

```bash
./SOprojekt1 --help
```

### Przykład

```bash
./SOprojekt1 500 50 4
```

Uruchamia algorytm na grafie o 500 wierzchołkach, 50% gęstości, używając 4 wątków.

---

## Wyjście programu

Program wypisuje na standardowe wyjście czasy wykonania:

```
Czas dzialania dla 1 watku: 123456 microseconds

Czas dzialania dla 4 watkow: 34567 microseconds
```

Wyniki są również dopisywane do pliku **`results.txt`** w formacie:

```
liczba_wierzchołków  gęstość  liczba_wątków  czas_1_watek[µs]  czas_N_watkow[µs]
```

---

## Struktura projektu

```
SOprojekt1/
├── main.cpp            # Punkt wejścia programu; implementacja algorytmu Dijkstry,
│                       # logika uruchamiania sekwencyjnego i wielowątkowego
├── GenerateGraph.cpp   # Implementacja generowania losowego grafu
├── GenerateGraph.h     # Interfejs klasy GenerateGraph
├── FileHandler.cpp     # Implementacja zapisu wyników do pliku
├── FileHandler.h       # Interfejs klasy FileHandler
├── CMakeLists.txt      # Konfiguracja budowania (CMake)
└── README.md           # Dokumentacja projektu
```

---

## Uwagi

- Graf jest generowany jako **skierowany** — cykl łączący wszystkie wierzchołki gwarantuje spójność.
- Algorytm Dijkstry działa w wersji klasycznej O(V²); każde wywołanie dotyczy jednego wierzchołka startowego.
- Przy wielowątkowości każdy wątek niezależnie przetwarza przypisany mu zakres wierzchołków startowych — nie ma potrzeby synchronizacji (brak współdzielonych zapisów).
- Gęstość grafu musi być wystarczająco duża, by móc wygenerować graf spójny; w przeciwnym wypadku program zgłosi wyjątek.