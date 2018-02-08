#include "running_rt_queue.h"
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <mutex>


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
	unsigned int max_rt_elements;
	
	/*REAL-TIME buffer*/
	T *cyclic_buffer;

	/*Burst handling buffer*/
	std::queue<T> extra_data;
	std::mutex extra_data_mutex;
public:
	rt_queue() {}
	~rt_queue() {}
	void init(int max_rt_elements) {
		reader_pos = 0;
		writer_pos = 0;
		cyclic_buffer = new T[max_rt_elements];
		this->max_rt_elements = max_rt_elements;
	}

	void push(T&& new_element) {
		unsigned int next_writer_pos = writer_pos + 1;
		if (next_writer_pos > max_rt_elements) {
			next_writer_pos = 0;
		}
		if (next_writer_pos == reader_pos )
		{
			printf("extra_data! pushing new element rhs %s\n", new_element.data);
			std::lock_guard<std::mutex> lk(extra_data_mutex);
			extra_data.push(new_element);
		}
		else
		{
			printf("push new element rhs %s\n", new_element.data);
			cyclic_buffer[writer_pos] = new_element;
			writer_pos = next_writer_pos;
		}
	}

	void push(T& new_element) {
		unsigned int next_writer_pos = writer_pos + 1;
		if (next_writer_pos > max_rt_elements) {
			next_writer_pos = 0;
		}
		if (next_writer_pos == reader_pos)
		{
			printf("extra_data! pushing new element rhs %s\n", new_element.data);
			std::lock_guard<std::mutex> lk(extra_data_mutex);
			extra_data.push(new_element);
		}
		else
		{
			printf("push new element rhs %s\n", new_element.data);
			cyclic_buffer[writer_pos] = new_element;
			writer_pos = next_writer_pos;
		}
	}


	std::shared_ptr<T> pop() {
		printf("pop element\n");

		if (reader_pos == writer_pos)
			return NULL;

		unsigned int next_reader_pos = reader_pos + 1;
		if (next_reader_pos >= max_rt_elements) {
			next_reader_pos = 0;
		}
		auto ret = std::make_shared<T>(cyclic_buffer[reader_pos]);		

		
		if (reader_pos == writer_pos % 10)
		{
			/*when we suspect that the extra data has info we move it into main buffer*/
			//check queue
			std::lock_guard<std::mutex> lk(extra_data_mutex);
			if (extra_data.size())
			{
				auto ret = extra_data.front();
				extra_data.pop();
				cyclic_buffer[reader_pos] = ret;
			}
		}
		else
		{
			/*Normal increament of reader_pos*/
			reader_pos = next_reader_pos;
		}

		return ret;


	}

};



running_rt_queue::running_rt_queue()
{
}


running_rt_queue::~running_rt_queue()
{
}


void running_rt_queue::operator()()
{
	rt_queue<rt_queue_data> main_queue;// = new rt_queue<rt_queue_data>();
	main_queue.init(10);
	

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
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::shared_ptr<rt_queue_data> obj = main_queue.pop();
			if (obj)
				std::cout << " object received: "<< obj->data <<std::endl;
		}
		
	});
	producer.join();
	consumer.join();

}