#!/bin/env python3

import re
import subprocess
import argparse

def load_dictionary(path):
    terms = {}
    try:
        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith("#") and "=" in line:
                    en, fr = line.split("=", 1)
                    terms[en.strip()] = fr.strip()
    except FileNotFoundError:
        print(f"[⚠️] Fichier dictionnaire non trouvé : {path}")
    return terms

def protect_terms(text, dictionary):
    """
    Remplace les termes définis dans le dictionnaire par des tokens __TERMx__,
    et renvoie le texte transformé + un mapping des tokens vers leur traduction.
    """
    protected = {}
    tokenized = text
    for i, (term, translation) in enumerate(dictionary.items()):
        #print(f"Dict '{term}' -> '{translation}'")
        pattern = re.escape(term)
        token = f"__TERM{i}__"
        # Remplace toutes les occurrences du terme insensible à la casse
        tokenized, count = re.subn(rf'\b{pattern}\b', token, tokenized, flags=re.IGNORECASE)
        if count > 0:
            print(f"Dict {term} -> {token} -> {translation}")
            protected[token] = translation
    return tokenized, protected

def restore_terms(text, protected):
    """
    Remplace les tokens __TERMx__ dans le texte par leur traduction.
    """
    for token, translation in protected.items():
        #text = text.replace(token, translation)
        text = re.sub(re.escape(token), translation, text, flags=re.IGNORECASE)
    return text

def translate_text(text, dictionary):
    if not text.strip():
        return text
    tokenized, protected = protect_terms(text, dictionary)
    try:
        result = subprocess.run(
            ['trans', '-b', ':fr'],
            input=tokenized,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            encoding='utf-8'
        )
        translated = result.stdout.strip()
        return restore_terms(translated, protected)
    except Exception as e:
        print(f"[⚠️] Erreur de traduction : {e}")
        return text

def is_html_line(line):
    html_inline_tags = [
        r'<img[^>]*>',
        r'<br\s*/?>',
        r'<hr\s*/?>',
        r'<.*?style=.*?>'
    ]
    return bool(re.match('|'.join(html_inline_tags), line.strip(), re.IGNORECASE))

def process_line(line, dictionary):
    if not line.strip() or re.match(r'^\s*\|[-| ]+\|\s*$', line):
        return line
    if is_html_line(line) or re.search(r'!\[.*?\]\(.*?\)', line):
        return line
    if re.match(r'^\s*\|.+\|\s*$', line):
        return line
    if line.strip().startswith("```"):
        return line
    if re.match(r'^\s*#+\s', line):
        match = re.match(r'^(\s*#+\s)(.*)', line)
        if match:
            prefix, content = match.groups()
            translated = translate_text(content, dictionary)
            return prefix + translated + "\n"

    # Protection du code inline
    code_matches = list(re.finditer(r'`[^`]+`', line))
    protected = {}
    new_line = line
    for i, match in enumerate(code_matches):
        token = f"__CODE{i}__"
        protected[token] = match.group()
        new_line = new_line.replace(match.group(), token, 1)

    # Liens Markdown
    link_matches = list(re.finditer(r'\[([^\]]+)\]\(([^)]+)\)', new_line))
    for i, match in enumerate(link_matches):
        text, url = match.groups()
        translated_text = translate_text(text, dictionary)
        replaced = f"[{translated_text}]({url})"
        new_line = new_line.replace(match.group(), replaced, 1)

    translated = translate_text(new_line, dictionary)

    for token, original in protected.items():
        translated = translated.replace(token, original)

    return translated + "\n"

def main():
    parser = argparse.ArgumentParser(
        description="""
        Traduction intelligente d'un fichier Markdown en utilisant un dictionnaire de prétraitement.

        Ce script lit un fichier Markdown source, applique une traduction automatique en tenant compte 
        d'un dictionnaire personnalisé de termes techniques, puis génère un nouveau fichier traduit.
        """
    )

    parser.add_argument(
        "--in", dest="input_file",
        default="Assembly.md",
        help="Fichier Markdown source à traduire (par défaut : 'Assembly.md')"
    )

    parser.add_argument(
        "--out", dest="output_file",
        default="Auto-Translate-Assemblage.md",
        help="Fichier Markdown de sortie avec contenu traduit (par défaut : 'Auto-Translate-Assemblage.md')"
    )

    parser.add_argument(
        "--dict",
        default="termes.txt",
        help="Fichier de dictionnaire des termes techniques (par défaut : 'termes.txt')"
    )


    args = parser.parse_args()

    dictionary = load_dictionary(args.dict)

    in_code_block = False
    with open(args.input_file, "r", encoding="utf-8") as fin, open(args.output_file, "w", encoding="utf-8") as fout:
        for line in fin:
            if line.strip().startswith("```"):
                in_code_block = not in_code_block
                fout.write(line)
                continue
            if in_code_block:
                fout.write(line)
            else:
                fout.write(process_line(line, dictionary))

    print(f"✅ Traduction terminée : {args.output_file}")

if __name__ == "__main__":
    main()

