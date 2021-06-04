#include <stdio.h> 
#include <iostream> 

#include "heap_timer.h" 
bool handle_timeout(HeapTimer<>::TimerNodePtr node ){

	std::cout << "handle timeout 222 " << std::time(0) << std::endl; 

	return true; 
}

int main(int argc, char * argv[]){


	HeapTimer<>  heapTimer ; 


	heapTimer.start_timer(2, handle_timeout); 

//	heapTimer.start_timer(3, [](HeapTimer<>::TimerNodePtr node ){
//			auto unode = node->get_user_node<UserTimerNode<int>  > (); 
//			std::cout << "handle timeout 111 " << std::time(0) << std::endl ; 
//			//std::cout << "user data is  " << unode->user_data << std::endl ; 
//			return true; 
//
//			}, true , 3333); 


	heapTimer.start(); 


	return 0; 


} 
