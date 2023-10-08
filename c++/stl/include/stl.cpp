#include "iostream"
#include "vector"
#include "algorithm"
#include "numeric"

void display(int x){
    std::cout << " " << x << std::endl;
}

int main(){
    std::vector<int> vector1{3, 4, 5, 2, 9};

    std::cout << std::count_if(vector1.begin(), vector1.end(), [](int x){return x > 0;}) << std::endl;

    std::sort(vector1.begin(), vector1.end(), [](int x, int y){ return x > y; });

    std::for_each(vector1.begin(), vector1.end(), [](int x){ std::cout << x << " ";});

    int sum = std::accumulate(vector1.begin(), vector1.end(), 0);
    int product = std::accumulate(vector1.begin(), vector1.end(), 1, [](int x, int y){return x * y;});
    int sq_sum = std::accumulate(vector1.begin(), vector1.end(), 0, [](int x, int y){return x + y * y;});
    int rec_sum = std::accumulate(vector1.begin(), vector1.end(), 0., [](double x, int y){return x + 1. / x;});

    std::cout << sum << " " << product << " " << sq_sum << " " << rec_sum << std::endl;

    std::vector<int> v;
    std::transform(vector1.begin(), vector1.end(), v.begin(), [](int x){return x * x;});


    return 0;
}