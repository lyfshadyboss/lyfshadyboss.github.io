#include<iostream>
#include<algorithm>
#include<iomanip>

using namespace std;

void initial(int* ctoi) {
  for(int i = 0; i <= 9; i++) {
    ctoi[i + '0'] = i;
  }

  ctoi['A'] = ctoi['B'] = ctoi['C'] = 2;
  ctoi['D'] = ctoi['E'] = ctoi['F'] = 3;
  ctoi['G'] = ctoi['H'] = ctoi['I'] = 4;
  ctoi['J'] = ctoi['K'] = ctoi['L'] = 5;
  ctoi['M'] = ctoi['N'] = ctoi['O'] = 6;
  ctoi['P'] = ctoi['R'] = ctoi['S'] = 7;
  ctoi['T'] = ctoi['U'] = ctoi['V'] = 8;
  ctoi['W'] = ctoi['X'] = ctoi['Y'] = 9;

  return;
}

int main() {
  int ctoi['Z' + 1];
  initial(ctoi);

  int n;
  while(cin >> n) {
    int* sort_out = new int[n];

    int i;
    for(i = 0; i < n; i++) {
      int x = 0;
      char s[20];

      cin>>s;

      for(int j = 0; s[j] != '\0'; j++) {
	if(s[j] == '-' || s[j] == 'Q' || s[j] == 'Z') {
	  continue;
	}
	x = x * 10 + ctoi[s[j]];
      }

      sort_out[i] = x;
    }

    sort(sort_out, sort_out + n);

    bool flag = true;

    for(i = 0; i < n;) {
      int time = 0;
      int k = sort_out[i];
      bool sign = false;

      while(k == sort_out[i] && i < n) {
	time++;
	i++;

	if(time == 2) {
	  flag = false;
	  sign = true;
	}
      }

      if(sign) {
	cout << setfill('0') << setw(3) << k / 10000;
	cout << '-';
	cout << setfill('0') << setw(4) << k % 10000;
	cout << ' ' << time << endl;
      }
    }

    if(flag) {
      cout << "No duplicates." << endl;
    }

    delete sort_out;
  }
  
  return 0;
}
