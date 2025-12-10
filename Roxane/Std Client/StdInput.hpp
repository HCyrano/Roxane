/*
 *  StdInput.cpp
 *
 *  Input/output routines for the engine and the IOStd protocol.
 *
 *  Created by Bruno Causse on 10/12/25.
 *
 *  Most of the code below in taken from Robert Hyatt's chess
 *  program "crafty" (thanks). See http://www.craftychess.com/
 *  for the crafty source code.
 *
 *
 */

#ifndef STANDARD_INPUT_H
#define STANDARD_INPUT_H

#include <iostream> // Pour std::cin/std::cout (alternative à stdio.h)
#include <cstdio>   // Pour FILE*
#include <cstdlib>  // Pour malloc/free, si nécessaire
#include <cstring>  // Pour memcpy, strlen
#include <cerrno>   // Pour errno
#include <unistd.h> // Pour read
#include <string>   // La bibliothèque string de C++ est souvent meilleure
#include <vector>   // Pour gérer les arguments de manière dynamique

// On peut utiliser la constante du code C
#define BUFFER_SIZE (1024 * 16)

class StdInput {
public:
    // Constructeur et Destructeur
    StdInput();
    ~StdInput();

    // Fonctions/Méthodes publiques
    void SetReadStream(FILE *stream);
    void ReadClear();
    int ReadInput();

	// Nouvelle fonction C++ (recommandée)
	std::string Read();

    // On pourrait utiliser std::string pour buffer et std::vector<std::string> pour args
    // int ReadParse(char *buffer, char *args[], const char *delims); 
    
    // Une version plus C++ pour ReadParse pourrait être :
    std::vector<std::string> ReadParse(const std::string& input, const std::string& delims);

private:
    // Membres de la classe remplaçant les variables globales
    FILE *input_stream_ = stdin;
    char cmd_buffer_[BUFFER_SIZE];
    char line_buffer_[BUFFER_SIZE];
    char *args_[512]; // Nous conservons le tableau de char* du code C pour l'instant
    
    // État d'initialisation
    bool initialized_ = false;

    // Fonction interne pour la lecture (similaire à SetReadStream)
    void InitializeArguments();
};

#endif  // STANDARD_INPUT_H
