//
// Created by lenovo on 10/11/2023.
//
#include "iostream"
#include "vector"

class A{
    int x_;
    friend std::ostream& operator<<(std::ostream& ostream, const A& a);
public:
    A() = delete;
    A(int x) : x_(x){}
};

std::ostream& operator<<(std::ostream& ostream, const A& a){
    ostream << a.x_;
    return ostream;
}

namespace yic{
    template<typename T>
    class vector{
    public:
        vector(int n = 10);
        vector(const vector<T>& v);
        vector(vector<T>&& v);

        inline const T& operator[](int index) const { return data_[index]; }
        [[nodiscard]] inline int size() const { return size_; }
        bool empty() const;
        void push_back(const T& t);
        void push_back(T&& t);

        virtual ~vector();

    private:
        int capacity_;
        T* data_;
        int size_;
    };

    template<typename T>
    vector<T>::vector(int n) : capacity_(n), data_(nullptr), size_(0) {
        data_ = (T*) malloc(sizeof(T)*capacity_);
    }
    template<typename T>
    vector<T>::vector(const vector<T> &v) : capacity_(v.capacity_), size_(v.size_) {
        data_ = (T*) malloc(sizeof (T)*size_);
        for(int i = 0; i < size_; i++){
            new (size_ + i) T(v.data_[i]);
        }
    }
    template<typename T>
    vector<T>::vector(vector<T> &&v) : capacity_(v.capacity_), size_(v.size_), data_(v.data_) {
        v.capacity_ = 0;
        v.size_ = 0;
        v.data_ = nullptr;
    }
    template<typename T>
    bool vector<T>::empty() const {
        if(!data_){
            return true;
        } else{
            return false;
        }
    }
    template<typename T>
    void vector<T>::push_back(const T &t) {
        new (data_ + size_) T(t);
        size_++;
   }
    template<typename T>
    void vector<T>::push_back(T &&t) {
        new (data_ + size_) T(std::move(t));
        size_++;
    }

    template<typename T>
    vector<T>::~vector() {
        if (data_){
            free(data_);
        }
        capacity_ = 0;
        size_ = 0;

    }
}

std::ostream& operator<<(std::ostream& ostream, const yic::vector<A>& v){
    for(int i = 0; i < v.size(); i++){
        i && std::cout <<",";
        std::cout << v[i];
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const std::vector<A>& v){
    for(auto i : v){
        std::cout << i << ", ";
    }
//    for(int i = 0; i < v.size(); i++){
//        i && std::cout <<",";
//        std::cout << v[i];
//    }
    return ostream;
}

int main(){
    std::vector<A> v;
    yic::vector<A> v2;

    std::cout << v.size() << std::endl;
    std::cout << v2.size() << std::endl;

    v.emplace_back(123);
    //可以直接在vector中构造新的`A`对象，而不需要调用拷贝构造函数
    //push_back 接受一个已存在的元素作为参数，会调用该元素的拷贝构造函数来创建一个新的副本，并将副本插入到vector中
    A v3(456);
    v.push_back(v3);
    //只能使用push_back，因为`std::vector`没有提供可以直接在容器中构造新的`Car`对象的方式

    v2.push_back(987);
    v2.push_back(654);

    if (!v.empty()) {
        //std::cout << v[0] << std::endl;
        std::cout << v << std::endl;
    } else {
        std::cout << "Vector v is empty." << std::endl;
    }

    if(!v2.empty()){
        //std::cout << v[0] << std::endl;
        std::cout << v2 << std::endl;
    } else{
        std::cout << "v2 is empty. " << std::endl;
    }

    //std::cout << v[0] << std::endl;
    //std::cout << v2[0] << std::endl;


    return 0;
}