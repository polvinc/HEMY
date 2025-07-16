#!/usr/bin/python3

import re
import subprocess

INPUT_FILE = "Assembly.md"
OUTPUT_FILE = "Auto-Translate-Assemblage.md"

def translate_text(text):
    """Appelle translate-shell pour traduire une chaîne en français"""
    if not text.strip():
        return text
    try:
        result = subprocess.run(
            ['trans', '-b', ':fr'],
            input=text,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            encoding='utf-8'
        )
        return result.stdout.strip()
    except Exception as e:
        print(f"[⚠️] Erreur de traduction : {e}")
        return text

def process_line(line):
    # Ne pas traduire les lignes vides ou les séparateurs de tableau
    if not line.strip() or re.match(r'^\s*\|[-| ]+\|\s*$', line):
        return line

    # Ne pas traduire les images ![alt](url)
    if re.search(r'!\[.*?\]\(.*?\)', line):
        return line

    # Ne pas traduire les lignes de tableaux (| col1 | col2 |) → fragile
    if re.match(r'^\s*\|.+\|\s*$', line):
        return line

    # Ne pas traduire les blocs de code
    if line.strip().startswith("```"):
        return line

    # Gérer les titres : conserver les #
    if re.match(r'^\s*#+\s', line):
        match = re.match(r'^(\s*#+\s)(.*)', line)
        if match:
            prefix, content = match.groups()
            translated = translate_text(content)
            return prefix + translated + "\n"

    # Protéger les `code inline`
    code_matches = list(re.finditer(r'`[^`]+`', line))
    protected = {}
    new_line = line
    for i, match in enumerate(code_matches):
        token = f"__CODE{i}__"
        protected[token] = match.group()
        new_line = new_line.replace(match.group(), token, 1)

    # Protéger les liens [texte](url) → on traduit le texte, on garde le lien
    link_matches = list(re.finditer(r'\[([^\]]+)\]\(([^)]+)\)', new_line))
    for i, match in enumerate(link_matches):
        text, url = match.groups()
        translated_text = translate_text(text)
        replaced = f"[{translated_text}]({url})"
        new_line = new_line.replace(match.group(), replaced, 1)

    # Traduire le reste
    translated = translate_text(new_line)

    # Réintégrer les blocs protégés
    for token, original in protected.items():
        translated = translated.replace(token, original)

    return translated + "\n"

def main():
    in_code_block = False
    with open(INPUT_FILE, "r", encoding="utf-8") as fin, open(OUTPUT_FILE, "w", encoding="utf-8") as fout:
        for line in fin:
            if line.strip().startswith("```"):
                in_code_block = not in_code_block
                fout.write(line)
                continue
            if in_code_block:
                fout.write(line)
            else:
                fout.write(process_line(line))

    print(f"✅ Traduction terminée avec succès : {OUTPUT_FILE}")

if __name__ == "__main__":
    main()

