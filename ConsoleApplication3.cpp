// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include <iterator>
#include <iostream>
#include <vector>


vector<int> get_number(istream& is) {
	string snum;
	vector<int> vnum;
	unsigned int dig = 1;
	int n = 0;

	is >> snum;

	for (auto it = snum.crbegin(); it != snum.crend(); ++it) {
		n += (*it - '0') * dig;
		dig *= dig_size;
		if (dig == base) {
			vnum.push_back(n);
			n = 0;
			dig = 1;
		}
	}

	if (n != 0) {
		vnum.push_back(n);
	}

	return vnum;
}

void extend_vec(vector<int>& v, size_t len) {
	while (len & (len - 1)) {
		++len;
	}

	v.resize(len);
}

vector<int> karatsuba_mul(const vector<int>& x, const vector<int>& y) {
	auto len = x.size();
	vector<int> res(2 * len);

	if (len <= len_f_naive) {
		return naive_mul(x, y);
	}

	auto k = len / 2;

	vector<int> Xr{ x.begin(), x.begin() + k };
	vector<int> Xl{ x.begin() + k, x.end() };
	vector<int> Yr{ y.begin(), y.begin() + k };
	vector<int> Yl{ y.begin() + k, y.end() };

	vector<int> P1 = karatsuba_mul(Xl, Yl);
	vector<int> P2 = karatsuba_mul(Xr, Yr);

	vector<int> Xlr(k);
	vector<int> Ylr(k);

	for (int i = 0; i < k; ++i) {
		Xlr[i] = Xl[i] + Xr[i];
		Ylr[i] = Yl[i] + Yr[i];
	}

	vector<int> P3 = karatsuba_mul(Xlr, Ylr);

	for (auto i = 0; i < len; ++i) {
		P3[i] -= P2[i] + P1[i];
	}

	for (auto i = 0; i < len; ++i) {
		res[i] = P2[i];
	}

	for (auto i = len; i < 2 * len; ++i) {
		res[i] = P1[i - len];
	}

	for (auto i = k; i < len + k; ++i) {
		res[i] += P3[i - k];
	}

	return res;
}

void finalize(vector<int>& res) {
	for (auto i = 0; i < res.size(); ++i) {
		res[i + 1] += res[i] / base;
		res[i] %= base;
	}
}

