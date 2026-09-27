# Trabalho Prático 1

**DCC023 – Redes de Computadores**  
Gabriel Wedson Mendonça de Souza Veloso  
Matrícula: 2024421649  
Universidade Federal de Minas Gerais (UFMG)  
Belo Horizonte – MG – Brasil  
velosogabriel@ufmg.br

## Arquivos da atividade

Os programas de cada parte estão nesta pasta `scratch`:

- `lab1-part1.cc`: Parte 1, com múltiplos enlaces ponto a ponto entre os clientes e um servidor.
- `lab1-part2.cc`: Parte 2, com uma rede CSMA e um segundo enlace ponto a ponto até outro servidor.
- `lab1-part3.cc`: Parte 3, com duas redes Wi-Fi interligadas por um enlace ponto a ponto.

## Modificações realizadas

Os exemplos do tutorial do ns-3 foram adaptados para estudar diferentes topologias e seus efeitos no RTT.

Na Parte 1, foram adicionados vários clientes, controle do número de clientes e pacotes por linha de comando, sub-redes para cada enlace e envio UDP Echo para um servidor central.

Na Parte 2, a topologia CSMA recebeu um segundo enlace ponto a ponto e um novo servidor. Foram comparados os RTTs da topologia original e da modificada, incluindo o atraso adicional do primeiro pacote associado à resolução de endereços.

Na Parte 3, a rede CSMA foi substituída por uma segunda rede Wi-Fi. Foram configurados pontos de acesso, estações, SSIDs diferentes e mobilidade nas estações. Os resultados foram usados para comparar a variação do RTT no Wi-Fi com a da Parte 2.
