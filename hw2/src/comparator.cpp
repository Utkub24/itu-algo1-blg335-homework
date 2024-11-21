#include "comparator.hpp"

IItemAccessor::~IItemAccessor() {}

double AgeAccessor::getProperty(const Item &item) const {
  return item.age;
}

double TypeAccessor::getProperty(const Item &item) const {
  return item.type;
}

double OriginAccessor::getProperty(const Item &item) const {
  return item.origin;
}

double RarityAccessor::getProperty(const Item &item) const {
  return item.rarityScore;
}

IItemAccessor *createItemAccessor(const std::string &attribute) {
  if (!attribute.compare(AGE_STR_NAME))
    return new AgeAccessor();
  else if (!attribute.compare(TYPE_STR_NAME))
    return new TypeAccessor();
  else if (!attribute.compare(ORIGIN_STR_NAME))
    return new OriginAccessor();
  else if (!attribute.compare(RARITY_SCORE_STR_NAME))
    return new RarityAccessor();
  else
    return new AgeAccessor();
}

ItemComparsion createItemComparisonFunction(const IItemAccessor &accessor, bool ascending) {
  if (ascending) {
    return [&accessor](const Item &l, const Item &r) -> bool {
      return accessor.getProperty(l) > accessor.getProperty(r);
    };
  } else {
    return [&accessor](const Item &l, const Item &r) -> bool {
      return accessor.getProperty(l) < accessor.getProperty(r);
    };
  }
}

ItemComparator::ItemComparator(const std::string &attribute, bool ascending)
    : ascending(ascending) {
  accessor = createItemAccessor(attribute);
  comparison_function = createItemComparisonFunction(*accessor, ascending);
}

bool ItemComparator::operator()(const Item &l, const Item &r) const {
  return comparison_function(l, r);
}

double ItemComparator::getKey(const Item &tweet) const {
  return accessor->getProperty(tweet);
}

bool ItemComparator::isAscending() const {
  return ascending;
}
