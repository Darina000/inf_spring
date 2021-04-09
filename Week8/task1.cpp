
#include <iostream>
#include <random>
#include <vector>
#include <mutex>
#include <thread>
#include <iterator>
#include <future>

class Data{
    private:
        std::mutex mutex_;
        long double count_;
    
    
    public:
        Data():count_(0){};
    
        void add(long double count){
            std::lock_guard<std::mutex> lock(mutex_);
            count_+=count;
        }
        long double print(){
            std::lock_guard<std::mutex> lock(mutex_);
            return count_;
        }

};


void Ppi( Data* data, int begin, int end,int j){
    std::random_device rd;
    std::mt19937 gen(rd());
    int points_in_circle = 0;
    for(auto i =begin;  i<end; i++){
        std::uniform_real_distribution<> dist(0.0, 1.0);
        uniform_real_distribution<> dist2(0.0, 10.0);
        
        double x = dist(gen);
        double y = dist(gen);

        if (x * x + y * y <= 1.0) {
            ++points_in_circle;
        }
    }
    data->add(points_in_circle);
}





int main() {

    long long int N=1000000000;
    int n = std::thread::hardware_concurrency();
    Data counter;
    std::vector <std::thread> thr(n);
    std::vector<std::future<void>> futures(n);
    
    
    for(int j=0; j<n-1; j++){
        std::packaged_task<void(Data* , int, int, int)> task(Ppi);
        futures[j] = task.get_future();
        thr[j] = std::thread(move(task),&counter, N/n*j, N/n*(j+1), j);
    }
    
    
    Ppi(&counter, N/n*(n-1), N, std::thread::hardware_concurrency()-1);
    for(int j=0; j<5; j++) {thr[j].join();}
    long double pi = 4*(counter.print())/N;
    
    std::cout<< pi << std::endl;
    
    double sum = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    
    for(int i=0; i<N; i++){
        std::uniform_real_distribution<> dist(0.0, 10.0);
        std::uniform_real_distribution<> dist2(0.0, 10.0);
        if(sqrt(pow(dist(gen),2)+pow(dist2(gen),2))<10.0){
            sum++;
        }
    }
    
    std::cout<<"Thread pi calculation: "<< 4*sum/N <<std::endl;
    return 0;
}
