#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>

void quelques_prints (void)
{
    printf ("Juste");
    printf (" histoire");
    printf (" de tester...\n");
    printf ("...que la redirection");
    printf (" marche !\n");
}

void rediriger_vers (void (*f)(void), char *file)
{
    int saved_stdout; // Pour sauvegarder le descripteur de fichier stdout d'origine

    // Sauvegarder le descripteur de fichier stdout d'origine
    saved_stdout = dup(STDOUT_FILENO);

    if (saved_stdout == -1) {
        perror("Erreur lors de la sauvegarde de stdout");
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier de sortie en mode écriture (crée le fichier s'il n'existe pas)
    int fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd_out == -1) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    // Rediriger stdout vers le fichier de sortie
    if (dup2(fd_out, STDOUT_FILENO) == -1) {
        perror("Erreur lors de la redirection de stdout");
        exit(EXIT_FAILURE);
    }

    // Fermer le descripteur de fichier du fichier de sortie (stdout est maintenant redirigé vers le fichier)
    close(fd_out);

    // Appeler la fonction f qui écrira dans stdout (qui est maintenant redirigé vers le fichier)
    f();

    // Rétablir la sortie standard d'origine
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("Erreur lors de la réinitialisation de stdout");
        exit(EXIT_FAILURE);
    }

    // Fermer le descripteur de fichier sauvegardé
    close(saved_stdout);
}

int main(int argc, char *argv[])
{
    printf ("*** DEBUT ***\n");

    rediriger_vers(quelques_prints, "sortie.txt");

    printf ("*** FIN ***\n");

    return EXIT_SUCCESS;
}
