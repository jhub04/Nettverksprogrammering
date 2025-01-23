#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <list>

using namespace std;

class Workers {

  public:
  size_t num_threads;
  vector<thread> threads; // Stores threads
  list<function<void()>> tasks; // Stores tasks (void functions)
  mutex tasks_mutex;
  condition_variable condition;
  bool stop;

  // Constructor
  Workers(size_t num_threads) : num_threads(num_threads), stop(false) {};

  // Method to create threads.
  // Each thread is continually checking for new tasks to run
  // If the tasks list is empty, the thread waits for the condition variable
  void start() {
    for (size_t i = 0; i<num_threads; i++) {
      // Place a thread on the end of the threads vector
      threads.emplace_back([this] () {
        while (true) {
          function<void()> task;
          {
            // Locks the tasks list
            unique_lock<mutex> lock(tasks_mutex);
            condition.wait(lock, [this] {return !tasks.empty() || stop;});
            if (stop && tasks.empty()) {
              return;
            }
            task = *tasks.begin();
            tasks.pop_front();
          }
          // Runs task
          if (task) {
            task();
          }
        }
      });
    }
  };

  // Method to add tasks to the tasks list
  void post(function<void()> task) {
    {
      //Locks the list when the task is added
      lock_guard<mutex> lock(tasks_mutex);
      tasks.push_back(std::move(task));
    }
    // Notifies one waiting thread
    condition.notify_one();
  };

  // Method to safely exit the theads
  void join() {
    {
      lock_guard<mutex> lock(tasks_mutex);
      stop = true;
    }
    condition.notify_all();

    for (thread &t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }

  };

  // Destructor
  ~Workers() {
    join();
  }
};

int main() {
  Workers worker_threads(4);
  Workers event_loop(1);

  worker_threads.start();
  event_loop.start();

  worker_threads.post([] {
    cout << "Hello from thread A" << endl;
  });

  worker_threads.post([] {
    cout << "Hello from thread B" << endl;
  });

  event_loop.post([] {
    cout << "Hello from thread C" << endl;
  });

  event_loop.post([] {
    cout << "Hello from thread D" << endl;
  });

  worker_threads.join();
  event_loop.join();
  return 0;
}
