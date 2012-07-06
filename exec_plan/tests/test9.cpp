#include <iostream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

int main() {

	vector<int> a;
	vector<double> b, c;

	int col;
	while (cin >> col) {
		if (col == 0) {
			int x;
			cin >> x;
			a.push_back(x);
		}
		else if (col == 1) {
			double x;
			cin >> x;
			b.push_back(x);
		}
		else {
			assert(col == 2);
			double x;
			cin >> x;
			c.push_back(x);
		}
	}

	assert(a.size() == b.size() && b.size() == c.size());

	map<int, double> M;

	for (int i = 0; i < a.size(); i++)
		if (b[i] > c[i])
			M[a[i]] += b[i] - c[i];

	for (auto it = M.begin(); it != M.end(); it++)
		cout << it->first + it->second << endl;

	return 0;
}
