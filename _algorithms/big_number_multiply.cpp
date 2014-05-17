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

int main() {
  string a, b;

  while(cin >> a >> b) {
    vector<int> c(a.size() + b.size(), 0);
    multiply(a, b, c);

    // output
    int i = 0;

    while(c[i] == 0) {
      i++;
    }

    for (i; i < c.size(); i++) {
      cout << c[i];
    }

    cout << endl;
  }

  return 0;
}
