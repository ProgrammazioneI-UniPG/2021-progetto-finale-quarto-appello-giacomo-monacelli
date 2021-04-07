#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gamelib.h"

static time_t t;
static struct Giocatore* giocatori = NULL;
static struct Stanza* stanza_inizio = NULL;
static struct Stanza* prossima_stanza  = NULL;
static struct Stanza* lista_stanze  = NULL;
static struct Stanza* prima_stanza_iniziale = NULL;
static unsigned short int quest_da_finire;
static unsigned short int fine_quest;
static bool movimento_impostore;
static int numero_giocatori;
static int numero_impostori;
static int impostori_casuali;
static int tipo_casuale_stanza;
static int giocatore_selezionato;
static int caso;
static int astronauti_assassinati;
static int astronauti_da_uccidere;
static int impostori_scoperti;
static int turno_casuale;
static int numero_stanze;
static int stanza_con_botola;
static int stanza_casuale_botola;
static void creazione_stanza(struct Stanza* stanza_creata, struct Stanza* stanza_precedente); //funzione riga 129
static void stampa_giocatori(); //funzione riga 223
static void stampa_stato(int Stato); //funzione riga 169
static void stampa_nome(int Nome); //funzione riga 187
static int ritorna_tipo_stanza(); //funzione riga 153
static void prima_stanza(); //funzione riga 115
static void tipo_stanza(int Tipo); //funzione riga 140
static void inizio_gioco(); //funzione riga 110
static void giocatori_stanza(); //funzione riga 397
static void pulizia(); //funzione riga 421
static void avanza(); //funzione riga 433
static void esegui_quest(); //funzione riga 498
static void chiamata_emergenza(); //funzione riga 518
static void uccidi_astronauta(); //funzione riga 666
static void usa_botola(); //funzione riga 731
static void sabotaggio(); //funzione riga 794

//Funzione per impostare la partita del gioco
void Imposta_Gioco() {
 srand((unsigned)time(&t));
 printf(" Inserire il numero dei giocatori per questa partita: \n"); //Faccio inserire il numero dei giocatori, verificando che sia un numero idoneo per giocare
 scanf("%d", &numero_giocatori);
 while (getchar()!='\n'); // Svuoto il buffer
 if(numero_giocatori < 4 || numero_giocatori > 10) {
  while(numero_giocatori < 4 || numero_giocatori > 10) {
   printf("Errore giocatori, inserire nuovamente\n");
   scanf("%d", &numero_giocatori);
  }
 }
 printf("Il numero di giocatori è valido\n"); //Imposto il numero di impostori  della partita casualmente in base al numero di giocatori
  if (numero_giocatori<6) {
    numero_impostori=1;
  } else if(numero_giocatori>5 && numero_giocatori<9) {
     impostori_casuali=rand()%5;
     if (impostori_casuali>=0 && impostori_casuali<3) {
       numero_impostori=2;
     } else {
       numero_impostori=1;
    }
   } else if(numero_giocatori>=9) {
     impostori_casuali=rand()%5;
     if (impostori_casuali>=0 && impostori_casuali<3) {
       numero_impostori=3;
     } else {
       numero_impostori=2;
     }
    }
 giocatori=(struct Giocatore *) calloc(numero_giocatori, sizeof(struct Giocatore)); //Array di giocatori
 prima_stanza();
 stampa_giocatori();
 printf("Inserire il numero di quest: \n"); //Faccio inserire il numero di quest verificando che sia un numero idoneo per giocare
 scanf ("%hu", &quest_da_finire);
 if (quest_da_finire < numero_giocatori || quest_da_finire > 20) {
   while (quest_da_finire < numero_giocatori || quest_da_finire > 20) {
     printf ("Il numero di quest inserite non è adatto, inserire di nuovo: \n");
     scanf ("%hu", &quest_da_finire);
   }
 }
 printf ("Il numero di quest inserito è corretto:%hu\n", quest_da_finire);
 printf ("Premere 1 per giocare o 2 per impostare di nuovo il gioco: \n"); //Lascio all'utente la scelta di iniziare a giocare o di impostare da capo la partita
 static int impostazione_gioco_corretta;
 scanf ("%d", &impostazione_gioco_corretta);
 switch (impostazione_gioco_corretta) {
   case 1:
     pulizia();
     inizio_gioco();
     break;
   case 2:
     free (giocatori);
     giocatori=NULL;
     free (stanza_inizio);
     stanza_inizio=NULL;
     pulizia();
     break;
   default:
     printf ("La scelta non è valida!\n");
     break;
 }
}

//Si può iniziare la partita
static void inizio_gioco () {
  conferma_impostazione=true;
}

//Creazione della prima stanza
static void prima_stanza() {
 stanza_inizio=(struct Stanza*)malloc(sizeof(struct Stanza));
 stanza_inizio->Avanti=NULL;
 stanza_inizio->Destra=NULL;
 stanza_inizio->Sinistra=NULL;
 stanza_inizio->StanzaPrecedente=NULL;
 stanza_inizio->Next=NULL;
 stanza_inizio->Tipo=ritorna_tipo_stanza();
 stanza_inizio->chiamata_emergenza=false;
 lista_stanze=stanza_inizio;
 prima_stanza_iniziale=stanza_inizio;
}

//Funzione necessaria per creare una nuova stanza
static void creazione_stanza(struct Stanza* stanza_creata, struct Stanza* stanza_precedente) {
  stanza_creata->Avanti=NULL;
  stanza_creata->Destra=NULL;
  stanza_creata->Sinistra=NULL;
  stanza_creata->Next=NULL;
  stanza_creata->StanzaPrecedente=stanza_precedente;
  stanza_creata->Tipo=ritorna_tipo_stanza();
  stanza_creata->chiamata_emergenza=false;
}

//Stampa il tipo della stanza
static void tipo_stanza(int Tipo) {
 if (Tipo==3) {
   printf("Botola\n");
 } else if(Tipo==2) {
   printf("Quest complicata\n");
 } else if(Tipo==1) {
   printf("Quest semplice\n");
 } else {
   printf("Vuota\n");
 }
}

//Creo il tipo della stanza casualemte
static int ritorna_tipo_stanza() {
 tipo_casuale_stanza=rand()%101;
 if (tipo_casuale_stanza>=0 && tipo_casuale_stanza<=25) {
   tipo_casuale_stanza=3;
 } else if (tipo_casuale_stanza>=26 && tipo_casuale_stanza<=40) {
   tipo_casuale_stanza=2;
 } else if (tipo_casuale_stanza>=41 && tipo_casuale_stanza<=70) {
   tipo_casuale_stanza=1;
 } else if (tipo_casuale_stanza>70) {
   tipo_casuale_stanza=0;
 }
 return tipo_casuale_stanza;
}


//Stampa lo stato del giocatore
static void stampa_stato(int Stato) {
  switch(Stato) {
  case 0:
   printf("Astronauta\n");
   break;
  case 1:
   printf("Impostore\n");
   break;
  case 2:
   printf("Assassinato\n");
   break;
  case 3:
   printf("Defenestrato\n");
   break;
  }
}

//Stampa il nome del giocatore
static void stampa_nome(int Nome) {
  switch(Nome) {
  case 0:
   printf("Rosso\n");
   break;
  case 1:
   printf("Blu\n");
   break;
  case 2:
   printf("Giallo\n");
   break;
  case 3:
   printf("Bianco\n");
   break;
  case 4:
   printf("Nero\n");
   break;
  case 5:
   printf("Rosa\n");
   break;
  case 6:
   printf("Arancione\n");
   break;
  case 7:
   printf("Marrone\n");
   break;
  case 8:
   printf("Ciano\n");
   break;
  case 9:
   printf("Verde\n");
   break;
  }
}

// Stampo i giocatori in ordine casuale
static void stampa_giocatori() {
 for (int i = 0; i < numero_giocatori; i++) {
   giocatori[i].Posizione=stanza_inizio;
   if (numero_impostori!=0) {
     giocatori[i].Stato=rand()%2;
     if (giocatori[i].Stato==1) {
       numero_impostori--;
     }
   } else {
     giocatori[i].Stato=0;
   }
   giocatori[i].Nome=i;
 }
 for (int i = 0; i < numero_giocatori; ++i) {
   giocatore_selezionato=giocatori[i].Nome;
   caso=rand()%numero_giocatori;
   giocatori[i].Nome=giocatori[caso].Nome;
   giocatori[caso].Nome=giocatore_selezionato;
 }
 for (int i = 0; i < numero_giocatori; ++i) {
   printf("\n%d° Giocatore \n", i+1);
   printf("\nTipo di Stanza: ");
   tipo_stanza(giocatori[i].Posizione->Tipo);
   printf("\nStato: ");
   stampa_stato(giocatori[i].Stato);
   printf("\nColore: ");
   stampa_nome(giocatori[i].Nome);
   printf("\n\n");
 }
}

//Funzione per giocare
void Gioca() {
  static int astronauti;
  static int impostori;
  static int conta_turni;
  static int stop_turno;
  static int scelta_astronauta;
  static int scelta_impostore;
  static int scelta_movimento;
  prossima_stanza = stanza_inizio;
  int turno[numero_giocatori];
  for (int i=0;i < numero_giocatori; i++) {
    if (giocatori[i].Stato==0) {
      astronauti++;
    } else if (giocatori[i].Stato==1) {
      impostori++;
    }
  }
  while (quest_da_finire > fine_quest && impostori < astronauti-astronauti_assassinati && impostori > impostori_scoperti) { //condizioni per far finire la partita
    if (conta_turni < numero_giocatori) {
      turno_casuale=rand()%numero_giocatori; //stampo in ordine casuale per ogni giro di turni l'ordine di gioco dei giocatori
      if (turno[turno_casuale]!=1 && turno[turno_casuale]!=2 && turno[turno_casuale]!=3) { //controllo per verificare se il giocatore può giocare o deve attendere 1 o 2 turni
        if (giocatori[turno_casuale].Stato==0) {
          printf("\nSei il giocatore: ");
          stampa_nome(giocatori[turno_casuale].Nome); //stampo il nome del giocatore
          printf("\nTi trovi nella stanza: ");
          prossima_stanza = giocatori[turno_casuale].Posizione;
          tipo_stanza(prossima_stanza->Tipo); //stampo il tipo della stanza dove il giocatore di trova
          giocatori_stanza();
          printf ("\nCosa vuoi fare?\n1)Avanza\n2)Esegui quest\n3)Effettua chiamata d'emergenza\n"); //chiedo al giocatore cosa voglia fare, in base alla risposta ci sarà la rispettiva funzione
          scanf ("%d", &scelta_astronauta);
          switch (scelta_astronauta) {
            case 1:
              printf ("Dove vuoi andare?\n1)Avanti\n2)Destra\n3)Sinistra\n4)Rimani fermo\n");
              scanf ("%d", &scelta_movimento);
              if (scelta_movimento<1 || scelta_movimento>4) { //richiedo l'opzione finchè non ne verrà scelta una valida
                 while (scelta_movimento<1 || scelta_movimento>4) {
                   printf ("Dove vuoi andare?\n1)Avanti\n2)Destra\n3)Sinistra\n4)Rimani fermo\n");
                   scanf ("%d", &scelta_movimento);
                 }
              }
              avanza(scelta_movimento);
              turno[turno_casuale]=1;
              break;
            case 2:
              esegui_quest(turno, turno_casuale);
              break;
            case 3:
              chiamata_emergenza();
              turno[turno_casuale]=1;
              break;
            default:
              printf("Scelta non valida!\n");
              turno[turno_casuale]=1;
              break;
          }
          conta_turni++;
        } else if (giocatori[turno_casuale].Stato==1) {
          printf("\nSei il giocatore: ");
          stampa_nome(giocatori[turno_casuale].Nome); //stampo il nome del giocatore
          printf("\nTi trovi nella stanza: ");
          prossima_stanza = giocatori[turno_casuale].Posizione;
          tipo_stanza(prossima_stanza->Tipo); //stampo il tipo della stanza dove il giocatore di trova
          giocatori_stanza();
          printf ("\nCosa vuoi fare?\n1)Avanza\n2)Uccidi astronauta\n3)Usa la botola\n4)Effettua sabotaggio della stanza\n5)Effettua chiamata d'emergenza\n");  //chiedo al giocatore cosa voglia fare, in base alla risposta ci sarà la rispettiva funzione
          scanf ("%d", &scelta_impostore);
          switch (scelta_impostore) {
            case 1:
              printf ("Dove vuoi andare?\n1)Avanti\n2)Destra\n3)Sinistra\n4)Rimani fermo\n");
              scanf ("%d", &scelta_movimento);
              if (scelta_movimento<1 || scelta_movimento>4) { //richiedo l'opzione finchè non ne verrà scelta una valida
                 while (scelta_movimento<1 || scelta_movimento>4) {
                 printf ("Dove vuoi andare?\n1)Avanti\n2)Destra\n3)Sinistra\n4)Rimani fermo\n");
                 scanf ("%d", &scelta_movimento);
                 }
              }
              avanza(scelta_movimento);
              turno[turno_casuale]=1;
              break;
            case 2:
              uccidi_astronauta();
              turno[turno_casuale]=1;
              break;
            case 3:
              usa_botola();
              turno[turno_casuale]=1;
              break;
            case 4:
              sabotaggio();
              turno[turno_casuale]=2;
              break;
            case 5:
              chiamata_emergenza();
              turno[turno_casuale]=1;
              break;
            default:
              printf("Scelta non valida!\n");
              turno[turno_casuale]=1;
              break;
          }
          conta_turni++;
        } else if (giocatori[turno_casuale].Stato==2) {
            printf("\nSta giocando il giocatore : ");
            stampa_nome(giocatori[turno_casuale].Nome);
            printf ("\nSei stato ucciso!\nDovrai aspettare la fine della partita\n");
            turno[turno_casuale]=1;
            conta_turni++;
        } else if (giocatori[turno_casuale].Stato==3) {
            printf("\nSta giocando il giocatore : ");
            stampa_nome(giocatori[turno_casuale].Nome);
            printf ("\nSei stato defenestrato!\nDovrai aspettare la fine della partita\n");
            turno[turno_casuale]=1;
            conta_turni++;
        }
        if (quest_da_finire-fine_quest<=0) { //sono state completate tutte le quest
          printf("Tutte le quest sono state completate, hanno vinto gli ASTRONAUTI!\n");
          fine_gioco=true;
        }
        pulizia();
      }
    } else {
      stop_turno=0;
      for (int i=0; i<numero_giocatori; i++) {
        turno[i]--; //se il giocatore non ha compiuto azioni che prevedono lo stare fermo 1 o 2 turni allora potrà giocare subito di nuovo
      }
      for (int i=0; i<numero_giocatori; i++) {
        if (turno[i]>=1) {
          stop_turno++;
        }
      }
      conta_turni=0+stop_turno;
      }
    }
  if (impostori==impostori_scoperti) { //sono stati scoperti tutti gli impostori
    printf("Tutti gli impostori sono stati scoperti, hanno vinto gli ASTRONAUTI!\n");
    fine_gioco=true;
  } else if (astronauti-astronauti_assassinati==impostori) { //gli impostori in caso di parità numerica
    printf("Tutti gli astronauti sono stati eliminati, hanno vinto gli IMPOSTORI!\n");
    fine_gioco=true;
  }
}

//Funzione che controlla lo stato e il numero di giocatori presenti in una stanza
static void giocatori_stanza() {
  astronauti_da_uccidere=0; //variabile che tiene conto degli astronauti utile poi per la funzione uccidi_astronauta
  for (int i = 0; i <numero_giocatori; i++) {
    if (i!= turno_casuale) {
      if (giocatori[turno_casuale].Posizione==giocatori[i].Posizione) {
        if (giocatori[i].Stato==2) {
          printf("\nNella stanza c'è un giocatore assassinato."); //i giocatori nella stanza potranno usare quindi la chiamata d'emergenza
        }
        if (giocatori[i].Stato!=3) { //non conto i giocatori defenestrati
          if (giocatori[i].Stato==1) {
            printf("\nIn questa stanza c'è il giocatore: ");
            stampa_nome(giocatori[i].Nome);
          } else if (giocatori[i].Stato==0) {
            printf("\nIn questa stanza c'è il giocatore: ");
            stampa_nome(giocatori[i].Nome);
            astronauti_da_uccidere++;
          }
        }
      }
    }
  }
}

//Funzione per pulire la schermata
static void pulizia(){
  static int reset;
  printf("\nPremere un numero per andare avanti: ");
  scanf("%d", &reset);
  switch(reset){
    default:
      system("clear");
      break;
  }
}

//Funzione che permette ai giocatori di muoversi nella direzione scelta o di rimanere fermi
static void avanza(int movimento) {
    switch (movimento) {
      case 1:
        prossima_stanza=giocatori[turno_casuale].Posizione;
        if (prossima_stanza->Avanti!=NULL) { //verifico se la stanza già esiste
          giocatori[turno_casuale].Posizione=prossima_stanza->Avanti;
          printf ("\nSei arrivato nella stanza: ");
          tipo_stanza(prossima_stanza->Avanti->Tipo);
        } else {
          prossima_stanza->Avanti=(struct Stanza*)malloc(sizeof(struct Stanza)); //creo la stanza siccome nessuno ci è ancora stato
          creazione_stanza(prossima_stanza->Avanti, prossima_stanza);
          printf ("\nSei arrivanto in una nuova stanza: ");
          tipo_stanza(prossima_stanza->Avanti->Tipo);
          giocatori[turno_casuale].Posizione=prossima_stanza->Avanti;
          lista_stanze->Next=giocatori[turno_casuale].Posizione;
          lista_stanze=giocatori[turno_casuale].Posizione;
          }
        giocatori_stanza();
        break;
      case 2:
        prossima_stanza=giocatori[turno_casuale].Posizione;
        if (prossima_stanza->Destra!=NULL) { //verifico se la stanza già esiste
          giocatori[turno_casuale].Posizione=prossima_stanza->Destra;
          printf ("\nSei arrivato nella stanza: ");
          tipo_stanza(prossima_stanza->Destra->Tipo);
        } else {
          prossima_stanza->Destra=(struct Stanza*)malloc(sizeof(struct Stanza)); //creo la stanza siccome nessuno ci è ancora stato
          creazione_stanza(prossima_stanza->Destra, prossima_stanza);
          printf ("\nSei arrivanto in una nuova stanza: ");
          tipo_stanza(prossima_stanza->Destra->Tipo);
          giocatori[turno_casuale].Posizione=prossima_stanza->Destra;
          lista_stanze->Next=giocatori[turno_casuale].Posizione;
          lista_stanze=giocatori[turno_casuale].Posizione;
          }
        giocatori_stanza();
        break;
      case 3:
        prossima_stanza=giocatori[turno_casuale].Posizione;
        if (prossima_stanza->Sinistra!=NULL) { //verifico se la stanza già esiste
          giocatori[turno_casuale].Posizione=prossima_stanza->Sinistra;
          printf ("\nSei arrivato nella stanza: ");
          tipo_stanza(prossima_stanza->Sinistra->Tipo);
        } else {
          prossima_stanza->Sinistra=(struct Stanza*)malloc(sizeof(struct Stanza)); //creo la stanza siccome nessuno ci è ancora stato
          creazione_stanza(prossima_stanza->Sinistra, prossima_stanza);
          printf ("\nSei arrivanto in una nuova stanza: ");
          tipo_stanza(prossima_stanza->Sinistra->Tipo);
          giocatori[turno_casuale].Posizione=prossima_stanza->Sinistra;
          lista_stanze->Next=giocatori[turno_casuale].Posizione;
          lista_stanze=giocatori[turno_casuale].Posizione;
          }
        giocatori_stanza();
        break;
      case 4:
        prossima_stanza=giocatori[turno_casuale].Posizione;
        printf ("\nSei rimasto nella stessa stanza dove ti trovavi\n");
        giocatori_stanza();
        break;
      default:
        printf("\nLa scelta non è valida!\n");
        break;
    }
}

//Funzione che permette all'utente di completare una quest semplice o complicata in base al tipo di stanza che poi diventerà vuota
static void esegui_quest(int turno_giocatore[], int giocatore_che_gioca) {
  if (giocatori[turno_casuale].Posizione->Tipo==1) {
    giocatori[turno_casuale].Posizione->Tipo=0;
    fine_quest++;
    printf("Rimangono %hu quest da completare\n", quest_da_finire-fine_quest);
    printf("La stanza adesso è vuota\n");
    turno_giocatore[giocatore_che_gioca]=2; //con una quest semplice il giocatore starà fermo un turno
  } else if (giocatori[turno_casuale].Posizione->Tipo==2) {
    giocatori[turno_casuale].Posizione->Tipo=0;
    fine_quest+=2;
    printf("Rimangono %hu quest da completare\n", quest_da_finire-fine_quest);
    printf("La stanza adesso è vuota\n");
    turno_giocatore[giocatore_che_gioca]=3; //con una quest complicata il giocatore starà fermo 2 turni
  } else if (giocatori[turno_casuale].Posizione->Tipo==0 || giocatori[turno_casuale].Posizione->Tipo==3) {
    printf ("Non ci sono quest presenti nella stanza in cui ti trovi\n");
    turno_giocatore[giocatore_che_gioca]=1;
  }
}

//Funzione usata sia dagli astronauti che dagli impostori in caso nella stanza ci sia un giocatore assassinato
static void chiamata_emergenza(){
  static int decisione_chiamata;
  static int astronauti_morti;
  static int impostore_defenestrato;
  static int astronauta_defenestrato;
  static int defenestrazione_casuale;
  static int minimo_giocatori_chiamata;
  static bool defenestrazione_effettuata;
  if(giocatori[turno_casuale].Posizione->chiamata_emergenza==false) { //controllo se è già stata effettuata la chiamata d'emergenza
    astronauti_morti=0;
    for(int i=0; i<numero_giocatori; i++) { //verifico la presenza di giocatori assassinati
      if(giocatori[i].Stato==2 && giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
        astronauti_morti++;
      }
    }
    if(astronauti_morti>0) { //se vi sono astronauti assassinati
      printf("Nella stanza vi sono astronauti assassinati, vuoi fare la chiamata d'emergenza?\n1) Sì\n2) No\n");
      scanf("%d", &decisione_chiamata);
      switch(decisione_chiamata){
        case 1:
          impostore_defenestrato=30; //probabilità standard
          astronauta_defenestrato=30; //probabilità standard
          for (int i=0; i<numero_giocatori; i++){
            if (giocatori[turno_casuale].Posizione==giocatori[i].Posizione) {
              if (giocatori[i].Stato==0) { //probabilità per gli astronauti
                for (int j=0; j<numero_giocatori; j++) { //controllo se vi sono altri giocatori presenti
                  if (i!=j) {
                    if( giocatori[j].Posizione==giocatori[turno_casuale].Posizione) {
                      if (giocatori[j].Stato==1) {
                        astronauta_defenestrato+=20;
                      } else if(giocatori[j].Stato==0) {
                        astronauta_defenestrato-=30;
                      }
                    }
                  }
                }
              } else if(giocatori[i].Stato==1) { //probabilità per gli impostori
                for (int j=0; j<numero_giocatori; j++) {
                  if (i!=j) {
                    if (giocatori[j].Posizione==giocatori[turno_casuale].Posizione) {
                      if(giocatori[j].Stato==0) {
                        impostore_defenestrato+=20;
                      } else if (giocatori[j].Stato==1) {
                        impostore_defenestrato-=30;
                      }
                    }
                  }
                }
              }
            }
          }
          minimo_giocatori_chiamata=0; //verifico se oltre all'assassinato vi sono altri giocatri nella stessa stanza
          defenestrazione_effettuata=false;
          for (int i=0; i<numero_giocatori; i++) {
            if (giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
              if (giocatori[i].Stato==0 || giocatori[i].Stato==1) {
                minimo_giocatori_chiamata++;
              }
            }
          }
          if(minimo_giocatori_chiamata>=2) {
            if (impostore_defenestrato>astronauta_defenestrato) { //caso in cui l'impostore ha probabilità maggiori di essere defenestrato
              while (!defenestrazione_effettuata) {
                for (int i=0; i<numero_giocatori; i++) {
                  if(giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
                    if(giocatori[i].Stato==1) {
                      printf("Il giocatore che è stato defenestrato : ");
                      stampa_nome(giocatori[i].Nome);
                      printf("\nEra un impostore\n");
                      impostori_scoperti++;
                      giocatori[i].Stato=3;
                      defenestrazione_effettuata=true;
                    }
                  }
                }
              }
            } else if (astronauta_defenestrato>impostore_defenestrato) { //caso in cui l'astronauta ha probabilità maggiori di essere defenestrato
              while(!defenestrazione_effettuata) {
              for(int i=0; i<numero_giocatori; i++) {
                if(giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
                  if(giocatori[i].Stato==0) {
                    printf("Il giocatore che è stato defenestrato : ");
                    stampa_nome(giocatori[i].Nome);
                    printf("\nNon era un impostore\n");
                    astronauti_assassinati++;
                    giocatori[i].Stato=3;
                    defenestrazione_effettuata=true;
                  }
                }
              }
            }
          } else if(astronauta_defenestrato==impostore_defenestrato) { //caso in cui le probabilità siano uguali
            defenestrazione_casuale=rand()%2; //affido al caso la defenestrazione del giocatore
            switch(defenestrazione_casuale) {
              case 0: //caso in cui esca l'impostore
                while (!defenestrazione_effettuata) {
                  for (int i=0;i<numero_giocatori;i++) {
                    if (giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
                      if (giocatori[i].Stato==1) {
                        printf("Il giocatore che è stato defenestrato è: ");
                        stampa_nome(giocatori[i].Nome);
                        printf("\nEra un impostore!\n");
                        impostori_scoperti++;
                        giocatori[i].Stato=3;
                        defenestrazione_effettuata=true;
                      }
                    }
                  }
                }
                break;
              case 1: //caso in cui esca l'astronauta
                while (!defenestrazione_effettuata) {
                  for (int i=0;i<numero_giocatori;i++) {
                    if (giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
                      if (giocatori[i].Stato==0) {
                        printf("Il giocatore che è stato defenestrato è: ");
                        stampa_nome(giocatori[i].Nome);
                        printf("\nNon era un impostore!\n");
                        astronauti_assassinati++;
                        giocatori[i].Stato=3;
                        defenestrazione_effettuata=true;
                      }
                    }
                  }
                }
                break;
            }
          }
        } else {
          printf ("Non vi è un numero sufficiente di giocatori nella stanza per effettuare una defenestrazione\n");
        }
          giocatori[turno_casuale].Posizione->chiamata_emergenza=true; //non si potrà più effettuare la chiamata d'emergenza in questa stanza
          break;
        case 2:
          printf("Hai deciso di non usare la chiamata d'emergenza\n");
          break;
        default:
          printf("Scelta non valida!\n");
      }
    } else {
      printf("Non vi sono presenti astronauti assassinati in questa stanza\n");
    }
  } else {
    printf("Chiamata d'emergenza già attivata in questa stanza\n");
  }
}

//Funzione che permette all'impostore di uccidere un astronauta nella stessa stanza
static void uccidi_astronauta() {
  static bool assassinio_avvenuto;
  static int conta_giocatori;
  static int vittima_scelta;
  static int astronauta_eliminato;
  static int probabilita;
  static int probabilita_parziale;
  if (astronauti_da_uccidere!=0) {
    assassinio_avvenuto=false;
    conta_giocatori=0;
    while (!assassinio_avvenuto && conta_giocatori<numero_giocatori) {
      if (conta_giocatori!=turno_casuale) {
        if (giocatori[turno_casuale].Posizione==giocatori[conta_giocatori].Posizione && giocatori[conta_giocatori].Stato==0) {
          stampa_nome(giocatori[conta_giocatori].Nome);
          printf("\nSe vuoi uccidere questo giocatore premi 1, altrimenti premi 2 per vedere il prossimo\n");
          scanf("%d", &vittima_scelta);
          switch (vittima_scelta) {
            case 1:
              printf("Hai ucciso il giocatore: ");
              stampa_nome(giocatori[conta_giocatori].Nome);
              astronauta_eliminato=conta_giocatori;
              astronauti_assassinati++;
              giocatori[conta_giocatori].Stato=2;
              assassinio_avvenuto=true;
              break;
            case 2:
              printf("Hai risparmiato (per ora) il giocatore: ");
              stampa_nome(giocatori[conta_giocatori].Nome);
              break;
            default:
              printf("Scelta non valida!\n");
              break;
          }
        }
      }
      conta_giocatori++;
    }
  } else {
    printf ("Non ci sono astronauti da assassinare\n");
  }
  if (assassinio_avvenuto) {
    probabilita_parziale=0;
    for (int i=0; i<numero_giocatori; i++) {
      if (i!=turno_casuale && i!=astronauta_eliminato && giocatori[i].Stato==0) {
        if (giocatori[i].Posizione==giocatori[turno_casuale].Posizione) {
          probabilita_parziale+=50;
        } else if (giocatori[i].Posizione==giocatori[turno_casuale].Posizione->StanzaPrecedente) {
          probabilita_parziale+=20;
        }
      }
    }
  }
  probabilita=rand()%101;
  if (probabilita>0 && probabilita<=probabilita_parziale) {
    stampa_nome(giocatori[turno_casuale].Nome);
    printf(" era un impostore, ora verrà defenestrato\n");
    giocatori[turno_casuale].Stato=3;
    impostori_scoperti++;
  }
  else {
    printf ("Sei salvo, non verrai defenestrato!\n");
  }
}

//Funzione usata dagli impostori per spostarsi in un'altra stanza che abbia una botola
static void usa_botola() {
  numero_stanze=0;
  stanza_con_botola=0;
  movimento_impostore=false;
  stanza_inizio=prima_stanza_iniziale;
  if (giocatori[turno_casuale].Posizione->Tipo==3) { //verifico se il giocatore si trova in una stanza con una botola
    printf("Hai deciso di usare la botola nella stanza\n");
    while(stanza_inizio!=NULL) { //scorro tutte le stanze che ci sono al momento
      if(stanza_inizio->Tipo==3 && stanza_inizio!=giocatori[turno_casuale].Posizione) {
        stanza_con_botola++; //aumento il numero delle stanze con botola
      }
      numero_stanze++; //aumento il numero di stanze prive di botola
      stanza_inizio=stanza_inizio->Next; //scorro le stanze
    }
    stanza_inizio=prima_stanza_iniziale; //faccio tornare il puntatore all'inzio grazie al puntatore d'appoggio
    printf("Ci sono %d stanze totali\n", numero_stanze);
    int conta_botole[numero_stanze]; //creo un array di appoggio per le stanze e le stanze con botola
    for (int i=0; i<numero_stanze; i++) {
      if (stanza_inizio->Tipo==3 && stanza_inizio!=giocatori[turno_casuale].Posizione) {
        conta_botole[i]=1;
      } else {
        conta_botole[i]=0;
      }
      stanza_inizio=stanza_inizio->Next;
    }
    stanza_inizio=prima_stanza_iniziale;
    if(stanza_con_botola>0) {
      printf("%d hanno una botola, ora verrai spostato in un'altra stanza anch'essa con una botola\n", stanza_con_botola);
      stanza_casuale_botola=rand()%numero_stanze; //stanza a caso per il giocatore
      while(!movimento_impostore) {
        if (conta_botole[stanza_casuale_botola]==0) {
          stanza_casuale_botola=rand()%numero_stanze;
        } else {
          while (stanza_casuale_botola!=0) {
            stanza_inizio=stanza_inizio->Next;
            stanza_casuale_botola--;
          }
          movimento_impostore=true;
        }
      }
      giocatori[turno_casuale].Posizione=stanza_inizio; //aggiorno la posizione del giocatore
      printf("Sei stato portato nella stanza: ");
      tipo_stanza(giocatori[turno_casuale].Posizione->Tipo);
      giocatori_stanza();
    }
    else { //mando il giocatore in una stanza qualsiasi
      printf("Non vi sono ancora stanze con una botola, verrai spostato in una stanza casuale qualsiasi\n");
      stanza_casuale_botola=rand()%numero_stanze;
      while(stanza_casuale_botola!=0) {
        stanza_inizio=stanza_inizio->Next;
        stanza_casuale_botola--;
      }
      giocatori[turno_casuale].Posizione=stanza_inizio;
      printf("Sei stato portato nella stanza: ");
      tipo_stanza(giocatori[turno_casuale].Posizione->Tipo);
      giocatori_stanza();
    }
  } else {
  printf("Nella stanza non vi è nessuna botola\n");
  }
}

//Funzione sabotaggio dell'impostore per trasformare la stanza contente quest in una stanza vuota
static void sabotaggio() {
  if (giocatori[turno_casuale].Posizione->Tipo==1) {
    giocatori[turno_casuale].Posizione->Tipo=0; //la stanza diventa vuota
    printf("La stanza adesso è vuota\n");
  } else if (giocatori[turno_casuale].Posizione->Tipo==2) {
    giocatori[turno_casuale].Posizione->Tipo=0; //la stanza diventa vuota
    printf("La stanza adesso è vuota\n");
  } else {
    printf("Nella stanza non sono presenti quest da sabotare\n");
  }
}

//Funzione per terminare il gioco, dealloco la memoria usata
 void Termina_Gioco() {
   if (giocatori!=NULL) {
     free(giocatori);
     giocatori=NULL;
   } else if (stanza_inizio!=NULL) {
     free(stanza_inizio);
     stanza_inizio=NULL;
   } else if (prossima_stanza!=NULL) {
    free(prossima_stanza);
     prossima_stanza=NULL;
   }
 }
