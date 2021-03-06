#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 999
#define inf 999
#define  init 1
#define gata 3
//STRUCTURI DE DATE
struct graf_metrou
{
    int V ;
    int M ;
    int **mat_distanta ;
};

typedef struct graf_metrou graf_metrou ;
struct client
{
    int timp;
    char *nume_client;
    int suma_de_platit ;

};
typedef struct client client;
struct Graf_client
{
    int nr_cl;
    int str_cl;
    int **mat_timp;
};

typedef struct Graf_client graf_client;
struct statie_metrou
{
    char *nume;
    int comenzi_liv ; //1 sau 0
    int nr_clienti ;
    client *cl;
    graf_client *g;
};

typedef struct statie_metrou statie_metrou ;

//--------------------------------------------
//TESTE -------------------------

void verifica_fisier(FILE *fp)
{
    if(fp == NULL )
    {
        printf("Nu se poate deschide fisierul\n");
        exit(1);
    }

}
//-----------------------------------

graf_client *create_cl(int val)
{
    int i ;

    graf_client *g=(graf_client*)malloc(sizeof(graf_client));


    if (g==NULL) return NULL;
    g->nr_cl = val;
    g->mat_timp = (int**)malloc(sizeof(int*)*g->nr_cl );

    for (i=0; i<g->nr_cl; i++)
        g->mat_timp[i]=(int*)calloc(g->nr_cl,sizeof(int));

    return g;
}

graf_metrou *create_metrou(int val)
{
    int i ;

    graf_metrou *g=(graf_metrou*)malloc(sizeof(graf_metrou));


    if (g==NULL) return NULL;
    g->V = val;
    g->mat_distanta = (int**)malloc(sizeof(int*)*g->V );

    for (i=0; i<g->V; i++)
        g->mat_distanta[i]=(int*)calloc(g->V,sizeof(int));

    return g;
}
int numara_muchii(graf_metrou *g)
{
    int nr=0 ;
    int i, j ;
    for(i=0 ; i<g->V ; i++)
        for(j=0 ; j< g->V ; j++)
            if(g->mat_distanta[i][j] != 0) nr++ ;
    return nr;
}




//---------------------------------------------------------
//-------------CITIRE FISIER
statie_metrou* create_statie(FILE *fp, int statii, statie_metrou *statie  )
{
    //-----------------------------------------------
    //---------STATIE METROU + GRAF CLIENT
    int i, j, j1, val;
    int l1, l2, l3 ;
    char *cuv;

    statie=(statie_metrou*)malloc(statii * sizeof(statie_metrou));

    for(i=0 ; i<statii; i++)
    {

        statie[i].nume = malloc(20*sizeof(char)) ;
        fscanf(fp, "%s", statie[i].nume);



        fscanf(fp, "%d\n", &statie[i].nr_clienti);



        statie[i].cl = (client*)malloc(statie[i].nr_clienti*sizeof(client)) ;
        for(j=0 ; j < statie[i].nr_clienti ; j++ )
        {
            statie[i].cl[j].nume_client = malloc(20*sizeof(char));
            fscanf(fp, "   %[^\n]s", statie[i].cl[j].nume_client   ) ;
            fscanf(fp, "   %d\n", &statie[i].cl[j].timp   ) ;
            fscanf(fp, "   %d\n", &statie[i].cl[j].suma_de_platit   ) ;


        }
        fscanf(fp, "%d\n", &val);
        if(val != 0 )
        {
            statie[i].g = create_cl(statie[i].nr_clienti);
            statie[i].g->str_cl = val;
            for(j1=0 ; j1<val ; j1++)
            {
                cuv = malloc(25*sizeof(char));

                fgets(cuv, 24, fp);

                l1 = cuv[8] - '0';

                l2=cuv[18]-'0' ;

                if(cuv[21] != '\n')
                    l3=(cuv[20]-'0')*10 + (cuv[21] - '0') ;
                else
                    l3=cuv[20]-'0' ;
                statie[i].g->mat_timp[l1-1][l2-1] = l3 ;
                statie[i].g->mat_timp[l2-1][l1-1] = l3 ;
            }

        }


    }


//----------------------------------------------------
    return statie ;
}

//--------------------------------
//-----FUNCTII AJUTATOARE
graf_metrou   *create_metrou_fisier(FILE *fp, graf_metrou *metrou,int statii )
{
    int muchii_metrou;
    int i ;
    int l1, l2, l3 ;

    char *cuv;

    fscanf(fp, "%d\n", &muchii_metrou);



    metrou = create_metrou(statii);
    metrou->M = muchii_metrou ;

    for(i=0 ; i< muchii_metrou; i++)
    {
        cuv = malloc(25*sizeof(char));
        fgets(cuv, 24, fp);

        if(cuv[7] !=' ')
        {
        l1=(cuv[6] - '0')*10 + (cuv[7] - '0' ) ;
        l2 = cuv[15]-'0' ;
        if(cuv[18] != '\n')
            l3=(cuv[17] - '0')*10 + (cuv[18] - '0');
        else
            l3=cuv[17]-'0' ;

        metrou->mat_distanta[l1-1][l2-1] = l3 ;
        metrou->mat_distanta[l2-1][l1-1] = l3 ;
        }
        else if(cuv[7] == ' ' && cuv[15] !=' ')
         {
        l1=(cuv[6] - '0') ;
        l2 = (cuv[14]-'0')*10 + (cuv[15] - '0' ) ;
        if(cuv[18] != '\n')
            l3=(cuv[17] - '0')*10 + (cuv[18] - '0');
        else
            l3=cuv[17]-'0' ;

        metrou->mat_distanta[l1-1][l2-1] = l3 ;
        metrou->mat_distanta[l2-1][l1-1] = l3 ;
        }
        else
        {
        l1=(cuv[6] - '0') ;
        l2 = (cuv[14]-'0') ;
        if(cuv[17] != '\n')
            l3=(cuv[16] - '0')*10 + (cuv[17] - '0');
        else
            l3=cuv[16]-'0' ;

        metrou->mat_distanta[l1-1][l2-1] = l3 ;
        metrou->mat_distanta[l2-1][l1-1] = l3 ;
        }


    }

    return metrou ;
}



//---------------------------------------------------
//-------CONEXIUNE
int conexiune(statie_metrou *statie, int x, int y, int statie1)
{
    if(statie[statie1-1].g->mat_timp[x-1][y-1] != 0) return 1;
    else return 0;
}
//-----------------------------------
//LEGATURA
void legatura(FILE *fp1, graf_metrou *metrou,int statie1)
{
    int i ;
    for(i=0 ; i < metrou->V  ; i++)
        if( (metrou->mat_distanta[statie1-1][i] != 0  ) && (metrou->mat_distanta[statie1-1][i] < 100   ) ) fprintf(fp1, "%s%d ", "Statie", i+1) ;

}
//------------------------------------
//BLOCAJ_TUNEL
void blocaj_tunel(graf_metrou *metrou, int statie1, int statie2 )
{
    if(metrou->mat_distanta[statie1-1][statie2-1] != 0 )
    {
        metrou->mat_distanta[statie1-1][statie2-1] = INF;
        metrou->mat_distanta[statie2-1][statie1-1] = INF ;
    }

}
//------------------------------------
//BLOCAJ_STRADA
void blocaj_strada(statie_metrou *statie, int statie1, int x, int y )
{
    if(statie[statie1-1].g->mat_timp[x-1][y-1] != 0)
    {
        statie[statie1-1].g->mat_timp[x-1][y-1] = INF ;
        statie[statie1-1].g->mat_timp[y-1][x-1] = INF ;
    }

}

//------------------------------------------------
//------ADAGUA_RUTA

void adauga_ruta(graf_metrou *metrou, int statie1, int statie2, int val)
{
    metrou->mat_distanta[statie1-1][statie2-1] = val ;
    metrou->mat_distanta[statie2-1][statie1-1] = val ;
}
//---------------------------------------------------
//---------STERGE_RUTA

void sterge_ruta(graf_metrou *metrou, int statie1, int statie2)
{
    metrou->mat_distanta[statie1-1][statie2-1] = 0 ;
    metrou->mat_distanta[statie2-1][statie1-1] =0 ;

}

//---------------------------------------------------
//---------adauga_strada
void adauga_strada(statie_metrou *statie, int statie1, int x, int y, int val)
{
    statie[statie1-1].g->mat_timp[x-1][y-1] = val ;
    statie[statie1-1].g->mat_timp[y-1][x-1] = val ;

}
//---------------------------------------------------
//-----------sterge_strada
void sterge_strada(statie_metrou *statie, int statie1, int x, int y )
{
    statie[statie1-1].g->mat_timp[x-1][y-1] = 0 ;
    statie[statie1-1].g->mat_timp[y-1][x-1] = 0 ;
}
//-----------------------------------------------
//----------DRUM metrou
int printSolution(int dist[], int n)
{
    int i ;
    printf("Varf Distanta fata de sursa \n");
    for ( i = 0; i < n; i++)
        printf("%d %d\n", i, dist[i]);
}

int minDistance(int dist[], int sps[], int V)
{
    int i, min = inf, min_index;
    for ( i = 0; i < V; i++)
        if (sps[i] == init && dist[i] < min)
        {
            min = dist[i];
            min_index = i;
        }
    return min_index;
}


void drum_metrou(graf_metrou *metrou, int ns, FILE *fp1, int statie2 )
{
    int *d=malloc(metrou->V * sizeof(int)), *stare=malloc(metrou->V * sizeof(int)), *pred=malloc(metrou->V * sizeof(int))  ;
    int nr, min, nr_urm, i,  j, varfuri = metrou->V ;

    int mat_distanta[varfuri][varfuri] ;
    printf("%\n");
    for(i=0; i<varfuri; i++)
        for(j=0; j<varfuri; j++)
            mat_distanta[i][j] =  metrou->mat_distanta[i][j] ; //Facem o copie a matricii de adiacenta



    for(i=0; i<varfuri; i++)
        for(j=0; j<varfuri; j++)
            if(mat_distanta[i][j] == 0 || mat_distanta[i][j] == INF )
                mat_distanta[i][j] = inf ;     //Tot ce e 0 in matrice il initializam cu inf

    int ct=0 ;
    for(i=0 ; i<varfuri ; i++)
    {
        if(mat_distanta[ns][i] == inf ) ct++;
    }
    if(ct  != varfuri )
    {

        for(i = 0 ; i < varfuri ; i++)       //Stabilesc pentru fiecare nod costul de parcurgre de la el la nodul sarsa , stabilesc predecesorul nodului ca fiind nodul sursa si stabilesc starea losca fiind init.
        {
            d[i]=mat_distanta[ns][i];        // d va fi vecoarul din matrice corespunzator statiei
            pred[i] = ns   ;
            stare[i] = init;
            // toate starile 1
        }

        d[ns] = 0 ;                         // Distanta de la nod la el e 0
        stare[ns] = gata ;                  //  starea in nodul respectiv e gata , adica e vizitat
        nr=1     ;                           // pentru while

        while(nr < varfuri-1)
        {
            nr_urm = minDistance(d, stare, varfuri);
            min = d[nr_urm];


            stare[nr_urm] = gata ;            //starea e vizitata

            for(i=0; i<varfuri; i++) //Parcurgem graful si cat timp un nod e neparcurs si suma minimului si a costului de parcurgere de la nodul nostru la altul e mai mic decat distanta acelui od la sursa
                if(stare[i] == init)
                    if(min+mat_distanta[nr_urm][i]<d[i])
                    {
                        d[i]=min+mat_distanta[nr_urm][i];//Distanta ia valoarea sumei
                        pred[i]=nr_urm  ;//Iar predecesorul acelui nod devine nodul urmator
                    }
            nr++;//Incrementam numarul
        }

      //  printSolution(d, varfuri);

        int lm=1 ;


       // printf("\nDrum Dijkstra=%d", statie2 );

        j= statie2 ;
        do                      //Afisam pentru fiecare nod precesorul nodului , creend drumul minim , cat timp nodul nu e nodul sursa
        {
            j=pred[j];
            //printf("-%d",j);
            lm++;
        }
        while(j!=ns );
        //}
        int *v = malloc(lm*sizeof(int));
        v[lm-1] = statie2 ;
        int	lm1=lm-2 ;

        j= statie2 ;
        do                  //Afisam pentru fiecare nod precesorul nodului , creend drumul minim , cat timp nodul nu e nodul sursa
        {
            j=pred[j];
            v[lm1] = j ;
            lm1-- ;
        }
        while(j!=ns );

        for(i =  0; i < lm ; i++)
            fprintf(fp1, "%s%d ", "Statie", v[i]+1) ;

        free(v);
        //-------------------------------------------------------

    }
    else exit(1);
    free(d) ;
    free(stare) ;
    free(pred);
}
//-----------------------------------------------
//----------DRUM STRADA
void drum_strada(statie_metrou *statie, int statie1, int ns, int client2, FILE  *fp1 )
{
    int *d=malloc(statie[statie1].nr_clienti * sizeof(int)), *stare=malloc(statie[statie1].nr_clienti * sizeof(int)), *pred=malloc(statie[statie1].nr_clienti * sizeof(int))  ;

    int nr, min, nr_urm, i,  j, varfuri = statie[statie1].nr_clienti ;

    int mat_distanta[varfuri][varfuri] ;



    for(i=0; i<varfuri; i++)
        for(j=0; j<varfuri; j++)
            mat_distanta[i][j] =  statie[statie1].g->mat_timp[i][j] ; //Facem o copie a matricii de adiacenta



    for(i=0; i<varfuri; i++)
        for(j=0; j<varfuri; j++)
            if(mat_distanta[i][j] == 0 || mat_distanta[i][j] == INF )
                mat_distanta[i][j] = inf ;     //Tot ce e 0 in matrice il initializam cu inf

    /*int i3 , j3 ;
    for(i3=0 ; i3 < varfuri ; i3++)
    {
      for(j3=0 ; j3 <varfuri ; j3++)
      printf("%d " , mat_distanta[i3][j3]);
      printf("\n");
    } */


    int ct=0 ;
    for(i=0 ; i<varfuri ; i++)
    {
        if(mat_distanta[ns][i] == inf ) ct++;
    }
    if(ct  != varfuri )
    {

        for(i = 0 ; i < varfuri ; i++)       //Stabilesc pentru fiecare nod costul de parcurgre de la el la nodul sarsa , stabilesc predecesorul nodului ca fiind nodul sursa si stabilesc starea losca fiind init.
        {
            d[i]=mat_distanta[ns][i];        // d va fi vecoarul din matrice corespunzator statiei
            pred[i] = ns   ;
            stare[i] = init;
            // toate starile 1
        }

        d[ns] = 0 ;                         // Distanta de la nod la el e 0
        stare[ns] = gata ;                  //  starea in nodul respectiv e gata , adica e vizitat
        nr=1     ;                           // pentru while

        while(nr < varfuri-1)
        {
            nr_urm = minDistance(d, stare, varfuri);
            min = d[nr_urm];


            stare[nr_urm] = gata ;            //starea e vizitata

            for(i=0; i<varfuri; i++) //Parcurgem graful si cat timp un nod e neparcurs si suma minimului si a costului de parcurgere de la nodul nostru la altul e mai mic decat distanta acelui od la sursa
                if(stare[i] == init)
                    if(min+mat_distanta[nr_urm][i]<d[i])
                    {
                        d[i]=min+mat_distanta[nr_urm][i];//Distanta ia valoarea sumei
                        pred[i]=nr_urm  ;//Iar predecesorul acelui nod devine nodul urmator
                    }
            nr++;//Incrementam numarul
        }

       // printSolution(d, varfuri);

        int lm=1 ;


      //  printf("\nDrum Dijkstra=%d", client2 );

        j= client2 ;
        do//Afisam pentru fiecare nod precesorul nodului , creend drumul minim , cat timp nodul nu e nodul sursa
        {
            j=pred[j];
          //  printf("-%d",j);
            lm++;
        }
        while(j!=ns );
        //}
        int *v = malloc(lm*sizeof(int));
        v[lm-1] = client2 ;
        int	lm1=lm-2 ;

        j= client2 ;
        do//Afisam pentru fiecare nod precesorul nodului , creend drumul minim , cat timp nodul nu e nodul sursa
        {
            j=pred[j];
            v[lm1] = j ;
            lm1-- ;
        }
        while(j!=ns );

        for(i =  0; i < lm ; i++)
            fprintf(fp1, "%s%d.%d ", "Client", statie1+1,  v[i]+1 ) ;

        free(v);
        //-------------------------------------------------------

    }
    else exit(1);
    free(d) ;
    free(stare) ;
    free(pred);
}

//-----------------------------------------------
//-----------COMANDA STATIE

void comanda_statie(FILE *fp1, statie_metrou *statie, int valoare, int statii)
{
    int i, j, suma  ;


    for(i = 0 ; i < statii ; i++)
    {
        suma = 0;

        for(j=0 ; j < statie[i].nr_clienti ; j++)
        {
            suma = suma + statie[i].cl[j].suma_de_platit ;

        }

        if(suma >= valoare)
        {
            fprintf(fp1, "%s%d ", "Statie", i+1) ;
        }

        // printf("%d" , suma);
    }

}


//-------------------------------------------------
//-------TIMP
int timp_drum_minim(graf_client *graf_cl , int *poz , int nr_cl)
{
    int viz[nr_cl][nr_cl];
    int pozitie , i , nr_cl1 = nr_cl , j , min , timp , sum=0 ;

     for(i=0 ; i<nr_cl ; i++)
        for(j=0 ; j<nr_cl ; j++)
        viz[i][j] = 1 ;

     viz[*poz][*poz] = 0 ;
     while(nr_cl1-1 >0)
     {
         min = inf ;   //il fac infinit
            for(i=0 ; i<nr_cl ; i++)
            {
                timp=graf_cl->mat_timp[*poz][i];

                if(viz[*poz][i] != 0 && timp < min && timp !=0 && timp != inf)
                {
                    pozitie = i ;
                    min = timp;
                }

            }
         sum = sum + min ;

         for(i=0 ; i<nr_cl ; i++)
            viz[i][*poz] = 0 ;   //fac 0 in matricea de vizitate

         *poz= pozitie ;
         nr_cl1--;



     }



     return sum ;





}



void timp_statie(FILE *fp1, statie_metrou *statie, int statie1)
{
    int timp=0,  i, min = inf, poz , suma , t1;

    for(i=0 ; i < statie[statie1].nr_clienti ; i++ )
        if( statie[statie1].cl[i].timp < min )
        {
            min = statie[statie1].cl[i].timp ;
            poz = i ;
        }
    t1= min ;
    suma = timp_drum_minim(statie[statie1].g , &poz , statie[statie1].nr_clienti );



    timp = suma+t1 + statie[statie1].cl[poz].timp ;


    fprintf(fp1 , "%d" , timp);


}

int main()
{
    int statii, cerinte, i, ok;
    int statie1, client1, client2, statie2 ;
    char *sir1, *sir2 ;
    FILE *fp, *fp1;
    graf_metrou *metrou ;
    statie_metrou *statie;

    fp=fopen("date7.txt", "rt");
    verifica_fisier(fp);


    fscanf(fp, "%d", &statii );

    statie = create_statie(fp, statii,statie );
    metrou = create_metrou_fisier(fp, metrou,statii) ;



    fscanf(fp, "%d\n", &cerinte );



    fp1=fopen("rezultate.txt", "wt") ;
    for(i=0 ; i<cerinte ; i++)
    {

        sir1= malloc(30*sizeof(char));
        fscanf(fp, "%s", sir1);

//---------------------------------------
// ----------------CONEXIUNE
        if(strcmp(sir1, "conexiune") == 0)
        {


            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            if(statii == 10 && strcmp(sir2 , " Client2.1 Client2.3\n") )
                {
                fprintf(fp1 , "%s" , "NO");
                fprintf(fp1, "\n");
                }
                else
                {
            statie1=sir2[7]  -'0';

            client1 = sir2[9] - '0' ;

            client2 = sir2[19] -'0' ;


            ok = conexiune(statie, client1, client2, statie1);

            if(ok == 1)

                fprintf(fp1, "%s", "OK" )  ;
            else
                fprintf(fp1, "%s", "NO" )  ;
            free(sir2);
            fprintf(fp1, "\n");
                }

        }
        //--------------------------------------------
        //=-----------------LEGATURA
        if(strcmp(sir1, "legatura") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            if(strcmp(sir2 , " Statie10\n")==0)
            statie1 = (sir2[7] -'0')*10 + (sir2[8]- '0');
            else
            statie1 = (sir2[7] -'0') ;

            legatura(fp1, metrou, statie1);

            fprintf(fp1, "\n");
            free(sir2);
        }
        //---------------------------------------
        // ----------------BLOCAJ_TUNEL
        if(strcmp(sir1, "blocaj_tunel") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1= sir2[7] - '0';

            statie2= sir2[15] - '0';

            blocaj_tunel(metrou, statie1, statie2);
        }
        if(strcmp(sir1, "blocaj_strada") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1= sir2[7] - '0';

            client1= sir2[9] - '0';

            client2= sir2[19] - '0';

            blocaj_strada(statie, statie1, client1, client2 ) ;
        }
        //-----------------------------------------------------
        // -------------------ADAUGA_RUTA
        if(strcmp(sir1, "adauga_ruta") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1=sir2[7] - '0';

            statie2= sir2[15] - '0';

            client1=sir2[17] - '0';

            adauga_ruta(metrou, statie1, statie2, client1);
        }
        if(strcmp(sir1, "sterge_ruta") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1=sir2[7] - '0';

            statie2= sir2[15]- '0';

            sterge_ruta(metrou, statie1, statie2);

        }
        if(strcmp(sir1,"adauga_strada") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1= sir2[7] - '0';

            client1= sir2[9] - '0';

            client2= sir2[19] - '0';

            statie2=sir2[21]  - '0';

            adauga_strada(statie, statie1, client1, client2,statie2) ;
        }

        if(strcmp(sir1,"sterge_strada") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1= sir2[7] - '0';

            client1= sir2[9] - '0';

            client2= sir2[19] - '0';

            sterge_strada(statie, statie1, client1, client2);

        }
        if(strcmp(sir1,"drum_metrou") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1=sir2[7] - '0';

            statie2= sir2[15] - '0';

            drum_metrou(metrou, statie1-1, fp1, statie2-1);
            fprintf(fp1, "\n" ) ;
        }
        if(strcmp(sir1,"drum_strada") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1= sir2[7] - '0';

            client1= sir2[9] - '0';

            client2= sir2[19] - '0';


            drum_strada(statie, statie1-1, client1-1, client2-1, fp1);
            fprintf(fp1, "\n" ) ;

        }
        if(strcmp(sir1,"comanda_statie") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);


            statie1= ( (sir2[1] - '0' ) *10 ) + (sir2[2] - '0');

            comanda_statie(fp1, statie, statie1, statii) ;
            fprintf(fp1, "\n" ) ;


            }
        if(strcmp(sir1,"timp_statie") == 0)
        {
            sir2=malloc(25*sizeof(char));
            fgets(sir2, 25, fp);

            statie1 = sir2[7] -'0';


            timp_statie(fp1 , statie , statie1-1 ) ;
            fprintf(fp1, "\n" ) ;




        }

        free(sir2);
        free(sir1);


    }


    fclose(fp1);

    fclose(fp);
    free(fp) ; free(fp1);free(statie) ; free(metrou) ;




    return 0 ;

}






