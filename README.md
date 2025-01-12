# CPPND: Program a Concurrent Traffic Simulation

This is the project for the fourth course in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213): Concurrency. 

<img src="data/traffic_simulation.gif"/>

Throughout the Concurrency course, you have been developing a traffic simulation in which vehicles are moving along streets and are crossing intersections. However, with increasing traffic in the city, traffic lights are needed for road safety. Each intersection will therefore be equipped with a traffic light. In this project, you will build a suitable and thread-safe communication protocol between vehicles and intersections to complete the simulation. Use your knowledge of concurrent programming (such as mutexes, locks and message queues) to implement the traffic lights and integrate them properly in the code base.

## My Linux System
* Ubuntu 20.04.2 LTS

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./traffic_simulation`.

## Project Tasks

When the project is built initially, all traffic lights will be green. When you are finished with the project, your traffic simulation should run with red lights controlling traffic, just as in the .gif file above. See the classroom instruction and code comments for more details on each of these parts. 

- **Task FP.1** : Define a class `TrafficLight` which is a child class of `TrafficObject`. The class shall have the public methods `void waitForGreen()` and `void simulate()` as well as `TrafficLightPhase getCurrentPhase()`, where `TrafficLightPhase` is an enum that can be either `red` or `green`. Also, add the private method `void cycleThroughPhases()`. Furthermore, there shall be the private member `_currentPhase` which can take `red` or `green` as its value.

Within [TrafficLight.h](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.h) between lines [42-57](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.h#L42).

- **Task FP.2** : Implement the function with an infinite loop that measures the time between two loop cycles and toggles the current phase of the traffic light between red and green and sends an update method to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. Also, the while-loop should use `std::this_thread::sleep_`for to wait 1ms between two cycles. Finally, the private method `cycleThroughPhases` should be started in a thread when the public method `simulate` is called. To do this, use the thread queue in the base class.

Within [TrafficLight.cpp](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.cpp) between lines [60-104](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.cpp#L60).

- **Task FP.3** : Define a class `MessageQueue` which has the public methods send and receive. Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. Also, the class should define an `std::dequeue` called `_queue`, which stores objects of type `TrafficLightPhase`. Finally, there should be an `std::condition_variable` as well as an `std::mutex` as private members.

Whithin [TrafficLight.h](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.h) between lines [22-34](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.h#L22).

- **Task FP.4** : Implement the method `Send`, which should use the mechanisms `std::lock_guard<std::mutex>` as well as `_condition.notify_one()` to add a new message to the queue and afterwards send a notification. Also, in class `TrafficLight`, create a private member of type `MessageQueue` for messages of type `TrafficLightPhase` and use it within the infinite loop to push each new `TrafficLightPhase` into it by calling send in conjunction with move semantics.

Task FP.4a is in [TrafficLight.cpp](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.cpp) between lines [23-31](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.cpp#L23). Task FP.4b is in [TrafficLight.h](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.h) on line [66](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.h#L66).

- **Task FP.5** : The method receive should use `std::unique_lock<std::mutex>` and `_condition.wait()` to wait for and receive new messages and pull them from the queue using move semantics. The received object should then be returned by the receive function. Then, add the implementation of the method `waitForGreen`, in which an infinite while-loop runs and repeatedly calls the `receive` function on the message queue. Once it receives `TrafficLightPhase::green`, the method returns.

Task FP.5a is in [TrafficLight.cpp](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.cpp) between lines [10-21](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.cpp#L10) and Task FP.5b between lines [43-53](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/TrafficLight.cpp#L43).

- **Task FP.6** : In class Intersection, add a private member `_trafficLight` of type `TrafficLight`. In method `Intersection::simulate()`, start the simulation of `_trafficLight`. Then, in method `Intersection::addVehicleToQueue`, use the methods `TrafficLight::getCurrentPhase` and `TrafficLight::waitForGreen` to block the execution until the traffic light turns green.

Task FP.6a is in [Intersection.h, line 9 and 58](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/Intersection.h#L9) and [Intersection.cpp, line 113](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/Intersection.cpp#L113). Task FP.6b on [line 90](https://github.com/prasadshingne/CppND-Program-a-Concurrent-Traffic-Simulation/blob/b32d7fd5a427b5ef1a839d8463298d0e7d45329f/src/Intersection.cpp#L90).

## Output looks like:

<img src="data/traffic_simulation_out.gif"/>

