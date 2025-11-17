<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=soft&height=50&color=4a6ea9&text=Jogo%20da%20Memória%20-%20Projeto%20de%20Técnicas%20e%20Desenvolvimento%20de%20Algoritmo&fontSize=20&fontAlign=50&fontAlignY=55&fontColor=FFFFFF">
</p>

<h3 align="center">👥 Contribuidores:</h3>

<div align="center">
    <a href="https://github.com/FillipeBerssot"><img src="https://img.shields.io/badge/Fillipe%20Ribeiro%20-%20black?style=for-the-badge&logo=github"></a>
    <a  href="https://github.com/gabrielrfanthoni"><img src="https://img.shields.io/badge/gabriel%20anthoni%20-%20black?style=for-the-badge&logo=github"></a>
</div>

<br>

<p align="center">Projeto de criação de um jogo em linguagem C, desenvolvido para praticar e aprimorar lógica de algoritmos.</p>

<hr>

<h3 align="center">🃏 O Jogo:</h3>

Este projeto apresenta um Jogo da Memória desenvolvido em linguagem C, oferecendo dois modos de jogo:

- 🎮 **Jogador vs Jogador**
- 🤖 **Jogador vs Computador**

O tabuleiro é formado por **pares de caracteres embaralhados e distribuídos aleatoriamente.**
Cada casa é identificada por linha e coluna de 1 a 6, e a seleção é feita digitando um comando de dois dígitos (ex.: `12` = linha 1, coluna 2).

A cada rodada, o jogador escolhe duas posições:

- Se os caracteres coincidirem, o jogador ganha 1 ponto e pode jogar novamente.
- Caso contrário, os caracteres são ocultados e a vez passa para o próximo jogador (ou para o computador).

O jogo termina quando todos os pares forem encontrados e o tabuleiro estiver completo.
