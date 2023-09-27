//
// Created by lenovo on 9/25/2023.
//
#include "iostream"
#include "memory"
#include "chrono"

class Timer{
public:
    Timer(){
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        Stop();
    }

    void Stop(){
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();
        //time_since_epoch()是自时间起始点到现在的时长
        //转换成正确的时间，然后计数，给出startTime微秒计数
        std::chrono::duration<long long int, std::ratio<1, 1000000>> end = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

        auto duration = end - start;
        auto ms = duration * 0.001; //转换成毫秒

        std::cout << std::chrono::duration<long long int, std::ratio<1, 1000000>>(duration) << "us" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};

int main(){
    int value = 0;
    for(int i = 0; i < 10000; i++){
        value += 2;
    }

    std::cout << value << std::endl;

    __debugbreak();
}