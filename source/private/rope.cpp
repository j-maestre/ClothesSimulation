#include "public/rope.h"
#include <assert.h>

void Rope::InitRope(){
        


    //Rope* rope_tmp = new Rope();
    //assert(rope_tmp && "new Rope is nullptr");
    
    m_points = new Point[m_numParticles];
    assert(m_points && "New Points is null");

}

Rope::Rope(float lenght, unsigned int particles) : m_lenght(lenght), m_numParticles(particles){
    m_points = nullptr;
}

Rope::Rope(const Rope&){

}

Rope::Rope(Rope&&){

}

Rope::~Rope(){
    if (m_points) {
        delete[] m_points;
    }
}

void Rope::Update(float dt){

}

void Rope::GetPosition(int index, float& x, float& y, float& z){

    x = m_points[index].position[0];
    y = m_points[index].position[1];
    z = m_points[index].position[2];
}

void Rope::SetStepSize(float step){

}

void Rope::SetMass(float mass){

}
