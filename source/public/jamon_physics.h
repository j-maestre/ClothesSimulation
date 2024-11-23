#include "rope.h"
#include "cloth.h"
#include "wind_turbine.h"


namespace JE {

class JamonPhysics {

private:
	JamonPhysics() = default;

public:
	JamonPhysics(const JamonPhysics&) = delete;
	JamonPhysics(JamonPhysics&&) = delete;
	
    static JamonPhysics* get_instance() {
        static JamonPhysics instance;
		return &instance;
	}

	~JamonPhysics() { 
        delete m_instance; 
    }

	void add_clothe(Cloth* c) { std::lock_guard<std::mutex> lock(m_cloth_mtx); m_cloths.push_back(c); }
    void add_rope(Rope* r) { std::lock_guard<std::mutex> lock(m_rope_mtx); m_ropes.push_back(r); }

	void init_clothe_thread() {
        m_cloth_thread = std::thread([this]() {

            auto previous_time = std::chrono::high_resolution_clock::now();


            while (m_keep_runing) {
                std::lock_guard<std::mutex> lock(m_cloth_mtx);
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration elapsed = current_time - previous_time;

                float delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0f;
                float acum = delta_time;

                // Active wait until dt equal to fixed time step
                while (acum < m_fixed_update_time_step) {
                    //printf("wait... DT: %f/%f \n", acum, fixed_time_step);
                    auto loop_time = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<float> loop_elapsed = loop_time - current_time;
                    acum += loop_elapsed.count();
                    current_time = loop_time;
                }

                for (Cloth* c : m_cloths) {
                    c->Update(m_fixed_time_step);
                }

                previous_time = current_time;
            }

            printf("Cloth Thread closed\n");
            });
	}

    void init_rope_thread() {
        m_rope_thread = std::thread([this]() {

            auto previous_time = std::chrono::high_resolution_clock::now();


            while (m_keep_runing) {
                std::lock_guard<std::mutex> lock(m_rope_mtx);
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration elapsed = current_time - previous_time;

                float delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0f;
                float acum = delta_time;

                // Active wait until dt equal to fixed time step
                while (acum < m_ropes_fixed_time_step) {
                    //printf("wait... DT: %f/%f \n", acum, fixed_time_step);
                    auto loop_time = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<float> loop_elapsed = loop_time - current_time;
                    acum += loop_elapsed.count();
                    current_time = loop_time;
                }

                for (Rope* r : m_ropes) {
                    r->Update(m_ropes_fixed_update_time_step);
                }

                previous_time = current_time;
            }

            printf("Rope Thread closed\n");
            });
    }



    // I have vectors of Cloth* and Rope* so sometimes destructors of that objects are called before this thread is closed and I get a nullptr error, 
    // so I have to ensure the threads are closed before
    void close_threads() {
        m_keep_runing = false;
        if (m_cloth_thread.joinable())m_cloth_thread.join();
        if (m_rope_thread.joinable())m_rope_thread.join();
    }

    void set_time_step(float v) { if(v > 0.0f) m_fixed_time_step = v; }

private:
	JamonPhysics* m_instance;

    std::thread m_cloth_thread;
    std::thread m_rope_thread;
    std::mutex m_cloth_mtx;
    std::mutex m_rope_mtx;

	std::vector<Cloth*> m_cloths;
	std::vector<Rope*> m_ropes;

    bool m_keep_runing = true;

    // Time step used to update particles position
    float m_fixed_time_step = 1.0f / 20.0f;

    // Time step used to call update position (I tested and its more realistic)
    float m_fixed_update_time_step = m_fixed_time_step * 0.5f;

    // This works better with ropes
    float m_ropes_fixed_time_step = 1.0f / 60.0f;
    float m_ropes_fixed_update_time_step = 1.0f / 60.0f;

};

}