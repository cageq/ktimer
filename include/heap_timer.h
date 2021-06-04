#pragma once 
#include "min_heap.h"
#include <functional> 
#include <unordered_map> 
#include <time.h> 
#include <chrono> 
#include <thread> 

class TimerNode ; 
using TimerHandler  = std::function<bool (std::shared_ptr<TimerNode >  )> ; 

using TimePoint  = std::chrono::time_point<std::chrono::system_clock>; 
#ifdef TIME_SCALE //TODO should move to template param  
using TimeScale  = TIME_SCALE; 
#else 
//using TimeScale  = std::chrono::microseconds; 
using TimeScale  = std::chrono::seconds; 
#endif 

template <class T> 
struct TimeUnit{
	constexpr static const char * short_notion = ""; 
}; 

template <> 
struct TimeUnit<std::chrono::microseconds>{
	constexpr static const char * short_notion = "ms"; 
}; 

template <> 
struct TimeUnit<std::chrono::milliseconds>{
	constexpr static const char * short_notion = "us"; 
}; 

template <> 
struct TimeUnit<std::chrono::seconds>{
	constexpr static const char * short_notion = "s"; 
}; 


struct TimerNode{
	TimerNode(){} 
	TimerNode(int32_t t , const TimerHandler & h, bool l = true   ) {
		handler   = h ; 
		interval  = t; 
		loop      = l ; 
		expire_time =   get_now() + TimeScale(t)  ; 
	}

	template < class T> 
		T * get_user_node(){
			return static_cast<T*>(this); 
		}

	static 	TimePoint get_now(){
		return std::chrono::system_clock::now();
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
	virtual ~UserTimerNode(){}	
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

		bool stop_timer(uint32_t timerId){
			std::lock_guard<Mutex> guard(timer_mutex); 
			auto itr = timer_nodes.find(timerId); 
			if (itr  != timer_nodes.end() ){
				itr->second->stopped = true; 
				timer_nodes.erase(itr); 
				return true; 
			}
			return false; 
		}


		void handle_timeout(TimerNodePtr node ) {
			bool rst = node->handler(node ); 
			if ( rst ) {
				if (node->loop && !node->stopped){
					node->expire_time += TimeScale(node->interval); 
					heap_tree.insert(node); 
				}
			}else {
				std::lock_guard<Mutex> guard(timer_mutex); 
				node->stopped = true; 
				timer_nodes.erase(node->timer_id ); 
			}

		}
		void start( bool async = false){ 
			is_running = true; 
			timer_start_point  = std::chrono::system_clock::now();
			if (async) {
				work_thread = std::thread(&HeapTimer::run, this); 
			}
			else {
				run(); 
			}
		}
		void stop(){
			is_running = false; 
			work_thread.join(); 
		}

	private: 
		uint32_t add_timer(TimerNodePtr    node ){
			static uint32_t timer_index = base_timer_index ; 
			node->timer_id = timer_index ++ ; 
			heap_tree.insert(node); 

			std::lock_guard<Mutex> guard(timer_mutex); 
			timer_nodes[node->timer_id] = node; 
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
						heap_tree.dump([this](uint32_t idx, TimerNodePtr node ){
								printf("[%d, %u, %ld%s] ",idx, node->timer_id,  
										std::chrono::duration_cast<std::chrono::microseconds>( node->expire_time - timer_start_point  ).count(),
										TimeUnit<TimeScale>::short_notion  );  
								}); 
						printf("\n"); 
					}
					std::tie(hasTop, node)  = heap_tree.top(); 
					cur = TimerNode::get_now(); 
				}

				if (node ) {
					auto nextExpire = node->expire_time - cur ; 
					if (nextExpire  > TimeScale(0) ) {
						std::this_thread::sleep_for(nextExpire);
					}
				} else {
					std::this_thread::sleep_for(TimeScale(1));
				}
			}
		}

		std::chrono::time_point<std::chrono::system_clock> timer_start_point ; 
		std::thread work_thread; 
		MinHeap<TimerNodePtr , CompareTimeNode , Mutex>  heap_tree; 
		bool is_running = false; 
		Mutex timer_mutex; 
		std::unordered_map<uint32_t , TimerNodePtr>  timer_nodes ; 
}; 


