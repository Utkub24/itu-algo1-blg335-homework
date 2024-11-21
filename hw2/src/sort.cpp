#include "comparator.hpp"
#include "methods.h"
#include <vector>

std::vector<Item> countingSortItems(const std::vector<Item> &items, const ItemComparator &comparator) {
  if (items.empty())
    return items;

  // Manually find min and max ages
  int minAge = comparator.getKey(items.front());
  int maxAge = comparator.getKey(items.front());

  for (const auto &item : items) {
    auto value = static_cast<int>(comparator.getKey(item));
    minAge = minm(minAge, value);
    maxAge = maxm(maxAge, value);
  }

  int range = maxAge - minAge + 1;

  // Create count array and initialize with zeros
  std::vector<std::vector<Item>> countArray(range);

  // Store items in count array based on age
  for (const auto &item : items) {
    auto value = static_cast<int>(comparator.getKey(item));
    countArray[value - minAge].push_back(item);
  }

  // Rebuild the sorted array
  std::vector<Item> sortedItems;
  sortedItems.reserve(items.size());

  if (comparator.isAscending()) {
    // Ascending order
    for (int i = 0; i < range; i++) {
      const auto &bucket = countArray[i];
      for (const auto &item : bucket) {
        sortedItems.push_back(item);
      }
    }
  } else {
    // Descending order
    for (int i = range - 1; i >= 0; i--) {
      const auto &bucket = countArray[i];
      for (const auto &item : bucket) {
        sortedItems.push_back(item);
      }
    }
  }

  return sortedItems;
}

std::vector<Item> countingSort(std::vector<Item> &items, const std::string &attribute, bool ascending) {
  ItemComparator comparator{attribute, ascending};

  return countingSortItems(items, comparator);
}

void heapifyItems(std::vector<Item> &items, int n, int i, const ItemComparator &comparator) {
  int largest = i;       // Initialize largest/smallest as root
  int left = 2 * i + 1;  // Left child
  int right = 2 * i + 2; // Right child

  // Compare with left child
  if (comparator.isAscending()) {
    // Max heap for ascending order
    if (left < n && comparator.getKey(items[left]) > comparator.getKey(items[largest])) {
      largest = left;
    }
    // Compare with right child
    if (right < n && comparator.getKey(items[right]) > comparator.getKey(items[largest])) {
      largest = right;
    }
  } else {
    // Min heap for descending order
    if (left < n && comparator.getKey(items[left]) < comparator.getKey(items[largest])) {
      largest = left;
    }
    // Compare with right child
    if (right < n && comparator.getKey(items[right]) < comparator.getKey(items[largest])) {
      largest = right;
    }
  }

  // If largest/smallest is not root
  if (largest != i) {
    std::swap(items[i], items[largest]);
    // Recursively heapify the affected sub-tree
    heapifyItems(items, n, largest, comparator);
  }
}

// Function to heapify a subtree rooted with node i in the array of items
void heapify(std::vector<Item> &items, int n, int i, bool descending) {
  ItemComparator comparator{RARITY_SCORE_STR_NAME, !descending};

  heapifyItems(items, n, i, comparator);
}

std::vector<Item> heapSortItems(std::vector<Item> &items, const ItemComparator &comparator) {
  if (items.empty()) {
    return items;
  }

  int size = items.size();

  std::vector<Item> sorted = items;

  // build heap
  for (int i = size / 2 - 1; i >= 0; i--) {
    heapifyItems(sorted, size, i, comparator);
  }

  // extract elements from heap one by one
  for (int i = size - 1; i > 0; i--) {
    // move current root to end
    std::swap(sorted[0], sorted[i]);

    // call heapify on the reduced heap
    heapifyItems(sorted, i, 0, comparator);
  }

  return sorted;
}

// Function to perform heap sort on rarityScore scores of items
std::vector<Item> heapSortByRarity(std::vector<Item> &items, bool descending) {
  ItemComparator comparator{RARITY_SCORE_STR_NAME, !descending};

  return heapSortItems(items, comparator);
}
