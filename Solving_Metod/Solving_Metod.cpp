#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <queue>
#include <stack>

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


Vector Solving_Metod::Genetic_Algorithm::Parallel_Slice(const Backpack& quest, const Vector& start){
    std::uniform_int_distribution<> population_percent(0, 100);
    std::uniform_int_distribution<> random_representatives(0, Count_Representatives - 1);
    std::uniform_int_distribution<> random_gap(1, quest.Get_Count_Items() - 2);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Vector> population_old(Count_Representatives, start);
    std::vector<Vector> population_new(2*Count_Representatives + Count_Immigrant);


    for(int i = 1; i < Count_Representatives; i++)
        population_old[i] = Vector(quest.Get_Count_Items()).Random_Binary(population_percent(gen));
    

    int partition = Count_Representatives / Count_Parallel_Stream;
    int over_count = Count_Representatives % Count_Parallel_Stream;

    int migration_rasp = Count_Immigrant / Count_Parallel_Stream;
    int over_count_migration = Count_Immigrant % Count_Parallel_Stream;



    std::vector<std::thread> threads(Count_Parallel_Stream);
    std::vector<std::vector<int>> seed_whirlpool_of_birth(3, std::vector<int>(Count_Representatives));
    std::vector<int> seed_migration(Count_Immigrant);

    auto whirlpool_of_birth = [&] (int start, int end){
        for(int i = start; i < end; i++){
            int gap = seed_whirlpool_of_birth[0][i];
            int k1 = seed_whirlpool_of_birth[1][i];
            int k2 = seed_whirlpool_of_birth[2][i];

            population_new[i] = (population_old[k1].Slice(0, gap)).Concatenation(population_old[k2].Slice(gap, quest.Get_Count_Items()));
        }

        for(int i = start + Count_Representatives; i < end + Count_Representatives; i++)
            population_new[i] = population_old[i - Count_Representatives];
    };
    auto migration = [&] (int start, int end){
        for(int i = start + 2*Count_Representatives; i < end + 2*Count_Representatives; i++){
            int k = seed_migration[i - 2*Count_Representatives];
            population_new[i] = Vector(population_old[k]);
            population_new[i].Random_Mixing();
        }
    };



    for(int p = 0; p < Count_Generations; p++){
        for(int i = 0; i < Count_Representatives; i++){
            seed_whirlpool_of_birth[0][i] = random_gap(gen);
            seed_whirlpool_of_birth[1][i] = random_representatives(gen);
            seed_whirlpool_of_birth[2][i] = random_representatives(gen);
        }
        for(int i = 0; i < Count_Immigrant; i++)
            seed_migration[i] = random_representatives(gen);


        for(int i = 0; i < over_count; i++)
            threads[i] = std::thread(whirlpool_of_birth, i*(partition + 1), (i + 1)*(partition + 1));

        for(int i = over_count; i < Count_Parallel_Stream; i++)
            threads[i] = std::thread(whirlpool_of_birth, over_count*(partition + 1) + (i - over_count)*partition, over_count*(partition + 1) + (i - over_count + 1)*partition);
        


        for(int i = 0; i < over_count_migration; i++){
            threads[i].join();
            threads[i] = std::thread(migration, i*(migration_rasp + 1), (i + 1)*(migration_rasp + 1));
        }


        for(int i = over_count_migration; i < Count_Parallel_Stream; i++){
            threads[i].join();
            threads[i] = std::thread(migration, over_count_migration*(migration_rasp + 1) + (i - over_count_migration)*migration_rasp, over_count_migration*(migration_rasp + 1) + (i - over_count_migration + 1)*migration_rasp);
        }


        for(int i = 0; i < Count_Parallel_Stream; i++)
            threads[i].join();
/*
        for(int i = 0; i < 2*Count_Representatives + Count_Immigrant; i++)
            std::cout << i << "    " << population_new[i];*/

        std::sort(population_new.begin(), population_new.end(), [&quest](const Vector& a, const Vector& b) {
                return quest.Cost_Finction(a) > quest.Cost_Finction(b);
            });
        
        for(int i = 0; i < Count_Representatives; i++)
            population_old[i] = population_new[i];
    }


    return population_old[0];
}





Vector Solving_Metod::Branch_And_Bound::Priority_Queue(const Backpack& quest, const Vector& start){
    
    struct Solution{
        const Backpack* quest;
        Vector sol_vec;
        int cost;
        int id_poz;
    
        Solution(const Backpack* quest1, const Vector& sol_vec1, int id_poz1): quest(quest1), sol_vec(sol_vec1), id_poz(id_poz1){cost = quest1 -> Cost_Finction(sol_vec1);}

        bool operator < (const Solution& sol11) const{return cost < sol11.cost;}
    };

    std::priority_queue<Solution> queue_sol;


    Vector best_vector(quest.Get_Count_Items());
    int best_cost = 0;

    queue_sol.push(Solution(&quest, best_vector, 0));
    

    while (queue_sol.size() > 0){
        Solution sol = queue_sol.top();
        queue_sol.pop();

        if(best_cost < sol.cost){
            best_vector = sol.sol_vec;
            best_cost = sol.cost;
        }

        if(sol.id_poz < quest.Get_Count_Items()){
            Solution sol1(&quest, sol.sol_vec, sol.id_poz + 1);
            sol.sol_vec[sol.id_poz] = !sol.sol_vec[sol.id_poz];
            Solution sol2(&quest, sol.sol_vec, sol.id_poz + 1);

            if(sol1.cost >= 0)
                queue_sol.push(sol1);

            if(sol2.cost >= 0)
                queue_sol.push(sol2);
        }
    }
    
    return best_vector;
}

Vector Solving_Metod::Branch_And_Bound::Queue(const Backpack& quest, const Vector& start){
    
    struct Solution{
        const Backpack* quest;
        Vector sol_vec;
        int cost;
        int id_poz;
    
        Solution(const Backpack* quest1, const Vector& sol_vec1, int id_poz1): quest(quest1), sol_vec(sol_vec1), id_poz(id_poz1){cost = quest1 -> Cost_Finction(sol_vec1);}
    };

    std::queue<Solution> queue_sol;


    Vector best_vector(quest.Get_Count_Items());
    int best_cost = 0;

    queue_sol.push(Solution(&quest, best_vector, 0));
    

    while (queue_sol.size() > 0){
        Solution sol = queue_sol.front();
        queue_sol.pop();

        if(best_cost < sol.cost){
            best_vector = sol.sol_vec;
            best_cost = sol.cost;
        }

        if(sol.id_poz < quest.Get_Count_Items()){
            Solution sol1(&quest, sol.sol_vec, sol.id_poz + 1);
            sol.sol_vec[sol.id_poz] = !sol.sol_vec[sol.id_poz];
            Solution sol2(&quest, sol.sol_vec, sol.id_poz + 1);

            if(sol1.cost >= 0)
                queue_sol.push(sol1);

            if(sol2.cost >= 0)
                queue_sol.push(sol2);
        }
    }
    
    return best_vector;
}

Vector Solving_Metod::Branch_And_Bound::Stack(const Backpack& quest, const Vector& start){
    
    struct Solution{
        const Backpack* quest;
        Vector sol_vec;
        int cost;
        int id_poz;
    
        Solution(const Backpack* quest1, const Vector& sol_vec1, int id_poz1): quest(quest1), sol_vec(sol_vec1), id_poz(id_poz1){cost = quest1 -> Cost_Finction(sol_vec1);}
    };

    std::stack<Solution> queue_sol;


    Vector best_vector(quest.Get_Count_Items());
    int best_cost = 0;

    queue_sol.push(Solution(&quest, best_vector, 0));
    

    while (queue_sol.size() > 0){
        Solution sol = queue_sol.top();
        queue_sol.pop();

        if(best_cost < sol.cost){
            best_vector = sol.sol_vec;
            best_cost = sol.cost;
        }

        if(sol.id_poz < quest.Get_Count_Items()){
            Solution sol1(&quest, sol.sol_vec, sol.id_poz + 1);
            sol.sol_vec[sol.id_poz] = !sol.sol_vec[sol.id_poz];
            Solution sol2(&quest, sol.sol_vec, sol.id_poz + 1);

            if(sol1.cost >= 0)
                queue_sol.push(sol1);

            if(sol2.cost >= 0)
                queue_sol.push(sol2);
        }
    }
    
    return best_vector;
}

