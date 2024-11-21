#include "comparator.hpp"
#include "methods.h"
#include <vector>

int getMaxItems(std::vector<Item> &items, const ItemComparator &comparator) {
  auto max = comparator.getKey(items.front());

  for (const auto &item : items) {
    auto value = static_cast<int>(comparator.getKey(item));
    max = maxm(max, value);
  }

  return max;
}

int getMax(std::vector<Item> &items, const std::string &attribute) {
  ItemComparator comparator{attribute, true};

  return getMaxItems(items, comparator);
}

bool areItemsSimilar(const Item &l, const Item &r) {
  return l.type == r.type && l.origin == r.origin;
}

double probabilityOfItemInList(const Item &item, const std::vector<Item> &items) {
  // NOTE: doesn't check if item is actually in items
  int countTotal = items.size();
  if (countTotal == 0)
    return 0;

  int countSimilar = 0;
  for (const auto &i : items) {
    if (areItemsSimilar(i, item)) {
      countSimilar++;
    }
  }

  double probability = static_cast<double>(countSimilar) / countTotal;

  return probability;
}

std::vector<Item> getItemsWithinAgeWindow(const std::vector<Item> &items, int minAge, int maxAge) {
  // <ranges> could've been very useful here
  std::vector<Item> filtered;
  filtered.reserve(items.size()); // reserve more than enough memory

  for (const auto &item : items) {
    int itemAge = item.age;
    if (itemAge >= minAge && itemAge <= maxAge) {
      filtered.push_back(item);
    }
  }

  filtered.shrink_to_fit(); // not necessary

  return filtered;
}

double calculateRarityForItem(const Item &item, const std::vector<Item> &items, int ageWindow, int maxAge) {
  int itemAge = item.age;
  int windowMin = itemAge - ageWindow;
  int windowMax = itemAge + ageWindow;

  std::vector<Item> filtered = getItemsWithinAgeWindow(items, windowMin, windowMax);
  double probability = probabilityOfItemInList(item, filtered);
  double rarity = (1 - probability) * (1 + static_cast<double>(itemAge) / maxAge);

  return rarity;
}

// min = age - ageWindow
// max = age + ageWindow
// rarityScore = (1 - probability) * (1 + item.age/ageMax)
void calculateRarityScores(std::vector<Item> &items, int ageWindow) {
  int maxAge = getMax(items, AGE_STR_NAME);

  for (auto &item : items) {
    item.rarityScore = calculateRarityForItem(item, items, ageWindow, maxAge);
  }
}
