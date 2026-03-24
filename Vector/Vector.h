#pragma once

#include <string>


class Vector{
    int len = 0;
    int* vector = nullptr;

public:
    Vector(){}
    Vector(int n, int q = 0);
    Vector(int n, std::string wat);
    Vector(int len, int* vec);

    Vector(const Vector& vec);
    Vector(Vector&& vec) noexcept;

    int Len() const {return len;}
    int Dim() const {return len;}

    void Print() const;
    Vector& Random_Binary(double Percentage_Units);
    void Random_Mixing();

    Vector Slice(int start, int end, bool inversion = false) const;
    Vector Concatenation(const Vector& vec1) const;
    


    int& operator[] (int i);
    const int& operator[] (int i) const;

    bool operator = (const Vector& vec);

    friend std::ostream& operator << (std::ostream& output, const Vector& vec);


    ~Vector(){if(vector) delete[] vector;}
};


