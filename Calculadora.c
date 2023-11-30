#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>
#include <string.h> // Adicionado para usar a função strcmp

#define PI 3.14159265358979323846

typedef struct No
{
    double dado;
    struct No *proximo;
} No;

typedef struct
{
    No *topo;
} Pilha;

void inicializar(Pilha *pilha)
{
    pilha->topo = NULL;
}

void empilhar(Pilha *pilha, double valor)
{
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL)
    {
        printf("Erro: Falha na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    novoNo->dado = valor;
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
}

double desempilhar(Pilha *pilha)
{
    if (pilha->topo != NULL)
    {
        No *temp = pilha->topo;
        double valor = temp->dado;
        pilha->topo = temp->proximo;
        free(temp);
        return valor;
    }
    else
    {
        printf("Erro: Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

double converterGrausEmRadianos(double graus)
{
    return graus * (PI / 180.0);
}

double avaliarExpressaoPosfixada(const char *expressao)
{
    Pilha pilha;
    inicializar(&pilha);
    double operando1, operando2;

    for (int i = 0; expressao[i] != '\0'; ++i)
    {
        if (isdigit(expressao[i]) || (expressao[i] == '.'))
        {
            empilhar(&pilha, strtod(&expressao[i], NULL));
            while (isdigit(expressao[i]) || expressao[i] == '.')
            {
                i++;
            }
            i--;
        }
        else if (expressao[i] == ' ' || expressao[i] == '\t')
        {
            continue;
        }
        else
        {
            if (expressao[i] == '-' && (i == 0 || expressao[i - 1] == ' '))
            {
                i++;
                empilhar(&pilha, -strtod(&expressao[i], NULL));
                while (isdigit(expressao[i]) || expressao[i] == '.')
                {
                    i++;
                }
                i--;
            }
            else
            {
                char operador[4] = {0};
                int j = 0;

                while (isalpha(expressao[i]) && j <= 3)
                {
                    operador[j++] = expressao[i++];
                }

                if (j > 0)
                {
                    operador[j] = '\0';

                    if (strcmp(operador, "sen") == 0)
                    {
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, sin((operando1)));
                    }
                    else if (strcmp(operador, "cos") == 0)
                    {
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, cos((operando1)));
                    }
                    else if (strcmp(operador, "tan") == 0)
                    {
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, tan((operando1)));
                    }
                    else if (strcmp(operador, "log") == 0)
                    {
                        if (operando1 > 0)
                        {
                            operando1 = desempilhar(&pilha);
                            empilhar(&pilha, log10(operando1));
                        }
                        else
                        {
                            printf("Erro: Log de número negativo\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if (strcmp(operador, "r") == 0)
                    {
                        if (operando2 > 0)
                        {
                            operando2 = desempilhar(&pilha);
                            operando1 = desempilhar(&pilha);

                            empilhar(&pilha, pow(operando1, 1.0 / operando2));
                        }
                        else
                        {
                            printf("Erro: Raiz de número negativo\n");
                            exit(EXIT_FAILURE);
                        }
                    }

                    else
                    {
                        printf("Erro: Operador ou função desconhecido\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    switch (expressao[i])
                    {
                    case '+':
                        operando2 = desempilhar(&pilha);
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, operando1 + operando2);
                        break;
                    case '-':
                        operando2 = desempilhar(&pilha);
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, operando1 - operando2);
                        break;
                    case '*':
                        operando2 = desempilhar(&pilha);
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, operando1 * operando2);
                        break;
                    case '/':
                        operando2 = desempilhar(&pilha);
                        operando1 = desempilhar(&pilha);
                        if (operando2 != 0.0)
                        {
                            empilhar(&pilha, operando1 / operando2);
                        }
                        else
                        {
                            printf("Erro: Divisão por zero\n");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    case '^':
                        operando2 = desempilhar(&pilha);
                        operando1 = desempilhar(&pilha);
                        empilhar(&pilha, pow(operando2, operando1));
                        break;
                    default:
                        printf("Erro: Operador inválido\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    return desempilhar(&pilha);
}

int main()
{
    setlocale(LC_ALL, "pt_BR.utf8");
    const char *expressao = "3 4 + 5 tan *";
    double resultado = avaliarExpressaoPosfixada(expressao);

    printf("Resultado: %f\n", resultado);

    return 0;
}
