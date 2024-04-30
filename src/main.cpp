#include <iostream>
#include <unordered_set>
#include <sstream>
#include <memory>
#include "../include/searcher.h"
#include <fstream>


std::unique_ptr<std::unordered_set<std::string>> splitExtensions(const std::string& exts){
    auto extensions = std::make_unique<std::unordered_set<std::string>>();
    std::string extension; 
    std::stringstream ss(exts);
    while (std::getline(ss, extension, ' ')){
        extensions->insert(extension);
    }

    return extensions;
}

int main(){

    std::string path;
    std::string searchString;
    std::string fileExtensions;
    bool includeHiddenFiles;

    #ifdef READ_FROM_FILE
    std::string inputFilePath;
    std::cout << "Enter the path to the config file: ";
    std::cin >> inputFilePath;
    std::ifstream file(inputFilePath);

    if (file.is_open()){
        std::string line;
        std::getline(file, line);
        path = line;
        std::getline(file, line);
        searchString = line;
        std::getline(file, line);
        fileExtensions = line;
        std::getline(file, line);
        includeHiddenFiles = std::stoi(line);

        #ifdef DEBUG // Confirm input
        std::cout << "Path: " << path << std::endl;
        std::cout << "Search String: " << searchString << std::endl;
        auto x = splitExtensions(fileExtensions);

        for (const auto& ext : *x){
            std::cout << ext << " ";
        }

        std::cout << std::endl;
        std::cout << "Include Hidden Files: " << includeHiddenFiles << std::endl;
        #endif
    }

    #else
    std::cout << "Enter the path to start the search from: ";
    std::cin >> path;

    std::cout << "Enter the string to search for in files: ";
    std::cin >> searchString;

    std::cout << "Enter file extensions to search for (leave empty for all files): ";
    std::cin >> fileExtensions;

    std::cout << "Include search in hidden files? (1 for yes, 0 for no): ";
    std::cin >> includeHiddenFiles;

        #ifdef DEBUG // Confirm input
        std::cout << "Path: " << path << std::endl;
        std::cout << "Search String: " << searchString << std::endl;
        auto x = splitExtensions(fileExtensions);

        for (const auto& ext : *x){
            std::cout << ext << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Include Hidden Files: " << includeHiddenFiles << std::endl;
        #endif
    #endif

    Searcher_t searcher(path, searchString, splitExtensions(fileExtensions), includeHiddenFiles);
    searcher.search();

    return 0;
}