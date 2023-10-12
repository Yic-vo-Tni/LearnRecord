//
// Created by lenovo on 10/13/2023.
//
#include "iostream"
#include "ctime"
#include "cstdlib"

class Nilou{
public:
    virtual void talents(){
        std::cout << " " << std::endl;
    }
};

class E : public Nilou{
    void talents() override{
        std::cout << "Dance of Haftkarsvar" << std::endl;
    }
};

class Q : public Nilou{
    void talents() override{
        std::cout << "Dance of Abzendegi" << std::endl;
    }
};

class passive : public Nilou{
    void talents() override{
        std::cout << "Court of Dancing Petals" << std::endl;
    }
};

class flair : public Nilou{
    void talents() override{
        std::cout << "Dreamy Dance of Aeons" << std::endl;
    }
};

int main(){
    const int n = 10;
    srand(time(0));

    auto** nilou = new Nilou*[10];
    for(int i = 0; i < n; i++){
        switch (rand()%4) {
            case 0:
                nilou[i] = new E;
                break;
            case 1:
                nilou[i] = new Q;
                break;
            case 2:
                nilou[i] = new passive;
                break;
            case 3:
                nilou[i] = new flair;
                break;
        }
    }
    for(int i = 0; i < n; i++){
        nilou[i]->talents();
    }

    return 0;
}