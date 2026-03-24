#include <iostream>
#include <thread>
#include <random>
#include <algorithm>

#include "Solving_Metod.h"



Vector Solving_Metod::Genetic_Algorithm::Slice(const Backpack& quest, const Vector& start){
    std::uniform_int_distribution<> population_percent(0, 100);
    std::uniform_int_distribution<> random_representatives(0, Count_Representatives - 1);
    std::uniform_int_distribution<> random_gap(1, quest.Get_Count_Items() - 2);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Vector> population_old(Count_Representatives, start);
    std::vector<Vector> population_new(2*Count_Representatives + Count_Immigrant);


    for(int i = 1; i < Count_Representatives; i++)
        population_old[i] = Vector(quest.Get_Count_Items()).Random_Binary(population_percent(gen));
    

    for(int p = 0; p < Count_Generations; p++){
        for(int i = 0; i < Count_Representatives; i++){
            int gap = random_gap(gen);
            int k1 = random_representatives(gen);
            int k2 = random_representatives(gen);

            population_new[i] = (population_old[k1].Slice(0, gap)).Concatenation(population_old[k2].Slice(gap, quest.Get_Count_Items()));
        }

        for(int i = Count_Representatives; i < 2*Count_Representatives; i++)
            population_new[i] = population_old[i - Count_Representatives];

        for(int i = 2*Count_Representatives; i < 2*Count_Representatives + Count_Immigrant; i++){
            int k = random_representatives(gen) / 10;
            population_new[i] = Vector(population_old[k]);
            population_new[i].Random_Mixing();
        }

        std::sort(population_new.begin(), population_new.end(), [&quest](const Vector& a, const Vector& b) {
                return quest.Cost_Finction(a) > quest.Cost_Finction(b);
            });
        
        for(int i = 0; i < Count_Representatives; i++)
            population_old[i] = population_new[i];
    }

    return population_old[0];
}


















