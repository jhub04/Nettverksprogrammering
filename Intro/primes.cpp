#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>

using namespace std;

void find_primes(int low, int high, list<int>& primes) {
  for (int i = low; i <= high; i++) {
    int counter = 0;

    for (int num = i; num >= 1; num--) {
      if (i % num == 0) {
        counter++;
      }
    }

    if (counter == 2) {
      primes.push_back(i);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "You need to enter 2 arguments!" << endl;
    return 1;
  }

  if (argv[2] <= argv[1]) {
    cerr << "Digit 2 needs to be larger than digit 1" << endl;
    return 1;
  }

  int low = atoi(argv[1]);
  int high = atoi(argv[2]);

  int mid = low + (high + low) / 2;

  list<int> primes;
  mutex prime_mutex;

  thread t1([&primes, &prime_mutex, &low, &mid] {
    prime_mutex.lock();
    find_primes(low, mid, primes);
    prime_mutex.unlock();

  });
  thread t2([&primes, &prime_mutex, &mid, &high] {
    prime_mutex.lock();
    find_primes(mid+1, high, primes);
    prime_mutex.unlock();

  });

  t1.join();
  t2.join();

  primes.sort();

  for (int prime : primes) {
    cout << prime << endl;
  }

  return 0;

}
