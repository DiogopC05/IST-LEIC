% Nome: Diogo de Figueiredo Pereira Fontes Carreira 
% Numero: IST1109955

:- use_module(library(clpfd)). 
:- set_prolog_flag(answer_write_options,[max_depth(0)]). 
:- consult('puzzlesAcampar.pl').

% 4.1 CONSULTAS
/* --------------------------------------------------------------------
Predicado auxiliar adiciona_tuplos/3 que eh verdade se o terceiro 
argumento (tuplo) eh a soma de dois tuplos (soma das componentes 
respetivas).
---------------------------------------------------------------------*/
adiciona_tuplos((X1,X2),(Y1,Y2),(R1,R2)):-
    R1 is X1 + Y1,
    R2 is X2 + Y2.
   
/* -------------------------------------------------------------------- 
Predicado vizinhanca/2 que eh verdade se os elementos da lista Vizinhanca
pertencerem as posicoes da vizinhanca (cima,esquerda,direita,baixo) da 
posicao dada.
---------------------------------------------------------------------*/
vizinhanca((Linha, Coluna), Vizinhanca) :-
    ListAux = [(-1,0),(0,-1),(0,1),(1,0)],
    maplist(adiciona_tuplos((Linha,Coluna)),ListAux,Vizinhanca).
    
/* -------------------------------------------------------------------- 
Predicado vizinhanca/2 que eh verdade se os elementos da lista Vizinhanca
pertencerem as posicoes da vizinhanca alargada da posicao dada.
---------------------------------------------------------------------*/
vizinhancaAlargada((Linha, Coluna), Vizinhanca) :-
    ListAux = [(-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0),(1,1)],
    maplist(adiciona_tuplos((Linha,Coluna)),ListAux,Vizinhanca).

/* -------------------------------------------------------------------- 
Predicado todasCelulas/2 que eh verdade se os elementos da lista 
Todascelulas corresponde a todas as posicoes do tabuleiro.
---------------------------------------------------------------------*/
todasCelulas(Tabuleiro,TodasCelulas):-
    length(Tabuleiro,OrdemTabuleiro),
    findall((Linha, Coluna),
            (between(1, OrdemTabuleiro, Linha), 
            between(1, OrdemTabuleiro, Coluna)),
            TodasCelulas).
    
/* -------------------------------------------------------------------- 
Predicado todasCelulas/3 que eh verdade se os elementos da lista Todas 
celulas corresponde a todas as posicoes do tabuleiro com o objeto 
introduzido (Relva, Tendas, Arvores).
---------------------------------------------------------------------*/
todasCelulas(Tabuleiro,TodasCelulas,Objeto):-
    nonvar(Objeto),
    findall((Linha,Coluna),
    (nth1(Linha,Tabuleiro,LinhaParaIndice),
    nth1(Coluna,LinhaParaIndice, X),
    Objeto == X),
    TodasCelulas).

todasCelulas(Tabuleiro,TodasCelulas,Objeto):-
    var(Objeto),
    findall((Linha,Coluna),
    (nth1(Linha,Tabuleiro,LinhaParaIndice),
    nth1(Coluna,LinhaParaIndice, X),
    var(X)),
    TodasCelulas).

/* -------------------------------------------------------------------- 
Predicado auxiliar numero_objeto_linha/3 que determina o numero de um 
dado objeto de uma lista.
---------------------------------------------------------------------*/
numero_objeto_linha(Objeto,Lst,Contador):-
    nonvar(Objeto),
    include(==(Objeto), Lst, LstComObjeto),
    length(LstComObjeto,Contador).

numero_objeto_linha(Objeto,Lst,Contador):-
    var(Objeto),
    include(var, Lst, LstComObjeto),
    length(LstComObjeto,Contador).

/* -------------------------------------------------------------------- 
Predicado calculaObjectosTabuleiro/4, que eh verdade se Tabuleiro for 
um tabuleiro, Objecto for o tipo de objecto que se procura, e 
ContagemLinhas e ContagemColunas forem, respectivamente, listas com o 
numero desses objectos por linha e por coluna.
---------------------------------------------------------------------*/
calculaObjectosTabuleiro(Tabuleiro, ContagemLinhas, ContagemColunas,Objeto):-
    maplist(numero_objeto_linha(Objeto),Tabuleiro,ContagemLinhas),
    transpose(Tabuleiro,TabTransposto),
    maplist(numero_objeto_linha(Objeto),TabTransposto,ContagemColunas).

/* --------------------------------------------------------------------  
Predicado celulaVazia/2 eh verdade se na posicao (Linha,Coluna), o 
tabuleiro nao tiver nada ou tiver prenchido por relva.
---------------------------------------------------------------------*/
celulaVazia(Tabuleiro, (Linha,Coluna)):-
    todasCelulas(Tabuleiro,CelulasNada,_),
    member((Linha,Coluna),CelulasNada).

celulaVazia(Tabuleiro, (Linha,Coluna)):-
    todasCelulas(Tabuleiro, CelulasRelva, r),
    member((Linha,Coluna),CelulasRelva).

celulaVazia(Tabuleiro, (Linha,Coluna)):-
    todasCelulas(Tabuleiro,TodasCelulas),
    \+member((Linha,Coluna),TodasCelulas).

% 4.2 INSERCAO DE TENDAS E RELVA
/* -------------------------------------------------------------------- 
Predicado auxiliar tenda_ou_relva/1 e verdade se o primeiro argumento
e corresponde a um objeto referindo tenda(t), ou relva(r).
---------------------------------------------------------------------*/
tenda_ou_relva(TendaOuRelva):- TendaOuRelva == t.
tenda_ou_relva(TendaOuRelva):- TendaOuRelva == r.

/* -------------------------------------------------------------------- 
Predicado InsereObjectoCelula/3 eh verdade se o primeiro argumento
e tabuleiro, e o tuplo corresponde a posicao que se quer colocar
o objeto tenda ou relva.
---------------------------------------------------------------------*/
insereObjectoCelula(Tabuleiro,TendaOuRelva,(Linha,Coluna)):-
    nth1(Linha, Tabuleiro, LinhaAtualizada),
    nth1(Coluna, LinhaAtualizada, TendaOuRelva).

insereObjectoCelula(Tabuleiro,_,(_,_)):-
    nth1(_, Tabuleiro, LinhaAtualizada),
    nth1(_, LinhaAtualizada, _).

/* --------------------------------------------------------------------  
Predicado InsereObjectoEntrePosicoes/4 que eh verdade se o primeiro 
argumento e tabuleiro, e os tuplos referentes a mesma linha e colunas 
diferentes correspondem as posicoes que se querem colocar o objeto 
TentaOuRelva.
---------------------------------------------------------------------*/
insereObjectoEntrePosicoes(Tabuleiro, TendaOuRelva, (Linha, Coluna1), (Linha, Coluna2)):-
    findall((Linha,Colunas),between(Coluna1, Coluna2, Colunas),Pos),
    maplist(insereObjectoCelula(Tabuleiro,TendaOuRelva),Pos).

% 4.3 ESTRATEGIAS
/* --------------------------------------------------------------------  
Predicado compara_e_adiciona_linhasrelva/4 que adiciona relva na linha 
do tabuleiro se nessa linha o numero de tendas coincide com o numero de
tendas-objetivo.
---------------------------------------------------------------------*/
compara_e_adiciona_linhasrelva(Tabuleiro, Index, ContObjetivo, Contagem) :-
    ContObjetivo =:= Contagem,
    length(Tabuleiro,OrdemTabuleiro),
    insereObjectoEntrePosicoes(Tabuleiro, r, (Index, 1), (Index, OrdemTabuleiro)).

compara_e_adiciona_linhasrelva(_, _, _, _).

/* --------------------------------------------------------------------  
Predicado Relva/1 que eh verdade se o primeiro argumento e tabuleiro que
corresponde ao tabuleiro se se adicionar a cada linha do tabuleiro relva
se e so se o numero de tendas coincide com o numero de tendas-objetivo 
em cada linha.
---------------------------------------------------------------------*/
relva(Puzzle) :-
    Puzzle = (Tabuleiro, CLinhas, CColunas),
    calculaObjectosTabuleiro(Tabuleiro, ContagemLinhas, ContagemColunas, t),
    length(Tabuleiro,OrdemTabuleiro),
    % Processa linhas
    numlist(1,OrdemTabuleiro,IndicesLinha),
    maplist(compara_e_adiciona_linhasrelva(Tabuleiro),IndicesLinha,ContagemLinhas,CLinhas),
    % Processa colunas
	transpose(Tabuleiro,TabuleiroTransposto),
	numlist(1,OrdemTabuleiro,IndicesColuna),
	maplist(compara_e_adiciona_linhasrelva(TabuleiroTransposto),IndicesColuna,ContagemColunas,CColunas),
	
    transpose(TabuleiroTransposto,Tabuleiro).

/* --------------------------------------------------------------------
Predicado auxiliar retirar_vizinhancas/3 que eh verdade se a lista do 
terceiro argumento eh o resultado de retirar os elementos que nao sao
comuns das duas listas, util no caso de saber quais sao as posicoes 
onde nao se podem colocar tendas.
---------------------------------------------------------------------*/
retirar_vizinhancas(_, [], []).
retirar_vizinhancas([], _, []).

retirar_vizinhancas([H1 | T1], L2, [H1 | Res]) :-
    \+ member(H1, L2),
    retirar_vizinhancas(T1, L2, Res).

retirar_vizinhancas([_ | T1], L2, Res) :-
    retirar_vizinhancas(T1, L2, Res).
    
/* --------------------------------------------------------------------
Predicado auxiliar todas_vizinhancas/3 que eh verdade se o segundo 
argumento e a lista com todas as vizinhancas de um dado objeto.
---------------------------------------------------------------------*/ 
todas_vizinhancas(Tabuleiro,VizinhancaTotal,Objeto):- 
    todasCelulas(Tabuleiro,TodosObjetos,Objeto),
    findall(Vizinhancas, 
        (member((Linha, Coluna), TodosObjetos),
        vizinhanca((Linha, Coluna),Vizinhancas)),
        VizinhancasNAlisadas),

	append(VizinhancasNAlisadas,VizinhancaTotal).

/* -------------------------------------------------------------------- 
Predicado auxiliar todas_vizinhancas_alargadas/3 que eh verdade se o 
segundo argumento e a lista com todas as vizinhancas alargadas de um 
dado objeto.
---------------------------------------------------------------------*/  
todas_vizinhancas_alargadas(Tabuleiro,VizinhancaTotal,Objeto):- 
    todasCelulas(Tabuleiro,TodosObjetos,Objeto),
    findall(Vizinhancas, 
        (member((Linha, Coluna), TodosObjetos),
        vizinhancaAlargada((Linha, Coluna),Vizinhancas)),
        VizinhancasNAlisadas),

	append(VizinhancasNAlisadas,VizinhancaTotal).

/* -------------------------------------------------------------------- 
Predicado inacessiveis/1 eh verdade se Tabuleiro e um tabuleiro que, apos
a aplicacao do predicado, tem relva em todas as posicoes inacessiveis.
---------------------------------------------------------------------*/     
inacessiveis(Tabuleiro):-
   todas_vizinhancas(Tabuleiro,VizinhancaTotal,a),
   todasCelulas(Tabuleiro,TodasCelulas),
   retirar_vizinhancas(TodasCelulas,VizinhancaTotal,SemVizinhancas),
   maplist(insereObjectoCelula(Tabuleiro, r), SemVizinhancas).

/* -------------------------------------------------------------------- 
Predicado auxiliar compara_e_adiciona_tenda/4 eh verdade se apos a 
aplicacao do predicado o Tabuleiro corresponde a adicao das tendas 
possiveis numa linha desse tabuleiro, em posicoes sem objetos t,r,a.
---------------------------------------------------------------------*/ 
compara_e_adiciona_tenda(Tabuleiro, Index, ContObjetivo, Contagem) :-
    ContObjetivo =:= Contagem,
    todasCelulas(Tabuleiro,PosicoesVazias,_),
    findall(
        (Index,Coluna),
        (member((Index,Coluna),PosicoesVazias),
        celulaVazia(Tabuleiro,(Index,Coluna))),
        PosicoesVaziasLinha),

    maplist(insereObjectoCelula(Tabuleiro, t),PosicoesVaziasLinha).

compara_e_adiciona_tenda(_, _, _, _).

/* -------------------------------------------------------------------- 
Predicado aproveita/1 eh verdade se Puzzle e um puzzle que, apos a
aplicacao do predicado, tem tendas em todas as linhas e colunas as quais 
faltavam colocar X tendas e que tinham exactamente X posicoes livres.
---------------------------------------------------------------------*/   
aproveita(Puzzle):-
    Puzzle = (Tabuleiro, CLinhas, CColunas),
    calculaObjectosTabuleiro(Tabuleiro, ContagemVaziosLinhas, ContagemVaziosColunas, _),
    length(Tabuleiro,OrdemTabuleiro),
    % Processa linhas
    numlist(1,OrdemTabuleiro,IndicesLinha),
    maplist(compara_e_adiciona_tenda(Tabuleiro),IndicesLinha,ContagemVaziosLinhas,CLinhas),
    % Processa colunas
    transpose(Tabuleiro,TabuleiroTransposto),
	numlist(1,OrdemTabuleiro,IndicesColuna),
	maplist(compara_e_adiciona_tenda(TabuleiroTransposto),IndicesColuna,ContagemVaziosColunas,CColunas),
	
    transpose(TabuleiroTransposto,Tabuleiro).
    
/* -------------------------------------------------------------------- 
Predicado limpaVizinhancas/1 e verdade se Puzzle e um puzzle que, apos 
a aplicacao do predicado, tem relva em todas as posicoes a volta de uma 
tenda.
---------------------------------------------------------------------*/  
limpaVizinhancas(Puzzle):-
    Puzzle = (Tabuleiro, _, _),
    % Colocar relva em todas as vizinhancas alargadas das tendas
    todas_vizinhancas_alargadas(Tabuleiro,VizinhancaTotal,t),
    maplist(insereObjectoCelula(Tabuleiro, r), VizinhancaTotal).

/* -------------------------------------------------------------------- 
Predicado auxiliar separa_em_sublistas/2 que e verdade se o segundo 
argumento e uma lista de listas cada uma com quatro elementos. Essa 
lista e obtida, por outra dada como primeiro argumento.
---------------------------------------------------------------------*/  
separa_em_sublistas([], []):-!.

separa_em_sublistas([X1, X2, X3, X4 | Rest], [[X1, X2, X3, X4] | RestSublists]) :-
    separa_em_sublistas(Rest, RestSublists).

/* -------------------------------------------------------------------- 
Predicado auxiliar avalia_vizinhancas/2 que e verdade se apenas um 
elemento da lista das posicoes dada como argumento sao todas vazias, 
e se for o resultado de adicionar uma tenda nessa posicao.
---------------------------------------------------------------------*/  
avalia_vizinhancas(Tabuleiro,ListaPos):-
    todasCelulas(Tabuleiro,CelulasNada,_),
    todasCelulas(Tabuleiro,CelulasTenda,t),
    retirar_vizinhancas(ListaPos,CelulasTenda,ListaPosCheck),
    length(ListaPosCheck,4),
    % So pode haver uma solucao para o objetivo
    findall(Posicao, (member(Posicao,ListaPos),member(Posicao,CelulasNada)),CelulasVaziasVizinhanca),
    length(CelulasVaziasVizinhanca,1),

    CelulasVaziasVizinhanca = [PosicaoParaAdicionar],
    insereObjectoCelula(Tabuleiro,t,PosicaoParaAdicionar).

avalia_vizinhancas(_,_).
    
/* -------------------------------------------------------------------- 
Predicado unicaHipotese/1 e verdade se Puzzle e um puzzle que, apos a 
aplicacao do predicado, todas as arvores que tinham apenas uma posicao 
livre na sua vizinhanca que lhes permitia ficar ligadas a uma tenda, tem 
agora uma tenda nessa posicao.
---------------------------------------------------------------------*/  
unicaHipotese(Puzzle):-
    Puzzle = (Tabuleiro, _, _),
    todas_vizinhancas(Tabuleiro,VizinhancaTotal,a),
    separa_em_sublistas(VizinhancaTotal,VizinhancaTotalsub),
    % Para poder avancar sobre todas as vizinhancas alargadas
    maplist(avalia_vizinhancas(Tabuleiro),VizinhancaTotalsub).

% 4.4 TENTATIVA E ERRO
/* -------------------------------------------------------------------- 
Predicado valida/2 e verdade se os argumentos sao listas com todas as 
suas coordenadas em que existem arvores e tendas, sendo avaliado para 
verdade se for possivel estabelecer uma relacao bijetiva entre uma 
arvore e uma tenda.
---------------------------------------------------------------------*/  
valida(LArv, LTen) :-
    length(LArv, N),
    length(LTen, N),
    avalia_arvore(LArv, LTen, _).
    
/* -------------------------------------------------------------------- 
Predicado auxiliar avalia_arvore/3 e verdade se for possivel estabelecer
uma lista que liga uma so arvore a uma e a uma so tenda, na sua vizinhanca
---------------------------------------------------------------------*/  
avalia_arvore([], _, []).
avalia_arvore([Arvore | Resto], Tendas, [[Arvore, X] | RestoList]) :-
    vizinhanca(Arvore, Vizinhanca),
    member(X, Vizinhanca),
    member(X, Tendas),
    avalia_arvore(Resto, Tendas, RestoList),
    \+ member([_, X], RestoList).

/* -------------------------------------------------------------------- 
Predicado auxiliar puzzleDiferentes/3 e verdade se os dois puzzles dados
como argumento sao diferentes.
---------------------------------------------------------------------*/  
puzzleDiferentes(Puzzle, Original):-
    Puzzle = (TabuleiroPuzzle,_,_),
    Original = (TabuleiroOG,_,_),
    todasCelulas(TabuleiroPuzzle, TodasVaziasP, _),
    todasCelulas(TabuleiroOG, TodasVaziasO, _),
    TodasVaziasP \== TodasVaziasO.
    
/* -------------------------------------------------------------------- 
Predicado auxiliar resolve_aux/1 que e verdade se o Puzzle estiver 
resolvido.
---------------------------------------------------------------------*/     
resolve_aux(Puzzle):-
    Puzzle = (Tabuleiro, _, _),
    % Caso base eh quando o puzzle fica sem espacos vazios
    todasCelulas(Tabuleiro,[],_),!.

resolve_aux(Puzzle) :-
    copy_term(Puzzle, Original),
    % Aplicacao de predicados da estrategia
    resolve_aux_pred(Puzzle),
    puzzleDiferentes(Puzzle,Original),
    % So falha se os predicados nao fizerem avancar o tabuleiro
    resolve_aux(Puzzle),!.

resolve_aux(Puzzle) :-
    Puzzle = (Tabuleiro, _, _),
    copy_term(Puzzle, Original),
    insereObjectoCelula(Tabuleiro, t, _),
    puzzleDiferentes(Puzzle,Original),
    resolve_aux(Puzzle),
    % Se falhar o valida_puzzle, entao colocar tenda noutra posicao
    valida_puzzle(Puzzle),!.

/* -------------------------------------------------------------------- 
Predicado auxiliar resolve_aux_pred/1 que e verdade se o Puzzle for a 
aplicacao de todos os predicados da estrategia.
---------------------------------------------------------------------*/     
resolve_aux_pred(Puzzle):-
	relva(Puzzle),
    unicaHipotese(Puzzle),
    limpaVizinhancas(Puzzle),!.
    
/* -------------------------------------------------------------------- 
Predicado auxiliar valida_puzzle/1 que e verdade se Puzzle esta resolvido,
ou seja se a contagem das tendas corresponde a contagem do puzzle, e se
eh possivel estabelecer bijecao entre arvores e tendas.
---------------------------------------------------------------------*/ 
valida_puzzle(Puzzle) :-
    Puzzle = (Tabuleiro, CLinhas, CColunas),
    todasCelulas(Tabuleiro, TodasTendas, t),
    todasCelulas(Tabuleiro, TodasArvores, a),
    calculaObjectosTabuleiro(Tabuleiro, CLinhas, CColunas, t),
    valida(TodasArvores, TodasTendas).

/* -------------------------------------------------------------------- 
Predicado auxiliar resolve/1 e verdade se Puzzle e um puzzle que, apos 
a aplicacao do predicado, fica resolvido.
---------------------------------------------------------------------*/ 
resolve(Puzzle):-
    Puzzle = (Tabuleiro, _, _),
    inacessiveis(Tabuleiro),!,
    resolve_aux(Puzzle).
