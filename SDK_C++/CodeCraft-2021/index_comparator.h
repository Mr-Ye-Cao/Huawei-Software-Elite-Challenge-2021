// @Author: Yu Xin

#pragma once

template <class custom_iterator, class custom_object>
class IndexComparator {
  public:
    IndexComparator(custom_iterator begin, custom_iterator end, bool (*comparator)(const custom_object&, const custom_object&)) :
      begin_(begin), end_(end), comparator_(comparator) {}
    ~IndexComparator() = default;
    bool operator()(uint32_t a, uint32_t b) const
        { return comparator_(*(begin_ + a), *(begin_ + b)); }

  private:
    const custom_iterator begin_;
    const custom_iterator end_;
    bool (*comparator_)(const custom_object&, const custom_object&);
};
