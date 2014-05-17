#include <iostream>
#include <vector>
#include <string>

using namespace std;

void multiply(string a, string b, vector<int>& c) {
  int i, j;

  for(i = 0; i < a.size(); i++) {
    for(j = 0; j < b.size(); j++) {
      c[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    }
  }

  for(i = c.size() - 1; i >= 0; i--) {
    if (c[i] >= 10) {
      c[i - 1] += c[i] / 10;
      c[i] %= 10;
    }
  }
}

int erase_point(string &s) {
  size_t point_loc = s.find('.');
  if (point_loc != string::npos) {
    s.erase(point_loc, 1);
  } else {
    point_loc = s.size();
  }

  return s.size() - point_loc;
}

int main() {
  string number;
  int power;

  while(cin >> number >> power) {
    string tmp = "1";

    while(power > 0) {
      string a = string(number);
      int points_a = erase_point(a);
      int points_tmp = erase_point(tmp);

      vector<int> c(a.size() + tmp.size(), 0);
      multiply(a, tmp, c);

      vector<char> result;
      int i = 0;
      while(c[i] == 0 && i < c.size() - points_a - points_tmp) i++;

      for(i; i < c.size(); i++) {
	    if (i == c.size() - points_a - points_tmp) {
	        result.push_back('.');
	    }
	    result.push_back(c[i] + '0');
      }

      tmp = string(result.begin(), result.end());

      // remove trailing zero
      if ((points_a + points_tmp > 0) && tmp[tmp.size() - 1] == '0') {
	    for(i = tmp.size() - 1; i >= 0; i--) {
	        if (tmp[i] != '0')break;
	    }
      }

      if (i < tmp.size() - 1) {
	    tmp[i] == '.' ? tmp.erase(i) : tmp.erase(i + 1);
      }
    
      power--;
    }

    cout << tmp << endl;
  }

  return 0;
}
