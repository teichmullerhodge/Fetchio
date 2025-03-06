g++ -O3 main.cpp -lcurl -pedantic -Wall -Wextra -o app.elf && valgrind ./app.elf --leak-check=full
