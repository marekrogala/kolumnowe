#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <utility>

using namespace std;

int main() {

	vector<int> a,b;
	vector<double> c;

	int col;
	while (cin >> col) {
		if (col == 0) {
			int x;
			cin >> x;
			a.push_back(x);
		}
		else if (col == 1) {
			int x;
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

	map<pair<int,int>,double> M1;

	for (int i = 0; i < a.size(); i++)
		if (b[i] > 0)
			M1[make_pair(a[i],b[i])] += c[i];

	map<int, int> M2;

	for (auto it = M1.begin(); it != M1.end(); ++it)
		if (it->second > 20)
			M2[it->first.first]++;

	for (auto it = M2.begin(); it != M2.end(); ++it)
		if (it->second < 5)
			cout << it->first << " " << it->second << endl;

	return 0;
}
