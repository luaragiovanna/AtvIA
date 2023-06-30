#include <iostream>
#include <stack>
#include <vector>
#include <set>

using namespace std;

// Estrutura para representar um estado
struct Estado {
    int mE; //missionarios do lado esquerdo
    int cE; //canibais do lado esq
    int mD; //missionarios q estao ao lado direito
    int cD; //canibais no ldo direito
    bool barco; // true se o barco está à direita

    Estado(int me, int ce, int md, int cd, bool bd) //estado com parametros listados na strict
        : mE(me),
          cE(ce),
          mD(md),
          cD(cd),
          barco(bd) {}

    //DEFINE A ORDEM DOS OBJETOS 'ESTADOS'
    //AUXILIA NO ARMAZENAMENTO PARA DESCOBRIR SE O ESTADO VISITADO
    //comparar o Estado atual com o novo estado que esta sendo passado no parametro
    bool operator<(const Estado& other) const {
        //COMPARAÇÃO DE QUANTIDADE DE MISSIONARIOS E CANIBAIS NO ESTADO ATUAL E NOVO ESTADO
        if (mE != other.mE) 
            return mE < other.mE;
        if (cE != other.cE)
            return cE < other.cE;
        if (mD != other.mD)
            return mD < other.mD;
        if (cD != other.cD)
            return cD < other.cD;
        //compara qtde de missionarios e canibais, se certificando q a qtde dos estados de cada um se mantem correta antes da travessia do barco 
        return barco < other.barco;
    }
};

// Função para imprimir o estado atual
void imprimirEstado(const Estado& estado) {
    cout << "esq(" << estado.mE << " missionario(s), " << estado.cE << " canibal(ais)) -> "
         << "dir(" << estado.mD << " missionario(s), " << estado.cD << " canibal(ais)) -> "
         << "barco(" << (estado.barco ? "direita" : "esquerda") << ")" << endl;
}

// Função para verificar se um estado é válido
// n° de canibais <= missionarios   //num negatvo de miss ou canibais
bool estadoValido(const Estado& estado) {
    if (estado.mE < 0 || estado.cE < 0 ||
        estado.mD < 0 || estado.cD < 0 ||
        (estado.mE != 0 && estado.mE < estado.cE) ||
        (estado.mD != 0 && estado.mD < estado.cD))
        return false;

    return true;
}

// Função para verificar se um estado é o estado objetivo (solução)
// passar todo mundo pro outro lado
bool estadoObjetivo(const Estado& estado) {
    return estado.mE == 0 && estado.cE == 0 && //se n tem missionario esq e canibal esq e se tem 3 missionario lado dir e 3 canibal chegou no objetivo
           estado.mD == 3 && estado.cD == 3;
}

// Função para gerar os próximos estados possíveis a partir do estado atual
vector<Estado> criarProxEstados(const Estado& estado) {
    vector<Estado> proximosEstados;

    if (estado.barco) {
        if (estado.mD > 0) { //se missionarios maior q zero
            proximosEstados.emplace_back( //insere proximos estados no vetor
                //adiciona os possiveis proximos estados ao vetor proximosEstados

                estado.mE + 1, estado.cE, estado.mD - 1, //movendo 1 missi para lado esquerdo me+1 e tira do mD
                estado.cD, !estado.barco);
        }

        if (estado.cD > 0) { // se cnibal dir maior q zero
            proximosEstados.emplace_back( //coloca no vetor de possibilidades
                estado.mE, estado.cE + 1, estado.mD, //umcanibal do lado dir vai pro esquerdo
                estado.cD - 1, !estado.barco);
        }

        if (estado.mD > 0 && estado.cD > 0) {
            proximosEstados.emplace_back( //Mover um missionário e um canibal do lado direito para o lado esquerdo: 
                estado.mE + 1, estado.cE + 1, estado.mD - 1,
                estado.cD - 1, !estado.barco);
        }

        if (estado.mD > 1) {
            proximosEstados.emplace_back(
                estado.mE + 2, estado.cE, estado.mD - 2,
                estado.cD, !estado.barco);
        }

        if (estado.cD > 1) {
            proximosEstados.emplace_back(
                estado.mE, estado.cE + 2, estado.mD,
                estado.cD - 2, !estado.barco);
        }
    } else {
        if (estado.mE > 0) {
            proximosEstados.emplace_back(
                estado.mE - 1, estado.cE, estado.mD + 1,
                estado.cD, !estado.barco);
        }

        if (estado.cE > 0) {
            proximosEstados.emplace_back(
                estado.mE, estado.cE - 1, estado.mD,
                estado.cD + 1, !estado.barco);
        }

        if (estado.mE > 0 && estado.cE > 0) {
            proximosEstados.emplace_back(
                estado.mE - 1, estado.cE - 1, estado.mD + 1,
                estado.cD + 1, !estado.barco);
        }

        if (estado.mE > 1) { //move dois missionarios p/ dir
            proximosEstados.emplace_back(
                estado.mE - 2, estado.cE, estado.mD + 2,
                estado.cD, !estado.barco);
        }

        if (estado.cE > 1) {
            proximosEstados.emplace_back(
                estado.mE, estado.cE - 2, estado.mD,
                estado.cD + 2, !estado.barco);
        }
    }

    return proximosEstados;//todos emplace_back adiciona chamada ao vetor 
}

void resolverMissionariosCanibais() {
    stack<Estado> pilha; // Cria uma pilha para armazenar os estados a serem explorados
    set<Estado> visitados; // Cria um conjunto para armazenar os estados visitados
    pilha.push(Estado(3, 3, 0, 0, false)); 

    while (!pilha.empty()) { // Loop principal enquanto a pilha não estiver vazia
        Estado estadoAtual = pilha.top(); // Obtém o estado atual retirando o elemento do topo da pilha
        pilha.pop(); // Remove o estado atual da pilha

        if (!estadoValido(estadoAtual) || visitados.count(estadoAtual) > 0)
            continue; // Se o estado não for válido ou já tiver sido visitado, passa para a próxima iteração

        visitados.insert(estadoAtual); // Adiciona o estado atual ao conjunto de estados visitados
        imprimirEstado(estadoAtual); // Imprime o estado atual na saída

        if (estadoObjetivo(estadoAtual))
            break; // Se o estado atual for o estado objetivo, encerra a busca

        vector<Estado> proximosEstados = criarProxEstados(estadoAtual); // Gera os próximos estados possíveis a partir do estado atual

        for (const auto& proximoEstado : proximosEstados) {
            pilha.push(proximoEstado); // Adiciona os próximos estados gerados à pilha para serem explorados posteriormente
        }
    }
}


int main() {


    cout << "          Lado Esquerdo            RIO             Lado Direito          " << endl;
    cout << "         ___________               | |              ___________        " << endl;
    cout << "        /           \               | |            /            \         "<< endl;
    cout << "        |   M M M   |              | |            | OBJETIVO   |       " << endl;
    cout << "        |   C C C   |              | |            |            |       " << endl;
    cout << "        |           |              | |            |            |       " << endl;
    cout << "        |           |              | |            |            |       " << endl;
    cout << "        |           |              | |            |            |       " << endl;
    cout << " _______|___________|_______       | |      ______|___________ |_______" << endl;
    cout << endl;


    resolverMissionariosCanibais();

    cout << endl;
    cout << "          Lado Esquerdo            RIO             Lado Direito          " << endl;
    cout << "         ___________               | |              ___________        " << endl;
    cout << "        /           \               | |            /            \         "<< endl;
    cout << "        |   S0      |              | |            |  M M M     |       " << endl;
    cout << "        |           |              | |            | C C C      |       " << endl;
    cout << "        |           |              | |            |            |       " << endl;
    cout << "        |           |              | |            |            |       " << endl;
    cout << "        |           |              | |            |            |       " << endl;
    cout << " _______|___________|_______       | |      ______|___________ |_______" << endl;

    return 0;
}
