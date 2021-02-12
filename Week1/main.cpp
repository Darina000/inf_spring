
#include <iostream>
#include <chrono>
#include <vector>

class Timer
{
private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
    
public:
    Timer(): m_StartTime(std::chrono::system_clock::now()), m_bRunning(false), m_EndTime(std::chrono::system_clock::now())  {
        
    }
    ~Timer(){
        m_bRunning = false;
    }
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }
    
    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }
    
    void contin(){
        m_bRunning = true;
    }
    
    double elapsedMicroseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;
        
        if(m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }
        
        return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
    }
    
};

void f(){
    std::vector <int> v;
    v.reserve(10000);
    
    for (auto i=10000; i>=0; --i ){
        v.push_back(i);
    }
}


int main()
{
    Timer timer;
    timer.start();
    
    std::cout << "Microsecond: " << timer.elapsedMicroseconds()<< std::endl;
    
    f();

    std::cout << "Microsecond: " << timer.elapsedMicroseconds() << std::endl;
    
    timer.stop();
    
    f();
    
    std::cout << "Microseconds: " << timer.elapsedMicroseconds() << std::endl;
    
    timer.contin();
    
    f();
    
    std::cout << "Microseconds: " << timer.elapsedMicroseconds() << std::endl;
    
    
}
