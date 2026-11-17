
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cctype>

using namespace std;

struct Cerere {
    int id;
    char culoare;
};

class Resursa {
private:
    mutex mtx;
    condition_variable cv;

    deque<Cerere> asteptare;

    char culoareActiva = '\0';
    int fireActive = 0;

public:
    void adaugaCerere(int id, char culoare) {
        asteptare.push_back({id, culoare});
    }

    void acces(int id, char culoare) {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&]() {
            if (culoareActiva != '\0' &&
                culoareActiva != culoare)
                return false;

            for (const auto& c : asteptare) {
                if (c.id == id)
                    return true;

                if (c.culoare != culoare)
                    return false;
            }

            return false;
        });

        auto it = find_if(
            asteptare.begin(),
            asteptare.end(),
            [id](const Cerere& c) {
                return c.id == id;
            }
        );

        asteptare.erase(it);

        culoareActiva = culoare;
        fireActive++;

        cout << "Firul " << id << " (" << culoare
             << ") a intrat in resursa." << endl;

        cv.notify_all();

        lock.unlock();

        this_thread::sleep_for(
            chrono::milliseconds(700)
        );

        lock.lock();

        fireActive--;

        cout << "Firul " << id << " (" << culoare
             << ") a eliberat resursa." << endl;

        if (fireActive == 0)
            culoareActiva = '\0';

        cv.notify_all();
    }
};

int main() {
    Resursa resursa;
    vector<thread> fire;
    vector<char> culori;

    int n;

    cout << "Numarul de fire: ";
    cin >> n;

    if (!cin || n <= 0) {
        cout << "Numar invalid de fire!" << endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        char culoare;

        cout << "Culoarea firului " << i + 1
             << " (A/N): ";

        cin >> culoare;

        culoare = toupper(
            static_cast<unsigned char>(culoare)
        );

        if (culoare != 'A' && culoare != 'N') {
            cout << "Culoare invalida! Folositi A sau N."
                 << endl;
            return 1;
        }

        culori.push_back(culoare);
        resursa.adaugaCerere(i + 1, culoare);
    }

    cout << "\nPornirea firelor...\n" << endl;

    for (int i = 0; i < n; i++) {
        fire.emplace_back(
            &Resursa::acces,
            &resursa,
            i + 1,
            culori[i]
        );
    }

    for (auto& fir : fire) {
        fir.join();
    }

    cout << "\nToate firele au terminat executia!"
         << endl;

    return 0;
}

