#include <stdio.h>

int main() {
    FILE *fp;
    fp = fopen("imagem.ppm","r");
    char formato[3];
    fscanf(fp, "%s", formato);
    printf("%s\n", formato);
    int coluna, linha;
    fscanf(fp, "%d %d", &coluna, &linha); 
    printf("%d %d\n", coluna, linha);
    int valor;
    fscanf(fp, "%d", &valor); 
    printf("%d\n", valor);
    int i, j;
    int r, g, b;

	for(j=0; j<linha; j++)
	{
		for(i=0; i<coluna; i++)
		{  	
  			fscanf(fp, "%d %d %d", &r, &g, &b);
		  	printf("%d %d %d\n", r, g, b);
		}
	}
    fclose(fp);

    return 0;
}