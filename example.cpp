#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;

int main() {

	queue<int*> q;
	int hit[2] = {1, 4};
	q.push(hit);
	int* a = q.front();
	q.pop();
	cout << hit[0] << endl;
	

		
	return 0;

}
