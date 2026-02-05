#include "SaveSystem.h"
#include "../Types/SaveData.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

std::string GetSavePath(int slot);

bool SaveExists(int slot) {
    std::ifstream file("saves/save" + std::to_string(slot) + ".txt");
    return file.good();
}

void SaveToSlot(const SaveData& data, int slot) {
    std::ofstream file("saves/save" + std::to_string(slot) + ".txt");
    file << data.level << "\n";
    file << data.coins << "\n";
    file << data.health << "\n";
    file << data.name << "\n";
}

bool LoadFromSlot(SaveData& data, int slot) {
    std::ifstream file("saves/save" + std::to_string(slot) + ".txt");
    if (!file.good()) return false;

    file >> data.level;
    file >> data.coins;
    file >> data.health;
    file >> data.name;
    return true;
}

SaveData LoadPreview(int slot) {
    SaveData data;
    LoadFromSlot(data, slot);
    return data;
}

bool DeleteSave(int slot) {
    std::string path = GetSavePath(slot);

    std::cout << "Attempting to delete: [" << path << "]\n";

    int result = std::remove(path.c_str());

    if (result != 0) {
        std::cout << "FAILED TO DELETE: " << path << "\n";
    } else {
        std::cout << "DELETED: " << path << "\n";
    }

    return result == 0;
}

std::string GetSavePath(int slot) {
    return "saves/save" + std::to_string(slot) + ".txt";
}
