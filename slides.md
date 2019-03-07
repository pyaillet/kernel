---
patat:
    theme:
        code: [dullBlack,onRgb#F0F0F0]
        codeBlock: [dullBlack,onRgb#F0F0F0]
...

```
 _____________________________
< Dis, c'est quoi un kernel ? >
 -----------------------------
   \
    \
        .--.
       |o_o |
       |:_/ |
      //   \ \
     (|     | )
    /'\_   _/`\
    \___)=(___/
```

[comment]: # Bonjour et merci de votre intérêt pour les kernels, je trouve que c'est un
[comment]: # sujet dont on ne parle pas assez :wink:
[comment]: # J'imagine que vous n'avez pas tous le même niveau de connaissance sur le
[comment]: # kernel linux, on va essayer de démystifier tout ça.
[comment]: # Je vais certainement faire des raccourcis ou des approximations,
[comment]: # essentiellement pour rendre les choses compréhensibles mais aussi à cause de
[comment]: # mon incompétence, je vous laisserai faire le tri.


# Points abordés

- Comment démarre une machine
- C'est quoi init
- C'est quoi la différence entre GNU et Linux
- C'est quoi ce bazar dans /proc
- C'est quoi le VFS
- C'est quoi un appel système
- Comment est-ce qu'on compile un kernel
- C'est quoi un module kernel
- Comment est-ce qu'on en crée un

[comment]: # L'objectif c'est qu'à la fin de cette présentation vous ayez les
[comment]: # idées plus claires sur ces différents points


# Boot

```
+----------+  +---------+
|   BIOS   +->+   MBR   |
+----------+  +---------+

+----------+  +---------+
|   UEFI   +->+   GPT   |
+----------+  +---------+

+----------+
|  Other.. |
+----------+
```

[comment]: # Firmware \(UEFI ou BIOS) qui détecte le/les bootloaders grâce à GPT ou MBR et
[comment]: # qui leur donne la main.
[comment]: # Limitations MBR:
[comment]: # - Disque de 2To maximum 
[comment]: # - 4 partitions primaires
[comment]: #   - Possibilité d'avoir plus de partitions (extended contenant logicals)
[comment]: # Avec GPT:
[comment]: # - Les limites viennent de l'OS
[comment]: # - CRC appliqués pour détecter/corriger des erreurs


# Bootloader

```
+----------+  +---------+
|   BIOS   +->+   MBR   +-------------v
+----------+  +---------+    +-----------------+
                             |                 |
+----------+  +---------+    |    Bootloader   |
|   UEFI   +->+   GPT   +--->+   Grub, lilo,   |
+----------+  +---------+    |       ...       |
                             |                 |
+----------+                 +-----------------+
|  Other.. +--------------------------^
+----------+
```


# Kernel

```
+----------+  +---------+
|   BIOS   +->+   MBR   +-------------v
+----------+  +---------+    +-----------------+
                             |                 |   +--------------+
+----------+  +---------+    |    Bootloader   |   |    Kernel    |
|   UEFI   +->+   GPT   +--->+   Grub, lilo,   +-->+              |
+----------+  +---------+    |       ...       |   |     Linux    |
                             |                 |   +--------------+
+----------+                 +-----------------+
|  Other.. +--------------------------^
+----------+
```

[comment]: # Différents steps
[comment]: # Alloue la mémoire
[comment]: # Décompresse le kernel en mémoire
[comment]: # Initialise le hardware
[comment]: # Sur x86, active le mode protégé



# Kernel

```
+----------+  +---------+
|   BIOS   +->+   MBR   +-------------v
+----------+  +---------+    +-----------------+
                             |                 |   +--------------+
+----------+  +---------+    |    Bootloader   |   |    Kernel    |
|   UEFI   +->+   GPT   +--->+   Grub, lilo,   +-->+              |
+----------+  +---------+    |       ...       |   |     Linux    |
                             |                 |   +--------------+
+----------+                 +-----------------+
|  Other.. +--------------------------^
+----------+
```

```shell
Uncompressing linux... Ok, booting the kernel.
audit(1117047658.214:0) : Initialized
Kernel panic - not syncing : VFS: Unable to mount root fs on unknown-block(0,0)
```

[comment]: # Monte la partition root (readonly)
[comment]: # Passe la main au userspace `init`


# Init

```
+----------+  +---------+
|   BIOS   +->+   MBR   +-------------v
+----------+  +---------+    +-----------------+
                             |                 |   +--------------+   +----------+
+----------+  +---------+    |    Bootloader   |   |    Kernel    |   |          |
|   UEFI   +->+   GPT   +--->+   Grub, lilo,   +-->+              +-->+   Init   |
+----------+  +---------+    |       ...       |   |     Linux    |   |          |
                             |                 |   +--------------+   +----------+
+----------+                 +-----------------+
|  Other.. +--------------------------^
+----------+
```

[comment]: # Précision sur le delta, Linux =/= projet GNU
[comment]: # Aujourd'hui plus flou, tout le userspace n'est pas GNU
[comment]: # Systemd, init actuellement utilisé



# pstree

    systemd-+-accounts-daemon-+-{gdbus}
            |                 `-{gmain}
            |-acpid
            |-2*[agetty]
            |-atd
            |-cron
            |-dbus-daemon
            |-dhclient
            [...]

[comment]: # Responsabilités de init
[comment]: # - Initialiser le système d'un point de vue utilisateur (monter les partitions...)
[comment]: # - Lancer les services de l'OS (sshd, Xorg, ...)
[comment]: # - Adopter les processus orphelins (parler de Docker) (À voir)



# /proc

```
/proc
├── 1
│   ├── /cwd -> /
│   ├── /fd
│   ├── /ns
│   ├── /root -> /
│   ├── cmdline
│   ├── environ
│   ├── exe -> /lib/systemd/systemd
│   ├── limits
│   ├── loginuid
│   ├── mem
│   └── mounts
├── cmdline
├── cpuinfo
├── mounts
├── uptime
└── version

142 directories, 48 files
```

[comment]: # read !tree /proc -L 2 --dirsfirst
[comment]: # Une des responsabilité du kernel en revanche, c'est de gérer l'état des processus
[comment]: # Cet état est exposé dans /proc/<pid>
[comment]: # Commenter l'arbre...



# VFS

```
sysfs on /sys type sysfs
proc on /proc type proc
udev on /dev type devtmpfs
devpts on /dev/pts type
tmpfs on /run type tmpfs
/dev/sda1 on / type ext4
vagrant on /vagrant type vboxsf
```

[comment]: # On voit donc que contrairement à d'autres OS le FS ne correspond pas à la simple
[comment]: # projection du contenu des disques/partitions
[comment]: # On passe par un VFS
[comment]: # Tout est fichier



# File access

```
+-----------------------+
| Read file /etc/passwd |
+-----------+-----------+
            v
+-----------------------+
|  File system driver   |
+-----------+-----------+
            v
+-----------+-----------+
|  Block device driver  |
+-----------+-----------+
            v
+-----------+-----------+
|       Hardware        |
+-----------------------+
```

[comment]: # Si on veut accéder à un fichier sur un disque par exemple



# libc / syscall

```c
int open(const char *pathname, int flags);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

[comment]: # On voit donc bien que lorsqu'on va vouloir accéder à un fichier il va falloir
[comment]: # utiliser le kernel

# libc / syscall

```c
int open(const char *pathname, int flags);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

| Syscall Number | Syscall name |
|---|--------------|
| 0 | read |
| 1 | write |
| 2 | open |
| 3 | close |

[comment]: # On voit donc bien que lorsqu'on va vouloir accéder à un fichier il va falloir
[comment]: # utiliser le kernel

# libc / syscall

```c
int open(const char *pathname, int flags);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

| Syscall Number | Syscall name |
|---|--------------|
| 0 | read |
| 1 | write |
| 2 | open |
| 3 | close |

```asm
    .text
ENTRY (syscall)
    movq %rdi, %rax        /* Syscall number -> rax.  */
    movq %rsi, %rdi        /* shift arg1 - arg5.  */
        [...]
    syscall                /* Do the system call.  */
```

[comment]: # Environ 400 syscalls... Pour le réseau, les droits, tout ce qui
[comment]: # tourne autour des conteneurs



# /dev

```
/dev
├── console
├── /input          <- clavier, souris, ...
├── kmsg            <- messages kernel, idem dmesg
├── mem             <- la mémoire
├── /net            <- certains périphériques réseaux
├── null            <- le fameux /dev/null
├── random          <- source de données aléatoire
├── sda
├── sda1            <- les disques
├── tty             <- les terminaux
└── zero            <- des zéros, plein de zéros...
[...]
```

[comment]: # Il y a aussi d'autres façon d'interagir avec le noyau, notamment
[comment]: # avec les devices
[comment]: # Pour créer ces devices devices on développera un module du noyau



# modules? Monolithic != Modular

```
$ lsmod
Module                  Size  Used by
vboxsf                 49152  1
vboxvideo              49152  1
input_leds             16384  0
vboxguest             286720  3 vboxsf,vboxvideo
psmouse               131072  0
e1000                 135168  0
pata_acpi              16384  0
video                  40960  0
```

```
$ ls /lib/modules/$(uname -r)/kernel/ubuntu/vbox/vboxguest/
vboxguest.ko
```

[comment]: # Pour résumer, les modules sont des drivers qu'on va pouvoir
[comment]: # charger/décharger dynamiquement



# Démo !

```
 __________________________
< C'est l'heure de la démo >
 --------------------------
   \
    \
        .--.
       |o_o |
       |:_/ |
      //   \ \
     (|     | )
    /'\_   _/`\
    \___)=(___/
```



# Thank you !

   ▄▄▄▄▄▄▄  ▄  ▄▄  ▄▄▄▄  ▄▄▄▄▄▄▄  
   █ ▄▄▄ █ ▄▄█▀█▄█▀█▀ ▀█ █ ▄▄▄ █  
   █ ███ █  ▀█▀ ▀█▀ ▄██  █ ███ █  
   █▄▄▄▄▄█ ▄▀▄ ▄▀▄ ▄▀█▀█ █▄▄▄▄▄█  
   ▄▄▄ ▄▄▄▄█▀█▄▀▀█▄██ █ ▄▄   ▄    
   █▄    ▄ ▄▄▀▀ ▄▀▀▄   █▄█ ▄▀▄▄█  
   ██▄  █▄▄▀  ▀▄  ███ ▀ ▄▄▀▀▄ █▄  
   ▄▄▀▄▀ ▄▀ ▄ ██▀▄ ▀▀  ▀▀█ ▄█ ▄█  
   ▀▄ ▀█▀▄▀▀█▄▄▀▀█▀██▀▄█▀█▄ █ █▄  
   ▄▀▄ ▀▀▄▀▀█▀▀ ▄▀ █▀▀  █▀▄▄▀▀▄█  
   ▄▀  ██▄▄██▄▀▄  ▄██▄██████▀ ▀   
   ▄▄▄▄▄▄▄ █ ▄██▀▄ █▄ ██ ▄ █▄▀██  
   █ ▄▄▄ █ ██ ▄▀▀█  █▀ █▄▄▄█▀ ▀█  
   █ ███ █ ▄ █▀ ▄▀ ▀▄▄▄▀  ██▄▀▀█  
   █▄▄▄▄▄█ █▀▀▀▄  ▀██▀███ ▄█  █▄  

   https://github.com/pyaillet/kernel
