#include <iostream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

int main() {

	vector<int> a;
	vector<double> b;

	int col;
	while (cin >> col) {
		if (col == 0) {
			int x;
			cin >> x;
			a.push_back(x);
		}
		else {
			assert(col == 1);
			double x;
			cin >> x;
			b.push_back(x);
		}
	}

	assert(a.size() == b.size());

	map<int,double> M;

	for (int i = 0; i < a.size(); i++)
		if (b[i] > 2.0 && b[i] < 10.0 && a[i] > 0)
			M[a[i]] += 1;

	for (auto it = M.begin(); it != M.end(); it++)
		if (it->second > 4)
			cout << it->first << " " << it->second << endl;

	return 0;
}
