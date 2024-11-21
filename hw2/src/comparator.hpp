#pragma once

#include "item.h"
#include <functional>

#define maxm(a, b) a > b ? a : b
#define minm(a, b) a < b ? a : b

const std::string AGE_STR_NAME = "age";
const std::string TYPE_STR_NAME = "type";
const std::string ORIGIN_STR_NAME = "origin";
const std::string RARITY_SCORE_STR_NAME = "rarity"; // ?

class IItemAccessor {
public:
  virtual ~IItemAccessor();
  virtual double getProperty(const Item &item) const = 0;
};

class AgeAccessor final : public IItemAccessor {
public:
  double getProperty(const Item &item) const override;
};

class TypeAccessor final : public IItemAccessor {
public:
  double getProperty(const Item &item) const override;
};

class OriginAccessor final : public IItemAccessor {
public:
  double getProperty(const Item &item) const override;
};

class RarityAccessor final : public IItemAccessor {
public:
  double getProperty(const Item &item) const override;
};

using ItemComparsion = std::function<bool(const Item &, const Item &)>;

class ItemComparator {
public:
  ItemComparator(const std::string &attribute, bool ascending);
  bool operator()(const Item &l, const Item &r) const;
  double getKey(const Item &item) const;
  bool isAscending() const;

private:
  IItemAccessor *accessor;
  ItemComparsion comparison_function;
  bool ascending;
};
