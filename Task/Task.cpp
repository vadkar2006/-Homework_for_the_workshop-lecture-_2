#include <iostream>
#include <cmath>

#include "Task.h"






void Backpack::Print() const{
    std::cout << "Max_Volume: " << this -> Get_Max_Volume() << std::endl;
    std::cout << "Count_Items: " << this -> Get_Count_Items() << std::endl;

    for(int i = 0; i < this -> Get_Count_Items(); i++)
        std::cout << this -> X(i) << " " << this -> Y(i) << std::endl;
    std::cout << std::endl;
}
bool Backpack::Input(std::ifstream& input){
    if(!input.is_open())
        return false;
    
    this -> Clear();
    
    int Count_Items;
    input >> Count_Items >> this -> Max_Volume;
    this -> Set_Count_Pair(Count_Items);

    int** Items = new int*[2];
    Items[0] = new int[this -> Get_Count_Items()];
    Items[1] = new int[this -> Get_Count_Items()];

    this -> Set_Pair_Numbers(Items);

    for(int i = 0; i < this -> Get_Count_Items(); i++)
        input >> Items[0][i] >> Items[1][i];
    
    return true;
}


Backpack::Backpack(std::istream& console){
    int Count_Items;
    console >> Count_Items >> this -> Max_Volume;
    this -> Set_Count_Pair(Count_Items);

    int** Items = new int*[2];
    Items[0] = new int[this -> Get_Count_Items()];
    Items[1] = new int[this -> Get_Count_Items()];

    this -> Set_Pair_Numbers(Items);

    for(int i = 0; i < this -> Get_Count_Items(); i++)
        console >> Items[0][i] >> Items[1][i];
}
Backpack::Backpack(std::ifstream& file){
    int Count_Items;
    file >> Count_Items >> this -> Max_Volume;
    this -> Set_Count_Pair(Count_Items);

    int** Items = new int*[2];
    Items[0] = new int[this -> Get_Count_Pair()];
    Items[1] = new int[this -> Get_Count_Pair()];

    this -> Set_Pair_Numbers(Items);

    for(int i = 0; i < this -> Get_Count_Items(); i++)
        file >> Items[0][i] >> Items[1][i];
}

int Backpack::Volume_Finction(const Vector& items_vector) const{
    int w = 0;

    for(int i = 0; i < this -> Get_Count_Items(); i++)
        if(items_vector[i] != 0)
            w += this -> Y(i);
    
    return w;
}
int Backpack::Cost_Finction(const Vector& items_vector) const{
    int cost = 0;
    int volume = 0;

    for(int i = 0; i < this -> Get_Count_Items(); i++)
        if(items_vector[i] != 0){
            cost += this -> X(i);
            volume += this -> Y(i);
        }
    
    if(volume > Max_Volume)
        return Max_Volume - volume;
    return cost;
}
int Backpack::Max_Volume_Item_Index() const{
    int maxx_i = 0;
    for(int i = 0; i < this -> Get_Count_Items(); i++)
        if(this -> Y(i) > this -> Y(maxx_i))
            maxx_i = i;
    return maxx_i;
}
int Backpack::Min_Volume_Item_Index() const{
    int minn_i = 0;
    for(int i = 0; i < this -> Get_Count_Items(); i++)
        if(this -> Y(i) < this -> Y(minn_i))
            minn_i = i;
    return minn_i;
}

Vector Backpack::Solve(Vector (&f)(const Backpack&, const Vector&), const Vector& start) const{
    Vector total = f(*this, start);
    

    std::cout << "Benefit: " << this -> Cost_Finction(total) << std::endl;
    std::cout << this -> Volume_Finction(total) << " of " << Max_Volume << " weights used" << std::endl;
    std::cout << "Vector_Things: ";
    total.Print();

    return total;
}
Vector Backpack::Solve(Vector (&f)(const Backpack&, const Vector&)) const{
    return this -> Solve(f, Vector(this -> Get_Count_Items()));
}







void TSP::Print() const{
    std::cout << "Count_Point: " << this -> Get_Count_Point() << std::endl;

    for(int i = 0; i < this -> Get_Count_Point(); i++)
        std::cout << this -> X(i) << " " << this -> Y(i) << std::endl;
    std::cout << std::endl;
}
bool TSP::Input(std::ifstream& input){
    if(!input.is_open())
        return false;
    
    this -> Clear();
    
    int Count_Point;
    input >> Count_Point;
    this -> Set_Count_Pair(Count_Point);

    double** Coordinates = new double*[2];
    Coordinates[0] = new double[this -> Get_Count_Point()];
    Coordinates[1] = new double[this -> Get_Count_Point()];

    this -> Set_Pair_Numbers(Coordinates);


    for(int i = 0; i < this -> Get_Count_Point(); i++)
        input >> Coordinates[0][i] >> Coordinates[1][i];
    
    return true;
}


TSP::TSP(std::istream& console){
    int Count_Point;
    console >> Count_Point;
    this -> Set_Count_Pair(Count_Point);

    double** Coordinates = new double*[2];
    Coordinates[0] = new double[this -> Get_Count_Point()];
    Coordinates[1] = new double[this -> Get_Count_Point()];

    this -> Set_Pair_Numbers(Coordinates);


    for(int i = 0; i < this -> Get_Count_Point(); i++)
        console >> Coordinates[0][i] >> Coordinates[1][i];
}
TSP::TSP(std::ifstream& file){
    int Count_Point;
    file >> Count_Point;
    this -> Set_Count_Pair(Count_Point);

    double** Coordinates = new double*[2];
    Coordinates[0] = new double[this -> Get_Count_Point()];
    Coordinates[1] = new double[this -> Get_Count_Point()];

    this -> Set_Pair_Numbers(Coordinates);


    for(int i = 0; i < this -> Get_Count_Point(); i++)
        file >> Coordinates[0][i] >> Coordinates[1][i];
}


double TSP::Cost_Finction(const Vector& coord_vector) const{
    double lin = 0;

    for(int i = 0; i < coord_vector.Dim() - 1; i++){
        double x = (this -> X(coord_vector[i]) - this -> X(coord_vector[i+1]));
        double y = (this -> Y(coord_vector[i]) - this -> Y(coord_vector[i+1]));
        
        lin += std::sqrt(x*x + y*y);
    }
    int k = coord_vector.Dim() - 1;
    double x = (this -> X(coord_vector[k]) - this -> X(coord_vector[0]));
    double y = (this -> Y(coord_vector[k]) - this -> Y(coord_vector[0]));
    
    lin += std::sqrt(x*x + y*y);

    return lin;
}


Vector TSP::Solve(Vector (&f)(const TSP&, const Vector&), const Vector& start) const{
    Vector total = f(*this, start);
    

    std::cout << "Length: " << this -> Cost_Finction(total) << std::endl;
    std::cout << "Vector_Things: ";
    total.Print();

    return total;
}
Vector TSP::Solve(Vector (&f)(const TSP&, const Vector&)) const{
    return this -> Solve(f, Vector(this -> Get_Count_Point(), "range"));
}


