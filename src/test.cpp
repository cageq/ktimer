#include <stdio.h> 
#include <iostream> 

#include "ktimer.h" 
bool handle_timeout(){

	std::cout << "handle timeout 222 " << std::time(0) << std::endl; 

	return true; 
}

int main(int argc, char * argv[]){


	ktimer::KTimer<>  heapTimer ; 


	heapTimer.start_timer(2, handle_timeout); 
	heapTimer.start_timer(3, []() {
		   
		
		std::cout << "handle timeout 333 " << std::time(0) << std::endl; 
			
		return true; //don't forget to return,   false will stop the timer 
			}	); 

	heapTimer.start(); 


	return 0; 


} 
