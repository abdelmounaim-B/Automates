#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ETAT_MAX 20

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                             //
//             ---------- this project was made by -- Abdelmounaim BOUBASTA -- Oumaima FITAH -- Mohammed AARAB ----------------                //
//                                                                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char line1[100] , line2[100] , nom_fichier[100], nom_fichier1[100], nom_fichier3[100],nom_fichier2[100],str[100];
int i, j, choix, Choix,choise;

struct Transition {
    int etat_actuel;
    char symbol;
    int etat_suivante;
};

struct Automate {
    int etat_initial[ETAT_MAX];
    int etat_final[ETAT_MAX];
    int  num_etat_initial, num_etat_final,nbr_lignes;
    struct Transition *transitions;
    
};

int menu_1 (){
		printf("\n voulez vous travailer avec :   ");
	    printf("\n   un automate simple        -> 1");
	    printf("\n   l'etoile d'un automate        -> 2");
	    printf("\n   l'union de deux automates        -> 3");
	    printf("\n   l'intersection de deux automates    -> 4");
	    printf("\n   effectuer des operations sur un automate-> 5");
	    printf("\n   quitter    -> 0");
	    printf("\n votre choix : ");
	    
		scanf("%d",&choix);
		return choix;
}

int menu_2 (){
	
		printf("\n voulez vous :   ");
	    printf("\n   supprimer les epsilons transitions -> 1");
	    printf("\n   determiniser un automate         -> 2");
	    printf("\n   minimiser un automate          -> 3");
	    printf("\n   retour au menu     -> 0");
	    printf("\n votre choix : ");
		scanf("%d",&choise);
		return choise ;
}

int count_numbers_in_string(char* str) {
    int num = 0, i;
    for ( i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            // If the current character is a digit, increment the count
            num++;
            // Skip over any subsequent digits until we reach a non-digit
            while (i < strlen(str) && isdigit(str[i])) {
                i++;
            }
        }
     }
    return num;}
	 
int compter_etats(Automate* Automate) {
    int max_state = -1;
    for (int i = 0; i < Automate->nbr_lignes - 2; i++) {
        if (Automate->transitions[i].etat_actuel > max_state) {
            max_state = Automate->transitions[i].etat_actuel;
        }
        if (Automate->transitions[i].etat_suivante > max_state) {
            max_state = Automate->transitions[i].etat_suivante;
        }
    } return max_state ;

}
	
int est_etat_final(Automate* Automate, int etat) {
    int i;
    for (i = 0; i < Automate->num_etat_final; i++) {
        if (Automate->etat_final[i] == etat) {
            return 1;
        }
    }
    return 0;
}

int est_etat_initial(Automate* Automate, int etat) {
    int i;
    for (i = 0; i < Automate->num_etat_initial; i++) {
        if (Automate->etat_initial[i] == etat) {
            return 1;
        }
    }
    return 0;
}

void lire_fichier_automate(char* nom_fichier, Automate* Automate) {
    FILE* fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);}


	// Count the number of lines in the file
    char line[100];
    Automate->nbr_lignes = 0;
    int i;
    while (fgets(line, sizeof(line), fichier)) {
        Automate->nbr_lignes++;
    }
    rewind(fichier);

    // Calculate the number off initial states
    int  line_number= 1;
    while (fgets(line1, sizeof(line1), fichier)) {
        if (line_number ==(Automate->nbr_lignes - 1) ) {
             Automate->num_etat_initial = count_numbers_in_string(line1);

        }
        line_number++;
    }
    rewind(fichier);
    // Calculate the number off final states
    int  line_number1= 1;
    while (fgets(line2, sizeof(line2), fichier)) {
        if (line_number1 ==(Automate->nbr_lignes ) ) {
             Automate->num_etat_final = count_numbers_in_string(line2);
        }
        line_number1++;
    }
	rewind(fichier);
	
     // Allocate memory for the transition table
    Automate->transitions = (struct Transition*) malloc((Automate->nbr_lignes - 2) * sizeof(struct Transition));

    for (int i = 0; i < Automate->nbr_lignes - 2; i++) {
        fscanf(fichier, "%d %d %c", &Automate->transitions[i].etat_actuel,
                                    &Automate->transitions[i].etat_suivante,
                                    &Automate->transitions[i].symbol);
    }
    for (int i = 0; i < Automate->num_etat_initial; i++) {
        fscanf(fichier, "%d", &Automate->etat_initial[i]);
    }
    for (int i = 0; i < Automate->num_etat_final; i++) {
        fscanf(fichier, "%d", &Automate->etat_final[i]);
    }
    // Close the file
    fclose(fichier);
	}

void generer_fichier_dot(Automate* Automate) {
    FILE* fichier_dot = fopen("graph.dot", "w");
    if (fichier_dot == NULL) {
        printf(" Erreur lors de l'ouverture du fichier .\n");
    }

    fprintf(fichier_dot, "digraph Automate {\n rankdir=LR\n");

    int i, j;
    int nb_etats = compter_etats(Automate);
    
    fprintf(fichier_dot, " node [shape=doublecircle, color=blue]; ");
    for (i = 1; i <= nb_etats; i++) {
        if (est_etat_final(Automate, i)) 
            fprintf(fichier_dot, "%d ", i);}
            
    fprintf(fichier_dot, "\n node [shape=doublecircle, color=green]; ");
	for (i = 0; i < Automate->num_etat_initial; i++) {
        fprintf(fichier_dot, "%d ", Automate->etat_initial[i]);
    }
    fprintf(fichier_dot, "\n node [shape=circle, color=black]; ");        
   // for (i = 1; i <= nb_etats; i++){
	//	if (est_etat_final(Automate, i) == false && est_etat_initial(Automate, i) == false )
            //fprintf(fichier_dot, "%d ", i);}

 	fprintf(fichier_dot,"\n");
    // ecrire les transitions
    for (i = 0; i < Automate->nbr_lignes - 2; i++) {
        fprintf(fichier_dot, "  %d -> %d [label=\"%c\"]\n",
                Automate->transitions[i].etat_actuel,
                Automate->transitions[i].etat_suivante,
                Automate->transitions[i].symbol);
    }

    // ecrire la fin du fichier DOT
    fprintf(fichier_dot, "}\n");
    fclose(fichier_dot);
    system("graph.dot");
}

void affichage_en_console (Automate Automate){

    printf("\n\tTable des transition :\n");
    printf("\n");
    printf("\tdepart-->arrive  :etiquet :\n");
    printf("\n");
    for (i = 0; i <Automate.nbr_lignes - 2  ; i++) {
    printf("\t %d------->%d\t :%c\n", Automate.transitions[i].etat_actuel,  Automate.transitions[i].etat_suivante, Automate.transitions[i].symbol);
    }
    printf("\n\tle nombre des Etats iniales est:[%d] ",Automate.num_etat_initial);
     printf("\n\tle nombre des Etats finalest:[%d] ",Automate.num_etat_final);
    printf("\n\tLes Etats initiaux :  ");
    for (i = 0; i < Automate.num_etat_initial; i++) {
        printf("\t%d ", Automate.etat_initial[i]);
    }
    printf("\n\tLes Etats finaux : ");
    for (i = 0; i < Automate.num_etat_final; i++) {
        printf("\t%d ", Automate.etat_final[i]);
    }
    printf("\n\n");

}

int tester_automate(Automate* Automate, char* mot) {
    int i, j, k;
    int longueur_mot = strlen(mot);
    // On parcourt tous les états initiaux de l'automate
    for (i = 0; i < Automate->num_etat_initial; i++) {
        int etat_actuel = Automate->etat_initial[i];

        // On parcourt chaque caractère du mot
        for (j = 0; j < longueur_mot; j++) {
            // On cherche une transition à partir de l'état actuel avec le symbole courant
            int transition_trouvee = 0;
            for (k = 0; k < Automate->nbr_lignes - 2; k++) {
                if ((Automate->transitions[k].etat_actuel == etat_actuel && Automate->transitions[k].symbol == mot[j] ) || (Automate->transitions[k].etat_actuel == etat_actuel && Automate->transitions[k].symbol == '$' && Automate->transitions[k].symbol == mot[j])/*|| (Automate->transitions[k].etat_actuel == etat_actuel && Automate->transitions[k].symbol == '$')*/) {
                    etat_actuel = Automate->transitions[k].etat_suivante;
                    transition_trouvee = 1;
                    break;
                }
            }
            if (!transition_trouvee) {
                // Si aucune transition n'a été trouvée, le mot n'appartient pas à l'automate à partir de cet état initial
                break;
            }
        }

        // Si l'état actuel est un état final, le mot est reconnu à partir de cet état initial
        if (est_etat_final(Automate, etat_actuel)) {
            return 1;
        }
    }

    // Si aucun état initial ne permet de reconnaître le mot, la fonction retourne 0
    return 0;
}

void tester_liste_mots(char* nom_fichier1, Automate* Automate) {
    FILE* fichier1 = fopen(nom_fichier1, "r");

    if (fichier1 == NULL) {
        printf(" Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char mot[100];
    while (fscanf(fichier1, "%s", mot) != EOF) {
        if (tester_automate(Automate, mot)) {
            printf(" Le mot '%s'  ** appartient **  a l'automate.\n", mot);
        } else {
            printf(" Le mot '%s'  ** n'appartient pas **  a l'automate.\n", mot);
        }
    }

    fclose(fichier1);
}

void tester (char* nom_fichier1, Automate* Automate,char* mot){
	   	do {
		  	
		    printf("\n voulez vous : \n   tester un mot \t     -> 1 \n   tester un fichier des mots -> 2 \n  quitter     ->0 \nvotre choix : ");
		    scanf("%d",&Choix);
	    
	    if (Choix == 2){
	    	
			printf("\n Entrez le nom du fichier contenant les mots  : ");
			scanf("%s", nom_fichier1);		
			tester_liste_mots(nom_fichier1, Automate);
			continue;     }
				
		else if (Choix == 1){
		
		    printf(" Entrez le mot a tester : ");
		    scanf("%s", str);
						 
			if (tester_automate( Automate, mot)) 
			
			    printf(" Le mot %s  ** appartient **  a l'automate.\n", str);			     

				else 
	       			 printf(" Le mot %s  ** n'appartient **  pas a l'automate.\n", str);
	       	 continue;      }
				
		else if (Choix == 0)
		 		printf(" heureux voire  !!\n"); 
		 		
	    else {
			printf(" entrer un choix valide !!\n"); 	
			continue; }		
	   
	}while (Choix !=0);
}

Automate etoile_automate(char* nom_fichier, Automate* Automate) {
        
	FILE* fichier = fopen(nom_fichier, "r");
	if (fichier == NULL) {
	    printf("Erreur lors de l'ouverture du fichier.\n");
	    exit(1);
    }

    // Count the number of lines in the file
    char line[100];
    Automate->nbr_lignes = 0;
    while (fgets(line, sizeof(line), fichier)) {
        Automate->nbr_lignes++;
    }
    rewind(fichier);

//     Calculate the number of initial states
    int line_number = 1;
    char line1[100];
    while (fgets(line1, sizeof(line1), fichier)) {
        if (line_number == Automate->nbr_lignes - 1) {
            Automate->num_etat_initial = count_numbers_in_string(line1);
        }
        line_number++;
    }
    rewind(fichier);

    // Calculate the number of final states
    int line_number1 = 1;
    char line2[100];
    while (fgets(line2, sizeof(line2), fichier)) {
        if (line_number1 == Automate->nbr_lignes) {
            Automate->num_etat_final = count_numbers_in_string(line2);
        }
        line_number1++;
    }
    rewind(fichier);

    
    Automate->transitions = (struct Transition*) malloc((Automate->nbr_lignes - 2 + Automate->num_etat_final ) * sizeof(struct Transition));
    int temp = 0;
    for (int i = 0; i < Automate->nbr_lignes - 2; i++) {
        fscanf(fichier, "%d %d %c", &Automate->transitions[i].etat_actuel,
                                    &Automate->transitions[i].etat_suivante,
                                    &Automate->transitions[i].symbol);
        temp++;
    }

    
    for (int i = 0; i < Automate->num_etat_initial; i++) {
        fscanf(fichier, "%d", &Automate->etat_initial[i]);
    }

    
    for (int i = 0; i < Automate->num_etat_final; i++) {
        fscanf(fichier, "%d", &Automate->etat_final[i]);
    }

    // Add transitions from final states to initial state with '$' symbol
    for (int i = 0; i < Automate->num_etat_final; i++) {
        Automate->transitions[temp].etat_actuel = Automate->etat_final[i];
        Automate->transitions[temp].etat_suivante = Automate->etat_initial[0];
        Automate->transitions[temp].symbol = '$';
        temp++;
    }
    Automate->nbr_lignes = Automate->nbr_lignes + Automate->num_etat_final;
    
    fclose(fichier);    

    
    return *Automate;
}

void union_automates(struct Automate *automate1, struct Automate *automate2, char* filename) {
    int i, j;
    FILE* fichier_union = fopen(filename, "w+");
    if (fichier_union == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", filename);
        exit(1);
    }

    int nombre_etats1 = compter_etats(automate1) + 1;
    int nombre_etats2 = compter_etats(automate2) + 1;

    int etat_initial = 0;
    int etat_final = nombre_etats1 + nombre_etats2 + 1;

    // Ajout des transitions de l'état initial de l'union vers les états initiaux des automates
    fprintf(fichier_union, "  %d %d %c\n", etat_initial, 1, '$');
    fprintf(fichier_union, "  %d %d %c\n", etat_initial, nombre_etats1 + 1, '$');

    // Ajout des transitions de l'automate 1
    for (i = 0; i < automate1->nbr_lignes - 2; i++) {
        fprintf(fichier_union, "  %d %d %c\n",
                automate1->transitions[i].etat_actuel + 1,
                automate1->transitions[i].etat_suivante + 1,
                automate1->transitions[i].symbol);
    }

    // Ajout des transitions de l'automate 2
    for (i = 0; i < automate2->nbr_lignes - 2; i++) {
        fprintf(fichier_union, "  %d %d %c\n",
                automate2->transitions[i].etat_actuel +1+ nombre_etats1,
                automate2->transitions[i].etat_suivante +1+ nombre_etats1,
                automate2->transitions[i].symbol);
    }

    // Ajout des transitions des états finaux des automates vers l'état final de l'union
    for (i = 0; i < automate1->num_etat_final; i++) {
        fprintf(fichier_union, "  %d %d %c\n",
                automate1->etat_final[i] + 1,
                etat_final,
                '$');
    }

    for (i = 0; i < automate2->num_etat_final; i++) {
        fprintf(fichier_union, "  %d %d %c\n",
                automate2->etat_final[i] + 1 + nombre_etats1,
                etat_final,
                '$');
    }

    // Ajout des états initiaux et finaux de l'union
    fprintf(fichier_union, "  %d \n", etat_initial);
    fprintf(fichier_union, "  %d \n", etat_final);
    fclose(fichier_union);
}

int TransitionExist(Automate a, Transition tr) {
	int i;
	for(i = 0; i < a.nbr_lignes ; i++) {
		if(	a.transitions[i].etat_actuel  == tr.etat_actuel  &&
			a.transitions[i].etat_suivante == tr.etat_suivante  &&
			a.transitions[i].symbol == tr.symbol  ){
			return 1;		
		}
	}
	return 0;
}

Automate intersection_automates() {
	system("@cls||clear");

	int num;
	char filename[30];
// Lire deux automates à partir de fichiers
	Automate a1,a2;
				printf(" Entrez le nom du fichier contenant l'automate 1 : ");
			    scanf("%s", nom_fichier1);
			    lire_fichier_automate(nom_fichier1, &a1);
			    printf(" Entrez le nom du fichier contenant l'automate 2 : ");
			    scanf("%s", nom_fichier2);
			    lire_fichier_automate(nom_fichier2, &a2);
// Initialiser l'automate résultant
    Automate resultat;
    
    resultat.num_etat_final = 0;
	resultat.num_etat_initial = 0;
	resultat.nbr_lignes  = 0;
    resultat.transitions = (Transition*) malloc(resultat.nbr_lignes  * sizeof(Transition));
    

	int i,j,k,found;
	
	char symbol[50], nb_symbol = 0;
	int etat1[50], nb_etat_1 = 0;
	int etat2[50], nb_etat_2 = 0;
	int etat3[50], nb_etat_3 = 0;
	// Collecter des informations sur les états et les symboles de l'automate 1

	for (i = 0; i < a1.nbr_lignes - 2; i++) {
    	found = 0;
    	    // Vérifier si l'état actuel de la transition actuelle appartient à l'ensemble d'états de a1

    	for(k = 0; k < nb_etat_1; k++) {
    		if(etat1[k] == a1.transitions[i].etat_actuel ) {// etat actuel
    			found = 1;
			}
		}
		    // Si l'état actuel n'appartient pas à l'ensemble d'états de a1, l'ajouter à l'ensemble d'états et augmenter le nombre d'états de a1

		if(!found) {
			etat1[nb_etat_1] = a1.transitions[i].etat_actuel;
			nb_etat_1++;
		} 

		found = 0;
		    // Vérifier si l'état suivant de la transition actuelle appartient à l'ensemble d'états de a1

    	for(k = 0; k < nb_etat_1; k++) {
    		if(etat1[k]== a1.transitions[i].etat_suivante) {
    			found = 1;
			}
		}
		    // Si l'état suivant n'appartient pas à l'ensemble d'états de a1, l'ajouter à l'ensemble d'états et augmenter le nombre d'états de a1

		if(!found) {
			etat1[nb_etat_1]= a1.transitions[i].etat_suivante;
			nb_etat_1++;
		} 
		
		found = 0;
		 // Vérifier si le symbole de la transition actuelle appartient à l'ensemble de symboles

		for(k = 0; k < nb_symbol; k++) {
    		if(symbol[k]== a1.transitions[i].symbol) {
    			found = 1;
			}
		}
		   
    // Si le symbole n'appartient pas à l'ensemble de symboles, l'ajouter à l'ensemble et augmenter le nombre de symboles

		if(!found) {
			symbol[nb_symbol]= a1.transitions[i].symbol;
			nb_symbol++;
		} 
    }
	// Faire la même chose pour a2

	for (i = 0; i < a2.nbr_lignes - 2; i++) {
    	found = 0;
    	for(k = 0; k < nb_etat_2; k++) {
    		if(etat2[k]== a2.transitions[i].etat_actuel) {
    			found = 1;
			}
		}
		if(!found) {
			etat2[nb_etat_2]= a2.transitions[i].etat_actuel;
			nb_etat_2++;
			found = 0;
		} 
		
		found = 0;
    	for(k = 0; k < nb_etat_2; k++) {
    		if(etat2[k]== a2.transitions[i].etat_suivante) {
    			found = 1;
			}
		}
		if(!found) {
			etat2[nb_etat_2]= a2.transitions[i].etat_suivante;
			nb_etat_2++;
		} 
		
		found = 0;
		for(k = 0; k < nb_symbol; k++) {
    		if(symbol[k]== a2.transitions[i].symbol) {
    			found = 1;
			}
		}
		if(!found) {
			symbol[nb_symbol]= a2.transitions[i].symbol;
			nb_symbol++;
		} 
    }
    printf("\n\n");
    for (i = 0; i < nb_etat_1; i++) {
        for (j = 0; j < nb_etat_2; j++) {
            etat3[nb_etat_3] = etat1[i] * 1000+ etat2[j]; 
        	nb_etat_3++;
        }
    }
    
    int str1_1, str1_2, str2_1, str2_2;
	for(i = 0; i < nb_etat_3; i++) {
        str1_1 = etat3[i] % 1000;
        str1_2 = (etat3[i] - str1_1) / 1000;
		for(j = 0; j < nb_etat_3; j++) {
			str2_1 = etat3[j] % 1000;
            str2_2 = (etat3[j] - str2_1) / 1000;
//            printf("%d %d; %d %d\n",str1_1, str1_2, str2_1, str2_2);
            
			for(k = 0; k < nb_symbol; k++) {
				Transition tr1;
				tr1.etat_actuel= str1_1;
				tr1.etat_suivante= str2_1;
				tr1.symbol= symbol[k];
				
				Transition tr2;
				tr2.etat_actuel= str1_2;
				tr2.etat_suivante= str2_2;
				tr2.symbol= symbol[k];
				
				
				if(TransitionExist(a1, tr1) && TransitionExist(a2, tr2)) {
				    resultat.transitions = (Transition*) realloc(resultat.transitions, (resultat.nbr_lignes  + 1) * sizeof(Transition));
				    resultat.transitions[resultat.nbr_lignes ].etat_actuel= etat3[i];
					resultat.transitions[resultat.nbr_lignes ].etat_suivante= etat3[j];
					resultat.transitions[resultat.nbr_lignes ].symbol=symbol[k];	
					resultat.nbr_lignes++;
				}
			}
		}	
	}
	
	resultat.nbr_lignes+=2;
	
	for(i = 0; i < a1.num_etat_initial; i++) {
		for(j = 0; j < a2.num_etat_initial; j++) {
		//	resultat.etat_initial = (int*) realloc(resultat.etat_initial, (resultat.num_etat_initial + 1) * sizeof(int));
        	resultat.etat_initial[resultat.num_etat_initial]= a1.etat_initial[i] * 1000 + a2.etat_initial[j];
        	resultat.num_etat_initial++;
		}	
	}

    for(i = 0; i < a1.num_etat_final; i++) {
		for(j = 0; j < a2.num_etat_final; j++) {
		//	resultat.etat_final = (int*) realloc(resultat.etat_final, (resultat.num_etat_final + 1) * sizeof(int));
        	resultat.etat_final[resultat.num_etat_final]= a1.etat_final[i] * 1000 + a2.etat_final[j];
        	resultat.num_etat_final++;
		}	
	}
	
//	printf("%d", resultat.num_etat_final);
	
	return resultat;
	
}

void supprimer_epsilon(Automate* automate, Automate* automate_sans_epsilon) {
    // Compter le nombre d'états de l'automate
    int num_etats = compter_etats(automate);

    // Allouer de la mémoire pour la nouvelle table de transition
    struct Transition* new_transitions = (struct Transition*) malloc(num_etats * sizeof(struct Transition));
    int new_nbr_transitions = 0;

    // Parcourir toutes les transitions de l'automate
    for (int i = 0; i < automate->nbr_lignes - 2; i++) {
        // Vérifier si la transition est une transition epsilon
        if (automate->transitions[i].symbol == '$') {
            // Ajouter une transition directe pour chaque paire d'états accessibles par la transition epsilon
            for (int j = 0; j < automate->num_etat_final; j++) {
                if (automate->transitions[i].etat_suivante == automate->etat_final[j]) {
                    for (int k = 0; k < automate->num_etat_initial; k++) {
                        if (automate->transitions[i].etat_actuel == automate->etat_initial[k]) {
                            new_transitions[new_nbr_transitions].etat_actuel = automate->etat_initial[k];
                            new_transitions[new_nbr_transitions].symbol = automate->transitions[i].symbol;
                            new_transitions[new_nbr_transitions].etat_suivante = automate->etat_final[j];
                            new_nbr_transitions++;
                        }
                    }
                }
            }
            // Ajouter une transition directe pour passer de l'état actuel à l'état suivant de la transition epsilon
            new_transitions[new_nbr_transitions].etat_actuel = automate->transitions[i].etat_actuel;
            new_transitions[new_nbr_transitions].symbol = automate->transitions[i+1].symbol; // étiquette de la transition suivante
            new_transitions[new_nbr_transitions].etat_suivante = automate->transitions[i+1].etat_suivante;
            new_nbr_transitions++;
        } else {
            // Ajouter la transition telle quelle à la nouvelle table de transition
            new_transitions[new_nbr_transitions].etat_actuel = automate->transitions[i].etat_actuel;
            new_transitions[new_nbr_transitions].symbol = automate->transitions[i].symbol;
            new_transitions[new_nbr_transitions].etat_suivante = automate->transitions[i].etat_suivante;
            new_nbr_transitions++;
        }
    }

    // Mettre à jour les données de l'automate sans epsilon
    automate_sans_epsilon->transitions = new_transitions;
    automate_sans_epsilon->nbr_lignes = new_nbr_transitions + 2;
    automate_sans_epsilon->num_etat_initial = 1;
    automate_sans_epsilon->etat_initial[0] = 0;
    automate_sans_epsilon->num_etat_final = 1;
    automate_sans_epsilon->etat_final[0] = num_etats;
}

void determinisation(Automate* aut , Automate* aut1){

	    aut1->transitions = (Transition*) malloc((2 * aut->nbr_lignes) * sizeof(Transition));
		char alph[100];
		int nbr_lettres,n;
		int i,j,z,k;
		z=0;
		if(aut->num_etat_initial>1){
		        z=1;
		}
		else{
		
		i=0;
		while (i<aut->nbr_lignes - 2 && z==0){
		 j=i+1;
		   while(j<aut->nbr_lignes - 2 && z==0){
		    if((aut->transitions[j].etat_actuel==aut->transitions[i].etat_actuel) && (aut->transitions[j].symbol==aut->transitions[i].symbol)){
		        z=1;
		        }
		    j++;
		   }
		i++;
		}}
		//determinisation
		if(z==0){
		  printf("\n c'est un automate fini deterministe!! \n"); 
		}
		else{
			
		int l=0,e,s,b,m,ligne,exist,t=1,v,p,q;
		int T[10],tab[10][10];
		//tableau des alphabets sans repetition
		nbr_lettres=0;
		alph[nbr_lettres]=aut->transitions[0].symbol;
		
		for(k=1;k<aut->nbr_lignes - 2;k++){
		     n=0;
		 while(aut->transitions[k].symbol!=alph[n]){
		      n++;}
		
		if(n>nbr_lettres){
		    nbr_lettres++;
		    alph[nbr_lettres]=aut->transitions[k].symbol;
		
		}}
		//initialisation du tabelau tab a -1
		for(k=0;k<10;k++){
		    for(i=0;i<10;i++){
		        tab[k][i]=-1;
		   }}
		// stocker les etats initiaux dans les premiers cases du tableau
		    for(k=0;k<aut->num_etat_initial;k++){
		    tab[l][k]=aut->etat_initial[k]; 
		}
		n=0;
		    aut1->num_etat_initial=0;
		    aut1->num_etat_final=0;
			
		 while(tab[l][0]!=-1 && l<10){
		
		for(i=0;i<=nbr_lettres;i++){
		    for(b=0;b<10;b++){
		    T[b]=-1;}
		m=0;
		s=0;
		int x,h,u,o;
		while(tab[l][m]!=-1){
			for(e=0;e<aut->nbr_lignes - 2;e++){
		            b=0;x=0;
		           
		            if((aut->transitions[e].etat_actuel==tab[l][m])&&(aut->transitions[e].symbol==alph[i])){
		                while(x==0 && b<s){
		                    if(aut->transitions[e].etat_suivante==T[b]){
		                        x=1;
		                    }b++;
		                }
		                if(x==0){
		                T[s]=aut->transitions[e].etat_suivante;
		                s++;}}
		    }
		    m++;
		   }
		ligne=0;
		exist=0;
		v=0;
		while(T[v]!=-1){v++;}
		
		while(ligne<=t && exist==0){
		 b=0;
		 while(tab[ligne][b]!=-1){b++;}
		 o=0;
		 if(b==v){
		    for(h=0;h<v;h++){
		        for(u=0;u<v;u++){
		            if(T[h]==tab[ligne][u]){
		                o++;
		            }
		        }
		    }
		if(o==v){
		    exist=1;
		}
		 }ligne++;
		  }
		
		if(T[0]!=-1){ 
		if(exist==1){
			    aut1->transitions[n].etat_actuel=l;
		        aut1->transitions[n].symbol=alph[i];
		        aut1->transitions[n].etat_suivante=ligne-1;
		        n++;
		}
		else{ 
		        aut1->transitions[n].etat_actuel=l;
		         aut1->transitions[n].symbol=alph[i];
		
		        for(b=0;b<10;b++){
		            tab[t][b]=T[b];
		        }
		        aut1->transitions[n].etat_suivante=t;
		        t++;
		        n++; 
		}}}
		l++;} aut1->nbr_lignes=n+2;
		//l'etats initial de l'automate deterministe
		aut1->num_etat_initial=1;
		aut1->etat_initial[0]=0;
		v=0;
		l=0;
		
		while(l<t){
		b=0;
		q=0;
		while(tab[l][b]!=-1 && q==0){
			i=0;
			
			while(i<aut->num_etat_final && q==0){
			
				if(tab[l][b]==aut->etat_final[i]){
					aut1->etat_final[v]=l;
					v++;
					q=1;
				}i++;
			}
			b++;
		}
		l++;}
		aut1->num_etat_final=v;
		//generer_fichier_dot(aut1, "determin.dot");
		//printf("\n Un fichier .dot est cree sous le nom de determin.dot.\n");
		
		}}
		

Automate* Transpose(Automate* Automate){
    struct Automate* transpose = (struct Automate*) malloc(sizeof(struct Automate));
    transpose->transitions = (struct Transition*) malloc((Automate->nbr_lignes - 2) * sizeof(struct Transition));

    for (int i = 0; i < Automate->nbr_lignes - 2; i++) {
        transpose->transitions[i].etat_actuel = Automate->transitions[i].etat_suivante;
        transpose->transitions[i].etat_suivante = Automate->transitions[i].etat_actuel;
        transpose->transitions[i].symbol = Automate->transitions[i].symbol;
    }

    for (int i = 0; i < Automate->num_etat_initial; i++) {
        transpose->etat_final[i] = Automate->etat_initial[i];
    }

    for (int i = 0; i < Automate->num_etat_final; i++) {
        transpose->etat_initial[i] = Automate->etat_final[i];
    }

    transpose->nbr_lignes = Automate->nbr_lignes;
    transpose->num_etat_initial = Automate->num_etat_final;
    transpose->num_etat_final = Automate->num_etat_initial;

    return transpose;
}

Automate* minimisation(Automate* aut, Automate* aut1) {
    Automate* aut3, *aut4, *aut5;
    aut3 = Transpose(aut);
    determinisation(aut3, aut1);
    aut4 = Transpose(aut1);
    aut5 = new Automate(); // Allocate memory for aut5
    determinisation(aut4, aut5);
    return aut5;
}





int main() {
	Automate* aut3;
	Automate* Automate_min,aut1;

   	do {
	    menu_1();
	    if (choix == 1){
	    	
	    		system("@cls||clear");
			    printf(" Entrez le nom du fichier contenant l'automate : ");
			    scanf("%s", nom_fichier);
			    Automate Automate;	    
			    lire_fichier_automate(nom_fichier, &Automate);
			    affichage_en_console(Automate);
			    generer_fichier_dot(&Automate);
			    tester(nom_fichier, &Automate, str);
				continue;     }
				
		else if (choix == 2){
			
				system("@cls||clear");
			    printf(" Entrez le nom du fichier contenant l'automate : ");
			    scanf("%s", nom_fichier);
			    Automate Automate;
			    
			    etoile_automate(nom_fichier, &Automate);
			    
			    affichage_en_console(Automate);
			    generer_fichier_dot(&Automate);
				tester(nom_fichier, &Automate, str); 
		 		continue;     }
	
		else if (choix == 3){
			
				system("@cls||clear");
 				Automate automate_union, automate1, automate2;
 				printf(" Entrez le nom du fichier contenant l'automate 1 : ");
 			    scanf("%s", nom_fichier1);
 			    lire_fichier_automate(nom_fichier1,&automate1);
 			    printf(" Entrez le nom du fichier contenant l'automate 2 : ");
 			    scanf("%s", nom_fichier2);
 			    lire_fichier_automate(nom_fichier2,&automate2);
 			    
 			    union_automates(&automate1,&automate2,nom_fichier3);
 			    
                lire_fichier_automate(nom_fichier3,&automate_union);
 			    affichage_en_console(automate_union);
 			    generer_fichier_dot(&automate_union);
 				tester(nom_fichier3, &automate_union, str); 
 				continue;  
				  }
 	
	 	else if (choix == 4){
	 		
	 			//// l'intersection ///
	 			
 				system("@cls||clear");
 				
				Automate resultat = intersection_automates();
				
			    generer_fichier_dot(&resultat);
			    tester(nom_fichier, &resultat, str);
					continue; 
				}
				
		else if (choix == 5){
			///////////////////////////////////////
				system("@cls||clear");
					
		        do {
		        	menu_2 ();
					if (choise == 1){
						
						//// supression des epsilons ////
						
						Automate automate_sans_epsilon, automate;
                        printf("Entrez le nom du fichier contenant l'automate : ");
                        scanf("%s", nom_fichier);
                        lire_fichier_automate(nom_fichier, &automate);
                        
                        supprimer_epsilon(&automate, &automate_sans_epsilon);
                        
                        affichage_en_console(automate_sans_epsilon);
                        generer_fichier_dot(&automate_sans_epsilon);
                        tester(nom_fichier3, &automate_sans_epsilon, str);
                        
						continue;
					}
					else if (choise == 2){ 
					
						//// determinisation ////
						
						Automate aut,aut1;
						printf("Entrez le nom du fichier contenant l'automate vous voulez determiniser : ");
               			scanf("%s", nom_fichier);
               			lire_fichier_automate(nom_fichier, &aut);
               			
               			determinisation(&aut,&aut1);
               			
               			generer_fichier_dot(&aut1);
                		tester(nom_fichier3, &aut1, str); 	
						continue;
					}
					else if (choise == 3){

						///// minimisation /////

					    printf("Entrez le nom du fichier contenant l'automate : ");
					    scanf("%s", nom_fichier);
					    struct Automate* Automate = new struct Automate(); 
					    lire_fichier_automate(nom_fichier, Automate);
					    
					    Automate_min = minimisation(Automate, new struct Automate());
					    
					    affichage_en_console(*Automate_min);
					    generer_fichier_dot(Automate_min);
					    tester(nom_fichier3, Automate_min, str);
					    continue;
					}
					else {
						printf(" entrer un choix valide !!\n"); 	
						continue; }	
					
				}while (choise !=0);	}
				////////////////////////////
		else if (choix == 0)
		 	printf(" Au revoire  !!\n");
		 	
	    else {
			printf(" entrer un choix valide !!\n"); 	
			continue; }		
	   
	}while (choix !=0);
	
    return 0;
  }

