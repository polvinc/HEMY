#!/bin/bash

INPUT_FILE="$1"
OUTPUT_FILE="$2"

# Crée un répertoire temporaire
TMP_DIR=/tmp
echo "Répertoire temporaire : $TMP_DIR"

# Crée l'environnement virtuel Python
python3 -m venv "$TMP_DIR/venv"
. $TMP_DIR/venv/bin/activate

# Installe les dépendances dans le venv
pip install --quiet --upgrade pip
pip install  deep-translator

. $TMP_DIR/venv/bin/activate

./smart_translate_md.py Assembly.md  Auto-Translate-Assemblage.md 

