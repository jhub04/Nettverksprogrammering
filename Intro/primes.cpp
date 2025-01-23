#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

void find_primes(int low, int high, list<int>& primes, mutex& prime_mutex) {
  list<int> local_primes;
  for (int i = low; i <= high; i++) {
    int counter = 0;
    for (int num = i; num >= 1; num--) {
      if (i % num == 0) {
        counter++;
      }
    }
    if (counter == 2) {
      local_primes.push_back(i);
    }
  }
  lock_guard<mutex> guard(prime_mutex);
  primes.insert(primes.end(), local_primes.begin(), local_primes.end());
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    cerr << "You need to enter 3 arguments: low, high, and numThreads!" << endl;
    return 1;
  }

  int low = atoi(argv[1]);
  int high = atoi(argv[2]);
  int numThreads = atoi(argv[3]);

  if (low > high) {
    cerr << "Invalid range. The first argument should be less than or equal to the second." << endl;
    return 1;
  }

  if (numThreads <= 0) {
    cerr << "Number of threads must be greater than 0." << endl;
    return 1;
  }

  list<int> primes;
  mutex prime_mutex;
  vector<thread> threads;
  int range = (high - low + 1) / numThreads;
  int remainder = (high - low + 1) % numThreads;

  int start = low;
  for (int i = 0; i < numThreads; ++i) {
    int end = start + range - 1;
    if (i < remainder) {
      end++;
    }
    threads.emplace_back(find_primes, start, end, ref(primes), ref(prime_mutex));
    start = end + 1;
  }

  for (auto& t : threads) {
    t.join();
  }

  primes.sort();
  for (int prime : primes) {
    cout << prime << " ";
  }
  cout << endl;

  return 0;
}