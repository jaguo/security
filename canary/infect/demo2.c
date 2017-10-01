#include<stdio.h>
#include<string.h>
int fun(char * str){
	char  str2[5];
	strcpy(str2,str);
	return 0;
}
int main(){
	char str[20];
	scanf("%s",str);
	fun(str);
	return 1;
}