#ifndef SEARCHER_H
#define SEARCHER_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <fstream>

#include "../include/threadPool.h"

class Searcher_t {

public:
    Searcher_t(const std::string&, 
               const std::string&, 
               std::unique_ptr<std::unordered_set<std::string>>, 
               bool
              );

    void search();
    ~Searcher_t();

private:
    void searchFile(const std::filesystem::path&);
    void processDirectory(const std::filesystem::path&);
    bool shouldSkipDir(const std::filesystem::path&);
    bool shouldSkipFile(const std::filesystem::path&);
    void debuggingOutput(const std::filesystem::path&, const std::string&);

    std::string rootPath;
    std::string searchString;
    std::unique_ptr<std::unordered_set<std::string>> fileExtensions;
    bool includeHidden;
    Thread_pool_t pool; 
    std::mutex outputMutex;
    bool fileFound = false;
    std::ofstream outputFile;
};

#endif // SEARCHER_H