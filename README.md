# dilemme
Projet ArchySys

Fichier contenant les détails de notre implémetation, qui sera utilisé pour le compte-rendu

Pour les stratégies nous avons décidé de les coder avec 3 arguments, un double pointeur qui contient les réponses des
deux stratégies qui sont en train de s'affronter; un indice i qui donne le numéro de la confrontation et donc par ce fait
jusqu'où on peut regarder dans le double pointeur pour connaitre les résultats passé et la case à remplir et un autre indice r
qui dit à la stratégie quelle ligne elle représente.


Pour la partie 2, on a modélisé l'écosystème de la manière suivante:
On fait une matrice 11x(n+1)(avec n le nombre de génération) qui comportera la population de chaque stratégies à chaque génération.
On fait un tableau g de 3 dimensions (11x11xn) où la case ij contient un pointeur de taille n qui est les résultats de la stratégie i
contre la stratégie j sur n confrontations.
On va remplir la première matrice en utilisant la formule du polycopié avec une boucle for, a chaque tour de la boucle for on calcul une 
matrice de gain en prenant une tranche du tableau en 3 dimensions ainsi on prend les réponses de la génération i pour faire la matrice de 
gain il n'y a donc qu'une seule confrontation entre chaque stratégie à chaque génération.
Quand on calcul la population on fait un arrondie de tel sorte que on arrondie à l'entier supérieur si la partie décimale est à plus de 
0.5 et à l'entier inférieur sinon.
Losqu'on calcul les populations on calcul que pour les 10 premières startégies et la dernière reçoit la population totale moins les autres 
populations afin de conserver une population totale constante.
L'éxécution du code affiche la matrice des populations de tel sorte qu'une ligne correspond à une stratégie et les colones symbolisent les 
générations, les stratégies sont dans l'ordre du dictionnaire c'est à dire : gentille/méchante/donnant-donnant/méfiante/périodique-
méchante/périodique-gentille/majorité-mou/majorité-dur/sondeur/donnant-donnant-dur/rancuniière.

