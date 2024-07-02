#include <stdio.h>
#include <stdlib.h>


int main(){
	FILE *fp = fopen("grammar.txt", "r");
	while(!feof(fp)){
		char value[20];
		fscanf(fp, "%19s", value);
		char c;
		c = fgetc(fp);
		printf("%s \t %d", value, c);
	}
	return 0;
}
