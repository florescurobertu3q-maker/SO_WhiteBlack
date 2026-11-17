
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>

using namespace std;

struct Cerere {
    int id;
    char culoare;
};

class Resursa {
private:
    mutex mtx;
    condition_variable cv;

    deque<shared_ptr<Cerere>> coada;

    int culoareActiva = 0;
    int fireActive = 0;

public:
    void acces(int id, char culoare) {
        auto cerere = make_shared<Cerere>();
        cerere->id = id;
        cerere->culoare = culoare;

        unique_lock<mutex> lock(mtx);

        coada.push_back(cerere);

        cv.wait(lock, [&]() {
            if (culoareActiva != 0 &&
                culoareActiva != (culoare == 'A' ? 1 : 2))
                return false;

            for (const auto& c : coada) {
                if (c == cerere)
                    return true;

                if (c->culoare != culoare)
                    return false;
            }

            return false;
        });

        auto it = find(coada.begin(), coada.end(), cerere);
        coada.erase(it);

        culoareActiva = (culoare == 'A' ? 1 : 2);
        fireActive++;

        cout << "Firul " << id << " (" << culoare
             << ") a intrat in resursa." << endl;

        cv.notify_all();

        lock.unlock();

        this_thread::sleep_for(chrono::milliseconds(700));

        lock.lock();

        fireActive--;

        cout << "Firul " << id << " (" << culoare
             << ") a eliberat resursa." << endl;

        if (fireActive == 0)
            culoareActiva = 0;

        cv.notify_all();
    }
};

int main() {
    Resursa resursa;
    vector<thread> fire;

    char culori[] = {
        'A', 'A', 'N', 'N', 'A',
        'N', 'A', 'N', 'N', 'A'
    };

    cout << "Pornirea programului...\n" << endl;

    for (int i = 0; i < 10; i++) {
        fire.emplace_back(
            &Resursa::acces,
            &resursa,
            i + 1,
            culori[i]
        );

        this_thread::sleep_for(chrono::milliseconds(50));
    }

    for (auto& fir : fire)
        fir.join();

    cout << "\nToate firele au terminat executia!" << endl;

    return 0;
}


