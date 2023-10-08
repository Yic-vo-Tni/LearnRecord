//
// Created by lenovo on 9/24/2023.
//
#include "iostream"
#include "unistd.h"
#include "fstream"
#include "filesystem"
namespace fs = std::filesystem;

int main(){


    fs::path filePath = "folder/file.txt";

    // 获取当前可执行文件的路径
    fs::path executablePath = fs::current_path();

    // 构建目标文件的绝对路径
    fs::path absolutePath = executablePath / filePath;

    // 去掉路径中的"cmake-build-debug"部分
    absolutePath = absolutePath.lexically_normal();

    std::cout << "Absolute Path: " << absolutePath << std::endl;


//    std::filesystem::path absolutePath = std::filesystem::absolute("vector.cpp");
//    std::cout << absolutePath << std::endl;
//
//    absolutePath = absolutePath.parent_path() / absolutePath.filename();
//    std::cout << absolutePath << std::endl;

    //char buf[256];
    //getcwd(buf, sizeof (buf));

    chdir("../resources/images");
    std::string imagePath = "JK_01.jpg";

    //std::cout << "Current working directory: " << buf << std::endl;
    std::cout << "Loading image: " << imagePath << std::endl;


//    std::filesystem::path relativePath = "vector.cpp";
//    std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
//    std::cout << absolutePath << std::endl;
//    std::filesystem::path buildDirPath = std::filesystem::current_path();
//    std::cout << buildDirPath << std::endl;
//    std::filesystem::path cleanAbsolutePath = absolutePath.lexically_relative("cmake-build-debug");
//    std::cout << cleanAbsolutePath << std::endl;
//    std::ifstream file(cleanAbsolutePath);
//    std::ifstream ifstream("./vector.cpp");
//    if (ifstream.is_open()){
//        std::cout << "nice" << std::endl;
//    } else{
//        std::cout << "cannot find" << std::endl;
//    }
//    std::filesystem::path absolutePath_a = std::filesystem::absolute("vector.cpp");
//    std::cout << absolutePath_a << std::endl;
//
//    if(file.is_open()){
//        std::cout << "success" << std::endl;
//    } else{
//        std::cout << "can not open file "<<  std::endl;
//    }



    return 0;
}