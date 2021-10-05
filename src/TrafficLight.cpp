#include <iostream>
#include <random>
#include "TrafficLight.h"

using namespace std;

/* Implementation of class "MessageQueue" */

 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    unique_lock<mutex> lockg(_mtx);
    _cond_var.wait(lockg, [this] {return !_queue.empty(); });
    T message = move(_queue.back());
    _queue.pop_back();
    return message;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    lock_guard<mutex> lockg(_mtx);
    _queue.clear(); // add clear for better performance --> https://knowledge.udacity.com/questions/586056
    _queue.push_back(move(msg));
    _cond_var.notify_one(); // notify after pushing into queue
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}


void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true){
        auto phase = _trLightPhasesQueue.receive();
        if(phase == TrafficLightPhase::green) {return;}
    }

}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started
    // in a thread when the public method „simulate“ is called. To do this, 
    // use the thread queue in the base class. 
    thread thr(&TrafficLight::cycleThroughPhases, this);
    threads.emplace_back(move(thr));

}

// virtual function which is executed in a thread

void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    
    // generate cycle duration as a random sampling between 4 and 6 seconds
    // https://diego.assencio.com/?index=6890b8c50169ef45b74db135063c227c
    random_device device;
    mt19937 gen(device());
    uniform_int_distribution<int> distribution(4000, 6000); //milliseconds
    int cyc_dur = distribution(gen);

    auto prev_time = chrono::system_clock::now(); // initialize

    while(true){
        this_thread::sleep_for(chrono::milliseconds(1));
        auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-prev_time).count();

        if(elapsed_time>=cyc_dur){
            if(TrafficLight::getCurrentPhase()==TrafficLightPhase::green){ TrafficLight::_currentPhase = red; }
            else { TrafficLight::_currentPhase = green; }
            cyc_dur = distribution(gen);
            prev_time = chrono::system_clock::now();
            _trLightPhasesQueue.send(move(_currentPhase));
        }
    }



}

