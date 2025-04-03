#include "../../include/Core/FileSystem.h"
#include <vector>
#include <filesystem>

std::vector<FileSystemItem> FileSystem::GetItems(const std::string& path) {
    std::vector<FileSystemItem> items;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        FileSystemItem item;
        item.name = entry.path().filename();
        item.type = entry.is_directory() ? ItemType::ITEM_DIRECTORY
                                         : ItemType::ITEM_FILE;

        items.push_back(item);
    }

    return items;
}
