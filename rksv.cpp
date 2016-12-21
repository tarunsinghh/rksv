#include <iostream>
#include<vector>
#include<thread>
#include <unistd.h>
#include<algorithm>
using namespace std;

void callback(int wait_time)
{
	cout<<"The process waited for ::"<<wait_time<<"seconds";cout<<"\n";

}
 

void doSleep(int wait_time,int actual_time)
{

	sleep(wait_time);

	if(actual_time != 0)
		callback(actual_time);
	else
		callback(wait_time);
		
}


class ASYNC
{	
	private:
	vector<int> buffer;
	std::vector<std::thread> v;
	unsigned int request_processed ;
	unsigned int request_dropped ;
	unsigned int max_request ;
	unsigned int max_thread ;

    
	public:

	ASYNC(unsigned int thread_pool_size, unsigned int request_buffer_size)
	{	
	
		max_request = request_buffer_size;
		max_thread  = thread_pool_size;
		request_processed = 0;
	
	}
	
	
	void async_wait(int sleep_time_in_ms, void (*callback)(int))
	{
		if(request_processed > max_request)
		{
			cout<< "Cant process your request at this time,try after some time";
			++ request_dropped;

		}
		else
		{
			
			if(find(buffer.begin(), buffer.end(), sleep_time_in_ms/1000) == buffer.end())
			{
  				buffer.push_back(sleep_time_in_ms/1000);
  				++ request_processed;
			} 

			
		}
	}

	void run() 
	{	
		int count =0;int request_pending = 0 ;	
		
		if(request_processed > max_thread)
			count = max_thread;
		else
			count = request_processed; 
		
	
		for(int i= 0; i< count;  ++ i)
		{
			v.push_back(std::thread(doSleep,buffer[i],0));
			request_pending = request_pending + 1 ;
				
		}
		
		while(request_pending != request_processed )
		{	
			v[0].join();
			v.erase(v.begin());
        	v.push_back(std::thread(doSleep,(buffer[request_pending]-buffer[request_pending - max_thread]),buffer[request_pending]));
			request_pending = request_pending + 1 ;
			
		}
		
		count = v.size();
		for(int i= 0; i< count; ++ i)
		{
			v[0].join();
			v.erase(v.begin());
		}
		
			
		request_processed = 0;
		buffer.clear();
		v.clear();

	}

	void exit()
	{
		std::exit;
	}
};

int main()
{
	ASYNC* async = new ASYNC(2,9);
	async->async_wait(1000, &callback);
	async->async_wait(2000, &callback);
	async->async_wait(4000, &callback);
	async->async_wait(8000, &callback);
	async->async_wait(9000, &callback);
	async->async_wait(5000, &callback);
	async->async_wait(9000, &callback);
	async->run(); 
	async->async_wait(7000, &callback);
	async->async_wait(13000,&callback);
	async->async_wait(11000,&callback);
	async->run(); 
	return 0;
}




