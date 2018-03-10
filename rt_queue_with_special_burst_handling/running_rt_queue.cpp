#include "running_rt_queue.h"
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
// ===============================
// AUTHOR      : Nir Levy
// CREATE DATE : 10/03/2018
// PURPOSE     : supplying realtime cyclic buffer (mutex free) 
//                  once buffer is full due to burstiness, close to RT handling (mutex + condition_variable) in stl::queue.
//               objects - rt_queue_data (can be modified)
//               RT_QUEUE_SIZE is the queue data
//               
//               Burstiness handling is only when cyclic buffer is full and push is required.
//               once reader try_pop it checks only for extra_buffer_in_use flag.
//               in case of extra_buffer_in_use equal false it do it's cyclic routine.
//               when 
// SPECIAL NOTES:
// ===============================
// Change History:
//
//==================================
struct rt_queue_data {
	char data[40] = { 0 };
	struct rt_queue_data(std::string const& message) {
		strcpy_s(data,message.c_str());
	}
	struct rt_queue_data() {
	}
};

template<typename T>
class rt_queue
{
	unsigned int reader_pos;
	unsigned int writer_pos;
	bool extra_buffer_in_use;
	unsigned int max_rt_elements;
	
	/*REAL-TIME buffer*/
	T *cyclic_buffer;

	/*Burst handling buffer*/
	std::queue<T> extra_data;
	mutable std::mutex extra_data_mutex;
	std::condition_variable extra_data_cond;
public:
	rt_queue() {}
	~rt_queue() {}

	void init(int max_rt_elements) {
		reader_pos = 0;
		writer_pos = 0;
		extra_buffer_in_use = false;
		cyclic_buffer = new T[max_rt_elements];
		this->max_rt_elements = max_rt_elements;
	}


	void push(T& new_element) {
		unsigned int next_writer_pos = writer_pos + 1;
		if (next_writer_pos >= max_rt_elements) {
			next_writer_pos = 0;
		}
		if (next_writer_pos == reader_pos)
		{			
			std::lock_guard<std::mutex> lk(extra_data_mutex);
			//printf("extra_data! pushing new element rhs %s\n", new_element.data);
			extra_data.push(new_element);
			extra_buffer_in_use = true;
			extra_data_cond.notify_all();			
		}
		else
		{
			//printf("push new element rhs %s\n", new_element.data);
			cyclic_buffer[writer_pos] = new_element;
			writer_pos = next_writer_pos;
		}
	}


	bool try_pop(T& value){
		printf("pop element\n");

		if ((reader_pos == writer_pos) && !extra_buffer_in_use)
			return false;

		unsigned int next_reader_pos = reader_pos + 1;
		if (next_reader_pos >= max_rt_elements) {
			next_reader_pos = 0;
		}
		value = cyclic_buffer[reader_pos];

		if (extra_buffer_in_use)
		{
			printf("queue_full, extra_data size:%zu\n", extra_data.size());
			/*when we suspect that the extra data has info we move it into main buffer*/
			//check queue
			std::unique_lock<std::mutex> lk(extra_data_mutex);			
			extra_data_cond.wait(lk, [this] {return (extra_data.size()); });
			if (extra_data.size())
			{	
				auto tmp = extra_data.front();
				extra_data.pop();
				cyclic_buffer[reader_pos] = tmp;
				reader_pos = next_reader_pos;
				if (!extra_data.size())
					extra_buffer_in_use = false;
			}
			else
			{
				abort();
				reader_pos = next_reader_pos;
			}
		}
		else
		{
			/*Normal increament of reader_pos*/
			reader_pos = next_reader_pos;
		}

		return true;


	}


//	bool try_pop(T& value)

};



running_rt_queue::running_rt_queue()
{
}


running_rt_queue::~running_rt_queue()
{
}

#define RT_QUEUE_SIZE (10)
void running_rt_queue::operator()()
{
	rt_queue<rt_queue_data> main_queue;// = new rt_queue<rt_queue_data>();
	main_queue.init(RT_QUEUE_SIZE);
	

	/*
	TEST case:
	producer thread put packets in burst and then 'lets' the consumer thread to catch it by sleeping twice as long
	each thread received lambda that runs put/get packets.
	*/
	std::thread producer([&](){
		static int i = 0;
		static std::string prefix = "Message ";
		std::string data;

		/*this is only for testing burst*/

		for (int i = 0; i < 20; i++)
		{
			data = "burst packets " + std::to_string(i);
			rt_queue_data message = rt_queue_data(data);
			main_queue.push(message);
		}

		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			data = prefix + std::to_string(i++);
			rt_queue_data message = rt_queue_data(data);
			main_queue.push(message);			
		}		
	});

	std::thread consumer([&]() {		
		while (true)
		{
			rt_queue_data obj;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if( main_queue.try_pop(obj))			
				std::cout << " object received: "<< obj.data <<std::endl;
		}
		
	});
	producer.join();
	consumer.join();

}