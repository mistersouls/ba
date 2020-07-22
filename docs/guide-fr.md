# Architecture globale

Notre langage se définit en trois instructions pour le début à savoir:
déclarer une variable, assigner une valeur à une variable et afficher une
valeur. Noter que nous étendrons les fonctionnalités progressivement selon 
notre avancement sur ce projet.

Ci-dessous la spécification de la syntaxe grammaire en lex:

#### `id: [a-zA-Z_][a-zA-Z0-9_]*`
#### `type: 'string' | 'number' | 'boolean'`
#### `declaration: {id} ':' {type} ';'`
#### `number: [0-9]+`
#### `expression: {id} | {number}`
#### `assignment: {id} '=' {expression}`
#### `print: 'print' {expression} ';'`

Ce qui permet d'avoir un exemple de programme valide comme ci-dessous:


```
x: number;
x = 1250;
print x;
print 1;

```

## Lexer

Lorsque le compilateur Ba reçoit une séquence, il est incapable de reconnaître
ne se reste qu'un atome. C'est là qu'intervient le lexer, il nous permet de savoir
dans une séquence, que par exemple que la chaîne number est un type. Dans la version
actuelle, les différents motifs sont:

- *TOKEN_ID*
- *TOKEN_TYPE*
- *TOKEN_COLON*
- *TOKEN_SEMICOLON*
- *TOKEN_ASSIGNMENT*
- *TOKEN_VALUE*
- *TOKEN_PRINT*

Le design pattern utilisé pour implémenter le lexer est la pile (liste chaînée).
Il permet de renvoyer une liste de motifs comme par exemple lorsqu'un programme 
est `x: number;` => `[TOKEN_SEMICOLON, TOKEN_TYPE, TOKEN_COLON, TOKEN_ID]`.
L'avantage d'utiliser la liste chaînée à la place des tableaux est que la taille
d'une liste augmente à chaque fois qu'il est nécessaire autant qu'il y'a de la mémoire
sur l'ordinateur cible.

Les fonctions classiques de la pile sont push, pop et peek. Le push dans notre programme
est *`add_token`* dont l'algorithme:

```
- Créer l'object à stocker
- Créer un nouvel élément de la pile lequel stock le nouvel objet.
- Le précédent du nouvel élément pointe sur la pile.
- La pile pointe sur le nouvel élément créé.

```

Ensuite la fonction pop équivalente à *`pop_token`*:

```
- Récupérer l'objet en haut de la pile c'est à dire l'actuel.
- Détruire l'élément actuel
- Renvoyer l'objet.

```

La fonction peek équivalente à *`peek_token`*:

```
- Renvoyer l'objet actuel du haut de la pile.

```

## Parser

Une fois que nous somme capable d'identifier les différents types de motifs,
il est aisé de construire notre abre de syntaxe abstraite. Il s'agit simplement
d'une représentation de la structure du programme à compiler. En d'autre de savoir
l'ensemble des motifs que représente par exemple une déclaration.
**Exemple**: `x: number;` =>

	    AST
	     |
	Declaration
	    /\
	   /  \
	  /    \
	 Id   Type

Dans cette section, le compilateur **BA** commence à émettre des erreurs lorsque la syntaxe est
grammairement incorrecte. Par exemple lorqu'un point virgule est omis, une erreur est affichée mais
n'empèche que Ba continue d'analyser le programme jusqu'à la fin.
L'approche appliquée pour parser le programme est que chaque grammaire dans la spécification du langage
est attribuée à une structure de donnée considérée comme un noeud. Les noeuds sont ainsi construits
lorsqu'ils sont conforme à la grammaire du langage.
Noter l'utilisation de l'AST pour les autres fonctionnalités à savoir par exemple l'affichage de l'AST, le binder
ou autres peuvent devenir rapidement bordellique. Afin d'eviter ça, nous utilisons le design pattern **visitor**.

Le visitor pattern permet de garder les structures de données simple et évite de rajouter de nouveaux membres à celles-ci 
lorsqu'il faut implémenter une nouvelle fonctionnalité. Chaque nouvelle opération se fait depuis un visitor et le
noeud ne fera qu'accepter celle-ci. Je donnerai plus d"explication dans les prochaines tutoriels.

## Binder


## Compiler


## VM: virtual machine


