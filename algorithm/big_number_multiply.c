#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

void multiply(char* a, char* b, char* c) {
  int i, j, len_a, len_b, len_c, *tmp;

  len_a = strlen(a);
  len_b = strlen(b);

  tmp = (int*)malloc(sizeof(int) * (len_a + len_b));

  memset(tmp, 0, len_c);

  for(i = 0; i < len_a; i++) {
    for(j = 0; j < len_b; j++) {
      tmp[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    }
  }

  for(i = len_a + len_b -1; i >= 0; i--) {
    if (tmp[i] >= 10) {
      tmp[i - 1] += tmp[i] / 10;
      tmp[i] %= 10;
    }
  }

  for(i; i < len_a + len_b; i++) {
    c[i] = tmp[i] + '0';
  }

  free(tmp);
}

void input(char* a, char* b) {
  fgets(a, MAX, stdin);
  fgets(b, MAX, stdin);

  char* ptr;
  if((ptr = strchr(a, '\n')) != NULL) {
    *ptr = '\0';
  }

  if((ptr = strchr(b, '\n')) != NULL) {
    *ptr = '\0';
  }  
}

void output(char* c) {
  int len = strlen(c);

  int i = 0;

  while(c[i] == '0') i++;

  for(i; i < len; i++) {
    printf("%c", c[i]);
  }

  printf("\n");
}

int main()
{
  char *a, *b, *c;

  a = (char *)malloc(sizeof(char) * MAX);
  b = (char *)malloc(sizeof(char) * MAX);

  input(a, b);

  c = (char *)malloc(sizeof(char) * (strlen(a) + strlen(b)));

  multiply(a, b, c);
  output(c);
  
  return 0;
}
