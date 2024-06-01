#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int fd_errlog; // Descripteur de fichier pour le fichier de journal des erreurs

    // Ouvrir le fichier de destination en mode écriture
    fd_errlog = open("ERREURS-LIRE.log", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd_errlog == -1) {
        perror("Erreur lors de l'ouverture du fichier de journal des erreurs");
        return 1;
    }

    // Utiliser dup2() pour rediriger stderr vers le fichier de destination
    if (dup2(fd_errlog, STDERR_FILENO) == -1) {
        perror("Erreur lors de la redirection de stderr");
        close(fd_errlog);
        return 1;
    }

    // Maintenant, tout ce qui serait écrit sur stderr sera redirigé vers le fichier ERREURS-LIRE.log

    // Exemple d'utilisation de stderr pour générer une erreur
    fprintf(stderr, "Ceci est un message d'erreur dans le fichier de journal des erreurs.\n");

    // Fermer le fichier de destination (optionnel, en fonction de la durée de vie du processus)
    close(fd_errlog);

    return 0;
}
