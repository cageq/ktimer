#pragma once
#include <cmath>
#include <functional>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>
struct NoneMutex {
  inline void lock() {}
  inline void unlock() {}
};

template <class T, class Compare = std::less<T>, class Mutex = NoneMutex>
class MinHeap {

public:
  explicit MinHeap(uint32_t initSize = 1024) { heap_data.reserve(initSize); }
  inline size_t size() const {
    std::lock_guard<Mutex> guard(heap_mutex);
    return heap_data.size();
  }
  inline bool empty() const {
    std::lock_guard<Mutex> guard(heap_mutex);
    return heap_data.empty();
  }
  void insert(const T &item) {
    std::lock_guard<Mutex> guard(heap_mutex);
    heap_data.push_back(item);
    if (heap_data.size() > 1) {
      sift_up(heap_data.size() - 1);
    }
  }

  void dump(std::function<void(uint32_t, T)> output) {
    std::lock_guard<Mutex> guard(heap_mutex);
    uint32_t idx = 0;
    for (auto &item : heap_data) {
      output(idx++, item);
    }
  }
  std::tuple<bool, T> pop() {
    static T empty;
    std::lock_guard<Mutex> guard(heap_mutex);
    if (!heap_data.empty()) {
      auto &top = heap_data[0];
      std::swap(heap_data[0], heap_data[heap_data.size() - 1]);
      heap_data.pop_back();
      sift_down(0);
      return {true, top};
    }
    return {false, empty};
  }

  // T pop() {
  //	if (!heap_data.empty()) {
  //		auto & top = heap_data[0];
  //		std::swap(heap_data[0], heap_data[heap_data.size() - 1]);
  //		heap_data.pop_back();
  //		sift_down(0);
  //		return top;
  //	}
  //	return T();
  //}

  std::tuple<bool, T> top() {
    static T empty;
    std::lock_guard<Mutex> guard(heap_mutex);
    if (heap_data.empty()) {
      return {false, empty};
    }
    return {true, heap_data[0]};
  }

private:
  bool less_item(const T &left, const T &right) { return Compare{}(left, right); }

  void sift_up(int k) {
    while (k > 0 && less_item(heap_data[k], heap_data[parent(k)])) {
      std::swap(heap_data[k], heap_data[parent(k)]);
      k = parent(k);
    }
  }

  void sift_down(int k) {

    while (left_child(k) < heap_data.size()) {
      uint32_t j = left_child(k);
      if (j + 1 < heap_data.size() && less_item(heap_data[j + 1], heap_data[j])) {
        j++;
      }

      if (less_item(heap_data[j], heap_data[k])) {
        break;
      }
      std::swap(heap_data[k], heap_data[j]);
      k = j;
    }
  }

private:
  uint32_t parent(uint32_t pos) { return pos == 0 ? 0 : ((pos - 1) >> 1); }
  uint32_t left_child(uint32_t pos) { return (pos << 1) + 1; }
  uint32_t right_child(uint32_t pos) { return (pos << 1) + 2; }
  std::vector<T> heap_data;
  Mutex heap_mutex;
};
