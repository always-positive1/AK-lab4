#include <iostream>
#include <filesystem>
#include <regex>
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;
namespace fs = std::filesystem;
using namespace fs;
bool isFileArchived(const std::filesystem::path& filePath) {
    DWORD attributes = GetFileAttributes(filePath.wstring().c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_ARCHIVE);
}

bool isFileHidden(const fs::path& filePath) {

    DWORD attributes = GetFileAttributes(filePath.wstring().c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_HIDDEN);

}

bool isFileReadOnly(const std::filesystem::path& filePath) {
    if (!std::filesystem::exists(filePath))
        return false; // Файл не существует

    DWORD attributes = GetFileAttributes(filePath.wstring().c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_READONLY);
}



int countFilesWithPattern(const string& directoryPath, const string& filePattern) {
    int count = 0;
    regex pattern(filePattern);
    
    for (const auto& entry : directory_iterator(directoryPath)) {
        const auto& filePath = entry.path();

        if (regex_search(filePath.filename().string(), pattern))
            count++; 
        
            
        
    }

    return count;
}

int countFilesInDirectory(const string& directoryPath) {
    int count = 0;
    int a = 0;
    int h = 0;
    int r = 0;
    for (const auto& entry : directory_iterator(directoryPath)) {
        /*const auto& filePath = entry.path();
        if (isFileReadOnly(filePath.filename().string())) {
            r++;
        }
        if (isFileHidden(filePath.filename().string())) {
            h++;
        }
        if (isFileArchived(filePath.filename().string())) {
            a++; 
        }*/
        const auto& filePath = entry.path();
        if (isFileReadOnly(filePath)) { 
            r++; 
        }
        if (isFileHidden(filePath)) { 
            h++; 
        }
        if (isFileArchived(filePath)) { 
            a++; 
        }
        count++;
    }
    cout << "number of archive files is " << a << endl; 
    cout << "number of readonly files is " << r << endl; 
    cout << "number of hidden files is " << h << endl; 

    return count;
}


int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        if (argc == 2 && (string(argv[1]) == "-h" || string(argv[1]) == "--help")) {
            cout << "Usage: ./file_counter <directory_path> [file_pattern]" << endl;
            cout << "Options:" << endl;
            cout << "  directory_path    Path to the directory" << endl;
            cout << "  file_pattern      Pattern for filtering files (optional)" << endl;
            return 0;
        }
        else {
            cout << "Invalid arguments. Use '-h' or '--help' for usage instructions." << endl;
            return 1;
        }
    }

    if (string(argv[1]) == "-h" || string(argv[1]) == "--help") {
        cout << "Usage: ./file_counter <directory_path> [file_pattern]" << endl;
        cout << "Options:" << endl;
        cout << "  directory_path    Path to the directory" << endl;
        cout << "  file_pattern      Pattern for filtering files (optional)" << endl;
        return 0;
    }

    const string directoryPath = argv[1];
    string filePattern;

    if (argc == 3) {
        filePattern = argv[2];

        try {
            int fileCount = countFilesInDirectory(directoryPath);
            cout << "Number of files in directory: " << fileCount << endl;
            int Patcount = countFilesWithPattern(directoryPath, filePattern);
            cout << "Number of files with pattern in directory: " << Patcount << endl;
            return 0;
        }
        catch (const filesystem_error& ex) {
            cout << "Error accessing directory: " << ex.what() << endl;
            return 1;
        }
    }
    if (argc == 2) {
        try {
            int fileCount = countFilesInDirectory(directoryPath);
            cout << "Number of files in directory: " << fileCount << endl;
        }
        catch (const filesystem_error& ex) {
            cout << "Error accessing directory: " << ex.what() << endl;
            return 1;
        }
    }
   // file_counter "C:\Users\Always_posi\OneDrive\Робочий стіл\Новая папка (4)"
}