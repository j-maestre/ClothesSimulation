#include "public/job_system.h"




void JobSystem::worker_thread() {
    //std::lock_guard<std::mutex> lg(m_cv);
    // Waiting
    //m_cv.wait(lg, [](){
      //return m_ready;
    //});
}

JobSystem::JobSystem() {

    m_stop = false;
    m_ready = false;
    auto threads_count = std::thread::hardware_concurrency();
    printf("threads-> %d\n", threads_count);

    // Creamos tantos thread como podamos y los metemos en el vector de threads
    for (int i = 0; i < threads_count; i++) {

        auto thread_worker = [this]() {
            do {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> locked{ m_mutex_task };
                    // Esperamos a que haya una tarea
                    m_condition_var.wait(locked, [this]() {
                        return !m_task_queue.empty() || m_stop;
                        });
                    if (!m_task_queue.empty()) {
                        task = m_task_queue.front();
                        m_task_queue.pop();
                    }
                }

                if (task) task();

            } while (!m_stop);
            };

        m_thread_vector.push_back(std::thread{ std::move(thread_worker) });
    }



}

//void JobSystem::add_task(std::function<void()> callback){
  //std::lock_guard<std::mutex> locked {m_mutex_task};
  //m_task_queue.push(std::move(callback));
//}

void JobSystem::wait_until_finish() {
    while (!m_task_queue.empty());
}

JobSystem::~JobSystem() {

    m_stop = true;
    m_condition_var.notify_all();
    int count = 1;
    // Esperamos a que terminen todos los hilos antes de destruir
    for (auto& t : m_thread_vector) {

        if (t.joinable()) {
            t.join();
            printf("Thread %d closed...\n", count);
        }
        count++;
    }

}