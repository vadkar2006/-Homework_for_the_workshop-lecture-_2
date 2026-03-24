#pragma once

#include <iostream>
#include <string>

#include "Vector/Vector.h"
#include "Task/Task.h"
#include "Solving_Metod/Solving_Metod.h"


void Solve_Backpack_Problem(std::string input, Vector (&f)(const Backpack&, const Vector&));


