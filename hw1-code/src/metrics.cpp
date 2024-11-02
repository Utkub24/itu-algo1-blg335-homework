#include "comparator.hpp"
#include "tweet.h"

int binarySearchTweets(const std::vector<Tweet> &tweets,
                       const ITweetAccessor &accessor, long long key) {
  int lower = 0;
  int upper = tweets.size();

  while (lower <= upper) {
    int mid = lower + (upper - lower) / 2;
    long long currentKey = accessor.getProperty(tweets[mid]);

    if (currentKey == key)
      return mid;

    if (currentKey < key)
      lower = mid + 1;
    else
      upper = mid - 1;
  }

  return -1;
}

int binarySearch(const std::vector<Tweet> &tweets, long long key,
                 const std::string &sortBy) {
  ITweetAccessor *accessor = createTweetAccessor(sortBy);

  return binarySearchTweets(tweets, *accessor, key);
}

int countAboveThresholdTweets(const std::vector<Tweet> &tweets,
                              const ITweetAccessor &accessor, int threshold) {
  int count = 0;

  for (const Tweet &tweet : tweets)
    if (accessor.getProperty(tweet) > threshold)
      count++;

  return count;
}

int countAboveThreshold(const std::vector<Tweet> &tweets,
                        const std::string &metric, int threshold) {
  ITweetAccessor *accessor = createTweetAccessor(metric);

  return countAboveThresholdTweets(tweets, *accessor, threshold);
}
