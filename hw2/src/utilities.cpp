#include "methods.h"
#include <fstream>
#include <sstream>
#include <string>

std::string serializeItem(const Item &item) {
  std::ostringstream stream;

  stream << item.age << ',' << item.type << ',' << item.origin << ',' << item.rarityScore;

  return stream.str();
}

Item deserializeItem(const std::string &item_str) {
  std::istringstream stream(item_str);

  std::string age_str;
  std::getline(stream, age_str, ',');

  std::string type_str;
  std::getline(stream, type_str, ',');

  std::string origin_str;
  std::getline(stream, origin_str, ',');

  std::string rarity_str;
  std::getline(stream, rarity_str, ',');

  int age = std::stoi(age_str);
  int type = std::stoi(type_str);
  int origin = std::stoi(origin_str);
  double rarity = std::stod(rarity_str);

  return Item{age, type, origin, rarity};
}

std::vector<Item> readItemsFromFile(const std::string &filename) {
  std::ifstream items_file(filename);

  std::vector<Item> items;

  std::string line;

  // Discard first line: age,type,origin,rarity
  std::getline(items_file, line);

  while (std::getline(items_file, line)) {
    items.push_back(deserializeItem(line));
  }

  return items;
}

void writeItemsToFile(const std::string &filename, const std::vector<Item> &items) {
  std::ofstream items_file(filename);

  items_file << "age,type,origin,rarity\n";

  for (const auto &item : items) {
    items_file << serializeItem(item) << '\n';
  }
}
