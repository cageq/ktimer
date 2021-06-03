#include <stdio.h> 
#include "heap_timer.h" 
#include <iostream> 

void handle_timeout(){

	printf("handle timeout2222 \n"); 
}

int main(int argc, char * argv[]){


	HeapTimer heapTimer ; 



	heapTimer.start_timer(3, [](TimerNode * node ){

			auto unode = node->get_user_node<UserTimerNode<int>  > (); 
			std::cout << " handle timeout  " << std::time(0) << std::endl ; 
			std::cout << "user data is  " << unode->user_data << std::endl ; 

			return true; 

			}, true , 3333); 

	//heapTimer.start_timer(2, handle_timeout); 

	heapTimer.start(); 


	return 0; 


} 
