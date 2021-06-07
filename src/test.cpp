#include <stdio.h> 
#include <iostream> 

#include "ktimer.h" 
bool handle_timeout(){

	std::cout << "handle timeout 222 " << std::time(0) << std::endl; 

	return true; 
}

int main(int argc, char * argv[]){


	ktimer::KTimer<std::chrono::seconds, std::mutex>  myTimer ; 


	myTimer.start_timer(2, handle_timeout); 
	myTimer.start_timer(3, []() {
		   
		
		std::cout << "handle timeout 333 " << std::time(0) << std::endl; 
			
		return true; //don't forget to return,   false will stop the timer 
			}); 

	myTimer.start_timer(5, []() {
			std::cout << "handle timeout 555 " << std::time(0) << std::endl; 
return true; 
			}); 
	myTimer.start(true); 

	getchar(); 

	return 0; 


} 
