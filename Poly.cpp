#include "Poly.h"
#include "cmath"

#include<iostream>
#include <fstream>

using namespace std;

// construtor default cria um Poly de dim 0 e aponta para lugar nenhum
Poly::Poly(): D(0), a(nullptr) {}

// construtor por copia, copia a dimensão D e aloca espaço para um array de tamanho D
Poly::Poly(const Poly& P){
    D = P.D;
    a = new double[D];
    for (unsigned i=0; i<D; i++) a[i] = P.a[i];
}

// construtor especifico cria um poly alocando um certo espaço na memoria para o array de coeficientes
// fazendo com que todos os coeficientes sejam 0, exceto o de maior grau, caso grau seja maior que 0
Poly::Poly(unsigned grau){
    D = grau+1;
    a = new double[D];

    for (int i = 0; i <= grau; i++){
        a[i] = i == grau ? 1.0 : 0.0;
    }
    a[0] = 0.0;
}

Poly::~Poly(){
    if (a!=nullptr){
        D = 0;
        delete[] a;
    }
}

void Poly::recriar(unsigned grau){
    delete[] a;
    D = grau+1;
    a = new double[D];

    for (int i = 0; i <= grau; i++){
        a[i] = i == grau ? 1.0 : 0.0;
    }
    a[0] = 0.0;
}

bool Poly::empty(void) const{
    if (D == 0) return true;
    return false;
}

bool Poly::isZero(void) const{
    if(D == 1 && a[0] == 0.0) return true;
    return false;
}

int Poly::getGrau(void) const{
    return int(D) - 1;
}

float Poly::getCoef(unsigned i) const{
    // adicionada a verificação de empty pq tava crashando ao tentar fazer getCoef(i) para um Poly vazio
    if (i <= getGrau() && !empty()) return a[i];
    return 0.0;
}

float Poly::getValor(float x) const{
    float temp = 0.0;
    if(empty()) return 0.0;
    for(int i = getGrau(); i>= 0; i--){
        temp += a[i]*pow(x, i);
    }
    return temp;
}

void Poly::setCoef(unsigned i, float coef){
    // na logica atual, nao permite mudar um coef maior que o grau atual do poly
    // mas não opera um empty
    if(getGrau() < 0){
        cout << "Seu polinomio eh vazio" << endl;
    }
    else if(i <= getGrau()){
        // se é diferente, então é menor que o maior grau, logo pode mudar sem restrição o coeficiente
        if(i != getGrau()){
            a[i] = coef;
        }
        // se não é menor, então é igual ao maior grau
        // aqui verificamos se o coeficiente não é 0 (evita diminuir o grau do polinomio)
        // ou se o grau é 0, assim não há restrição para o coeficiente não ser igual a 0
        else if(coef != 0.0 || i == 0){
            a[i] = coef;
        }
        else {
            cout << "Coeficiente = 0 no maior grau"<< endl;
        }
    }
    else {
        cout << "Grau maior que o maior grau do polinomio (recrie o polinomio com um grau maior)"<< endl;
    }
}

bool Poly::salvar(string arquivo){
    std::ofstream file;
    //file.exceptions(ifstream::failbit | ifstream::badbit);

    try{
        file.open(arquivo, std::ifstream::out|std::ifstream::trunc);
        file << "POLY ";
        if (getGrau() >= 0){
            file << getGrau()+1 << endl;
        }
        for(int i = 0; i <= getGrau(); i++){
            file << getCoef(i) << " ";
        }
        file.close();
        return true;
    }
    catch(std::ifstream::failure e){
        cerr << "Erro na escrita do arquivo";
        return false;
    }
}

bool Poly::ler(string arquivo){
    std::ifstream file;
    //file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    string aux;
    int dim;
    double *coef;

    try{
        file.open(arquivo, std::ifstream::in);
        file >> aux;
        //file >> ws;
        //cout << aux << " ";
        if (aux != "POLY"){
            cerr << "Palavra reservada POLY nao encontrada" << endl;
            return false;
        }
        if(!file.eof()){
            file >> dim;
            //cout << dim << endl;

            if (dim > 0){
                coef = new double [dim];
                int i = 0;

                while(!file.eof() && i < dim){
                    file >> coef[i];
                    //file >> ws;
                    //cout << coef[i] << " ";
                    i++;
                }
                //cout << endl;

                if(i != dim) {
                    cerr << "Arquivo contem menos coeficientes que a dimensao do poly" << endl;
                    return false;
                }

                if(dim > 0){
                    if(coef[dim-1] == 0.0){
                        cerr << "Coeficiente an eh zero e poly nao eh de grau 1" << endl;
                        return false;
                    }
                }
                recriar(dim-1); // caso no metodo ler precise recriar o poly
                for (int i = 0; i < dim; i++){

                    setCoef(i, coef[i]);
                    //cout << i << " " << coef[i] << " " << getCoef(i) << endl;
                }

                delete[] coef;
            }
            else{
                cerr << "Poly com dimensao negativa no arquivo" << endl;
                return false;
            }
        }
        else {
            cerr << "Apenas estava escrito POLY" << endl;
        }
        file.close();
        return true;
    }
    catch(std::ifstream::failure e){
        cerr << "Erro na escrita do arquivo";
        return false;
    }

}

void Poly::operator=(const Poly& P){
    if (this!=&P){
        //cout << "Operador = "<< P << endl;
        if (this->D!=P.D){
            if (this->a!=nullptr){
                delete[] this->a;
            }
            this->D = P.getGrau()+1;
            this->a = new double[this->D];
        }
        for (unsigned i=0; i<this->D; i++){
            this->setCoef(i, P.getCoef(i));
        }
    }
}

float Poly::operator[](unsigned i){
    return getCoef(i);
}

float Poly::operator()(float x){
    return getValor(x);
}

ostream& operator<<(ostream& X, const Poly& P){
    if(P.getGrau() >= 0){
        for(int i = P.getGrau(); i >= 0; i--){
            if(P.getCoef(i) == 0.0){
                if(i == 0 && P.getGrau() == 0){
                    X << P.getCoef(i);
                }
            }
            else{
                if(P.getCoef(i) < 0.0){
                    X << '-';
                }
                else{
                    if(i != P.getGrau()){
                        X << '+';
                    }
                }
                if((abs(P.getCoef(i)) != 1.0) || i == 0){
                    X << abs(P.getCoef(i));
                }
                if(i != 0){
                    if (abs(P.getCoef(i)) != 1.0){
                        X << '*';
                    }
                    X << 'x';
                    if (i > 1){
                        X << '^' << i;
                    }
                }
            }
        }
    }
    return X;
}


istream& operator>>(istream& X, Poly& P){
    double coef = 0.0;
    if(P.getGrau() >= 0){
        for(int i = P.getGrau(); i>= 0; i--){
            if(P.getGrau() != 0 && i == P.getGrau()){
                cout << "Insira um numero diferente de zero" << endl;
            }
            cout << "Insira o coef para x^" << i << endl;
            X >> coef;
            P.setCoef(i, coef);
        }
    }
    else{
        cerr << "Erro ao ler Poly vazio" << endl;
    }
    return X;
}


Poly Poly::operator+(const Poly& P){
    // somar com empty ou zero não altera o poly
    int maiorGrau = getGrau();
    if(P.getGrau() > getGrau()) maiorGrau = P.getGrau();

    for(int i = maiorGrau; i > 0; i--){
        if(getCoef(i) + P.getCoef(i) != 0.0) break;
        maiorGrau--;
    }

    Poly result(maiorGrau);

    for(int i = 0; i <= maiorGrau; i++){
        result.setCoef(i, getCoef(i) + P.getCoef(i));
    }
    return result;
}

Poly Poly::operator-(void){
    Poly result(*this);
    // garante o retorno de um poly empty caso a entrada seja empty
    if(getGrau()>=0){
        for(int i = 0; i <= getGrau(); i++){
            result.a[i] = -getCoef(i);
        }
    }
    return result;
}

Poly Poly::operator-(const Poly& P){
    Poly result;
    result = P;
    return *this+(-result);
}

Poly Poly::operator*(const Poly& P){
    // empty * zero -> empty
    // zero * empty -> empty
    // empty * P -> empty
    // P * empty -> empty
    // P * zero -> zero
    // zero * P -> zero

    if (this->empty()) return *this;
    if (P.empty()) return P;
    if (this->isZero()) return *this;
    if (P.isZero()) return P;

    int grau = getGrau() + P.getGrau();
    Poly result(grau);

    result.a[grau] = 0.0; // Zerar o unico coeficiente nao nulo
    for (int j = 0; j <= getGrau(); j++){
       for (int k = 0; k <= P.getGrau(); k++){
         result.a[j+k] += getCoef(j)*P.getCoef(k);
       }
    }
    return result;
}

Poly Poly::operator/(const Poly& P){
    Poly prov;
    int grau = getGrau() - P.getGrau();

    // dividir poly por zero da erro
    // dividir zero por qualquer poly da zero
    // dividir por empty ou dividir empty por alguem da empty

    if (this->empty()) return *this;
    if (P.empty()) return P;
    if (this->isZero()) return *this;

    if (P.isZero()){
            // o que eu faço para nao travar o codigo retornando void para o operator=
            // dividindo por zero
        cout << "Erro ao dividir por zero" << endl;
        return *this;
    }
    if (grau < 0) {
        Poly result(0);
        return result;
    }
    Poly result(grau);
    prov = *this;

    for (int i = grau; i >= 0; i--){
        result.setCoef(i, prov.getCoef(getGrau()+i-grau)/P.getCoef(P.getGrau()));
        for (int j = 0; j <= prov.getGrau(); j++){
            // se fizer direto prov = P - result*(*this)
            // o coef de prov na posicao de maior grau nunca vai ser 0,
            // pois o metodo setCoef nao permite (usado na soma de dois poly)
            prov.a[j] = prov[j] - (result*P)[j];
        }
    }
    return result;
}
Poly Poly::operator%(const Poly& P){
    Poly result;
    // dividir poly por zero da erro
    // dividir zero por qualquer poly da zero
    // dividir por empty ou dividir empty por alguem da empty

    if (this->empty()) return *this;
    if (P.empty()) return P;
    if (this->isZero()) return *this;

    if (P.isZero()){
            // o que eu faço para nao travar o codigo retornando void para o operator=
            // dividindo por zero ou por empty??
        cout << "Erro ao dividir por zero" << endl;
        return *this;
    }
    result = *this - ((*this / P) * P);
    return result;
}
