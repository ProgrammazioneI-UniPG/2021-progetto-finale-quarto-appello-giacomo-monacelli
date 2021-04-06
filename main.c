#include "gamelib.h"

int main() {
  printf("--------------BENVENUTO A TRANNOI!--------------\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⡄\n");
  printf("⠀⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿\n");
  printf("⠀⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇\n");
  printf("⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿\n");
  printf("⠀⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿ \n");
  printf("⠀⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  do {
      if (!conferma_impostazione) {
        printf("\n1) Imposta gioco\n2) Gioca\n3) Termina gioco\n");
        scanf("%d", &scelta);
        while (getchar()!='\n');
        switch(scelta) {
          case 1:
            Imposta_Gioco();
            break;
          case 2:
            printf ("Non hai ancora impostato il gioco!\n");
            break;
          case 3:
            Termina_Gioco();
            break;
          default:
            printf("Scelta non valida!\n");
            break;
        }
      } else if (conferma_impostazione && !fine_gioco) {
        printf("Hai impostato il gioco ora puoi:\n2) Giocare\n3) Terminare gioco\n");
        scanf("%d", &scelta);
        while (getchar()!='\n');
        switch(scelta) {
          case 2:
            Gioca();
            break;
          case 3:
            Termina_Gioco();
            break;
          default:
            printf("Scelta non valida!\n");
            break;
        }
      } else if (fine_gioco) {
        printf ("Il gioco è concluso, prema 3 per terminare... Alla prossima partita!\n");
        scanf("%d", &scelta);
        while (getchar()!='\n');
        switch (scelta) {
          case 3:
            Termina_Gioco();
            break;
          default:
            printf ("Scelta non valida!\n");
            break;
        }
      }
  } while(scelta !=3);
  return 0;
}
