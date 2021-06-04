#pragma once 
#include "min_heap.h"
#include <functional> 
#include <time.h> 
#include <chrono> 
#include <thread> 

class TimerNode ; 
using TimerHandler  = std::function<bool (std::shared_ptr<TimerNode >  )> ; 

//using TimePoint =  std::chrono::time_point<std::chrono::system_clock>; 
using TimePoint = time_t; 
struct TimerNode{
	TimerNode(){} 
	TimerNode(int32_t t , const TimerHandler & h, bool l = true   ) {
		handler = h ; 
		interval = t; 
		loop =l ; 
		expire_time =   get_now() + t  ; 
	}

	template < class T> 
		T * get_user_node(){
			return static_cast<T*>(this); 
		}

	static 	TimePoint get_now(){
		//std::chrono::system_clock::now();
		return time(0); 
	}
	uint32_t timer_id     = 0; 
	TimerHandler  handler = nullptr ; 
	int32_t  interval     = 0; 
	bool loop             = true; 
	bool stopped          = false; 
	TimePoint    expire_time  ; 
}; 
using TimerNodePtr = std::shared_ptr<TimerNode> ; 

struct CompareTimeNode {
	bool operator () (const TimerNodePtr  & node ,  const TimerNodePtr & other ){
		return node->expire_time < other->expire_time; 
	}
}; 

template <class T> 
struct UserTimerNode : public TimerNode{
	virtual ~UserTimerNode(){
	}	
	UserTimerNode(int32_t t , const TimerHandler & h, bool l = true   ):TimerNode(t, h, l)  {
	}
	T user_data ; 
}; 

template <class Mutex = std::mutex > 
class HeapTimer{

	public: 
		const uint32_t base_timer_index = 1024; 

		template <class T> 
			uint32_t start_timer(uint32_t interval , const TimerHandler & handler , bool loop , const T & udata ){
				auto node = std::make_shared<UserTimerNode<T> > (interval, handler, loop ); 
				node->user_data = udata; 
				return add_timer(node ); 
			}

		uint32_t start_timer(uint32_t interval , const TimerHandler & handler , bool loop = true){
			auto node = std::make_shared<TimerNode>(interval, handler, loop ); 
			return add_timer(node ); 
		}

		void handle_timeout(TimerNodePtr node ) {
			bool rst = node->handler(node ); 
			if ( rst ) {
				if (node->loop && !node->stopped){
					node->expire_time += node->interval; 
					heap_tree.insert(node); 
				}
			}else {
				node->stopped = true; 
			}

		}
		void start( bool async = false){ 
			is_running = true; 
			if (async) {
				//TODO need lock 
				work_thread = std::thread(&HeapTimer::run, this); 
			}
			else {
				run(); 
			}
		}
		void stop(){
			is_running = false; 
		}

	private: 
		uint32_t add_timer(TimerNodePtr    node ){
			static uint32_t timer_index = base_timer_index ; 
			node->timer_id = timer_index ++ ; 
			heap_tree.insert(node); 
			return node->timer_id; 
		}
		void run() {

			while(is_running){
				auto cur = TimerNode::get_now(); 
				bool hasTop = false; 
				TimerNodePtr  node = nullptr; 
				std::tie(hasTop, node)  = heap_tree.top(); 
				while (hasTop &&  node->expire_time <= cur) {
					heap_tree.pop(); 
					if (!node->stopped ) {
						handle_timeout(node); 
						heap_tree.dump([](uint32_t idx, TimerNodePtr node ){
								printf(" %d [ %u , %ld ] ",idx, node->timer_id, node->expire_time);  
								}); 
						printf("\n"); 
					}

					std::tie(hasTop, node)  = heap_tree.top(); 
					cur = TimerNode::get_now(); 
				}

				if (node ) {
					uint32_t nextExpire = node->expire_time - cur ; 
					if (nextExpire  > 0 ) {
						std::this_thread::sleep_for(std::chrono::microseconds(nextExpire * 1000000));
					}
				} else {
					std::this_thread::sleep_for(std::chrono::microseconds(100));
				}
			}
		}

		std::thread work_thread; 
		MinHeap<TimerNodePtr , CompareTimeNode , Mutex>  heap_tree; 
		bool is_running = false; 
}; 


