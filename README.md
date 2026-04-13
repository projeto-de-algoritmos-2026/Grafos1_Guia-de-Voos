# Grafos1_Guia-de-Voos

# Guia de Voos

Número da Lista: 60<br>
Conteúdo da Disciplina: Grafos 1 <br>

## Aluno
|Matrícula | Aluno |
| -- | -- |
| 22/1008211  | Jose Oliveira |

## Sobre 
O projeto tem como objetivo popular um grande grafo esparso com dados reais 
de aeroportos e rotas aéreas, permitindo a análise de conexões, distâncias 
e rotas entre aeroportos com a utilização dos algoritmos vistos em sala de aula:
BFS (menor número de escalas), Dijkstra (menor distância), 
Tarjan (componentes fortemente conectados) e busca de caminho mais longo simples.

## Screenshots
Adicione 3 ou mais screenshots do projeto em funcionamento.

## Instalação 
Linguagem: C<br>
Dependências:
- gcc (compilador C)
- make (ferramenta de automação de compilação)
- bibliotecas padrão do C (stdio.h, stdlib.h, string.h, math.h)
- sistema operacional Linux ou MacOS (para compatibilidade com comandos de terminal e bibliotecas)

## Windows
Instale o WSL2 (`wsl --install` no PowerShell como admin), abra o terminal Ubuntu e siga os passos do Linux.

## Uso
1. Clonar o repositório.
2. Entrar no diretório do backend: `cd backend`.
3. Instalar dependências: `make deps`.
4. Compilar: `make`.
5. Executar: `make run`.
6. Acessar `http://localhost:8080`.

## Outros 
O dataset utilizado é o OpenFlights, contendo dados reais de aeroportos e rotas aéreas de todo o mundo. Após o parsing, o grafo é composto por 6074 aeroportos e 66607 rotas direcionadas. Os pesos das arestas são calculados em tempo de carregamento usando a fórmula de Haversine, que calcula a distância em km entre dois pontos geográficos na superfície da Terra a partir de suas coordenadas de latitude e longitude.
O aeroporto mais isolado da rede a partir de Brasília é YPO (Peawanuck, Canadá), atingível em 8 escalas via BSB → MIA → YYZ → YTS → YMO → YFA → ZKE → YAT → YPO.
