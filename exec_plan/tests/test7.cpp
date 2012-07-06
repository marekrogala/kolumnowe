#include <iostream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

int main() {

	vector<double> a;
	vector<int> b;

	int c;

	while (cin >> c) {
		if (c == 0) {
			double x;
			cin >> x;
			a.push_back(x);
		}
		else {
			int x;
			cin >> x;
			b.push_back(x);
		}
	}
	assert(a.size() == b.size());

	map<int,double> M;

	for (int i = 0; i < a.size(); i++)
		M[b[i]] += a[i];

	for (auto it = M.begin(); it != M.end(); ++it)
		cout << it->second << endl;

	return 0;
}
