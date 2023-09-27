//
// Created by lenovo on 9/27/2023.
//
#include "iostream"
#include "Vector.h"

struct Vector3{
    float x = 0.f, y = 0.f, z = 0.f;

    Vector3(){}
    Vector3(float scalar) : x(scalar), y(scalar), z(scalar){}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z){
        std::cout << "copy\n";
    }
    Vector3(const Vector3&& other) : x(other.x), y(other.y), z(other.z){
        std::cout << "move\n";
    }
    Vector3& operator=(const Vector3& other){
        std::cout << "copy\n";
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
    Vector3& operator=(Vector3&& other){
        std::cout << "move\n";
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    ~Vector3(){
        std::cout << "destroy\n";
    }
};

template<typename T>
void Print(const Vector<T>& vector){
    for(size_t i = 0; i < vector.size(); i++){
        std::cout << vector[i] << std::endl;
    }
}


void PrintVector3(const Vector<Vector3>& vector3){
    for(size_t i = 0; i < vector3.size(); i++){
        std::cout << vector3[i].x << vector3[i].y << vector3[i].z << std::endl;
    }
}

int main(){
    Vector<Vector3 > vector;
    vector.push_back(Vector3(1.f));
    vector.push_back(Vector3(1.f, 3.f, 2.f));
    vector.push_back(Vector3());
    PrintVector3(vector);


    return 0;
}