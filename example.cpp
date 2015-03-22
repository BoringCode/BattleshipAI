#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;

int main() {

	queue<int* > q;
	int hit[2] = {1, 4};
	q.push(hit);
	int* a = q.front();
	q.pop();
	int hit2[2] = {10, 40};
	q.push(hit2);
	int* b = q.front();
	//q.pop();
	if (!q.empty()) {
		cout << a[0] << endl;
		cout << b[0] << endl;
	}
	else cout << "q is empty" << endl;
	

		
	return 0;

}
