#include <cstdint> //uint8_t
#include <chrono>
#include <functional> //std::function
#include <iostream>   //std::cout
#include <thread>     //std::thread
#include <map>

#include <stdint.h>
class Handler {
	enum class Phase: uint8_t {
	phase_0_idx = 0,
	phase_1_idx = 1,
	phase_2_idx = 2,
	indeterminate = (uint8_t)-1
	};


	Phase _phase;
public:

	typedef std::function<int(Handler&)> phase_handler;
	std::map<Handler::Phase, phase_handler> phase_handlers;


	int init();
	int operate(){
		return phase_handlers[_phase](*this);
	}
	virtual ~Handler(){}
	int phase_0();

	int phase_1(){
		std::cout<<"phase 1"<<std::endl;
		_phase = Phase::phase_2_idx;
		return 0;
	};
	int phase_2(){
		std::cout<<"phase 2"<<std::endl;
		std::chrono::seconds _sec(1);
	    std::this_thread::sleep_for(std::move(_sec));
		return 0;
	};
};

int Handler::init(){
	std::cout<<"init"<<std::endl;
	phase_handlers.insert(std::make_pair(Phase::phase_0_idx,std::mem_fn<int()>(&Handler::phase_0)));
	phase_handlers.insert(std::make_pair(Phase::phase_1_idx,std::mem_fn<int()>(&Handler::phase_1)));
	phase_handlers.insert(std::make_pair(Phase::phase_2_idx,std::mem_fn<int()>(&Handler::phase_2)));
	_phase = Phase::phase_0_idx;
	return 0;
}

int Handler::phase_0(){
	std::cout<<"phase 0"<<std::endl;
	_phase = Phase::phase_1_idx;
	return 0;
};

int main(){
	Handler h;
	h.init();
	while(true)
		h.operate();
}
