#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct to {
    char ** from;
    char * to;
    int c_from;
} t_to;

typedef struct rel {
    char *name;

    t_to *to;
    int c_to;

    char ** out;
    int c_out;
    int count;
} t_relazione;

int binarySearch(char **, int, char *);
int insertString(char **, int, char *, int);

int binaryRel(t_relazione *, int, char *);
int binaryTo(t_to *, int, char *);

int main() {
    //Redirect used during debugging
    //freopen("output.txt","w",stdout);
    //freopen("input.txt","r",stdin);
    //clock_t start = clock();

    //vars
    char name1[100], name2[100], namerel[100];

    char cmd[7];
    int i, c, j, k, q, r;
    bool go;

    int c_entita;
    int c_relazioni;
    char **entita = NULL;
    t_relazione *relazioni = NULL;

    t_relazione tmpREL;
    t_to tmpTO;
    char * tmpSTR;

    c_entita = 0;
    c_relazioni = 0;
    entita = (char **) malloc(c_entita * sizeof(char *));
    relazioni = (t_relazione *) malloc(c_relazioni * sizeof(relazioni));

    do {
        i = 0;
        c = getchar();
        while ((c != ' ' && c != '\n') && c != EOF && i<6) {
            cmd[i++] = (char) c;
            c = getchar();
        }
        cmd[i] = '\0';
        if(c == ' '){
            c = getchar();
            i = 0;
            while ((c != ' ' && c != '\n') && c != EOF) {
                if(c != '"')
                    name1[i++] = (char) c;
                c = getchar();
            }
            name1[i] = '\0';
            if(c == ' '){
                c = getchar();
                i = 0;
                while ((c != ' ' && c != '\n') && c != EOF) {
                    if(c != '"')
                        name2[i++] = (char) c;
                    c = getchar();
                }
                name2[i] = '\0';
                if(c == ' '){
                    c = getchar();
                    i = 0;
                    while ((c != ' ' && c != '\n') && c != EOF) {
                        if(c != '"')
                            namerel[i++] = (char) c;
                        c = getchar();
                    }
                    namerel[i] = '\0';
                }
            }
        }

        if (strcmp(cmd, "addent") == 0) {
            //ADDENT
            i = binarySearch(entita, c_entita, name1);
            if(i == c_entita && i>0){
                entita = (char **)realloc(entita,(c_entita+1)*sizeof(char *));
                entita[c_entita] = (char *)malloc(strlen(name1)* sizeof(char)+1);
                c_entita = insertString(entita, c_entita, name1, i);
            }
            else if(i==0){
                if(c_entita > 0){
                    if(strcmp(entita[i],name1) != 0){
                        entita = (char **)realloc(entita,(c_entita+1)*sizeof(char *));
                        entita[c_entita] = (char *)malloc(strlen(name1)* sizeof(char)+1);
                        c_entita = insertString(entita, c_entita, name1, 0);
                    }
                }
                else{
                    entita = (char **)realloc(entita,(c_entita+1)*sizeof(char *));
                    entita[c_entita] = (char *)malloc(strlen(name1)* sizeof(char)+1);
                    c_entita = insertString(entita, c_entita, name1, 0);
                }

            }
            else{
                if(strcmp(entita[i],name1) != 0){
                    entita = (char **)realloc(entita,(c_entita+1)*sizeof(char *));
                    entita[c_entita] = (char *)malloc(strlen(name1)* sizeof(char)+1);
                    c_entita = insertString(entita, c_entita, name1, i);
                }
            }
        }
        else if(strcmp(cmd, "delent") == 0){
            i = binarySearch(entita,c_entita, name1);
            if(i != c_entita){
                if(strcmp(entita[i],name1) == 0){
                    free(entita[i]);
                    for(j=i;j<c_entita-1;j++)
                        entita[j] = entita[j+1];
                    c_entita--;
                    entita = realloc(entita,c_entita* sizeof(char *));

                    for(i=0; i<c_relazioni; i++){
                        go = false;
                        k = binaryTo(relazioni[i].to,relazioni[i].c_to,name1);
                        if(k != relazioni[i].c_to){
                            if(strcmp(relazioni[i].to[k].to,name1) == 0){
                                //Remove TO 
                                for(r=0;r<relazioni[i].to[k].c_from;r++)
                                    free(relazioni[i].to[k].from[r]);
                                free(relazioni[i].to[k].to);
                                for(j=k;j<relazioni[i].c_to-1;j++)
                                    relazioni[i].to[j] = relazioni[i].to[j+1];
                                relazioni[i].c_to--;
                                relazioni[i].to = realloc(relazioni[i].to,relazioni[i].c_to*sizeof(t_to));
                                go = true;
                            }
                        }

                        for(k = 0; k<relazioni[i].c_to; k++){
                            q = binarySearch(relazioni[i].to[k].from,relazioni[i].to[k].c_from,name1);

                            //c_from-- done yet
                            if(q != relazioni[i].to[k].c_from){
                                if(strcmp(relazioni[i].to[k].from[q],name1) == 0){
                                    //Remove FROM
                                    free(relazioni[i].to[k].from[q]);
                                    for(j=q;j<relazioni[i].to[k].c_from-1;j++)
                                        relazioni[i].to[k].from[j] = relazioni[i].to[k].from[j+1];
                                    relazioni[i].to[k].c_from--;
                                    relazioni[i].to[k].from = realloc(relazioni[i].to[k].from,relazioni[i].to[k].c_from*sizeof(char *));
                                    go = true;

                                    if(relazioni[i].to[k].c_from == 0){
                                        //Remove TO
                                        free(relazioni[i].to[k].to);
                                        free(relazioni[i].to[k].from);
                                        for(j=k;j<relazioni[i].c_to-1;j++)
                                            relazioni[i].to[j] = relazioni[i].to[j+1];
                                        relazioni[i].c_to--;
                                        relazioni[i].to = realloc(relazioni[i].to,relazioni[i].c_to*sizeof(t_to));
                                        k--;
                                    }
                                }
                            }
                        }
                        if(relazioni[i].c_to == 0){
                            //Remove REL
                            for(j=0;j<relazioni[i].c_out;j++)
                                free(relazioni[i].out[j]);
                            free(relazioni[i].name);
                            for(j=i;j<c_relazioni-1;j++)
                                relazioni[j] = relazioni[j+1];
                            c_relazioni--;
                            relazioni = realloc(relazioni,c_relazioni*sizeof(t_relazione));
                            i--;
                            go = false;
                        }

                        //Recalc. max
                        if(go || relazioni[i].c_out == 0){
                            relazioni[i].count = 0;
                            for(j=0;j<relazioni[i].c_to;j++){
                                if(relazioni[i].to[j].c_from > relazioni[i].count){
                                    relazioni[i].count = relazioni[i].to[j].c_from;
                                    for(r = 0; r<relazioni[i].c_out; r++)
                                        free(relazioni[i].out[r]);
                                    relazioni[i].c_out = 1;
                                    relazioni[i].out = realloc(relazioni[i].out,relazioni[i].c_out* sizeof(char *));
                                    relazioni[i].out[0] = malloc(strlen(relazioni[i].to[j].to)* sizeof(char)+1);
                                    strcpy(relazioni[i].out[0],relazioni[i].to[j].to);
                                }
                                else if(relazioni[i].to[j].c_from == relazioni[i].count){
                                    r = binarySearch(relazioni[i].out,relazioni[i].c_out,relazioni[i].to[j].to);
                                    if(r != relazioni[i].c_out){
                                        if(strcmp(relazioni[i].out[r],name2) != 0){
                                            relazioni[i].out = realloc(relazioni[i].out,(relazioni[i].c_out+1)* sizeof(char *));
                                            relazioni[i].out[relazioni[i].c_out] = malloc(strlen(relazioni[i].to[j].to)* sizeof(char)+1);
                                            relazioni[i].c_out = insertString(relazioni[i].out, relazioni[i].c_out, relazioni[i].to[j].to, r);
                                        }
                                    }
                                    else{
                                        relazioni[i].out = realloc(relazioni[i].out,(relazioni[i].c_out+1)* sizeof(char *));
                                        relazioni[i].out[relazioni[i].c_out] = malloc(strlen(relazioni[i].to[j].to)* sizeof(char)+1);
                                        relazioni[i].c_out = insertString(relazioni[i].out, relazioni[i].c_out, relazioni[i].to[j].to, r);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(strcmp(cmd, "addrel") == 0){
            //ADDREL
            go = false;
            i = binarySearch(entita, c_entita, name1);
            if(i != c_entita) {
                if (strcmp(entita[i], name1) == 0) {
                    i = binarySearch(entita, c_entita, name2);
                    if(i != c_entita) {
                        if (strcmp(entita[i], name2) == 0) {
                            i = binaryRel(relazioni, c_relazioni, namerel);
                            if (i == c_relazioni) {
                                relazioni = (t_relazione *) realloc(relazioni, (c_relazioni + 1) * sizeof(t_relazione));
                                relazioni[c_relazioni].name = malloc(strlen(namerel) * sizeof(char) + 1);
                                strcpy(relazioni[c_relazioni].name, namerel);
                                relazioni[c_relazioni].count = 0;
                                relazioni[c_relazioni].c_out = 0;
                                relazioni[c_relazioni].out = malloc(relazioni[c_relazioni].c_out * sizeof(char *));
                                relazioni[c_relazioni].c_to = 0;
                                relazioni[c_relazioni].to = malloc(relazioni[c_relazioni].c_to * sizeof(t_to));

                                tmpREL = relazioni[c_relazioni];
                                for (j = c_relazioni; j > i; j--)
                                    relazioni[j] = relazioni[j - 1];
                                relazioni[i] = tmpREL;
                                c_relazioni++;
                                go = true;
                            } else {
                                if (strcmp(relazioni[i].name, namerel) != 0) {
                                    relazioni = (t_relazione *) realloc(relazioni,
                                                                        (c_relazioni + 1) * sizeof(t_relazione));
                                    relazioni[c_relazioni].name = malloc(strlen(namerel) * sizeof(char) + 1);
                                    strcpy(relazioni[c_relazioni].name, namerel);
                                    relazioni[c_relazioni].count = 0;
                                    relazioni[c_relazioni].c_out = 0;
                                    relazioni[c_relazioni].out = malloc(relazioni[c_relazioni].c_out * sizeof(char *));
                                    relazioni[c_relazioni].c_to = 0;
                                    relazioni[c_relazioni].to = malloc(relazioni[c_relazioni].c_to * sizeof(t_to));

                                    tmpREL = relazioni[c_relazioni];
                                    for (j = c_relazioni; j > i; j--)
                                        relazioni[j] = relazioni[j - 1];
                                    relazioni[i] = tmpREL;
                                    c_relazioni++;
                                    go = true;
                                }
                            }

                            k = binaryTo(relazioni[i].to, relazioni[i].c_to, name2);
                            if (k == relazioni[i].c_to) {
                                relazioni[i].to = (t_to *) realloc(relazioni[i].to,
                                                                   (relazioni[i].c_to + 1) * sizeof(t_to));
                                relazioni[i].to[relazioni[i].c_to].to = malloc(strlen(name2) * sizeof(char) + 1);
                                strcpy(relazioni[i].to[relazioni[i].c_to].to, name2);
                                relazioni[i].to[relazioni[i].c_to].c_from = 0;
                                relazioni[i].to[relazioni[i].c_to].from = malloc(
                                        relazioni[i].to[relazioni[i].c_to].c_from * sizeof(char **));

                                tmpTO = relazioni[i].to[relazioni[i].c_to];
                                for (j = relazioni[i].c_to; j > k; j--)
                                    relazioni[i].to[j] = relazioni[i].to[j - 1];
                                relazioni[i].to[k] = tmpTO;
                                relazioni[i].c_to++;
                                go = true;
                            } else {
                                if (strcmp(relazioni[i].to[k].to, name2) != 0) {
                                    relazioni[i].to = (t_to *) realloc(relazioni[i].to,
                                                                       (relazioni[i].c_to + 1) * sizeof(t_to));
                                    relazioni[i].to[relazioni[i].c_to].to = malloc(strlen(name2) * sizeof(char) + 1);
                                    strcpy(relazioni[i].to[relazioni[i].c_to].to, name2);
                                    relazioni[i].to[relazioni[i].c_to].c_from = 0;
                                    relazioni[i].to[relazioni[i].c_to].from = malloc(
                                            relazioni[i].to[relazioni[i].c_to].c_from * sizeof(char **));

                                    tmpTO = relazioni[i].to[relazioni[i].c_to];
                                    for (j = relazioni[i].c_to; j > k; j--)
                                        relazioni[i].to[j] = relazioni[i].to[j - 1];
                                    relazioni[i].to[k] = tmpTO;
                                    relazioni[i].c_to++;
                                    go = true;
                                }
                            }

                            q = binarySearch(relazioni[i].to[k].from, relazioni[i].to[k].c_from, name1);
                            if (q == relazioni[i].to[k].c_from) {
                                relazioni[i].to[k].from = (char **) realloc(relazioni[i].to[k].from,
                                                                            (relazioni[i].to[k].c_from + 1) *
                                                                            sizeof(char *));
                                relazioni[i].to[k].from[relazioni[i].to[k].c_from] = malloc(
                                        strlen(name1) * sizeof(char) + 1);
                                strcpy(relazioni[i].to[k].from[relazioni[i].to[k].c_from], name1);

                                tmpSTR = relazioni[i].to[k].from[relazioni[i].to[k].c_from];
                                for (j = relazioni[i].to[k].c_from; j > q; j--)
                                    relazioni[i].to[k].from[j] = relazioni[i].to[k].from[j - 1];
                                relazioni[i].to[k].from[q] = tmpSTR;
                                relazioni[i].to[k].c_from++;
                                go = true;
                            } else {
                                if (strcmp(relazioni[i].to[k].from[q], name1) != 0) {
                                    relazioni[i].to[k].from = (char **) realloc(relazioni[i].to[k].from,
                                                                                (relazioni[i].to[k].c_from + 1) *
                                                                                sizeof(char *));
                                    relazioni[i].to[k].from[relazioni[i].to[k].c_from] = malloc(
                                            strlen(name1) * sizeof(char) + 1);
                                    strcpy(relazioni[i].to[k].from[relazioni[i].to[k].c_from], name1);

                                    tmpSTR = relazioni[i].to[k].from[relazioni[i].to[k].c_from];
                                    for (j = relazioni[i].to[k].c_from; j > q; j--)
                                        relazioni[i].to[k].from[j] = relazioni[i].to[k].from[j - 1];
                                    relazioni[i].to[k].from[q] = tmpSTR;
                                    relazioni[i].to[k].c_from++;
                                    go = true;
                                }
                            }

                            if (go) {
                                if (relazioni[i].count < relazioni[i].to[k].c_from) {
                                    for (q = 0; q < relazioni[i].c_out; q++)
                                        free(relazioni[i].out[q]);

                                    relazioni[i].c_out = 0;
                                    relazioni[i].out = realloc(relazioni[i].out,
                                                               (relazioni[i].c_out + 1) * sizeof(char *));
                                    relazioni[i].out[relazioni[i].c_out] = malloc(
                                            strlen(relazioni[i].to[k].to) * sizeof(char) + 1);
                                    strcpy(relazioni[i].out[relazioni[i].c_out], relazioni[i].to[k].to);
                                    relazioni[i].c_out++;
                                    relazioni[i].count = relazioni[i].to[k].c_from;
                                } else if (relazioni[i].count == relazioni[i].to[k].c_from) {
                                    q = binarySearch(relazioni[i].out, relazioni[i].c_out, relazioni[i].to[k].to);

                                    relazioni[i].out = realloc(relazioni[i].out,
                                                               (relazioni[i].c_out + 1) * sizeof(char *));
                                    relazioni[i].out[relazioni[i].c_out] = malloc(
                                            strlen(relazioni[i].to[k].to) * sizeof(char) + 1);
                                    strcpy(relazioni[i].out[relazioni[i].c_out], relazioni[i].to[k].to);

                                    tmpSTR = relazioni[i].out[relazioni[i].c_out];
                                    for (j = relazioni[i].c_out; j > q; j--)
                                        relazioni[i].out[j] = relazioni[i].out[j - 1];
                                    relazioni[i].out[q] = tmpSTR;
                                    relazioni[i].c_out++;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(strcmp(cmd, "delrel") == 0) {
            i = binaryRel(relazioni, c_relazioni, namerel);
            if (i != c_relazioni) {
                if (strcmp(relazioni[i].name, namerel) == 0) {
                    k = binaryTo(relazioni[i].to, relazioni[i].c_to, name2);
                    if (k != relazioni[i].c_to) {
                        if (strcmp(relazioni[i].to[k].to, name2) == 0) {
                            q = binarySearch(relazioni[i].to[k].from, relazioni[i].to[k].c_from, name1);
                            if (q != relazioni[i].to[k].c_from) {
                                if (strcmp(relazioni[i].to[k].from[q], name1) == 0){
                                    free(relazioni[i].to[k].from[q]);
                                    for(j=q;j<relazioni[i].to[k].c_from-1;j++)
                                        relazioni[i].to[k].from[j] = relazioni[i].to[k].from[j+1];
                                    relazioni[i].to[k].c_from--;
                                    relazioni[i].to[k].from = realloc(relazioni[i].to[k].from,relazioni[i].to[k].c_from* sizeof(char *));
                                    go = true;

                                    //FROM empty
                                    if(relazioni[i].to[k].c_from == 0){
                                        free(relazioni[i].to[k].from);
                                        free(relazioni[i].to[k].to);
                                        for(j=k;j<relazioni[i].c_to-1;j++)
                                            relazioni[i].to[j] = relazioni[i].to[j+1];
                                        relazioni[i].c_to--;
                                        relazioni[i].to = realloc(relazioni[i].to,relazioni[i].c_to* sizeof(t_to));
                                        go = true;

                                        // TO VUOTO
                                        if(relazioni[i].c_to == 0){
                                            free(relazioni[i].to);
                                            free(relazioni[i].out);
                                            free(relazioni[i].name);
                                            for(j=i;j<c_relazioni-1;j++)
                                                relazioni[j] = relazioni[j+1];
                                            c_relazioni--;
                                            relazioni = realloc(relazioni,c_relazioni* sizeof(t_relazione));
                                            go = false;
                                        }
                                    }

                                    if(go){
                                        r = binarySearch(relazioni[i].out,relazioni[i].c_out,name2);
                                        if(r != relazioni[i].c_out){
                                            if(strcmp(relazioni[i].out[r],name2) == 0){
                                                free(relazioni[i].out[r]);
                                                for(j=r;j<relazioni[i].c_out-1;j++)
                                                    relazioni[i].out[j] = relazioni[i].out[j+1];
                                                relazioni[i].c_out--;
                                                relazioni[i].out = realloc(relazioni[i].out,relazioni[i].c_out* sizeof(char *));

                                                if(relazioni[i].c_out == 0){
                                                    relazioni[i].count = 0;
                                                    for(j=0;j<relazioni[i].c_to;j++){
                                                        if(relazioni[i].to[j].c_from > relazioni[i].count){
                                                            relazioni[i].count = relazioni[i].to[j].c_from;
                                                            for(r = 0; r<relazioni[i].c_out; r++)
                                                                free(relazioni[i].out[r]);
                                                            relazioni[i].c_out = 1;
                                                            relazioni[i].out = realloc(relazioni[i].out,relazioni[i].c_out* sizeof(char *));
                                                            relazioni[i].out[0] = malloc(strlen(relazioni[i].to[j].to)* sizeof(char)+1);
                                                            strcpy(relazioni[i].out[0],relazioni[i].to[j].to);
                                                        }
                                                        else if(relazioni[i].to[j].c_from == relazioni[i].count){
                                                            r = binarySearch(relazioni[i].out,relazioni[i].c_out,relazioni[i].to[j].to);
                                                            if(r != relazioni[i].c_out){
                                                                if(strcmp(relazioni[i].out[r],name2) != 0){
                                                                    relazioni[i].out = realloc(relazioni[i].out,(relazioni[i].c_out+1)* sizeof(char *));
                                                                    relazioni[i].out[relazioni[i].c_out] = malloc(strlen(relazioni[i].to[j].to)* sizeof(char)+1);
                                                                    relazioni[i].c_out = insertString(relazioni[i].out, relazioni[i].c_out, relazioni[i].to[j].to, r);
                                                                }
                                                            }
                                                            else{
                                                                relazioni[i].out = realloc(relazioni[i].out,(relazioni[i].c_out+1)* sizeof(char *));
                                                                relazioni[i].out[relazioni[i].c_out] = malloc(strlen(relazioni[i].to[j].to)* sizeof(char)+1);
                                                                relazioni[i].c_out = insertString(relazioni[i].out, relazioni[i].c_out, relazioni[i].to[j].to, r);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(strcmp(cmd, "report") == 0){
            if(c_relazioni == 0)
                fputs("none\n",stdout);
            else{
                for(i=0; i<c_relazioni; i++){
                    fputs("\"",stdout);
                    fputs(relazioni[i].name,stdout);
                    fputs("\" ",stdout);
                    for(j=0;j<relazioni[i].c_out;j++){
                        fputs("\"",stdout);
                        fputs(relazioni[i].out[j],stdout);
                        fputs("\" ",stdout);
                    }

                    printf("%d",relazioni[i].count);
                    if(i == c_relazioni-1)
                        fputs(";\n",stdout);
                    else
                        fputs("; ",stdout);
                }
            }
        }
        else if(strcmp(cmd, "allrel") == 0){
            printf("*************\n");
            for(i=0; i<c_relazioni; i++){
                printf("%s\n",relazioni[i].name);
                for(k=0; k<relazioni[i].c_to; k++){
                    printf("\t%s - %d\n",relazioni[i].to[k].to,relazioni[i].to[k].c_from);
                    for(q=0; q<relazioni[i].to[k].c_from; q++)
                        printf("\t\t%s\n",relazioni[i].to[k].from[q]);
                }
            }
            printf("*************\n");
        }
    } while (cmd[0] != 'e' || cmd[1] != 'n' || cmd[2] != 'd' || cmd[3] != '\0');

    //Debugging code
    /*
    j = 0;
    for(i=0; i<c_entita-1; i++)
        if(strcmp(entita[i],entita[i+1]) >= 0){
            j = 1;
            printf("** %s - %s **\n", entita[i],entita[i+1]);
        }
    if(j != 0){
        printf("Errore ordinamento array ENT!\n");
        for(j=0; j<c_entita; j++)
            printf("%s\n",entita[j]);
    }

    j = 0;
    for(i=0; i<c_relazioni-1; i++){
        if(strcmp(relazioni[i].name,relazioni[i+1].name) > 0){
            j = 1;
        }
        for(k=0; k<relazioni[i].c_to-1; k++){
            if(strcmp(relazioni[i].to[k].to,relazioni[i].to[k+1].to) > 0){
                j = 1;
            }
            for(q=0; q<relazioni[i].to[k].c_from-1; q++){
                if(strcmp(relazioni[i].to[k].from[q],relazioni[i].to[k].from[q+1]) > 0){
                    j = 1;
                }
            }
        }
    }
    if(j != 0)
        printf("Errore ordinamento array REL!\n");

    for(i=0; i<c_relazioni; i++){
        printf("%s\n",relazioni[i].name);
        for(k=0; k<relazioni[i].c_to; k++){
            printf("\t%s\n",relazioni[i].to[k].to);
            for(q=0; q<relazioni[i].to[k].c_from; q++)
                printf("\t\t%s\n",relazioni[i].to[k].from[q]);
        }
    }
    
    go = true;
    for(i=0; i<c_output-1; i++)
        if(strcmp(output[i].name,output[i+1].name) >= 0)
            go = false;
    if(go != true)
        printf("Errore ordinamento array OUT!\n");

    printf("*******\n");
    for(i=0; i<c_output; i++)
        for(j=0; j<output[i].c_out; j++)
            printf("%s\n",output[i].out[j]);
    printf("*******\n");
    for(i=0; i<c_relazioni; i++)
        printf("%s - %s - %s\n",relazioni[i].name,relazioni[i].to,relazioni[i].from);
        
    fclose(stdout);
    fclose(stdin);
    
    for(i=0;i<c_relazioni;i++) {
        free(relazioni[i].name);
        free(relazioni[i].to);
        free(relazioni[i].from);
    }
    free(relazioni);
    for(i=0;i<c_output;i++) {
        for(j=0;j<output[i].c_out;j++)
            free(output[i].out[j]);
        free(output[i].out);
        free(output[i].name);
    }
    free(output);
    for(i=0;i<c_entita;i++)
        free(entita[i]);
    free(entita);
    
    clock_t stop = clock();
    double elapsed = (double) (stop-start) / CLOCKS_PER_SEC;
    printf("\n Time elapsed: %.5f\n", elapsed);
    */
    return 0;
}

int binarySearch(char ** entita, int count, char * val){
    int f,m,l;
    f = 0;
    l = count - 1;
    m = l/2;
    if(count == 0){
        return 0;
    }
    while(f <= l){
        m = (f+l)/2;
        if(strcmp(entita[m],val) == 0)
            return m;
        else if(strcmp(entita[m],val) < 0)
            f = m+1;
        else l = m-1;
    }
    if(strcmp(entita[m],val) < 0)
            m++;
    return m;
}
int insertString(char ** entita, int count, char* val, int index){
    int i;
    char * tmp;
    strcpy(entita[count],val);
    //for(i=0;i<strlen(val);i++)
    //    entita[count][i] = val[i];
    if(index != count){
        tmp = entita[count];
        for(i=count; i>index; i--)
            entita[i] = entita[i-1];
        entita[index] = tmp;
    }
    count++;
    return count;
}
int binaryRel(t_relazione * relazioni, int count, char * val){
    int f,m,l;
    f = 0;
    l = count - 1;
    m = l/2;
    if(count == 0){
        return 0;
    }
    while(f <= l){
        m = (f+l)/2;
        if(strcmp(relazioni[m].name,val) == 0)
            return m;
        else if(strcmp(relazioni[m].name,val) < 0)
            f = m+1;
        else l = m-1;
    }
    if(strcmp(relazioni[m].name,val) < 0)
        m++;
    return m;
}
int binaryTo(t_to * to, int count, char * val){
    int f,m,l;
    f = 0;
    l = count - 1;
    m = l/2;
    if(count == 0){
        return 0;
    }
    while(f <= l){
        m = (f+l)/2;
        if(strcmp(to[m].to,val) == 0)
            return m;
        else if(strcmp(to[m].to,val) < 0)
            f = m+1;
        else l = m-1;
    }
    if(strcmp(to[m].to,val) < 0)
        m++;
    return m;
}
