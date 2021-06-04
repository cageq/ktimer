# KTimer

simple useable heap timer . 


## Samples 

```cpp 

#include "ktimer.h"

ktimer::KTimer<std::chrono::seconds, std::mutex>  myTimer ; //default ktimer::KTimer<> myTimer ; 


myTimer.start_timer(3, []() {
		std::cout << "handle timeout 333 " << std::time(0) << std::endl; 

		return true; //don't forget to return,   false will stop the timer 
		
}); 


```
