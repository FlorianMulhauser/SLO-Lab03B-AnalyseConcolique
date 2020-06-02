# SLO-Lab03B-AnalyseConcolique

@authors: Dupont Maxime, Mülhauser Florian



## 4. Analyser une Fonction simple



#### Question 4.1

> Que fait l'option `--only-output-states-covering-new` ? Pourquoi l'activer ?

L'option `--only-output-states-covering-new` permet de limiter le nombre de test generations (et donc de test case) au états qui couvrent effectivement des nouvelles instructions. C'est une feature qui permet, surtout dans les grand programmes, d'éviter que beaucoup de test cases  empruntent les même chemins, les calculent et les réexecutent chacun. Ça permet donc d'éviter un gachis de temps et de rendre plus efficient le programme.

On peut voir ça dans le tutorial pour Regexp.bc, Klee est passé par 7438 chemins, mais n'avait besoin d'écrire que 16 test cases, car il en écrit un que pour le premier state qui atteint un bug et les autres sont terminés en silence.



#### Question 4.2



>Que contiennent les fichiers en .ptr.err. Comment les ouvrez-vous ?

Lorsque Klee detect une erreur à l'execution du programme, il génere donc des test cases qui affichent ces bugs. Mais il écrit également des information supplémentaires spécifiques sur l'erreur dans le fichier `testN.TYPE.err`

Le cas du `.ptr.err` indique une erreur de mémoire: un accès (lecture ou ecriture) à des endroits invalides en mémoire

On peut afficher ce fichier en faisant un cat avec son addresse (dans le dossier klee-last).  ex: `cat klee-last/test000009.ptr.err`.

>Que contiennent les fichiers en .ktest. Comment les lisez-vous ?

Les test-case générés et leur informations (valeurs des objects, arguments etc) sont écrit dans les fichiers `.ktest`, nous pouvons lire ces fichiers binaires avec l'utilitaire `ktest-tool`, en executsant la commande: `ktest-tool klee-last/test000009.ktest`



#### Manipulation 4.3

Pour cela on installe vim dans notre docker, on copie colle le fichier. Mais pour utiliser KLEE il faut qu'on le lance avec des entrées symboliques, donc on ajoute un main à ce fichier:

```
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <klee/klee.h>


int chg_sign(int x, int y) {
        if (x > y){
                x = x + y;
                y = x - y;
                x = x -y; 
                if (x - y > 0)
                        assert(0);
                return x-y;
        }
        return x-y;
} 

int main() {
  int a;
  int b;
  klee_make_symbolic(&a, sizeof(a), "a");
  klee_make_symbolic(&b, sizeof(b), "b");
  return chg_sign(a, b);
}
```

Pour crée chgSign.bc, on utilise ensuite la commande:

`clang -I ../include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone chgSign.c`

enfin on execute `klee chgSign.bs`



#### Question 4.3

> Que fait la fonction fournie ?

La fonction chg_sign(x, y) var etourner l'opération x-y, en faisant en sort de retourner toujours un résultat négqtif ou nul (il inversera l'opération pour cela):

* Lorsque (x <= y), la fonction retourne x-y, donc un résultat a valeur négative (ou nul)
* Lorsque (x > y), la méthode va donner en plusieurs temps la double affectation (x=y) et (y=x) donc le x aura la valeur initiale de y et inversement.
* * a ce moment la il y a `if(x - y > 0){assert(0);}` Assert(0) sera toujours faux donc une erreur sera lancée si on rentre dans ce if là. Mais ce if ne devrait théoriquement (dans un monde mathématique et non informatique, comme nous allons le voir ensuite) pas arrivé.
* * enfin on retourne x-y, qui est donc la valeur initiale  y-x (l'opération invers pour garantir un résultat negatif ou nul)



#### Question 4.4

> Quel bug est trouvé par KLEE ? Donnez un input permettant d’obtenir ce bug et expliquez ce qui ne va pas dans la fonction.



![](images/4.4.PNG)

Comme affiché sur cette capture en rouge, le bug trouvé par klee va etre celui indiqué précedement:

Si on passe cette condition `if(x - y > 0){assert(0);}`, on atteint le assert(0) qui sera forcément faux et donc nous retournera une ASSERTION FAIL .

Pour voir l'input de cette erreur, et vérifier que c'est bien ça (et si c'est la seule erreur), on va voir dans le dossier klee-test les `testN.xxx.err`. Le seul est bien le `test000002.assert.err`. On utilise donc `ktest-tool test000002.ktest` pour avoir plus d'infos:

![](images/4.4_2.PNG)

On remarque que la façon de passer ce if est de créer un underflow. En effet les\ program a input x =1920 et y = -247482527

on a donc bien x > y on rentre dans le premier if. Sauf que lors de l'échange des valeurs, on fait l'opération

* x = x + y; -> x = -247 480 607‬
* y = x - y; -> y = -247 480 607‬ - (-247482527) = ... et non 1920 comme attendu
* x = x -y;  -> x = .... et non -247482527 comme attendu
* du coup on a bien `if(x - y > 0){assert(0);}` et le assert nous lance l'erreur.



## 5 Labyrinthe

On crée un nouveau folder, avec le fichier maze.c, on y copie colle son code en ajoutant `#include <klee/klee.h>  ` Pour rendre l'input symbolique on remplace le `read(0, program, ITERS);` par `  klee_make_symbolic(program, ITERS, program);`

Afin de pouvoir enregistrer les bonnes solutions, il faut qu'on fasse crasher le programme en cas de victoire, comme ça un rapport et un test sera effectué. Pour cela on ajoute `assert(0);` dans la boucle conditionnelle de la victoire

```
// Source:  http://feliam.wordpress.com/2010/10/07/the-symbolic-maze
/*
It's a maze!
 * Use a,s,d,w to move "through" it.
 */
 
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<klee/klee.h>
 
 
/**
  * Maze hardcoded dimensions
  */
#define H 7
#define W 11
/**
  * Tha maze map
  */
char maze[H][W] = { "+-+---+---+",
                    "| |     |#|",
                    "| | --+ | |",
                    "| |   | | |",
                    "| +-- | | |",
                    "|     |   |",
                    "+-----+---+" };
/**
  * Draw the maze state in the screen!
  */
void draw ()
{
        int i, j;
        for (i = 0; i < H; i++)
          {
                  for (j = 0; j < W; j++)
                                  printf ("%c", maze[i][j]);
                  printf ("\n");
          }
        printf ("\n");
}
 
 
/**
  * The main function
  */
int
main (int argc, char *argv[])
{
        int x, y;     //Player position
        int ox, oy;   //Old player position
        int i = 0;    //Iteration number
    #define ITERS 28
    char program[ITERS];
   
    
 
//Initial position
        x = 1;
        y = 1;
    maze[y][x]='X';
 
//Print some info
    printf ("Maze dimensions: %dx%d\n", W, H);
    printf ("Player pos: %dx%d\n", x, y);
    printf ("Iteration no. %d\n",i);
    printf ("Program the player moves with a sequence of 'w', 's', 'a' and 'd'\n");
    printf ("Try to reach the price(#)!\n");
 
//Draw the maze
    draw ();    
//Read the directions 'program' to execute...
   // read(0,program,ITERS);
    //on transforme en commande symbolique pour KLEE
    klee_make_symbolic(program, ITERS, program);
 
//Iterate and run 'program'
        while(i < ITERS)
          {
          //Save old player position
                  ox = x;
                  oy = y;
          //Move polayer position depending on the actual command
                  switch (program[i])
                    {
                    case 'w':
                            y--;
                            break;
                    case 's':
                            y++;
                            break;
                    case 'a':
                            x--;
                            break;
                    case 'd':
                            x++;
                            break;
                    default:
                        printf("Wrong command!(only w,s,a,d accepted!)\n");
                        printf("You loose!\n");
                        exit(-1);
                    }
 
          //If hit the price, You Win!!            
                  if (maze[y][x] == '#')
                    {
                            printf ("You win!\n");
                            printf ("Your solution <%42s>\n",program);
                            // ici on fait crash le programme en cas de visctoire
                            assert(0);
                            exit (1);
                    }
          //If something is wrong do not advance
                  if (maze[y][x] != ' '
                      &&
                      !(( (y == 5 || ( x == 8 && y == 1) ) && maze[y][x] == '|' && x > 0 && x < W)))
                    {
                            x = ox;
                            y = oy;
                    }
         
          //Print new maze state and info...
          printf ("Player pos: %dx%d\n", x, y);
          printf ("Iteration no. %d. Action: %c. %s\n",i,program[i], ((ox==x && oy==y)?"Blocked!":""));
         
          //If crashed to a wall! Exit, you loose
          if (ox==x && oy==y){
                    printf("You loose\n");
                exit(-2);
          }
          //put the player on the maze...
          maze[y][x]='X';
          //draw it
                  draw ();
          //increment iteration
                  i++;
                  //me wait to human
                  sleep(1);
          }
//You couldn't make it! You loose!       
printf("You loose\n");
}

```

On execute ensuite KLEE, avec l'option `--emit-all-errors`, car cette fois on ne cherche pas juste le bug, mais ce qui importe c'est tout les chemins possibles à ce dernier qu'on a fixé. Donc pour une fois on a besoin de toutes les erreurs pour avoir des rapports de chaques solutions.

`clang -I ../include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone maze.c` pour créer maze.bc

suivi de `klee --emit-all-errors maze.bc `

#### Question 5.1

> Listez tous les chemins gagnants trouvés par KLEE. Expliquez pourquoi chacun de ces chemins est gagnant en vous référant au code.

Suivant notre marche a suivre, nous découvrons qu'il y a 213 test, mais tous ne sont pas des erreurs (et donc des chemins gagnants).

![](images/5.1_1.PNG)



On fait donc un grep: (on note que nos erreurs sont des .external.err et non des assertions.err. C'est du au fait qu'on a oublié d'importer assert, mais le résultat est le meme).

![](images/5.1_2.PNG)

pour voir les solutions nous allons ouvrire les ktest de ces tests en question:

