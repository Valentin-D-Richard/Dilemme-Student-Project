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

Voici les valeurs:

100	70	76	82	92	98	103	107	110	108	109	108	107	107	106	105	105	105	105	105	105	105	105	105	105	105	105	105	105	105	105	
100	136	132	114	94	66	41	27	16	8	4	2	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	
100	70	110	111	168	172	218	240	272	273	298	302	307	314	317	318	321	322	323	324	325	326	326	326	326	326	326	326	326	326	326	
100	136	98	107	72	80	34	40	14	15	5	6	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	
100	136	132	41	51	36	5	5	3	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	
100	70	76	144	79	63	105	47	34	50	19	13	19	7	5	7	2	1	1	0	0	0	0	0	0	0	0	0	0	0	0	
100	70	110	111	156	159	202	209	236	237	251	255	259	262	265	266	268	269	270	271	272	273	273	273	273	273	273	273	273	273	273	
100	136	98	110	59	68	26	30	9	10	3	3	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	
100	136	49	34	50	43	43	41	37	31	28	23	19	16	13	11	9	7	6	5	4	3	2	2	2	2	2	2	2	2	2	
100	70	110	124	140	164	166	182	190	188	195	197	195	198	199	198	199	199	199	199	199	199	199	199	199	199	199	199	199	199	199	
100	70	109	122	139	151	157	172	179	180	188	191	191	194	195	195	196	197	196	196	195	194	195	195	195	195	195	195	195	195	195	

