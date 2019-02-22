# Dis, c'est quoi un kernel ?

[comment]: # slide 1
Bonjour et merci de votre intérêt pour les kernels, je trouve que c'est un
sujet dont on ne parle pas assez :wink:

J'imagine que vous n'avez pas tous le même niveau de connaissance sur le
kernel linux, on va essayer de démystifier tout ça.
Je vais certainement faire des raccourcis ou des approximations,
essentiellement pour rendre les choses compréhensibles mais aussi à cause de
mon incompétence, je vous laisserai faire le tri.

[comment]: # slide 2
L'objectif c'est qu'à la fin de cette présentation vous ayez une idée plus
précise de:
...

[comment]: # slide 3
Maintenant on fait comme le PC, et on démarre 



[comment]: # manip - zombi

Vous avez déjà certainement entendu parler de process zombies.
On va voir comment en créer un...

```shell
cat zombi.c
```

Vous avez dû remarquer que dans un programme en C, on a toujours une valeur de
retour (le int main() et le return 0 à la fin).

```shell
./zombi &
pstree $$
ps a
```

Lorsqu'un processus se termine, il attend que son parent lise son code de
retour. Tant que personne n'est venu lire ce code, le processus reste dans
l'état _Zombi_ ou \[defunct\]

[comment]: # manip kernel

