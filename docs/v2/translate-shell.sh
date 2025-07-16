#!/bin/bash
# pour utiliser sous ubuntu :
# sudo apt update
# sudo apt install translate-shell
#
# Vérifie que le fichier existe
if [ ! -f "Assembly.md" ]; then
  echo "Le fichier Assembly.md est introuvable."
  exit 1
fi

# Traduction automatique
echo "Traduction de Assembly.md en français..."
trans -b :fr < Assembly.md > Assemblage.md

echo "Fichier traduit : Assemblage.md"

