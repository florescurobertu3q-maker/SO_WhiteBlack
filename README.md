# SO_WhiteBlack
cd ~/SO_WhiteBlack/Windows -intră în folderul în care se află codul sursă pentru Windows.

g++ -std=c++17 -Wall -Wextra -pthread main_windows.cpp -o main_windows.exe 

g++ – compilatorul folosit pentru programul C++.
-std=c++17 – stabilește versiunea C++17.
-Wall -Wextra – activează avertismente suplimentare pentru a identifica eventuale greșeli în cod.
-pthread – activează suportul necesar pentru lucrul cu fire de execuție (thread-uri).
main_windows.cpp – fișierul sursă care trebuie compilat.
-o main_windows.exe – stabilește numele executabilului rezultat.

./main_windows.exe -pornește programul.

nano main_windows.cpp
nano main_linux.cpp -pentru afisarea codului

TEST1:
5
A
A
N
N
A

TEST2:
10
A
A
A
N
N
N
A
N
A
N

TEST3:
20
A
A
N
N
A
N
A
A
A
N
N
N
A
A
N
A
N
N
A
N
