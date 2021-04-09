#include <queue>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>




template <template<class...> class Container, typename T, typename C >
class T_P_Queue
{
private:

    mutable std::mutex mut;
    std::priority_queue < T, Container<T>, C> que;
    std::condition_variable m_condition_variable;
    
    
    
public:

    T_P_Queue(){};
    T_P_Queue(const T_P_Queue & other)
    {
        std::lock_guard < std::mutex > lock(other.mut);
        que = other.m_queue;
    }


    void push(T value)
    {
        std::lock_guard < std::mutex > lock(mut);
        que.push(value);
        m_condition_variable.notify_one();
    }

    void wait_and_pop(T & value)
    {
        std::unique_lock < std::mutex > lock(mut);

        m_condition_variable.wait(lock, [this] {return !que.empty(); });
        value = que.top();
        que.pop();
    }

    std::shared_ptr < T > wait_and_pop()
    {
        std::unique_lock < std::mutex > lock(mut);

        m_condition_variable.wait(lock, [this] {return !que.empty(); });
        auto result = std::make_shared < T > (que.top());
        que.pop();

        return result;
    }

    std::shared_ptr < T > try_pop()
    {
        
        
        std::lock_guard < std::mutex > lock(mut);

        if (que.empty())
        {
            return std::shared_ptr < T > ();
        }

        auto result = std::make_shared < T > (que.top());
        que.pop();

        return result;
    }
    
    bool try_pop(T & value)
    {
        std::lock_guard < std::mutex > lock(mut);

        if (que.empty())
        {
            return false;
        }

        value = que.top();
        que.pop();

        return true;
    }

    bool empty() const
    {
        std::lock_guard < std::mutex > lock(mut);
        return que.empty();
    }
    
    T_P_Queue & operator=(const T_P_Queue & other)
    {
        std::scoped_lock lock(mut, other.mut);
        que = other.m_queue;
    }
    
};

int main()
{
   
    T_P_Queue <std::vector, int, std::greater<int>> queue;

    queue.push(1);
    queue.push(3);
    int value;
    queue.wait_and_pop(value);
    std::cout << value <<std::endl;
    
    queue.push(7);
    std::cout << queue.try_pop(value) << " " << value << std::endl;

    return 0;
}
