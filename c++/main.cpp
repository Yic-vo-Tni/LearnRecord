//
// Created by lenovo on 9/25/2023.
//

// test use

#include "iostream"

[[nodiscard]] int calculateSum(int a, int b) {
    return a + b;
}

int main() {
    calculateSum(3, 4); // 警告：未使用calculateSum的返回值
    return 0;
}