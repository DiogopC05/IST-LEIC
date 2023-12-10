#########################################################
#       Projeto 2: Fundamentos da Programação
#                       GO
#         Diogo Fontes Carreira (ist1109955)
#
#                    2023/2024
#########################################################

####
#   TAD intersecao
#
#   O TAD imutável intersecao é usado para representar uma interseção do tabuleiro de
#   Go.
#
#   Representação interna: intersecao = (col, lin) col e lin são str e int, respetivamente
####

def numero_tab(num_p):
    # universal → booleano
    '''
        Recebe um argumento de qualquer tipo e devolve True caso pertença ao segundo elemento
        da interseção e False, caso contrário (1 a 19) - Função Auxiliar.
    '''
    return type(num_p) == int and 1 <= num_p <= 19

def letra_tab(let_p):
    # universal → booleano
    '''
        Recebe um argumento de qualquer tipo e devolve True caso pertença ao primeiro elemento
        da interseção e False, caso contrário (A a S) - Função Auxiliar.
    '''
    return type(let_p) == str and len(let_p) == 1 and 65 <= ord(let_p) <= 83


####
#   Construtores
####

def cria_intersecao(col_inter, lin_inter): 
    # str × int → intersecao
    '''
        Recebe os valores correspondentes às coordenadas de uma interseção e devolve a interseção
        correspondente. É validado os argumentos gerando um ValueError, caso os argumentos sejam
        inválidos.
    '''
    if letra_tab(col_inter) and numero_tab(lin_inter):
        # verificar coluna e linha
        return col_inter, lin_inter
    else:
        raise ValueError('cria_intersecao: argumentos invalidos')


####  
#   Seletor
####

def obtem_col(inter_t):
   # intersecao → str
   '''
        Devolve a coluna da interseção dada como argumento
   '''
   return inter_t[0]

def obtem_lin(inter_t):
    # intersecao → int
    '''
        Devolve a linha da interseção dada como argumento
    '''
    return inter_t[1]


####  
#   Reconhecedor
####

def eh_intersecao(pos_inter):
    # universal → booleano
    '''
        Devolve True caso o seu argumento seja um TAD intersecao
        e False caso contrário.
    '''
    if type(pos_inter) == tuple:
        if len(pos_inter) == 2 and letra_tab(obtem_col(pos_inter)) and numero_tab(obtem_lin(pos_inter)):
            return True
        else:
            return False
    else:
        return False


####  
#   Teste
####

def intersecoes_iguais(inter_1, inter_2):
    # universal x universal → booleano
    '''
        Devolve True apenas se dois argumentos são interseções e são
        iguais, e False caso contrário.
    '''
    if eh_intersecao(inter_1) and eh_intersecao(inter_2):
        if obtem_col(inter_1) == obtem_col(inter_2) and obtem_lin(inter_1) == obtem_lin(inter_2):
            return True
        else:
            return False
    else:
        return False


####  
#   Transformadores
####

def intersecao_para_str(inter_t):
    # intersecao → str
    '''
        Devolve a cadeia de caracteres que representa o seu argumento.
    '''
    return obtem_col(inter_t) + str(obtem_lin(inter_t))

def str_para_intersecao(inter_str):
    # str → intersecao
    '''
        Devolve a interseção representada pelo seu argumento.
    '''
    return cria_intersecao(inter_str[0], int(inter_str[1:]))


####  
#   Funções de Alto Nível
####

def obtem_intersecoes_adjacentes(inter_t, ult_inter):
    # intersecao x intersecao → tuplo
    '''
        Devolve um tuplo com as interseções adjacentes à interseção inter_t de 
        acordo com a ordem de leitura em que ult_inter corresponde à interseção
        superior direita do tabuleiro de Go.
    '''

    inter_adjacentes = []
    inter_col_max = obtem_col(ult_inter)
    inter_lin_max = obtem_lin(ult_inter)
    possivel_adjacente = [
                        (obtem_col(inter_t), obtem_lin(inter_t) + 1), # cima
                        (chr(ord(obtem_col(inter_t)) + 1),obtem_lin(inter_t)), # direita
                        (chr(ord(obtem_col(inter_t)) - 1),obtem_lin(inter_t)), # esquerda
                        (obtem_col(inter_t), obtem_lin(inter_t) - 1), # baixo
                        ] 
    for adj_i in range(len(possivel_adjacente)-1, -1, -1):
        if not eh_intersecao(possivel_adjacente[adj_i]):
                # Verificação se as interseções adjacentes são válidas
                possivel_adjacente.pop(adj_i)
        else:
            if obtem_col(possivel_adjacente[adj_i]) <= inter_col_max and obtem_lin(possivel_adjacente[adj_i]) <= inter_lin_max:
                inter_adjacentes += (cria_intersecao(obtem_col(possivel_adjacente[adj_i]),obtem_lin(possivel_adjacente[adj_i])),)
            else:
                possivel_adjacente.pop(adj_i)

    return tuple(inter_adjacentes)

def ordena_intersecoes(tup_inter):
    # tuplo -> tuplo
    '''
        Devolve um tuplo de interseções com as mesmas interseções 
        de tup_inter ordenadas de acordo com a ordem de leitura do tabuleiro de Go.
    '''
    # Usar lista para poder permutar elementos usando bubble sort
    lst_ord_tup = list(tup_inter) 
    tamanho_inter = len(lst_ord_tup)

    for itera_lista in range(tamanho_inter): 
        # Ordenar as interseções com os números
        for index_inter in range(tamanho_inter-1):
            if obtem_lin(lst_ord_tup[index_inter]) > obtem_lin(lst_ord_tup[index_inter + 1]):
                lst_ord_tup[index_inter],lst_ord_tup[index_inter+1] = lst_ord_tup[index_inter+1],lst_ord_tup[index_inter]
    
            elif obtem_lin(lst_ord_tup[index_inter]) == obtem_lin(lst_ord_tup[index_inter + 1]): 
                # Ordenar os interseções numerados por ordem crescente, e por ordem alfabetica
                if obtem_col(lst_ord_tup[index_inter]) > obtem_col(lst_ord_tup[index_inter + 1]):
                    lst_ord_tup[index_inter], lst_ord_tup[index_inter + 1] = lst_ord_tup[index_inter + 1], lst_ord_tup[index_inter]
        
    return tuple(lst_ord_tup)


####
#   TAD pedra
#
#   O TAD pedra é usado para representar as pedras do Go. As pedras podem pertencer
#   ao jogador branco (’O’) ou ao jogador preto (’X’). Por conviniência, é também definido
#   o conceito pedra neutra, que é uma pedra que não pertence a nenhum jogador.
#
#   Representação interna : 'O', 'X', '.' (str) → Pedra brancas, pretas e neutras, respetivamente
####

def cria_pedra_branca():
    # {} → pedra
    '''
        Devolve uma pedra pertencente ao jogador branco.
    '''
    return 'O'

def cria_pedra_preta():
    # {} → pedra
    '''
        Devolve uma pedra pertencente ao jogador preto.
    '''
    return 'X'

def cria_pedra_neutra():
    # {} → pedra
    '''
        Devolve uma pedra neutra.
    '''
    return '.'


####  
#   Reconhecedor
####

def eh_pedra(p_pedra):
    # universal → booleano
    '''
        Devolve True caso o seu argumento seja um TAD pedra e False 
        caso contrário.
    '''
    if p_pedra == 'X' or p_pedra == 'O' or p_pedra == '.':
        return True
    else:
        return False
    
def eh_pedra_branca(p_pedra_b):
    '''
        Devolve True caso a pedra dada como argumento seja do jogador branco e False 
        caso contrário.
    '''
    # pedra → booleano
    if p_pedra_b == 'O':
        return True
    else:
        return False
    
def eh_pedra_preta(p_pedra_p):
    # pedra → booleano
    '''
        Devolve True caso a pedra dada como argumento seja do jogador preto e False 
        caso contrário.
    '''
    if p_pedra_p == 'X':
        return True
    else:
        return False
    

####  
#   Teste
####

def pedras_iguais(pedra_1, pedra_2):
    # universal x universal → booleano
    '''
        Devolve True apenas se dois argumentos são pedras e são
        iguais, e False caso contrário.
    '''
    if eh_pedra(pedra_1) and eh_pedra(pedra_2):
        if pedra_1 == pedra_2: 
            return True
        else:
            return False
    else:
        return False
    
    
####  
#   Transformador
####

def pedra_para_str(pedra_tab):
    # pedra → str
    '''
        Devolve a cadeia de caracteres que representa o jogador dono
        da pedra, isto é, 'O', 'X' ou '.' para pedras do jogador branco, preto ou neutra
        respetivamente.
    '''
    if eh_pedra_branca(pedra_tab):
        return 'O'
    if eh_pedra_preta(pedra_tab):
        return 'X'
    else:
        return '.'


####  
#   Funções de Alto Nível
####

def eh_pedra_jogador(pedra_tab):
    # pedra → str
    '''
    Devolve True caso a pedra dada como argumento seja de um jogador e False caso contrário.
    '''
    if eh_pedra_preta(pedra_tab) or eh_pedra_branca(pedra_tab):
        return True
    else:
        return False


####
#   TAD goban
#
#   O TAD goban é usado para representar um tabuleiro do jogo Go e as pedras dos jogadores
#   que nele são colocadas
#
#   Representação interna : [[N, N, N, ...],[N, N, N, ...],[N, N, N,...],...] → Lista de sublistas em que os seus 
#                           elementos (str), representam as pedras possíveis.
#
#                           É definido pelo número de sublistas, que terá que ser igual ao número de elementos de 
#                           cada sublista (Só poderá ser entre 9, 13 e 19) - Dimensão do goban
#
#                           N → pedra neutra
#                           P → pedra preta
#                           B → pedra branca
####
  
def lado_tab_valido(n):
    # int → booleano
    '''
        Recebe um inteiro e devolve True caso seja uma dimensão do goban
        válida - Função Auxiliar.
    '''
    return n != 9 and n != 13 and n != 19 or type(n) != int

def eh_intersecao_valida_aux(n, inter_t):
    # int x intersecao → booleano
    '''
        Devolve True se a interseção for válida no goban e 
        False, caso contrário - Função Auxiliar.
    '''
    if eh_intersecao(inter_t):
        col_inter = obtem_col(inter_t)
        li_inter = obtem_lin(inter_t)

        #Verificar se está dentro do âmbito do território
        if ord(col_inter) <= n+65 and li_inter <= n:
            return True
        
        return False
    return False

def goban_val_er(n, pedras_b_t, pedras_p_t):
    # int x tuplo x tuplo → booleano
    '''
        Devolve False se os argumentos fizerem sentido no território, ou seja,
        se o n corresponder à dimensão correta, e se os elementos dos tuplos 
        dados como argumento correspondem a interseções válidas, caso contrário
        e dê um ValueError ou TypeError, devolve True - Função Auxiliar.
    '''
    try:
        return lado_tab_valido(n) or \
            type(pedras_p_t) != tuple or not all(eh_intersecao_valida_aux(n, i) and i not in pedras_b_t for i in pedras_p_t) or \
                type(pedras_b_t) != tuple or not all(eh_intersecao_valida_aux(n,k) and k not in pedras_p_t for k in pedras_b_t) or\
                len(pedras_p_t) != len(set(pedras_p_t)) or len(pedras_b_t) != len(set(pedras_b_t))
    except (ValueError, TypeError):
        return True
    

####
#   Construtores
####

def cria_goban_vazio(n):
    # int → goban
    '''
        Devolve um goban de tamanho n×n, sem interseções ocupadas. O construtor 
        verifica a validade do argumento, gerando um ValueError.
    '''
    if lado_tab_valido(n):
        raise ValueError("cria_goban_vazio: argumento invalido")
    
    return [['N' for i in range(n)] for i in range(n)]

def cria_goban(n, pedras_b_t, pedras_p_t):
    # int x tuplo x tuplo → goban
    '''
    Devolve um goban de tamanho n × n, com as interseções
    do tuplo ocupadas por pedras brancas e as interseções do tuplo ocupadas
    por pedras pretas. 
    
    O construtor verifica a validade dos argumentos, gerando
    um ValueError caso os seus argumentos não sejam válidos.
    '''
    try:
        if goban_val_er(n, pedras_b_t, pedras_p_t):
            raise ValueError("cria_goban: argumentos invalidos")
        
        # criar goban vazio e substituir todos os elementos pertencentes a cada elemento do tuplo
        tabuleiro_goban = cria_goban_vazio(n)

        for pedras_p_inter in pedras_p_t:
            tabuleiro_goban[ord(obtem_col(pedras_p_inter))-65][obtem_lin(pedras_p_inter)-1] = 'P'

        for pedras_b_inter in pedras_b_t:
            tabuleiro_goban[ord(obtem_col(pedras_b_inter))-65][obtem_lin(pedras_b_inter)-1] = 'B'

        return tabuleiro_goban
    
    except ValueError:
        raise ValueError("cria_goban: argumentos invalidos")

def cria_copia_goban(goban_tab):
    # goban → goban
    '''
        Recebe um goban e devolve a deep copy do goban
    '''
    return [colunas[:] for colunas in goban_tab]

####
#   Seletores
####

def obtem_ultima_intersecao(goban_tab):
    # goban → intersecao
    '''
        Devolve a interseção que corresponde ao canto superior direito do goban goban_tab
    '''
    n = len(goban_tab)
    ult_col = chr(n + 64)
    ult_lin = n

    return cria_intersecao(ult_col,ult_lin)

def obtem_pedra(goban_tab, inter_t):
    # goban x intersecao → pedra
    '''
        Devolve a pedra na interseção inter_t do goban goban_tab. Se a interseção
        não estiver ocupada, devolve uma pedra neutra.
    '''
    # Obter os index da interseção no TAD goban
    col_inter_index = ord(obtem_col(inter_t)) - 65
    lin_inter_index = obtem_lin(inter_t)-1
    inter_tab = goban_tab[col_inter_index][lin_inter_index]

    if inter_tab == 'P':
        return cria_pedra_preta()
    
    if inter_tab == 'B':
        return cria_pedra_branca()
    
    if inter_tab == 'N':
        return cria_pedra_neutra()

def obtem_cadeia(goban_tab, inter_t):
    # goban x intersecao → tuplo
    '''
        Devolve o tuplo formado pelas interseções (em ordem de leitura) das pedras
        da mesma cor que formam a cadeia que passa pela interseção inter_t. Se a 
        posição não estiver ocupada, devolve a cadeia de interseções livres.
    '''
    pedra_og = obtem_pedra(goban_tab, inter_t)

    # Criar uma lista para adicionar as interseções da cadeia
    cadeia = []

    # Criar uma lista para verificar as interseções já verificadas
    para_verificar = [inter_t]

    ult_inter = obtem_ultima_intersecao(goban_tab)

    while para_verificar:
        # Retirar a interseção atual e colocar na cadeia
        inter_t = para_verificar.pop()
        cadeia.append(inter_t)

        for nova_inter in obtem_intersecoes_adjacentes(inter_t, ult_inter):
            #Verificar se a interseção adjacente é do mesmo tipo da original
            if pedra_og == obtem_pedra(goban_tab, nova_inter) and nova_inter not in cadeia + para_verificar:
                para_verificar.append(nova_inter)

    # Ordenar a cadeia pela ordem de leitura
    return ordena_intersecoes(cadeia)


####
#   Modificadores
####

def coloca_pedra(goban_tab, inter_t, pedra_tab):
    # goban x intersecao x pedra → goban
    '''
        Modifica destrutivamente o goban goban_tab colocando a pedra 
        do jogador pedra_tab na interseção inter_t, e devolve o próprio goban.
    '''
    # Obter o index específico da interseção no goban
    col_inter_index = ord(obtem_col(inter_t)) - 65
    lin_inter_index = obtem_lin(inter_t)-1
    
    if eh_pedra_branca(pedra_tab):
        goban_tab[col_inter_index][lin_inter_index] = 'B'
        return goban_tab
            
    elif eh_pedra_preta(pedra_tab):
        goban_tab[col_inter_index][lin_inter_index] = 'P'
        return goban_tab
    
    elif not eh_pedra_preta(pedra_tab) and not eh_pedra_branca(pedra_tab):
        goban_tab[col_inter_index][lin_inter_index] = 'N'
        return goban_tab

def remove_pedra(goban_tab, inter_t):
    # goban x intersecao → goban
    '''
        Modifica destrutivamente o goban g removendo a pedra
        da interseção i, e devolve o próprio goban.
    '''
    return coloca_pedra(goban_tab, inter_t, cria_pedra_neutra())

def remove_cadeia(goban_tab, inter_t):
    # goban x tuplo → goban
    '''
        Modifica destrutivamente o goban goban_tab removendo as pedras
        nas interseções to tuplo inter_t, e devolve o próprio goban.
    '''
    for inter_cadeia in inter_t:
        remove_pedra(goban_tab, inter_cadeia)

    return goban_tab


####
#   Reconhecedor
####

def eh_goban(p_goban):
    # universal → booleano
    '''
        Devolve True caso o seu argumento seja um TAD goban e False
        caso contrário.
    '''
    return type(p_goban) == list and \
        all(type(coluna) == list and len(p_goban) == len(coluna) == len(p_goban[0]) and len(coluna) in (9, 13, 19) for coluna in p_goban) and \
        all(linha in ('N', 'P', 'B') for colunas in p_goban for linha in colunas)
        
def eh_intersecao_valida(goban, inter_t):
    # goban x intersecao → booleano
    '''
        Devolve True se inter_t é uma interseção válida dentro do
        goban g e False caso contrário.
    '''
    ult_inter = obtem_ultima_intersecao(goban)
    col_max, li_max = obtem_col(ult_inter), obtem_lin(ult_inter)
    
    if eh_intersecao(inter_t):
        col_inter = obtem_col(inter_t)
        li_inter = obtem_lin(inter_t)

        if col_inter <= col_max and li_inter <= li_max:
            return True
        
        return False
    
    return False
    
def gobans_iguais(p_goban1, p_goban2):
    # universal x universal → booleano
    '''
        Devolve True apenas se p_goban1 e p_goban2 forem gobans e forem iguais.
    '''
    if eh_goban(p_goban1) and eh_goban(p_goban2):
       return p_goban1 == p_goban2
    
    return False


####
#   Transformador
####

def goban_para_str(goban_tab):
    # goban → str
    '''
        Devolve a cadeia de caracteres que representa o goban
    '''
    ult_inter = obtem_ultima_intersecao(goban_tab)
    # Obter o número de linhas máxima do território, para poder iterar sobre esse número
    col_max, li_max = obtem_col(ult_inter), obtem_lin(ult_inter)
    colunas = [chr(i) for i in range(ord('A'), ord(col_max) + 1) if 'A' <= chr(i) <= 'S'] 
    goban_str = '  '

    # 'varrer'o teabuleiro de cima para baixo

    for colunas_letra in colunas: 
        # Primeira linha do tabuleiro corresponde à letra de cada coluna
        goban_str += ' ' + colunas_letra 
        if colunas_letra == colunas[-1]:
            goban_str += '\n'
    index_col_max = ord(col_max)-65
    index_col = 0
    while li_max >= 1: 
        # Cada linha do território com o numero da linha e o tipo de interseções
        
        if index_col == index_col_max + 1:
            index_col = 0
            linha_n = goban_tab[index_col]
        else:
            linha_n = goban_tab[index_col]

        if li_max < 10:
            goban_str += ' '
        goban_str += str(li_max)

        for inter_linha in linha_n:

            if eh_pedra_branca(obtem_pedra(goban_tab,cria_intersecao(chr(index_col+65), li_max))):
                goban_str += ' ' + cria_pedra_branca()
            
            elif eh_pedra_preta(obtem_pedra(goban_tab,cria_intersecao(chr(index_col+65), li_max))):
                goban_str += ' ' + cria_pedra_preta()
           
            else:
                goban_str += ' ' + cria_pedra_neutra()
            
            index_col += 1
        
        if li_max < 10:
            goban_str += '  '
        else:
            goban_str += ' '
        
        goban_str += str(li_max) + '\n'
        li_max -= 1
        
    goban_str += '  '
    
    # Última linha do território corresponde à letra de cada coluna
    for colunas_letra in colunas:
        goban_str += ' ' + colunas_letra 

    return goban_str

def n_goban(goban_tab):
    # goban → int
    '''
        Recebe o goban e devolve o seu número de linhas ou colunas - Função Auxiliar
    '''
    return obtem_lin(obtem_ultima_intersecao(goban_tab))

def obtem_str_inter(n):
    # int → tuplo
    '''
        Recebe o numero de linhas ou colunas do goban e devolve o 
        tuplo com a representação externa das todas as interseções possíveis - Função Auxiliar
    '''
    return tuple(tuple(f'{chr(i)}{j}' for i in range(ord('A'), ord('A') + n)) for j in range(1, n + 1))


####
#   Funções de Alto Nível
####

def obtem_territorios(goban_tab):
    # goban → tuplo
    '''
        Devolve o tuplo formado pelos tuplos com as interseções de
        cada território de goban_tab. A função devolve as interseções 
        de cada território ordenadas em ordem de leitura do tabuleiro de Go,
        e os territórios ordenados em ordem de leitura da primeira interseção 
        do território.
    '''
    todos_ter = []
    # Armazenar todas as cadeias vistas
    cadeias_vistas = ()

    n = n_goban(goban_tab)
    inter_total_str = obtem_str_inter(n)

    for colunas in inter_total_str:
        for inter_str in colunas:
            inter_tab = str_para_intersecao(inter_str)
            if obtem_pedra(goban_tab, inter_tab) == cria_pedra_neutra() and inter_tab not in cadeias_vistas:
                esta_cadeia = obtem_cadeia(goban_tab, inter_tab)
                todos_ter.append(esta_cadeia)
                cadeias_vistas += esta_cadeia

    return tuple(todos_ter)

def obtem_adjacentes_diferentes(goban_tab, inter_t):
    # goban x intersecao → tuplo
    '''
        Devolve o tuplo ordenado formado pelas interseções
        adjacentes às interseções do tuplo inter_t:

        1 - livres, se as interseções do tuplo t estão ocupadas por pedras de jogador;
        2 - ocupadas por pedras de jogador, se as interseções do tuplo t estão livres.
    '''
    todos_adj = []
    adj_vistos = []
    ult_inter = obtem_ultima_intersecao(goban_tab)

    # Função auxiliar no caso de inter_t ser uma interseção que corresponde a uma pedra de jogador
    def obtem_adjacentes_diferentes_livres(goban_tab,inter_t):
        for inter in inter_t:
            for adjacente in obtem_intersecoes_adjacentes(inter, ult_inter):
                if obtem_pedra(goban_tab, adjacente) == cria_pedra_neutra() and adjacente not in adj_vistos:
                    todos_adj.append(adjacente)
                    adj_vistos.append(adjacente)
        todos_adj_nr = list(set(todos_adj))
        return ordena_intersecoes(todos_adj_nr)
    
    # Função auxiliar no caso de inter_t ser uma interseção que corresponde a uma pedra neutra
    def obtem_adjacentes_diferentes_pedras(goban_tab,inter_t):
        for inter in inter_t:
            for adjacente in obtem_intersecoes_adjacentes(inter, ult_inter):
                if obtem_pedra(goban_tab, adjacente) == cria_pedra_branca() or obtem_pedra(goban_tab, adjacente) == cria_pedra_preta() and adjacente not in adj_vistos:
                    todos_adj.append(adjacente)
                    adj_vistos.append(adjacente)
        todos_adj_nr = list(set(todos_adj))
        return ordena_intersecoes(todos_adj_nr)
   
    if all(eh_pedra_preta(obtem_pedra(goban_tab,inter_tab)) or eh_pedra_branca(obtem_pedra(goban_tab,inter_tab)) for inter_tab in list(inter_t)):
        # Liberdades
        return tuple(obtem_adjacentes_diferentes_livres(goban_tab,inter_t))
    
    if all(not eh_pedra_preta(obtem_pedra(goban_tab,inter_tab)) or not eh_pedra_preta(obtem_pedra(goban_tab,inter_tab)) for inter_tab in list(inter_t)):
        # Fronteira
        return tuple(obtem_adjacentes_diferentes_pedras(goban_tab,inter_t))

def jogada(goban_tab, inter_t, pedra_tab):
    # goban x intersecao x pedra → goban
    '''
        Modifica destrutivamente o goban goban_tab colocando a pedra de jogador
        pedra_tab na interseção inter_t e remove todas as pedras do jogador contrário pertencentes a
        cadeias adjacentes à inter_t sem liberdades, devolvendo o próprio goban.
    '''
    ult_inter = obtem_ultima_intersecao(goban_tab)

    # Colocar
    coloca_pedra(goban_tab, inter_t, pedra_tab)

    cad_inter = obtem_intersecoes_adjacentes(inter_t, ult_inter)

    for inter_adj in cad_inter:
        adj_dif_cad = obtem_cadeia(goban_tab,inter_adj)

        if obtem_pedra(goban_tab,inter_adj) != obtem_pedra(goban_tab,inter_t):
            if len(obtem_adjacentes_diferentes(goban_tab, adj_dif_cad)) == 0:
                # Capturar
                remove_cadeia(goban_tab, adj_dif_cad)

    cad_pedra = obtem_cadeia(goban_tab, inter_t)

    if len(obtem_adjacentes_diferentes(goban_tab, cad_pedra)) == 0:
        # Remover a cadeia da pedra colocada se não tiver liberdades
        # Será posteriormente denominada de suicídio 
        remove_cadeia(goban_tab, cad_pedra)

    return goban_tab

def obtem_pedras_jogadores(goban_tab):
    # goban → tuplo
    '''
        Devolve um tuplo de dois inteiros que correspondem ao número de interseções
        ocupadas por pedras do jogador branco e preto, respetivamente.
    '''
    inter_total = obtem_str_inter(n_goban(goban_tab))
    count_preta = 0
    count_branca = 0

    # Percorrer todas as interseções através da sua representação externa
    for colunas in inter_total:
        for inter_str in colunas:

            # Mudar para a representação interna 
            inter_tab = str_para_intersecao(inter_str)

            if eh_pedra_branca(obtem_pedra(goban_tab, inter_tab)):
                count_branca += 1
            elif eh_pedra_preta(obtem_pedra(goban_tab,inter_tab)):
                count_preta += 1

    return count_branca,count_preta


####
#   Funções Adicionais
####

def calcula_pontos(goban_tab):
    # goban → tuplo
    '''
        Recebe um goban e devolve o tuplo de dois inteiros com as pontuações
        dos jogadores branco e preto, respetivamente.
    '''
    count_branca = obtem_pedras_jogadores(goban_tab)[0]
    count_preta = obtem_pedras_jogadores(goban_tab)[1]
    n = n_goban(goban_tab)

    if goban_tab != cria_goban_vazio(n):
        for territorio_tab in obtem_territorios(goban_tab):

            if all(eh_pedra_branca(obtem_pedra(goban_tab,adj_dif)) for adj_dif in obtem_adjacentes_diferentes(goban_tab,territorio_tab)):
                # Verificar se o território só tem fronteira de pedras brancas
                count_branca += len(territorio_tab)
                
            elif all(eh_pedra_preta(obtem_pedra(goban_tab,adj_dif)) for adj_dif in obtem_adjacentes_diferentes(goban_tab,territorio_tab)):
                # Verificar se o território só tem fronteira de pedras pretas
                count_preta += len(territorio_tab)

    return count_branca, count_preta

def eh_jogada_legal(goban_tab, inter_tab, pedra_tab, goban_l):
    # goban x intersecao x pedra x goban → booleano
    '''
        Recebe um goban goban_tab, uma interseção inter_tab, uma pedra de jogador pedra_tab 
        e um outro goban goban_l e devolve True se a jogada for legal ou
        False caso contrário.
    '''
    try:
        goban_med = cria_copia_goban(goban_tab)
        jogada(goban_med, inter_tab, pedra_tab)

        # A interseção está fora do âmbito do goban
        if not eh_intersecao_valida(goban_tab,inter_tab):
            return False
        
        # A pedra colocada desapareceu após a jogada 
        if obtem_pedra(goban_med,inter_tab) == cria_pedra_neutra():
            return False
        
        # A cadeia onde está colocada a pedra ficou sem liberdades (suicídio)
        if len(obtem_adjacentes_diferentes(goban_med, (obtem_cadeia(goban_med,inter_tab)))) == 0:
            return False
        
        # A interseção está ocupada por outra pedra de jogador
        if obtem_pedra(goban_tab,inter_tab) in (cria_pedra_branca(), cria_pedra_preta()):
            return  False
        
        # A jogada resulta em repetição
        return not gobans_iguais(goban_med,goban_l)
    
    except (ValueError, IndexError):
        return False


def turno_jogador(goban_tab, pedra_tab, goban_l):
    # goban x pedra x goban → booleano
    '''
        Recebe um goban goban_tab, uma pedra de jogador pedra_tab e um outro goban goban_l, 
        e oferece ao jogador que joga com pedras p a opção de passar ou de colocar uma pedra 
        própria numa interseção. Se o jogador passar, a função devolve False sem modificar os argumentos. 
        Caso contrário, a função devolve True e modifica destrutivamente o tabuleiro g de acordo com a 
        jogada realizada. 
    '''
    inter_nao_valida = True

    while inter_nao_valida:

        # Receber os inputs dos jogadores
        if eh_pedra_branca(pedra_tab):
             jogador_input = input("Escreva uma intersecao ou 'P' para passar [O]:")
        
        if eh_pedra_preta(pedra_tab):
             jogador_input = input("Escreva uma intersecao ou 'P' para passar [X]:")
        
        # Quando o jogador passa
        if jogador_input == 'P':
            return False
        try:
            inter_input = str_para_intersecao(jogador_input)

            # Averiguar se a jogada do Jogador foi legal
            if eh_jogada_legal(goban_tab, inter_input, pedra_tab, goban_l):
                jogada(goban_tab,inter_input, pedra_tab)
                return True
            
        except (ValueError,IndexError):
           inter_nao_valida = True
                

def go(n, inter_tb, inter_tp):
    # int x tuplo x tuplo → booleano
    '''
        Função principal que permite jogar um jogo completo do Go de dois
        jogadores. A função recebe um inteiro correspondente à dimensão do tabuleiro, e dois
        tuplos (potencialmente vazios) com a representação externa das interseções ocupadas
        por pedras brancas (inter_tb) e pretas (inter_tp) inicialmente. 
        
        O jogo termina quando os dois jogadores passam a vez de jogar consecutivamente. 
        A função devolve True se o jogador com pedras brancas conseguir ganhar o jogo, 
        ou False caso contrário. A função deve verificar a validade dos seus argumentos,
        gerando um ValueError com a mensagem caso os seus argumentos não sejam válidos. 
    '''
    try:
        if type(inter_tb) != tuple or type(inter_tp) != tuple or \
            any(len(inter_b) != 2 for inter_b in inter_tb) or \
            any(len(inter_p) != 2 for inter_p in inter_tp) and \
                inter_tb != inter_tp != () :
            raise ValueError('go: argumentos invalidos')
    
        inter_b = tuple(str_para_intersecao(inter_b) for inter_b in inter_tb)
        inter_p = tuple(str_para_intersecao(inter_p) for inter_p in inter_tp)
    
    except (ValueError, IndexError, TypeError):
        raise ValueError('go: argumentos invalidos')
   
    try:
        goban_tab = cria_goban(n, inter_b, inter_p)

    except (ValueError, IndexError, TypeError):
        raise ValueError('go: argumentos invalidos')
    
    jogo_go = True
    count_jogada = 0
    npassou_branca = True

    res = calcula_pontos(goban_tab)
    print(f'Branco (O) tem {res[0]} pontos\nPreto (X) tem {res[1]} pontos\n{goban_para_str(goban_tab)}')

    # no inicio os tabuleiros dos jogadores têm que ser iguais ao tabuleiro inicial
    goban_jogp = goban_jogb = cria_copia_goban(goban_tab)
    
    while jogo_go:

        if count_jogada % 2 == 0:
            npassou_preta = turno_jogador(goban_tab, cria_pedra_preta(), cria_copia_goban(goban_jogp))
            # Manter o tabuleiro do jogador preto após a sua jogada
            goban_jogp = cria_copia_goban(goban_tab)
       
        else:
            npassou_branca = turno_jogador(goban_tab, cria_pedra_branca(), cria_copia_goban(goban_jogb))
            # Manter o tabuleiro do jogador branco após a sua jogada
            goban_jogb = cria_copia_goban(goban_tab)

        count_jogada += 1
        res = calcula_pontos(goban_tab)
        # Se ambos os jogadores passaram a vez
        if not npassou_branca and not npassou_preta:

            print(f'Branco (O) tem {res[0]} pontos\nPreto (X) tem {res[1]} pontos')
            print(goban_para_str(goban_tab))

            # Validar os pontos e retornar o vencedor
            if res[0] >= res[1]:
                return True
            else:
                return False
            
        # Se o jogo continua
        elif npassou_branca or npassou_preta:
            print(f'Branco (O) tem {res[0]} pontos\nPreto (X) tem {res[1]} pontos')
            print(goban_para_str(goban_tab))