# Dis, c'est quoi un kernel ?

[comment]: # slide 1
Bonjour et merci de votre intérêt pour les kernels, je trouve que c'est un
sujet dont on ne parle pas assez :wink:

J'imagine que vous n'avez pas tous le même niveau de connaissance sur le
kernel linux, on va essayer de démystifier tout ça.
Je vais certainement faire des raccourcis ou des approximations,
essentiellement pour rendre les choses compréhensibles mais aussi à cause de
mon incompétence, je vous laisserai faire le tri.

[comment]: # L'objectif c'est qu'à la fin de cette présentation vous ayez les
[comment]: # idées plus claires sur ces différents points
précise de:
- Ce que sont BIOS/MBR et UEFI/GPT
- La différence entre Linux et GNU
- Ce que signifie VFS
- C'est quoi ce bazar dans `/proc`
- Pourquoi certains processus restent bloqués à l'état Z
- Comment créer un module linux minimal

[comment]: # slide 3
Maintenant on fait comme le PC, et on démarre 

Firmware dans tous les cas
BIOS MBR/UEFI GPT/Others

[comment]: # Firmware (UEFI ou BIOS) qui détecte le/les bootloaders grâce à GPT ou MBR et
[comment]: # qui leur donne la main.
[comment]: # Limitations MBR:
[comment]: # - Disque de 2To maximum 
[comment]: # - 4 partitions primaires
[comment]: #   - Possibilité d'avoir plus de partitions (extended contenant logicals)
[comment]: # Avec GPT:
[comment]: # - Les limites viennent de l'OS
[comment]: # - CRC appliqués pour détecter/corriger des erreurs


[comment]: # slide 4
Bootloader (Grub, lilo sur PC, Berryboot or uBoot sur raspberry)

[comment]: # slide 5
Alloue la mémoire
Décompresse le kernel en mémoire
Initialise le hardware
Sur x86, active le mode protégé
Monte la partition root (readonly)
Passe la main au userspace `init`

[comment]: # slide 6
Précision sur le delta, Linux =/= projet GNU
Aujourd'hui plus flou, tout le userspace n'est pas GNU

[comment]: # slide 7


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

