#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define STRING_SIZE 100

int main() {
	char s[STRING_SIZE];

	printf("���ڸ� �Է��ϼ���(100�� �̳�).\n");
	scanf("%s", &s);
	printf("�Էµ� ���ڴ� %s�Դϴ�.", s);

	return 0;
}
