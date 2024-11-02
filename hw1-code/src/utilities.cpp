#include "tweet.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Tweet deserializeTweet(const std::string &tweet_str) {
  // Expected format: tweetID,retweet_count,favorite_count
  std::istringstream stream(tweet_str);

  std::string tweet_id_str;
  std::getline(stream, tweet_id_str, ',');

  std::string retweet_count_str;
  std::getline(stream, retweet_count_str, ',');

  std::string favorite_count_str;
  std::getline(stream, favorite_count_str, ',');

  long long tweet_id = std::stoll(tweet_id_str);
  int retweet_count = std::stoi(retweet_count_str);
  int favorite_count = std::stoi(favorite_count_str);

  return Tweet{tweet_id, retweet_count, favorite_count};
}

void deserializeTweetIntoVector(const std::string &tweet_str,
                                std::vector<Tweet> &tweets) {
  // Expected format: tweetID,retweet_count,favorite_count
  std::istringstream stream(tweet_str);

  std::string tweet_id_str;
  std::getline(stream, tweet_id_str, ',');

  std::string retweet_count_str;
  std::getline(stream, retweet_count_str, ',');

  std::string favorite_count_str;
  std::getline(stream, favorite_count_str, ',');

  long long tweet_id = std::stoll(tweet_id_str);
  int retweet_count = std::stoi(retweet_count_str);
  int favorite_count = std::stoi(favorite_count_str);

  tweets.emplace_back(tweet_id, retweet_count, favorite_count);
}

std::string serializeTweeet(const Tweet &tweet) {
  std::ostringstream stream;

  stream << tweet.tweetID << ',' << tweet.retweetCount << ','
         << tweet.favoriteCount;

  return stream.str();
}

std::vector<Tweet> readTweetsFromFile(const std::string &filename) {
  std::ifstream tweets_file(filename);

  std::vector<Tweet> tweets;

  std::string line;

  // Discard first line: tweetID,retweet_count,favorite_count
  std::getline(tweets_file, line);

  while (std::getline(tweets_file, line)) {
    deserializeTweetIntoVector(line, tweets);
  }

  return tweets;
}

void writeTweetsToFile(const std::string &filename,
                       const std::vector<Tweet> &tweets) {
  std::ofstream tweets_file(filename);

  tweets_file << "tweetID,retweet_count,favorite_count\n";

  for (const Tweet &tweet : tweets) {
    tweets_file << serializeTweeet(tweet) << '\n';
  }
}
