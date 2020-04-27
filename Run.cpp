#include <Windows.h>
#include <iostream>
#include "Alloc2.h"

using namespace std;

int main() {
	size_t size = 1024 * 4096;
	size_t ps = 4096;
	int n = 300;
	Alloc2 al(size, ps);
	size_t** addrArray = new size_t * [n];
	cout << "--------- Початок --------" << endl;
	for (int i = 0; i < n; i++) {
		addrArray[i] = (size_t*)al.alloc_addr(rand());
		if (addrArray[i] == NULL) {
			cout << "ПОМИЛКА!!!" << endl;
		}
	}
	al.OUTPUT();
	for (int i = 0; i < n / 3; i++) {
		al.alloc_ReAddr(addrArray[i], rand());
	}

	al.OUTPUT();
	for (int i = n / 2; i < n; i++) {
		al.release_addr(addrArray[i]);
	}
	al.OUTPUT();
	cout << "-------- Завершення -----------" << endl;
	getchar();
	return 0;
}
