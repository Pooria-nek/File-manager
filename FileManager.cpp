#include "FileManager.h"

FileManager::FileManager() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
    }
}

// Read file with point delimiter
String FileManager::readFile(const String path, char point) {
    Serial.println(path);

    File file = SPIFFS.open(path);
    if (!file || file.isDirectory()) {
        Serial.println("- failed to open file for reading");
        return String();
    }

    String fileContent;
    while (file.available()) {
        fileContent = file.readStringUntil(point);
        Serial.println(fileContent);
    }
    return fileContent;
}

// Read file by char pointer
String FileManager::readFile(const char *path) {
    File file = SPIFFS.open(path, FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return "";
    }
    String fileContent;
    while (file.available()) {
        fileContent += char(file.read());
    }
    file.close();
    return fileContent;
}

// Read file by String path
String FileManager::readFile(const String path) {
    File file = SPIFFS.open(path, FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return "";
    }
    String fileContent;
    while (file.available()) {
        fileContent += char(file.read());
    }
    file.close();
    return fileContent;
}

// Write file to SPIFFS
void FileManager::writeFile(const String path, const String message) {
    Serial.println(path);

    File file = SPIFFS.open(path, "w", true);
    if (!file) {
        file = SPIFFS.open('/' + path, "w", true);
        if (!file) {
            Serial.println("- failed to open file for writing");
            return;
        }
    }
    if (file.print(message)) {
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

// Remove file from SPIFFS
void FileManager::removeFile(const String path) {
    if (exists(path)) {
        SPIFFS.remove(path);
        Serial.println("File removed");
    }
}

// Extract file name
String FileManager::nameFile(const String path) {
    int lastSlashIndex = path.lastIndexOf('/');
    String fileName = path.substring(lastSlashIndex + 1);

    int lastDotIndex = fileName.lastIndexOf('.');
    if (lastDotIndex != -1 && fileName.substring(lastDotIndex) == ".txt") {
        fileName = fileName.substring(0, lastDotIndex);
    }
    return fileName;
}

// Check if file exists
bool FileManager::exists(const String path) {
    File file = SPIFFS.open(path, "r");
    bool yes = !file.isDirectory();
    file.close();
    return yes;
}

// Validate D000 form
bool FileManager::isValidString(const String &input) {
    if (input.length() != 10) {
        return false;
    }

    String part1 = input.substring(0, 4);
    String part2 = input.substring(4, 7);
    String part3 = input.substring(7, 10);

    if (part1.charAt(0) != 'D') {
        return false;
    }

    int part1Number = part1.substring(1).toInt();
    if (part1Number < 1 || part1Number > 9) {
        return false;
    }

    int part2Number = part2.toInt();
    int part3Number = part3.toInt();
    if (part2Number < 1 || part2Number > 255 || part3Number < 1 || part3Number > 255) {
        return false;
    }

    return true;
}

// Get config value
String FileManager::getConfigValue(const String &key) {
    String content = readFile(FILE_CONF);
    int keyPos = content.indexOf(key + "=");
    if (keyPos == -1) return "";

    int valueStart = keyPos + key.length() + 1;
    int valueEnd = content.indexOf('\n', valueStart);
    if (valueEnd == -1) valueEnd = content.length();

    return content.substring(valueStart, valueEnd);
}

// Get config value from specified file
String FileManager::getConfigValue(const String path, const String &key) {
    String content = readFile(path);
    int keyPos = content.indexOf(key + "=");
    if (keyPos == -1) return "";

    int valueStart = keyPos + key.length() + 1;
    int valueEnd = content.indexOf('\n', valueStart);
    if (valueEnd == -1) valueEnd = content.length();

    return content.substring(valueStart, valueEnd);
}

// Set config value
void FileManager::setConfigValue(const String &key, const String &value) {
    String content = readFile(FILE_CONF);
    int keyPos = content.indexOf(key + "=");
    if (keyPos == -1) {
        content += key + "=" + value + "\n";
    } else {
        int valueStart = keyPos + key.length() + 1;
        int valueEnd = content.indexOf('\n', valueStart);
        if (valueEnd == -1) valueEnd = content.length();
        content = content.substring(0, keyPos) + key + "=" + value + "\n" + content.substring(valueEnd + 1);
    }
    writeFile(FILE_CONF, content.c_str());
}

// Set config value in specified file
void FileManager::setConfigValue(const String path, const String &key, const String &value) {
    if (value == "") return;

    String content = readFile(path);
    int keyPos = content.indexOf(key + "=");
    if (keyPos == -1) {
        content += key + "=" + value + "\n";
    } else {
        int valueStart = keyPos + key.length() + 1;
        int valueEnd = content.indexOf('\n', valueStart);
        if (valueEnd == -1) valueEnd = content.length();
        content = content.substring(0, keyPos) + key + "=" + value + "\n" + content.substring(valueEnd + 1);
    }
    writeFile(path, content.c_str());
}