#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

class FileManager {
public:
    // FileManager();
    FileManager(const String path);
    
    // File Operations
    String readFile(const String path, char point);
    String readFile(const char *path);
    String readFile(const String path);
    void writeFile(const String path, const String message);
    void removeFile(const String path);
    String nameFile(const String path);

    // Config Handling
    String getConfigValue(const String &key);
    String getConfigValue(const String path, const String &key);
    void setConfigValue(const String &key, const String &value);
    void setConfigValue(const String path, const String &key, const String &value);

    // Utility Methods
    bool exists(const String path);
    bool isValidString(const String &input);

private:
    String FILE_CONF = "";
    void listFiles();
    void sortFiles();
    void displayFiles();
};

#endif