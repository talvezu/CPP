/*
 * g++ -g3  -std=c++1z threads_order.cpp -lpthread
 */
#include <thread>
#include <iostream>
#include <string>
#include <random>
#include <semaphore.h>
#include <mutex>
std::vector<std::thread> threads;
sem_t _semaphore_a, _semaphore_b;

void announce(std::string&& s, sem_t *taken, sem_t *given)
{
    if (taken)
    {
        sem_wait(taken);
    }
    std::cout<<"thread "<<s<<" started"<<std::endl;
    if (given)
    {
        sem_post(given);
    }
}
void a()
{
    threads.push_back(std::thread(announce,std::move("from a"),nullptr, &_semaphore_a));
}
void b()
{
    threads.push_back(std::thread(announce,std::move("from b"),&_semaphore_a, &_semaphore_b));
}
void c()
{
    threads.push_back(std::thread(announce,std::move("from c"),&_semaphore_b, nullptr));
}
int main(){
    std::cout<<"hello main"<<std::endl;
    sem_init(&_semaphore_a, 0, 0);//last argument is count
    sem_init(&_semaphore_b, 0, 0);//last argument is count
    a();
    b();
    c();
    for (auto &th : threads)
        th.join();
 }
