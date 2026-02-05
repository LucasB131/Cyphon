#pragma once
#include <string>
#include "../types/SaveData.h"

bool SaveExists(int slot);
void SaveToSlot(const SaveData& data, int slot);
bool LoadFromSlot(SaveData& data, int slot);
bool DeleteSave(int slot);
std::string GetSavePath(int slot);
SaveData LoadPreview(int slot);