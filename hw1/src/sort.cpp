#include "comparator.hpp"
#include "tweet.h"
#include <utility>
#include <vector>

void bubbleSortTweets(std::vector<Tweet> &tweets,
                      const TweetComparator &comparator) {
  int size = tweets.size();
  bool swapped = false;

  for (int i = 0; i < size - 1; i++) {
    swapped = false;
    for (int j = 0; j < size - i - 1; j++) {
      if (comparator(tweets[j], tweets[j + 1])) {
        std::swap(tweets[j], tweets[j + 1]);
        swapped = true;
      }
    }

    if (!swapped)
      break;
  }
}

void bubbleSort(std::vector<Tweet> &tweets, const std::string &sortBy,
                bool ascending) {
  TweetComparator comparator(sortBy, ascending);

  bubbleSortTweets(tweets, comparator);
}

void insertionSortTweets(std::vector<Tweet> &tweets,
                         const TweetComparator &comparator) {
  int size = tweets.size();

  for (int i = 1; i < size; i++) {
    Tweet key = tweets[i];
    int j = i - 1;

    while (j >= 0 && comparator(tweets[j], key)) {
      /*std::swap(tweets[j], tweets[j + 1]);*/
      tweets[j + 1] = tweets[j];
      j--;
    }

    tweets[j + 1] = key;
  }
}

void insertionSort(std::vector<Tweet> &tweets, const std::string &sortBy,
                   bool ascending) {
  TweetComparator comparator(sortBy, ascending);

  insertionSortTweets(tweets, comparator);
}

void mergeTweets(std::vector<Tweet> &tweets, int left, int mid, int right,
                 const TweetComparator &comparator) {
  int left_n = mid - left + 1;
  int right_n = right - mid;

  std::vector<Tweet> left_vec(left_n);
  std::vector<Tweet> right_vec(right_n);

  for (int i = 0; i < left_n; i++)
    left_vec[i] = tweets[left + i];

  for (int i = 0; i < right_n; i++)
    right_vec[i] = tweets[mid + 1 + i];

  int i = 0, j = 0;
  int k = left;

  while (i < left_n && j < right_n) {
    if (comparator(right_vec[j], left_vec[i])) {
      tweets[k] = left_vec[i];
      i++;
    } else {
      tweets[k] = right_vec[j];
      j++;
    }
    k++;
  }

  while (i < left_n) {
    tweets[k] = left_vec[i];
    i++;
    k++;
  }

  while (j < right_n) {
    tweets[k] = right_vec[j];
    j++;
    k++;
  }
}

void merge(std::vector<Tweet> &tweets, int left, int mid, int right,
           const std::string &sortBy, bool ascending) {
  TweetComparator comparator(sortBy, ascending);

  mergeTweets(tweets, left, mid, right, comparator);
}

void mergeSortTweets(std::vector<Tweet> &tweets, int left, int right,
                     const TweetComparator &comparator) {
  if (left >= right)
    return;

  int mid = left + (right - left) / 2;
  mergeSortTweets(tweets, left, mid, comparator);
  mergeSortTweets(tweets, mid + 1, right, comparator);
  mergeTweets(tweets, left, mid, right, comparator);
}

void mergeSort(std::vector<Tweet> &tweets, int left, int right,
               const std::string &sortBy, bool ascending) {
  TweetComparator comparator(sortBy, ascending);

  mergeSortTweets(tweets, left, right, comparator);
}
