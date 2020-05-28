# SOPE - Trabalho Prático 2 - Etapa 1 - T6G07

Trabalho feito por: 
- Afonso Maria Rebordão Caiado de Sousa
- Nuno Filipe Amaral Oliveira
- Jorge Carlos Baptista Duarte

No geral, todas as funcionalidades requeridas no enunciado estão devidamente implementadas e comentadas. Deixamos aqui apenas alguns detalhes sobre o nosso projeto.

## Detalhes de implementação: 

Apesar de globalmente termos seguido as instruções e requisitos do enunciado do projecto, optou-se por, no caso específico do uso da função time(), fazer um pequeno desvio do enunciado.

Originalmente, optámos pela criação da função elapsedtime(), que nos é útil ao longo de todo o programa, quer em termos de legibilidade quer de funcionalidade e sem prejuízo da legitimidade do resultado apresentado para “inst”.

Ainda assim, mesmo que o valor retornado usando a função elapsedtime() seja mais legível, e de maneira a seguir o que é pedido no enunciado, escolhemos apresentar o valor retornado por time() na entrega final. As duas funcionam, bastando apenas comentar a linha: 
“timed = time(NULL);” em registers.c
De forma a que o resultado seja apresentado como inicialmente tínhamos feito, com melhor legibilidade, em [segundos.microsegundos].

## Nota sobre warnings:

Apesar de troubleshooting de forma a resolver quaisquer bugs/erros/warnings, não foi possível resolver todos os warnings. De notar que estes warnings não apareceram em todas as máquinas do grupo, o que dificultou ainda mais a tarefa de troubleshooting. Procuraremos no entanto continuar a tentar eliminar estes warnings durante o progresso da fase 2 do projecto.