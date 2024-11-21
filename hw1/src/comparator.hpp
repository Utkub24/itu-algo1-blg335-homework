#include "tweet.h"
#include <functional>

class ITweetAccessor {
public:
  virtual ~ITweetAccessor();
  virtual long long getProperty(const Tweet &tweet) const = 0;
};

class IDAccessor final : public ITweetAccessor {
public:
  virtual long long getProperty(const Tweet &tweet) const;
};

class RetweetCountAccessor final : public ITweetAccessor {
public:
  virtual long long getProperty(const Tweet &tweet) const;
};

class FavoriteCountAccessor final : public ITweetAccessor {
public:
  virtual long long getProperty(const Tweet &tweet) const;
};

using TweetComparison = std::function<bool(const Tweet &, const Tweet &)>;

class TweetComparator {
public:
  TweetComparator(const std::string &sort_by, bool ascending);
  bool operator()(const Tweet &l, const Tweet &r) const;
  long long getKey(const Tweet &tweet) const;

private:
  ITweetAccessor *m_accessor;
  TweetComparison m_comparison_function;
};

ITweetAccessor *createTweetAccessor(const std::string &sort_by);
TweetComparison createTweetComparisonFunction(const ITweetAccessor &accessor,
                                              bool ascending);
