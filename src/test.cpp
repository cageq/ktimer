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
	heapTimer.start_timer(3, [](HeapTimer<>::TimerNodePtr node) {
		   
		
		std::cout << "handle timeout 333 " << std::time(0) << std::endl; 
			
		return true; //don't forget to return,   false will stop the timer 
			}	); 

	heapTimer.start(); 


	return 0; 


} 
