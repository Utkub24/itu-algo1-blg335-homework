#include "constants.hpp"
#include "tweet.h"
#include <chrono>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> dir_entries_to_vec(std::string dir) {
  std::vector<std::string> vec;

  for (const auto &entry : std::filesystem::directory_iterator(dir)) {
    if (entry.is_regular_file())
      vec.push_back(entry.path());
  }

  return vec;
}

std::string extractFilename(const std::string &path) {
  // Find the position of the last '/'
  size_t lastSlash = path.find_last_of('/');

  // Calculate the starting position of the filename
  size_t start = lastSlash + 1; // +1 to move past the '/'

  // The position of the dot is guaranteed to be 4 characters from the end
  size_t dotPosition = path.size() - 4; // 4th character from the end

  // Extract the filename using substr
  return path.substr(start, dotPosition - start);
}

template <typename T> void print_table_row_entry(const T &value) {
  std::cout << std::setw(TABLE_COLUMN_WIDTH) << std::right << value;
}

template <typename T> void print_table_row(const std::vector<T> &values) {
  for (const auto &value : values) {
    print_table_row_entry(value);
  }

  std::cout << std::endl;
}

void print_file_names_from_paths(const std::vector<std::string> &paths) {
  print_table_row_entry("");

  for (const auto &path : paths)
    print_table_row_entry(extractFilename(path));

  std::cout << std::endl;
}

using MetricFunction = std::function<int(const std::vector<Tweet> &)>;
using SortFunction = std::function<void(std::vector<Tweet> &)>;

std::chrono::duration<double> measure_metric(const MetricFunction &metric,
                                             const std::vector<Tweet> &tweets) {
  auto start = std::chrono::high_resolution_clock::now();
  metric(tweets);
  auto end = std::chrono::high_resolution_clock::now();

  return end - start;
}

std::chrono::duration<double> measure_sort(const SortFunction &sort,
                                           std::vector<Tweet> &tweets) {
  auto start = std::chrono::high_resolution_clock::now();
  sort(tweets);
  auto end = std::chrono::high_resolution_clock::now();

  return end - start;
}

void measure_and_print_sort_datasets(
    const SortFunction &sort, const std::vector<std::string> &dataset_paths) {
  std::vector<double> times;

  for (const auto &dataset_path : dataset_paths) {
    std::vector<Tweet> tweets = readTweetsFromFile(dataset_path);
    times.push_back(measure_sort(sort, tweets).count());
  }

  print_table_row(times);
}

void measure_and_print_metric_datasets(
    const MetricFunction &metric,
    const std::vector<std::string> &dataset_paths) {
  std::vector<double> times;

  for (const auto &dataset_path : dataset_paths) {
    times.push_back(
        measure_metric(metric, readTweetsFromFile(dataset_path)).count());
  }

  print_table_row(times);
}

void print_binary_search(std::vector<std::string> dataset_paths) {
  const long long key = 1773335;

  auto binarySearchLambda = [key](const std::vector<Tweet> &tweets) -> int {
    return binarySearch(tweets, key, TWEET_ID_STR_NAME);
  };

  print_table_row_entry("BinarySearch");
  measure_and_print_metric_datasets(binarySearchLambda, dataset_paths);
}

void print_threshold(std::vector<std::string> dataset_paths) {
  const int threshold = 250;

  auto thresholdLambda = [threshold](const std::vector<Tweet> &tweets) -> int {
    return countAboveThreshold(tweets, FAVORITE_COUNT_STR_NAME, threshold);
  };

  print_table_row_entry("Threshold");
  measure_and_print_metric_datasets(thresholdLambda, dataset_paths);
}

void print_metric_methods(const std::vector<std::string> &dataset_paths) {
  print_file_names_from_paths(dataset_paths);
  print_binary_search(dataset_paths);
  print_threshold(dataset_paths);
}

void print_bubble_sort(const std::vector<std::string> &dataset_paths,
                       const std::string &sort_by) {
  auto bubbleSortLambda = [&sort_by](std::vector<Tweet> &tweets) {
    bubbleSort(tweets, sort_by, true);
  };

  print_table_row_entry("BubbleSort");
  measure_and_print_sort_datasets(bubbleSortLambda, dataset_paths);
}

void print_insertion_sort(const std::vector<std::string> &dataset_paths,
                          const std::string &sort_by) {
  auto insertionSortLambda = [&sort_by](std::vector<Tweet> &tweets) {
    insertionSort(tweets, sort_by, true);
  };

  print_table_row_entry("InsertionSort");
  measure_and_print_sort_datasets(insertionSortLambda, dataset_paths);
}

void print_merge_sort(const std::vector<std::string> &dataset_paths,
                      const std::string &sort_by) {
  auto mergeSortLambda = [&sort_by](std::vector<Tweet> &tweets) {
    mergeSort(tweets, 0, tweets.size(), sort_by, true);
  };

  print_table_row_entry("MergeSort");
  measure_and_print_sort_datasets(mergeSortLambda, dataset_paths);
}

void print_sort_methods(const std::vector<std::string> &dataset_paths,
                        const std::string &sort_by) {

  std::cout << "Sorting by: " << sort_by << "\n\n";
  print_file_names_from_paths(dataset_paths);
  print_bubble_sort(dataset_paths, sort_by);
  print_insertion_sort(dataset_paths, sort_by);
  print_merge_sort(dataset_paths, sort_by);
}

void print_report_tables() {
  std::vector<std::string> sizes_datasets =
      dir_entries_to_vec(SIZES_DATASETS_DIR);

  std::vector<std::string> permutations_datasets =
      dir_entries_to_vec(PERMUTATIONS_DATASETS_DIR);

  print_metric_methods(sizes_datasets);

  std::cout << std::endl;

  print_sort_methods(sizes_datasets, TWEET_ID_STR_NAME);

  std::cout << std::endl;

  print_sort_methods(permutations_datasets, FAVORITE_COUNT_STR_NAME);
}

void printHelp() { std::cout << "Help, wrong arguments!\nTry 'report'.\n"; }

int main(int argc, char **argv) {
  if (argc != 2) {
    printHelp();
    return -1;
  }

  if (argc == 2) {
    std::string first_arg = argv[1];
    if (!first_arg.compare("report")) {
      print_report_tables();
    } else {
      printHelp();
      return -1;
    }
  }
}
