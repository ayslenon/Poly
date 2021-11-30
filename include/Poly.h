#ifndef POLY_H
#define POLY_H

#include "string"
using namespace std;

class Poly{
    private:
        // dimensão do polinomio (grau + 1)
        unsigned D;
        // ponteiro que aponta para array de dimensão D
        double *a;

    public:
        // construtores
        // cria um poly com dim = 0 e apontando para lugar nenhum
        Poly();
        // cria um poly por copia
        Poly(const Poly& P);
        // cria um poly com dim = grau + 1
        // a forma do poly é
        // se d = 1, a0 = 0
        // se d > 1, an = 1, n = d-1 | an = 0, n != d-1
        explicit Poly(unsigned grau);

        // destrutor
        ~Poly();

        // metodos principais
        // muda o grau de um poly, apagando o poly anterior e criando um novo
        // ao recriar os coeficientes são todos do tipo do construtor especifico
        void recriar(unsigned grau);
        // se o poly em questão for vazio (dim = 0) então retorna true
        bool empty(void) const;
        // retorna true se o poly for de grau 0 (dim = 1) e a0 = 0
        bool isZero(void) const;
        // retorna dim - 1 (grau) do poly [= -1 se for empty, ou seja, d = 0]
        int getGrau(void) const;
        // retorna o coeficiente de x^i ( se i >= d retorna 0 )
        float getCoef(unsigned i) const;
        // retorna o valor de poly P(x), se Poly for empty, retorna 0
        float getValor(float x) const;
        // muda o coef da posição i se i for valido (i < dim)
        // se i == getGrau() e coef != 0 e i > 0
        // exibe mensagem se der erro nas verificações
        void setCoef(unsigned i, float coef);
        // retorna se o arquivo foi salvo com sucesso
        // linha 1: POLY D
        // linha 2: coefs de a0 ate an
        bool salvar(string arquivo);
        // retorna se o arquivo foi lido com sucesso
        // erro se n encontra a palavra POLY no começo
        // se D é < 0
        // se numero de coefs é menor q a dimensão
        // se an == 0 para D > 1 (grau > 0)
        bool ler(string arquivo);

        // sobrecargas de operadores
        // atribui um poly a outro, mas antes de atribuir libera a memoria do poly (this) caso dim != 0
        void operator=(const Poly& P);
        // retorna o mesmo que getCoef()
        float operator[](unsigned i);
        // retorna o mesmo que getValor()
        float operator()(float x);

        // escreve o valor de P()
        friend std::ostream& operator<<(std::ostream& X, const Poly& P);


        // tenta ler os valores de P()
        // em caso de an == 0 ou tentar inserir dados em um poly empty, retorna uma mensagem de erro
        friend std::istream& operator>>(std::istream& X, Poly& P);


        // operar com um Poly vazio ou nulo deve ser equivalente a 0
        // calcular quando o resultado tem um grau menor que as duas entradas
        Poly operator+(const Poly& P);
        Poly operator-(const Poly& P);

        // a operação - de um Poly vazio ou nulo é ele mesmo
        Poly operator-(void);

        // empty * zero = empty
        // empty * P = empty
        // zero * P = zero
        Poly operator*(const Poly& P);

        // extra
        Poly operator/(const Poly& P);
        Poly operator%(const Poly& P);

    protected:
};

#endif // POLY_H
