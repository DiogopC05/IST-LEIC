"""

LEIC-A Tecnico 

Projeto1: Montanhas e vales
Aluno: ist1109955 (Diogo Fontes Carreira)
Ano Letivo: 2023/2024

"""


#####################################################################################################

def eh_territorio(t_ter):
    # universal -> booleano
    """Esta função recebe um território como argumento.
    Devolve True se for território, False caso contrário.
    """
    
    if not type(t_ter) == tuple: 
        # Verificar se é tupla
        return False
    
    for subtuplos in t_ter:
        if not type(subtuplos) == tuple: 
            # Verificar se só existem subtuplos
            return False
    
        for entrada in subtuplos:
            if entrada != 0 and entrada != 1 or not type(entrada) == int: 
                # Verificar se só são as entradas 0, ou 1
                return False

    for index in range(len(t_ter)):
        if len(t_ter[0]) != len(t_ter[index]):  
            # Verificar se todos os subtuplos têm o mesmo comprimento
            return False
        
        if len(t_ter[index]) < 1 or len(t_ter[index]) > 99: 
            # Verificar se os subtuplos tem tamanho maior que 1 e menor que 99
            return False
        
    if len(t_ter) < 1 or len(t_ter) > 26: 
        # Verificar se existem entre 1 a 26 colunas
        return False
    
    return True
           
#####################################################################################################

def lista_coluna():
    # Sem argumento -> lista
    """Esta função devolve a lista das colunas máxima possível 
    de acordo com as regras do território, ou seja de A a Z, 
    por ordem alfabetica - Função auxiliar.
    """

    colunas_ter_max = []

    for ascii_valor in range(65,91): 
        # Usar o código ASCII para obter uma lista com os elementos de A a Z
        letra_M = chr(ascii_valor)
        colunas_ter_max.append(letra_M)

    return colunas_ter_max

def lista_linha():
    # Sem argumento -> lista
    """Esta função devolve a lista das linhas máxima possível
    de acordo com as regras do território, ou seja de 1 a 99, 
    por ordem crescente - Função auxiliar.
    """

    linhas_ter_max = []

    for num in range (1, 100): 
        # Iterar para obter uma lista com os inteiros de 1 a 99
        linhas_ter_max.append(num) 

    return linhas_ter_max

#####################################################################################################


def obtem_ultima_intersecao(t_ter):
    # territorio -> intersecao
    """Esta função recebe um território e devolve a interseção que 
    ocupa o extremo superior direito do território, usando os índices
    comuns da lista de linhas e colunas com os índices do território.
    """
    
    colunas_ter_max = lista_coluna()
    linhas_ter_max = lista_linha()
    # Obter o indice maximo de colunas
    i_ult_col = len(t_ter) - 1 
    # Obter o índice maximo de linhas
    i_ult_l = len((t_ter[0])) - 1 
    
    return (colunas_ter_max[i_ult_col], linhas_ter_max[i_ult_l],)
    

#####################################################################################################

def cria_linhas(ul_inter):
    # intersecao -> lista
    """Esta função recebe uma interseção que corresponde ao extremo 
    superior direito do território e devolve uma lista com todas as 
    linhas do território (representadas de 1 a 99) - Função auxiliar
    """
    
    # Obter a lista de todas as linhas que existem
    return list(range(1,ul_inter[1]+1)) 
 
def cria_colunas(ul_inter):
    # intersecao -> lista
    """Esta função recebe uma interseção que corresponde ao extremo 
    superior direito do território e devolve uma lista com todas as
    colunas do territorio (representadas de 1 a 99) - Função auxiliar
    """
    
    # Obter a lista de todas as colunas que existem
    return [chr(letra) for letra in range(65, ord(ul_inter[0])+1)]

def cria_intersecoes(ul_inter):
    # intersecao -> lista
    """Esta função recebe uma interseção que corresponde ao extremo 
    superior direito do território e devolve uma lista com todas as
    interseções do território do tipo('Letra', Número) - Função auxiliar
    """

    linhas = cria_linhas(ul_inter)
    colunas = cria_colunas(ul_inter)
    intersecoes_total = []

    for col in colunas:
        for li in linhas:
            intersecoes_total.append((col,li))
            

    return intersecoes_total


#####################################################################################################


def eh_intersecao(e_inter):
    # universal -> booleano
    """Esta função recebe uma interseção e devolve True se o 
    argumento corresponde a uma interseção ou Falso, caso contrário
    """

    if not isinstance(e_inter, tuple) or len(e_inter) != 2: 
        # Verificar se é tuplo e tem dois elementos
        return False
    
    # Buscar a lista de colunas e linhas possiveis
    col = lista_coluna() 
    li = lista_linha()

    if  not e_inter[0] in col or e_inter[1] not in li or not type(e_inter[1]) == int: 
        # Usar a semelhança de index do territorio e o das colunas e linhas totais
        return False
    
    return True
    

#####################################################################################################

def eh_intersecao_valida(t_ter, e_inter):
    # territorio X intersecao -> booleano
    """Esta função recebe um território e uma interseção e devolve True se o 
    argumento corresponde a uma interseção desse território ou Falso, caso contrário
    """

    if eh_territorio(t_ter) and eh_intersecao(e_inter):
        # Obtem a última interseção do extremo superior direito
        ul_inter = obtem_ultima_intersecao(t_ter) 
        inter_total = cria_intersecoes(ul_inter)
        
        if e_inter not in inter_total:
            return False
        
        return True
    return False

#####################################################################################################

def eh_intersecao_livre(t_ter, e_inter):
    # territorio X intersecao -> booleano
    """Esta função recebe um território e uma interseção e devolve True se o 
    argumento corresponde a uma interseção sem montanha ou Falso, caso seja montanha
    """
    

    ul_inter = obtem_ultima_intersecao(t_ter)
    li_v = cria_linhas(ul_inter)
    col_v = cria_colunas(ul_inter)
    index_col = col_v.index(e_inter[0]) 
    index_li = li_v.index(e_inter[1])

    # Obtem o index para a entrada respetiva no território
    ter_ponto = t_ter[index_col][index_li] 
    if ter_ponto == 0:
        return True
        
    return False   

#####################################################################################################

def obtem_intersecoes_adjacentes(t_ter, e_inter):
    # territorio X intersecao -> tuplo
    """Esta função recebe um território e uma interseção e 
    devolve um tuplo com todas as interseções adjacentes à mesma, por ordem de leitura, 
    ou seja: baixo, esquerda, direita, cima (quando se refere a adjacentes)
    """
    
    coluna, linha = e_inter

    inter_adjacente = []
    # Lista com as possibilidades de interseções adjacentes
    possivel_adjacente = [(coluna, linha - 1), # baixo
                            (chr(ord(coluna) - 1),linha), # esquerda
                            (chr(ord(coluna) + 1),linha), # direita
                            (coluna, linha + 1), # cima
                            ] 
    for adj in possivel_adjacente: 
        if eh_intersecao_valida(t_ter, adj):
            # Verificação se as interseções adjacentes são válidas
            inter_adjacente.append(adj)

    return tuple(inter_adjacente)

#####################################################################################################

def ordena_intersecoes(tup_inter):
    # tuplo -> tuplo
    """Esta função recebe um tuplo de interseções (possivelmente vazio) e devolve um tuplo 
    de interseções por ordem de leitura do território: começa sempre com ('A',1) dando 
    prioridade da esquerda para a direita e de seguida de baixo para cima.
    """

    if tup_inter == ((),):
        return tup_inter
    
    # Usar lista para puder permutar elementos usando bubble sort
    lst_ord_tup = list(tup_inter) 
    tamanho_inter = len(lst_ord_tup)

    for itera_lista in range(tamanho_inter): 
        # Ordenar os tuplos com os números
        for index_inter in range(tamanho_inter-1):
            if lst_ord_tup[index_inter][1] > lst_ord_tup[index_inter + 1][1]:
                lst_ord_tup[index_inter],lst_ord_tup[index_inter+1] = lst_ord_tup[index_inter+1],lst_ord_tup[index_inter]
    
            elif lst_ord_tup[index_inter][1] == lst_ord_tup[index_inter + 1][1]: 
                # Ordenar os tuplos numerados por ordem crescente, e por ordem alfabetica
                if lst_ord_tup[index_inter][0] > lst_ord_tup[index_inter + 1][0]:
                    lst_ord_tup[index_inter], lst_ord_tup[index_inter + 1] = lst_ord_tup[index_inter + 1], lst_ord_tup[index_inter]
        
    return tuple(lst_ord_tup)

#####################################################################################################

def obtem_linhas(inter_total, n):  
    # tuplo X int -> tuplo
    """Esta função recebe um tuplo de interseções e um inteiro e devolve o tuplo 
    de interseções da linha com o número correspondente ao argumento do tipo inteiro - Função auxiliar
    """

    inter_linha_n = ()

    for inter in inter_total:
        if inter[1] == n:
            #Obtem o tuplo das interseções da linha n
            inter_linha_n += (inter,)

    return inter_linha_n

#####################################################################################################

def territorio_para_str(t_ter):
    # territorio -> cad. carateres
    """Esta função recebe um território e devolve uma cadeia de caracteres que é a representação
    externa do tabuleiro, se o argumento não for território, levanta-se um ValueError
    """

    if not eh_territorio(t_ter):
        raise ValueError('territorio_para_str: argumento invalido')
    
    ult_inter = obtem_ultima_intersecao(t_ter)
    # Obter o número de linhas máxima do território, para poder iterar sobre esse número
    linha_max = ult_inter[1]
    colunas = cria_colunas(ult_inter)
    inter_total = ordena_intersecoes(cria_intersecoes(ult_inter))
    ter_str = '  '

    # 'varrer'o território de cima para baixo

    for colunas_letra in colunas: 
        # Primeira linha do território corresponde à letra de cada coluna
        ter_str += ' ' + colunas_letra 
        if colunas_letra == colunas[-1]:
            ter_str += '\n'

    while linha_max >= 1: 
        # Cada linha do território com o numero da linha e o tipo de interseções

        linha_n = obtem_linhas(inter_total, linha_max)

        if linha_max < 10:
            ter_str += ' '
        ter_str += str(linha_max)

        for inter_linha in linha_n:
            if eh_intersecao_livre(t_ter,inter_linha):
                ter_str += ' ' + '.'
            else:
                ter_str += ' ' + 'X'
        if linha_max < 10:
            ter_str += '  '
        else:
            ter_str += ' '
        ter_str += str(linha_max) + '\n'
        linha_max -= 1
        
    ter_str += '  '
    # Última linha do território corresponde à letra de cada coluna
    for colunas_letra in colunas:
        ter_str += ' ' + colunas_letra 

    return ter_str


#####################################################################################################

def explorar_adjacente(t_ter,e_inter, inter_cadeia):
    # território X intersecao X tuplo -> tuplo
    """Esta função recebe um território, uma interseção e um tuplo contendo essa interseção
    e devolve um tuplo com todas as interseções conectadas a essa interseção, 
    contando com a própria - Função auxiliar
    """

    adj_inters = obtem_intersecoes_adjacentes(t_ter, e_inter)
    # Adicionar à inter_cadeia todos os pontos adjacentes possíveis, formando um caminho
    for inter_adj in adj_inters:
        if inter_adj not in inter_cadeia and eh_intersecao_livre(t_ter, e_inter) == eh_intersecao_livre(t_ter, inter_adj):
            inter_cadeia += (inter_adj,)

            for adj in explorar_adjacente(t_ter, inter_adj, inter_cadeia): 
                # Obter as adjacentes das adjacentes, recursivamente
                if adj not in inter_cadeia:
                    inter_cadeia += (adj,)

    return inter_cadeia
        
#####################################################################################################

def obtem_cadeia(t_ter, e_inter):
    # território X intersecao -> tuplo
    """Esta função recebe um território, um território e uma interseção e devolve um tuplo 
    com todas interseções conectadas ordenadas por ordem de leitura do território, levanta
    erro quando os argumentos são inválidos.
    """

    if not eh_territorio(t_ter) or not eh_intersecao_valida(t_ter,e_inter):
        raise ValueError('obtem_cadeia: argumentos invalidos')
    
    inter_cadeia = (e_inter,)

    return ordena_intersecoes(explorar_adjacente(t_ter,e_inter, inter_cadeia))

##################################################################################################### 
       
def obtem_vale_aux(t_ter, e_inter):
    # território X intersecao -> tuplo
    """Esta função recebe um território e uma interseção que é montanha, e devolve um tuplo 
    de interseções contendo todos os vales dessa montanha, sem verificar argumentos - Função auxiliar
    """

    inter_vales = ()
    adjacentes_t = ()
    cadeia_montanha = obtem_cadeia(t_ter, e_inter)

    for mont_inter in cadeia_montanha:
        adjacentes_t += obtem_intersecoes_adjacentes(t_ter, mont_inter)

    for inter_adj in adjacentes_t:
        if eh_intersecao_livre(t_ter, inter_adj) and inter_adj not in inter_vales:
            inter_vales += (inter_adj,)

    return ordena_intersecoes(inter_vales)

def obtem_vale(t_ter, e_inter):
    # território X intersecao -> tuplo
    """Esta função recebe um território e uma interseção que é montanha, e devolve o 
    resultado da função auxiliar, levantando erro se os argumentos forem inválidos.
    """

    if not eh_territorio(t_ter) or not eh_intersecao_valida(t_ter,e_inter) or eh_intersecao_livre(t_ter, e_inter) or not eh_intersecao_valida(t_ter, e_inter):
        raise ValueError('obtem_vale: argumentos invalidos')
    
    return obtem_vale_aux(t_ter, e_inter)

#####################################################################################################

def verifica_conexao(t_ter, e_inter1, e_inter2):
    # território X intersecao X intersecao -> booleano
    """Esta função recebe um território e duas interseções, e devolve True se estiverem
    conectadas e False caso contrário, levantando erro se os argumentos forem inválidos.
    """

    if not eh_territorio(t_ter) or not eh_intersecao_valida(t_ter,e_inter1) or not eh_intersecao_valida(t_ter,e_inter2):
        raise ValueError('verifica_conexao: argumentos invalidos')
    
    
    cadeia_inter1 = obtem_cadeia(t_ter, e_inter1) 

    if e_inter2 not in cadeia_inter1:
        # Se uma não estiver na cadeia da outra, então não estão conectadas
        return False
    
    else:
        return True
    
#####################################################################################################

def calcula_numero_montanhas(t_ter):
    # território -> int
    """Esta função recebe um território e devolve um inteiro que representa o número de 
    montanhas no território, levantando erro se o argumento for inválido.
    """

    if not eh_territorio(t_ter):
        raise ValueError('calcula_numero_montanhas: argumento invalido')
    
    ult_inter = obtem_ultima_intersecao(t_ter)
    inter_total = cria_intersecoes(ult_inter)
    count_m = 0

    for inter in inter_total:
        if not eh_intersecao_livre(t_ter, inter):
            count_m += 1

    return count_m

#####################################################################################################

def calcula_numero_cadeias_montanhas(t_ter):
    # território -> int
    """Esta função recebe um território e devolve um inteiro que representa o número de 
    cadeias de montanhas no território, levantando erro se o argumento for inválido.
    """

    if not eh_territorio(t_ter):
        raise ValueError('calcula_numero_cadeias_montanhas: argumento invalido')
    ult_inter = obtem_ultima_intersecao(t_ter)
    inter_total = cria_intersecoes(ult_inter)

    obtem_aux = []
    count_inter = 0
    # Obter a cadeia para cada interseção, adiciona a obtem_aux os elementos da cadeia que encontrou
    for inter in inter_total:
        if inter not in obtem_aux and not eh_intersecao_livre(t_ter, inter):
            cadeia_inter = list(obtem_cadeia(t_ter, inter))
            obtem_aux += cadeia_inter
            count_inter += 1

    return count_inter

#####################################################################################################

def obtem_inter_montanha(t_ter):
    # território -> lista
    """Esta função recebe um território e devolve uma lista de interseções 
    de todas as montanhas do território - Função auxiliar.
    """

    ult_inter = obtem_ultima_intersecao(t_ter)
    inter_total = cria_intersecoes(ult_inter)
    inter_montanha = []

    for inter in inter_total:
        if not eh_intersecao_livre(t_ter, inter):
            inter_montanha += (inter,)

    return inter_montanha

#####################################################################################################

def calcula_tamanho_vales(t_ter):
    # território -> int
    """Esta função recebe um território e devolve um inteiro que representa o 
    número total de interseções diferentes que são vales do território,
    levantando erro se o argumento for inválido.
    """

    if not eh_territorio(t_ter):
        raise ValueError('calcula_tamanho_vales: argumento invalido') 
    
    inter_montanha = obtem_inter_montanha(t_ter)
    vales = ()
    
    for i_montanha in inter_montanha: 
        # Itera sobre todas as montanhas e encontra os vales, sobrepondo-os num tuplo
        for vale in obtem_vale_aux(t_ter, i_montanha):
            if vale not in vales:
                vales += (vale,)

    # O tamanho do tuplo será o número total de vales
    return len(vales) 


#####################################################################################################