# Tabela Hash
Neste trabalho foi implementado uma aplicação bancária afim de usar os métodos da tabela hash. Havendo 3 versões 
diferentes da aplicação.

## Versão 1
Uma chave composta por um inteiro correspondente ao número da conta corrente.


using AcctKey = int;


## Versão 2
Uma chave composta de dois campos definidos por um std::pair , que contém o nome
do cliente e o número da conta.

using AcctKey = std::pair< std::string, int >;


## Versão 3
Uma chave composta por meio de 4 campos definidos por uma std::tuple, que contém o nome do cliente, o código do banco, o número da agência e o número da conta.

using AcctKey = std::tuple< std::string, int, int, int >;


## Compilação e execução


#### Versão 1
```shell
g++ -std=c++11 -pedantic -I include/ src/pgm_ht.cpp -o bin/exe -D TIPO1 && ./bin/exe
```

#### Versão 2
```shell
g++ -std=c++11 -pedantic -I include/ src/pgm_ht.cpp -o bin/exe2 -D TIPO2 && ./bin/exe2
```

#### Versão 3
```shell
g++ -std=c++11 -pedantic -I include/ src/pgm_ht.cpp -o bin/exe3 -D TIPO3 && ./bin/exe3
```

## Autores
Adelino Afonso Fernandes Avelino

Irene Ginani Costa Pinheiro
