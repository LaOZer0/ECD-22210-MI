#!/bin/bash
# Уровни оптимизации
optimization_levels=(O0 O1 O2 O3 Os Ofast Og)
# Компиляция программы на всех уровнях оптимизации
for level in "${optimization_levels[@]}"; do
    gcc -$level -o "program_$level" Source1.cpp -lstdc++ -lm
    # Запуск цикла для разных значений n
    for n in 3050000000 4000000000 5030000000; do
        echo "Время работы программы при n = $n и уровне оптимизации $level:"
        time "./program_$level" "$n"
    done
done