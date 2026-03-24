#pragma once

#include <fstream>
#include "Vector/Vector.h"




class Backpack;
class TSP;



template<class Q, class M>
class Task{
    Q** Pair_Numbers = nullptr;

    int Count_Pair = 0;

protected:
    void Set_Count_Pair(int i){Count_Pair = i;}
    void Set_Pair_Numbers(Q** Pair_Numbers1){Pair_Numbers = Pair_Numbers1;}

    void Clear(){
        if(Pair_Numbers){
            if(Pair_Numbers[0])
                delete[] Pair_Numbers[0];
            
            if(Pair_Numbers[1])
                delete[] Pair_Numbers[1];
            
            delete[] Pair_Numbers;
        }
        Pair_Numbers = nullptr;
        Count_Pair = 0;
    }


public:
    virtual Q Cost_Finction(const Vector& items_vector) const = 0;
    virtual bool Input(std::ifstream& input) = 0;
    virtual Vector Solve(Vector (&f)(const M&, const Vector&), const Vector& start) const = 0;

    int Get_Count_Pair() const{return Count_Pair;}


    Q X(int i) const{return Pair_Numbers[0][i];}
    Q Y(int i) const{return Pair_Numbers[1][i];}



    virtual ~Task(){this -> Clear();};
};



class Backpack: public Task<int, Backpack>{
    long Max_Volume = 0;

public:
    Backpack(){}
    Backpack(std::istream& console);
    Backpack(std::ifstream& file);

    int Get_Max_Volume() const {return Max_Volume;};
    int Get_Count_Items() const {return this -> Get_Count_Pair();};

    int Cost_Finction(const Vector& items_vector) const;
    int Volume_Finction(const Vector& items_vector) const;
    int Max_Volume_Item_Index() const;
    int Min_Volume_Item_Index() const;
    bool Input(std::ifstream& input);


    Vector Solve(Vector (&f)(const Backpack&, const Vector&)) const;
    Vector Solve(Vector (&f)(const Backpack&, const Vector&), const Vector& start) const;


    void Print() const;
    void Clear(){this -> Task::Clear(); Max_Volume = 0;}

    ~Backpack(){this -> Clear();};
};

class TSP: public Task<double, TSP>{
public:
    TSP(){}
    TSP(std::istream& console);
    TSP(std::ifstream& file);

    int Get_Count_Point() const{return this -> Get_Count_Pair();}
    double Cost_Finction(const Vector& coord_vector) const;
    bool Input(std::ifstream& input);


    Vector Solve(Vector (&f)(const TSP&, const Vector&)) const;
    Vector Solve(Vector (&f)(const TSP&, const Vector&), const Vector& start) const;


    void Print() const;
    void Clear(){this -> Task::Clear();}

    ~TSP(){this -> Clear();}
};


