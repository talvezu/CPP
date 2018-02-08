#pragma once


class running_rt_queue
{
public:
	running_rt_queue();
	~running_rt_queue();
	void operator()();
};

