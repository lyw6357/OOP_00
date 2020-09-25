#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define STRING_SIZE 100

int main() {
	char s[STRING_SIZE];

	printf("문자를 입력하세요(100자 이내).\n");
	scanf("%s", &s);
	printf("입력된 문자는 %s입니다.", s);

	return 0;
}
