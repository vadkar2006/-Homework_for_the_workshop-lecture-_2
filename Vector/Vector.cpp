#include <iostream>
#include <random>
#include <algorithm>


#include "Vector.h"

Vector::Vector(int n, int q){
    if (n > 0){
        len = n;

        vector = new int[len];
        for(int i = 0; i < len; i++)
            vector[i] = q;
    }
}
Vector::Vector(int n, std::string wat): Vector(n){
    if(wat != "range")
        return;
    for(int i = 0; i < len; i++)
        vector[i] = i;
}
Vector::Vector(int len1, int* vec){
    if ((len1 > 0) && vec){
        len = len1;
        vector = vec;
    }
}
Vector::Vector(Vector&& vec) noexcept {
    len = vec.len;
    vector = vec.vector;

    vec.vector = nullptr;
    vec.len = 0;
}
Vector::Vector(const Vector& vec){
    if (!((vec.len > 0) && vec.vector))
        return;

    len = vec.len;
    vector = new int[len];

    for(int i = 0; i < len; i++)
        vector[i] = vec.vector[i];
}


void Vector::Print() const{
    //std::cout << "Lenght: " << this -> Len() << std::endl;
    for(int i = 0; i < this -> Len(); i++)
        std::cout << this -> vector[i] << " ";
    std::cout << std::endl;
}


Vector Vector::Slice(int start, int end, bool inversion) const{
    if(end <= start)
        return Vector();

    int new_len = end - start;
    Vector slic(new_len);

    if(!inversion)
        for(int i = 0; i < new_len; i++)
            slic[i] = vector[i + start];

    else
        for(int i = 0; i < new_len; i++)
            slic[i] = vector[end - i - 1];
    
    return slic;
}
Vector Vector::Concatenation(const Vector& vec1) const{
    Vector con(len + vec1.len);

    for(int i = 0; i < len; i++)
        con[i] = vector[i];
    
    for(int i = 0; i < vec1.len; i++)
        con[len + i] = vec1[i];
    
    return con;
}
Vector& Vector::Random_Binary(double Percentage_Units){
    std::uniform_int_distribution<> dist_int(0, 1000000);
    std::random_device rd;
    std::mt19937 gen(rd());

    Percentage_Units *= 10000; 

    if(!vector) 
        return *this;

    for(int i = 0; i < len; i++)
        vector[i] = dist_int(gen);

    for(int i = 0; i < len; i++){
        if(vector[i] <= Percentage_Units)
            vector[i] = 1;
        else
            vector[i] = 0;
    }

    return *this;
}
void Vector::Random_Mixing() {
    if (len < 2 || vector == nullptr)
        return;
    
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(vector, vector + len, gen);
}

int& Vector::operator[] (int i){
    return vector[i];
}
const int& Vector::operator[] (int i) const{
    return vector[i];
}


bool Vector::operator = (const Vector& vec){
    if(this == &vec)
        return false;
    
    if(vector)
        delete[] vector;
    vector = nullptr;

    len = vec.len;
    vector = new int[len];
    for(int i = 0; i < len; i++)
        vector[i] = vec.vector[i];
    
    return true;
}


std::ostream& operator << (std::ostream& output, const Vector& vec){
    for(int i = 0; i < vec.Len(); i++)
        output << vec.vector[i] << " ";
    output << std::endl;

    return output;
}

