#include <iostream>
#include <thread>
#include <mutex>
#include <list>

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

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "You need to enter 2 arguments!" << endl;
        return 1;
    }

    const int low = atoi(argv[1]);
    const int high = atoi(argv[2]);

    if (low > high) {
        cerr << "Invalid range. The first argument should be less than or equal to the second." << endl;
        return 1;
    }

    list<int> primes;
    int mid = low + (high - low) / 2;

    thread t1(find_primes, low, mid, primes);


    // Print the primes
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << '\n';

    return 0;
}
