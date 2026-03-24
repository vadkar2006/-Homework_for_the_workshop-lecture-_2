#include <iostream>
#include <string>

#include "Test.h"
#include "Vector/Vector.h"
#include "Task/Task.h"
#include "Solving_Metod/Solving_Metod.h"


void Solve_Backpack_Problem(std::string input, Vector (&f)(const Backpack&, const Vector&)){
    std::string way("../Data/data_1/");

    std::ifstream file(way + input);
    if(!file.is_open()){
        std::cout << "Not Open file " << input << " for way " << way + input << std::endl;
        return;
    }

    Backpack quest(file);

    Vector resh = quest.Solve(f);
}

