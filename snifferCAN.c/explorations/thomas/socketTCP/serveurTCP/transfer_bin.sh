# Script de copie de l'exécutable à l'utilisateur et à l'adresse IP spécifiés
user="pi"
hostname="10.42.0.42"
folder="/home/pi/"

elf=$1
elf_location="$folder$elf"

# $1 est le chemin de l'exécutable créé dans le Makefile
scp -P 2222 $1  $user@$hostname:$folder
