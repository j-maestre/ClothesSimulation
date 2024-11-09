#pragma once
#include <stdio.h>
#include <iostream>

#include <typeinfo>

#include <memory>
#include <queue>
#include <vector>

#include <functional>

#include <cstdarg>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>  

class JobSystem {

public:
    JobSystem();
    JobSystem(const JobSystem&) = delete;
    JobSystem(JobSystem&&) = delete;
    ~JobSystem();

    template<typename T>
    auto add_task(T callback) -> std::future<decltype(callback())> {
        std::lock_guard<std::mutex> locked{ m_mutex_task };

        using return_type = decltype(callback());

        std::packaged_task<return_type()> task(callback);
        std::shared_ptr<std::packaged_task<return_type()>> s_pointer = std::make_shared<std::packaged_task<return_type()>>(std::move(task));
        auto lambda = [s_pointer]() {
            return (*s_pointer)();
            };


        std::future<return_type> future = s_pointer->get_future();
        m_task_queue.push(std::move(lambda));
        m_condition_var.notify_one();

        return future;
    }

    template<typename T, typename A>
    auto add_task(T callback, A parameter) -> std::future<decltype(callback(parameter))> {
        std::lock_guard<std::mutex> locked{ m_mutex_task };

        using return_type = decltype(callback(parameter));
        using parameter_type = decltype(parameter);

        std::packaged_task<return_type(parameter_type)> task(std::bind(callback, parameter));

        std::shared_ptr<std::packaged_task<return_type(parameter_type)>> s_pointer = std::make_shared<std::packaged_task<return_type(parameter_type)>>(std::move(task));
        auto lambda = [s_pointer, parameter]() {
            return (*s_pointer)(parameter);
            };


        std::future<return_type> future = s_pointer->get_future();
        m_task_queue.push(std::move(lambda));
        m_condition_var.notify_one();

        return future;
    }

    void wait_until_finish();

    void worker_thread();


private:

    std::vector<std::thread> m_thread_vector;
    std::queue<std::function<void()>> m_task_queue;
    std::mutex m_mutex_task;
    std::condition_variable m_condition_var;
    bool m_ready;


    bool m_stop;

    //std::vector<std::future<void>> m_futures;

};