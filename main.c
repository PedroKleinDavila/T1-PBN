#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    int R;
    int G;
    int B;
} RGB;

void tonsCinza(RGB *pixels, char *fileName, int linha, int coluna);
void negativa(RGB *pixels, char *fileName, int linha, int coluna);
void raioX(int *pixels, char *fileName, int linha, int coluna);
void envelhecida(RGB *pixels, char *fileName, int linha, int coluna);
void rotaciona90(RGB *pixels, char *fileName, int linha, int coluna);
void rotaciona180(RGB *pixels, char *fileName, int linha, int coluna);

int main()
{
    char nomeImagem[100];
    printf("Digite o nome da imagem (formato .ppm): ");
    scanf("%s", nomeImagem);

    FILE *fp = fopen(nomeImagem, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    char formato[3];
    fscanf(fp, "%s", formato);
    int coluna, linha;
    fscanf(fp, "%d %d", &coluna, &linha);
    int valorMax;
    fscanf(fp, "%d", &valorMax);

    if (coluna < 400 || linha < 400)
    {
        printf("A imagem deve ter tamanho minimo 400x400 pixels.\n");
        fclose(fp);
        return 1;
    }

    RGB *pixels = malloc(linha * coluna * sizeof(RGB));

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            fscanf(fp, "%d %d %d", &pixels[index].R, &pixels[index].G, &pixels[index].B);
        }
    }
    fclose(fp);

    int opcao;
    do
    {
        printf("\n=== MENU DE OPCOES ===\n");
        printf("1. Converter para tons de cinza\n");
        printf("2. Converter para negativo\n");
        printf("3. Converter para raio-X\n");
        printf("4. Converter para envelhecida (sepia)\n");
        printf("5. Rotacionar 90 graus\n");
        printf("6. Rotacionar 180 graus\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            char nomeSaida[100];
            printf("Digite o nome do arquivo de saida (ex: cinza.ppm): ");
            scanf("%s", nomeSaida);
            tonsCinza(pixels, nomeSaida, linha, coluna);
            printf("Imagem em tons de cinza salva como %s\n", nomeSaida);
            break;
        }
        case 2:
        {
            char nomeSaida[100];
            printf("Digite o nome do arquivo de saida (ex: negativo.ppm): ");
            scanf("%s", nomeSaida);
            negativa(pixels, nomeSaida, linha, coluna);
            printf("Imagem negativa salva como %s\n", nomeSaida);
            break;
        }
        case 3:
        {
            char nomeSaida[100];
            printf("Digite o nome do arquivo de saida (ex: raiox.ppm): ");
            scanf("%s", nomeSaida);

            char tempFile[] = "temp_cinza.ppm";
            tonsCinza(pixels, tempFile, linha, coluna);

            FILE *fpTemp = fopen(tempFile, "r");
            fscanf(fpTemp, "%*s");
            fscanf(fpTemp, "%*d %*d");
            fscanf(fpTemp, "%*d");

            int *pixelsCinza = malloc(linha * coluna * sizeof(int));
            for (int j = 0; j < linha; j++)
            {
                for (int i = 0; i < coluna; i++)
                {
                    int index = j * coluna + i;
                    fscanf(fpTemp, "%d", &pixelsCinza[index]);
                }
            }
            fclose(fpTemp);

            raioX(pixelsCinza, nomeSaida, linha, coluna);
            free(pixelsCinza);
            remove(tempFile);
            printf("Imagem raio-X salva como %s\n", nomeSaida);
            break;
        }
        case 4:
        {
            char nomeSaida[100];
            printf("Digite o nome do arquivo de saida (ex: envelhecida.ppm): ");
            scanf("%s", nomeSaida);
            envelhecida(pixels, nomeSaida, linha, coluna);
            printf("Imagem envelhecida salva como %s\n", nomeSaida);
            break;
        }
        case 5:
        {
            char nomeSaida[100];
            printf("Digite o nome do arquivo de saida (ex: rot90.ppm): ");
            scanf("%s", nomeSaida);
            rotaciona90(pixels, nomeSaida, linha, coluna);
            printf("Imagem rotacionada 90 graus salva como %s\n", nomeSaida);
            break;
        }
        case 6:
        {
            char nomeSaida[100];
            printf("Digite o nome do arquivo de saida (ex: rot180.ppm): ");
            scanf("%s", nomeSaida);
            rotaciona180(pixels, nomeSaida, linha, coluna);
            printf("Imagem rotacionada 180 graus salva como %s\n", nomeSaida);
            break;
        }
        case 0:
        {
            printf("Saindo...\n");
            break;
        }
        default:
        {
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
        }
    } while (opcao != 0);

    free(pixels);
    return 0;
}

void tonsCinza(RGB *pixels, char *fileName, int linha, int coluna)
{
    FILE *fp_tonsCinza = fopen(fileName, "w");
    int valorMax = 0;
    int *cinza = malloc(linha * coluna * sizeof(int));

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            int c = (int)(0.299 * pixels[index].R + 0.587 * pixels[index].G + 0.114 * pixels[index].B);
            cinza[index] = c;
            if (c > valorMax)
                valorMax = c;
        }
    }

    fprintf(fp_tonsCinza, "P2\n");
    fprintf(fp_tonsCinza, "%d %d\n", coluna, linha);
    fprintf(fp_tonsCinza, "%d\n", valorMax);

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            fprintf(fp_tonsCinza, "%d\n", cinza[index]);
        }
    }

    fclose(fp_tonsCinza);
    free(cinza);
}

void negativa(RGB *pixels, char *fileName, int linha, int coluna)
{
    FILE *fp_negativa = fopen(fileName, "w");
    int valorMax = 0;
    RGB *negativa = malloc(linha * coluna * sizeof(RGB));

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            negativa[index].R = 255 - pixels[index].R;
            negativa[index].G = 255 - pixels[index].G;
            negativa[index].B = 255 - pixels[index].B;
            if (negativa[index].R > valorMax)
                valorMax = negativa[index].R;
            if (negativa[index].G > valorMax)
                valorMax = negativa[index].G;
            if (negativa[index].B > valorMax)
                valorMax = negativa[index].B;
        }
    }

    fprintf(fp_negativa, "P3\n");
    fprintf(fp_negativa, "%d %d\n", coluna, linha);
    fprintf(fp_negativa, "%d\n", valorMax);

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            fprintf(fp_negativa, "%d %d %d\n", negativa[index].R, negativa[index].G, negativa[index].B);
        }
    }

    fclose(fp_negativa);
    free(negativa);
}

void raioX(int *pixels, char *fileName, int linha, int coluna)
{
    FILE *fp_raioX = fopen(fileName, "w");
    int valorMax = 0;
    int *raiox = malloc(linha * coluna * sizeof(int));

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            raiox[index] = pow(pixels[index], 1.3);
            if (raiox[index] <= 255)
            {
                if (raiox[index] > valorMax)
                    valorMax = raiox[index];
            }
        }
    }

    fprintf(fp_raioX, "P2\n");
    fprintf(fp_raioX, "%d %d\n", coluna, linha);
    fprintf(fp_raioX, "%d\n", valorMax);

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            fprintf(fp_raioX, "%d\n", raiox[index]);
        }
    }

    fclose(fp_raioX);
    free(raiox);
}

void envelhecida(RGB *pixels, char *fileName, int linha, int coluna)
{
    FILE *fp_envelhecida = fopen(fileName, "w");
    int valorMax = 0;
    RGB *envelhecida = malloc(linha * coluna * sizeof(RGB));

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            envelhecida[index].R = pow(pixels[index].R, 1.1) + 10;
            envelhecida[index].G = pow(pixels[index].G, 1.1) + 10;
            envelhecida[index].B = pow(pixels[index].R, 0.9) - 10;
            if (envelhecida[index].R > valorMax)
                valorMax = envelhecida[index].R;
            if (envelhecida[index].G > valorMax)
                valorMax = envelhecida[index].G;
            if (envelhecida[index].B > valorMax)
                valorMax = envelhecida[index].B;
            if (valorMax > 255)
                valorMax = 255;
        }
    }

    fprintf(fp_envelhecida, "P3\n");
    fprintf(fp_envelhecida, "%d %d\n", coluna, linha);
    fprintf(fp_envelhecida, "%d\n", valorMax);

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int index = j * coluna + i;
            fprintf(fp_envelhecida, "%d %d %d\n", envelhecida[index].R, envelhecida[index].G, envelhecida[index].B);
        }
    }

    fclose(fp_envelhecida);
    free(envelhecida);
}

void rotaciona90(RGB *pixels, char *fileName, int linha, int coluna)
{
    FILE *fp_rotaciona = fopen(fileName, "w");
    int novaL = coluna;
    int novaC = linha;
    RGB *rotaciona = malloc(novaL * novaC * sizeof(RGB));
    int valorMax = 0;

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int oldIndex = j * coluna + i;
            int newIndex = i * novaC + ((linha - 1) - j);
            rotaciona[newIndex] = pixels[oldIndex];
            if (rotaciona[newIndex].R > valorMax)
                valorMax = rotaciona[newIndex].R;
            if (rotaciona[newIndex].G > valorMax)
                valorMax = rotaciona[newIndex].G;
            if (rotaciona[newIndex].B > valorMax)
                valorMax = rotaciona[newIndex].B;
            if (valorMax > 255)
                valorMax = 255;
        }
    }

    fprintf(fp_rotaciona, "P3\n");
    fprintf(fp_rotaciona, "%d %d\n", novaC, novaL);
    fprintf(fp_rotaciona, "%d\n", valorMax);

    for (int j = 0; j < novaL; j++)
    {
        for (int i = 0; i < novaC; i++)
        {
            int idx = j * novaC + i;
            fprintf(fp_rotaciona, "%d %d %d\n", rotaciona[idx].R, rotaciona[idx].G, rotaciona[idx].B);
        }
    }

    fclose(fp_rotaciona);
    free(rotaciona);
}

void rotaciona180(RGB *pixels, char *fileName, int linha, int coluna)
{
    FILE *fp_rotaciona = fopen(fileName, "w");
    RGB *rotaciona = malloc(linha * coluna * sizeof(RGB));
    int valorMax = 0;

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int oldIndex = j * coluna + i;
            int newIndex = (linha - 1 - j) * coluna + (coluna - 1 - i);
            rotaciona[newIndex] = pixels[oldIndex];
            if (rotaciona[newIndex].R > valorMax)
                valorMax = rotaciona[newIndex].R;
            if (rotaciona[newIndex].G > valorMax)
                valorMax = rotaciona[newIndex].G;
            if (rotaciona[newIndex].B > valorMax)
                valorMax = rotaciona[newIndex].B;
            if (valorMax > 255)
                valorMax = 255;
        }
    }

    fprintf(fp_rotaciona, "P3\n");
    fprintf(fp_rotaciona, "%d %d\n", coluna, linha);
    fprintf(fp_rotaciona, "%d\n", valorMax);

    for (int j = 0; j < linha; j++)
    {
        for (int i = 0; i < coluna; i++)
        {
            int idx = j * coluna + i;
            fprintf(fp_rotaciona, "%d %d %d\n", rotaciona[idx].R, rotaciona[idx].G, rotaciona[idx].B);
        }
    }

    fclose(fp_rotaciona);
    free(rotaciona);
}
