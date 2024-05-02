#include "../include/searcher.h"

Searcher_t::Searcher_t(std::string  root,
                       std::string  searchStr,
                       std::unique_ptr<std::unordered_set<std::string>> exts, 
                       bool hidden) : rootPath(std::move(root)), searchString(std::move(searchStr)),
                       includeHidden(hidden), fileExtensions(std::move(exts)) 
                       
                       { 
                        #ifdef DEBUG
                        std::cout<<"Searcher Created"<<std::endl;
                        std::cout<<"Here are the file extensions to search for: ";
                        for (const auto& ext : *fileExtensions){
                            std::cout<<ext<<" ";
                        }
                        std::cout<<std::endl;
                        std::cout<<"Include Hidden Files: "<<includeHidden<<std::endl;
                        std::cout<<"Root Path: "<<rootPath<<std::endl;
                        std::cout<<"Search String: "<<searchString<<std::endl;
                        #endif
                       }

void Searcher_t::search() {
    processDirectory(rootPath);
}

void Searcher_t::processDirectory(const std::filesystem::path& dirPath) {

    #ifdef ONE_INSTANCE
    if (fileFound) {
        return;
    }
    #endif

    try {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_directory()) {

                if (shouldSkipDir(entry.path())) {
                    continue;
                }

                pool.enqueue([this, entry] { processDirectory(entry.path()); });
            } else {

                if (shouldSkipFile(entry.path())) {
                    continue;
                }

                pool.enqueue([this, entry] { searchFile(entry.path()); });
            }
        }

    } catch (const std::filesystem::filesystem_error& e) {

        if (e.code() == std::make_error_code(std::errc::permission_denied)) {
            #ifdef DEBUG
            debuggingOutput(dirPath, "Permission Denied: ");
            #endif
            return;
        } else {
            #ifdef DEBUG
            debuggingOutput(dirPath, "Error due to file System: ");
            #endif
        }
    } 
}

void Searcher_t::searchFile(const std::filesystem::path& filePath) {

    #ifdef ONE_INSTANCE
    if (fileFound) {
        return;
    }
    #endif

    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(searchString) != std::string::npos) {
            std::unique_lock<std::mutex> lock(outputMutex);
            std::cout << "Found in file: " << filePath << std::endl;
            fileFound = true; // For printing only 1 instance of the output
            lock.unlock();
            return;
        }
    }
}


bool Searcher_t::shouldSkipDir(const std::filesystem::path& dirPath) const {
    // macOS specific
    if (dirPath.string().find("/Library") != std::string::npos) {
        #ifdef DEBUG
        debuggingOutput(dirPath, "Skipping /Library: ");
        #endif
        return true;
    }

    if (!includeHidden && dirPath.filename().string()[0] == '.') {
        #ifdef DEBUG
        debuggingOutput(dirPath, "Skipping Hidden Directory: ");
        #endif
        return true;
    }

    return false;
}

bool Searcher_t::shouldSkipFile(const std::filesystem::path& filePath) const {

    if (fileExtensions && !fileExtensions->empty() && !fileExtensions->contains(filePath.extension().string())) {
                #ifdef DEBUG
                debuggingOutput(filePath, "Skipping File (Not Suitable Extensions): ");
                #endif
        return true;
    }

    if (!includeHidden && filePath.filename().string()[0] == '.') {
        #ifdef DEBUG
        debuggingOutput(filePath, "Skipping Hidden File: ");
        #endif
        return true;
    }

    return false;
}

void Searcher_t::debuggingOutput(const std::filesystem::path& path, const std::string& line) {
    std::unique_lock<std::mutex> lock(outputMutex);
    std::cout << line << path << std::endl;
    lock.unlock();
}

Searcher_t::~Searcher_t() {
    pool.shutdown();
}