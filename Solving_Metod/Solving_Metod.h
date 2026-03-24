#pragma once

#include <vector>
#include <algorithm>

#include "Vector/Vector.h"
#include "Task/Task.h" 

namespace Solving_Metod{

    static int Count_Parallel_Stream = 16;

    static int Count_Generations = 50;
    static int Count_Representatives = 500;
    static int Count_Immigrant = Count_Representatives / 10;

    namespace Genetic_Algorithm{
        Vector Slice(const Backpack& quest, const Vector& start);

    }
}