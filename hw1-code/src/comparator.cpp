#include "comparator.hpp"
#include "constants.hpp"
#include <string>

ITweetAccessor::~ITweetAccessor() {}

long long IDAccessor::getProperty(const Tweet &tweet) const {
  return tweet.tweetID;
}

long long RetweetCountAccessor::getProperty(const Tweet &tweet) const {
  return tweet.retweetCount;
}

long long FavoriteCountAccessor::getProperty(const Tweet &tweet) const {
  return tweet.favoriteCount;
}

ITweetAccessor *createTweetAccessor(const std::string &sort_by) {
  if (!sort_by.compare(TWEET_ID_STR_NAME))
    return new IDAccessor();
  else if (!sort_by.compare(RETWEET_COUNT_STR_NAME))
    return new RetweetCountAccessor();
  else if (!sort_by.compare(FAVORITE_COUNT_STR_NAME))
    return new FavoriteCountAccessor();
  else
    return new IDAccessor();
}

TweetComparison createTweetComparisonFunction(const ITweetAccessor &accessor,
                                              bool ascending) {
  if (ascending) {
    return [&accessor](const Tweet &l, const Tweet &r) -> bool {
      return accessor.getProperty(l) > accessor.getProperty(r);
    };
  } else {
    return [&accessor](const Tweet &l, const Tweet &r) -> bool {
      return accessor.getProperty(l) < accessor.getProperty(r);
    };
  }
}

TweetComparator::TweetComparator(const std::string &sort_by, bool ascending) {
  m_accessor = createTweetAccessor(sort_by);
  m_comparison_function = createTweetComparisonFunction(*m_accessor, ascending);
}

bool TweetComparator::operator()(const Tweet &l, const Tweet &r) const {
  return m_comparison_function(l, r);
}

long long TweetComparator::getKey(const Tweet &tweet) const {
  return m_accessor->getProperty(tweet);
}
