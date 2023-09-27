//
// Created by lenovo on 9/25/2023.
//
//假如有一个返回数据的函数，比如读取一个文件，可能无法读取，或者返回空字符串
//如果文件为空，则我们应该有知道数据是否存在
#include <fstream>
#include "iostream"
#include "optional"

//std::string ReadFileAsString(const std::string &filepath, bool &success){
//    std::ifstream stream(filepath);
//    if (stream){
//        std::string data;
//        stream.close();
//        success = true;
//        return data;
//    }
//    success = false;
//    return std::string();
//}

std::optional<std::string > ReadFileAsString(const std::string &filepath){
    std::ifstream stream(filepath);
    if (stream){
        std::string data;
        return data;
    }
    return {};
}

int main(){
//    bool fileOpenfileSuccess;
//    std::string data = ReadFileAsString("data.txt", fileOpenfileSuccess);
//    if(data != " "){
//    }

    auto data = ReadFileAsString("../include/data.txt");
    std::string value = data.value_or("no present");
    std::cout << value << std::endl;
    //if existed, return the context of file
    //if not existed, return the context about you write
    if(data){
        //you can use data.has_value()
        //optional default has a bool
        std::cout << "file read" << std::endl;
    } else{
        std::cout << "failed to read the file" << std::endl;
    }

    return 0;
}