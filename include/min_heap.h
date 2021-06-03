#pragma once 
#include <functional>
#include <string> 
#include <vector> 
#include <tuple> 
#include <cmath> 


template <class T  , class Compare  = std::less<T> >
class MinHeap {

	public:

		explicit  MinHeap(uint32_t maxSize = 1024) {}
		inline size_t size() { return heap_data.size(); } 
		inline bool empty() { return heap_data.empty(); }
		void dump() {
			uint32_t level = (heap_data.size() - 1) >> 1;
			for (uint32_t i = 0; i <= level; i++) {
				std::string levelInfo;
				uint32_t beg = std::pow(2, i) - 1;
				uint32_t end = beg + std::pow(2, i);
				for (uint32_t j = beg; j < end && j < heap_data.size(); j++) {
					auto item = heap_data[j];
					levelInfo.append(std::to_string(item) + ", ");
				}
				//dlog("level {}    : {}", i, levelInfo);
			}

			std::string dumpInfo;
			for (auto& item : heap_data) {
				dumpInfo.append(std::to_string(item) + ", ");
			}
			//dlog("dump heap {}", dumpInfo);
		}

		void insert(const T& item) { heap_data.push_back(item); }

		void sift_up(int k) {
			while (k > 0 && Compare{}(heap_data[k] , heap_data[parent(k)] ) ) {
				std::swap(heap_data[k], heap_data[parent(k)]);
				k = parent(k);
			}
		}
		bool myless(const T & left, const T & right) {

			return Compare{}(left, right) ; 
		}

		void sift_down(int k) {
			while (left_child(k) < heap_data.size()) {
				uint32_t j = left_child(k);
				if (j + 1 < heap_data.size() && myless(heap_data[j +1 ],  heap_data[j]  ) )  {
					j++;
				}

				if (myless(heap_data[j] , heap_data[k] )  ) {
					break;
				}
				std::swap(heap_data[k], heap_data[j]);
				k = j;
			}
		}

		T pop() {
			if (!heap_data.empty()) {
				auto top = heap_data[0];
				std::swap(heap_data[0], heap_data[heap_data.size() - 1]);
				heap_data.pop_back();
				sift_down(0);
				return top;
			}
			return T();
		}

		std::tuple<bool , T & >  top() {
			static T empty; 
			if (heap_data.empty()) {
				return {false, empty} ;
			}
			return {true, heap_data[0]}; 
		}

	private:
		uint32_t parent(uint32_t pos) {
			if (pos == 0) {
				return 0;
			}
			return ((pos - 1) >> 1);
		}
		uint32_t left_child(uint32_t pos) { return pos * 2 + 1; }
		uint32_t right_child(uint32_t pos) { return pos * 2 + 2; }

		std::vector<T> heap_data;
};


