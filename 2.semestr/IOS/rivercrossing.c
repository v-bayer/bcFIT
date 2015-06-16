/** Predmet IOS - 3.projekt
Práce s procesy - Rivercrossing problem
Václav Bayer | xbayer05 | 2014  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define CLEAN_SEM \
    do {\
        sem_close(semafors->waiting);\
        sem_close(semafors->boarding_Serf);\
        sem_close(semafors->boarding_Hacker);\
        sem_close(semafors->member);\
        sem_close(semafors->landing);\
        sem_close(semafors->finish);\
        sem_close(semafors->checkready);\
        sem_close(semafors->boating);\
        fclose(semafors->streamOut); }\
    while (0)


/** Chybova hlaseni */
enum tecodes {
    EOK = 0,
    ECLWRONGCNT,
    ECLNOTNUM,
    ECLWRONGVAL,
    EFOPEN,
    EFORK,
    ESEMAPHORE,
    ESHM,
    ESHMCLEAR,
};

/** Chybova hlaseni */
const char const *ecode[] = {
    [EOK] =         "Vse je OK\n",
    [ECLWRONGCNT] = "Chybny pocet zadanych parametru\n",
    [ECLNOTNUM] =   "Jeden ze zadanych parametru neni cislo\n",
    [ECLWRONGVAL] = "Chybna hodnota jednoho z parametru\n",
    [EFOPEN] =      "Chyba pri otevirani souboru pro vystup\n",
    [EFORK] =       "Fork procesu se nepovedl\n",
    [ESEMAPHORE] =  "Chyba pri vytvareni semaforu\n",
    [ESHM] =        "Nepodarilo se alokovat sdilenou pamet\n",
    [ESHMCLEAR] =   "Nepodarilo se odalokovat sdilenou pamet\n",
};

typedef struct synchRead
{
    int members;        // pocet generovanych osob
    int serfSleep;          // doba uspani Serfu
    int hackerSleep;       // doba uspani Hackeru
    int boatSleep;          // doba uspani pro plavbu
    int pocet, molo_Hackers, molo_Serfs, dec, end;  // pocitadla
    sem_t *waiting;         // ze brehu na molo
    sem_t *boarding_Serf;  // z mola na lod
    sem_t *boarding_Hacker;  // z mola na lod
    sem_t *member;     // plavba
    sem_t *landing;     // z lodi na breh
    sem_t *finish;        // ukonceni
    sem_t *checkready;        // otevreni souboru
    sem_t *boating;         // vyreseni problemu s landingem
    FILE *streamOut;    //vystupni proud
} synch;

/** Prototypy funkci */
int getParameters(int argc, char *argv[]);
void fileopen();
void SerfProc(int i);
void HackerProc(int i);
void errorpid(pid_t pid);
int clearSources();
int allocMem();
void killProces();

int shm_id;
synch *semafors=NULL;

int main (int argc, char **argv){
    int errnum;

    // alokovani pameti
    errnum = allocMem();
    if (errnum != EOK) {
        fprintf(stderr, "%s", ecode[errnum]);
        return 2;
    }

    // overeni parametru
    errnum = getParameters(argc, argv);
    if (errnum != EOK) {
        fprintf(stderr, "%s", ecode[errnum]);
        return 1;
    }

    //generovani nahodnych cisel
    srand(time(NULL));

    semafors->waiting = sem_open("/bayer05_waiting", O_CREAT, S_IRUSR|S_IWUSR, 1);
    semafors->boarding_Serf = sem_open("/bayer05_boarding_Serf", O_CREAT, S_IRUSR|S_IWUSR, 0);
    semafors->boarding_Hacker = sem_open("/bayer05_boarding_Hacker", O_CREAT, S_IRUSR|S_IWUSR, 0);
    semafors->member = sem_open("/bayer05_member", O_CREAT, S_IRUSR|S_IWUSR, 0);
    semafors->landing = sem_open("/bayer05_landing", O_CREAT, S_IRUSR|S_IWUSR, 0);
    semafors->finish = sem_open("/bayer05_finish", O_CREAT, S_IRUSR|S_IWUSR, 0);
    semafors->boating = sem_open("/bayer05_boating", O_CREAT, S_IRUSR|S_IWUSR, 0);
    semafors->checkready = sem_open("/bayer05_checkready", O_CREAT, S_IRUSR|S_IWUSR, 1);

    // overeni korektnosti vytvareni semaforu
    if(semafors->waiting == SEM_FAILED
    || semafors->boarding_Serf == SEM_FAILED
    || semafors->boarding_Hacker == SEM_FAILED
    || semafors->member == SEM_FAILED
    || semafors->landing == SEM_FAILED
    || semafors->finish == SEM_FAILED
    || semafors->boating == SEM_FAILED
    || semafors->checkready == SEM_FAILED)
    {
        fprintf(stderr, "%s", ecode[ESEMAPHORE]);

        errnum = clearSources();
        if (errnum != EOK) {
            fprintf(stderr, "%s", ecode[errnum]);
            return 2;
        }
        return 2;
    }

    fileopen();
    semafors->pocet = 1;
    semafors->molo_Hackers = 0;
    semafors->molo_Serfs = 0;
    semafors->dec = 4;
    semafors->end = 0;

    pid_t pid, pid2, pid3;

    pid = fork();
    if (pid == 0)
    {
        for ( int i = 0 ; i < semafors->members; i++)
        {
            if (semafors->serfSleep != 0)
            {
                usleep(rand()%(semafors->serfSleep)*1000);
            }
            pid2 = fork();
            if (pid2 == 0)
            {
                SerfProc(i+1);

                exit(0);
            }
            else if (pid2 < 0)
            {
                errorpid(pid2);
            }
        }
        for ( int i = 0 ; i < semafors->members; i++)
        {
            wait(NULL);
        }
        exit(0);
    }
    else if (pid < 0)
    {
        errorpid(pid);
    }
    else
    {
        pid = fork();
        if(pid == 0)
        {
            for( int i = 0 ; i < semafors->members; i++)
            {
                if (semafors->hackerSleep != 0)
                {
                    usleep(rand()%(semafors->hackerSleep)*1000);
                }
                pid3 = fork();
                if (pid3 == 0)
                {
                    HackerProc(i+1);

                    exit(0);
                }
                else if (pid3 < 0)
                {
                    errorpid(pid3);
                }
            }
            for ( int i = 0 ; i < semafors->members; i++)
            {
                wait(NULL);
            }
            exit(0);
        }
        else if (pid < 0)
        {
            errorpid(pid);
        }
        else
        {
            signal(SIGTERM, killProces);
            signal(SIGINT, killProces);

            wait(NULL);
            wait(NULL);
           // exit(0);
        }
    }

    errnum = clearSources();
    if (errnum != EOK) {
        fprintf(stderr, "%s", ecode[errnum]);
        return 2;
    }

    return 0;
}

/** Kontrola procesu */
void errorpid(pid_t pid) {

    fprintf(stderr, "%s", ecode[EFORK]);

    kill(pid, SIGTERM);
    int errnum;
    errnum = clearSources();
    if (errnum != EOK) {
        fprintf(stderr, "%s", ecode[errnum]);
    }
    killProces(0);
    exit(2);
}

/** Odstraneni semaforu */
int clearSources(void) {
    // uvolneni semaforu
    if (sem_unlink("/bayer05_waiting") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_boarding_Serf") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_boarding_Hacker") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_member") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_landing") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_finish") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_boating") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};
    if (sem_unlink("/bayer05_checkready") < 0){fprintf(stderr, "%s", ecode[ESHMCLEAR]);};

    // uzavreni souboru pro zapis
    fclose(semafors->streamOut);

    if((shmctl(shm_id,IPC_RMID,NULL))==-1){perror("SHMCTL");}

    return EOK;
}

/** Ziskani a overeni parametru */
int getParameters(int argc, char *argv[]) {


    synch* semafors;

    if((semafors = shmat(shm_id, NULL, 0)) == NULL)
    {
        fprintf(stderr, "%s", ecode[ESHM]);
        exit(2);
    }
    char *testParam;

    int bad = 0;

    if (argc == 5) {
        // nacteni parametru do promennych
        semafors->members = (int) strtol(argv[1], &testParam, 10);
        if (testParam[0] != '\0') { bad = 1; }
        semafors->serfSleep = (int) strtol(argv[2], &testParam, 10);
        if (testParam[0] != '\0') { bad = 1; }
        semafors->hackerSleep = (int) strtol(argv[3], &testParam, 10);
        if (testParam[0] != '\0') { bad = 1; }
        semafors->boatSleep = (int) strtol(argv[4], &testParam, 10);
        if (testParam[0] != '\0') { bad = 1; }
        // kontrola hodnot parametru

        if(bad)
        {
            if(shmdt(semafors)==-1){}
            return ECLWRONGVAL;
        }

        if (semafors->members <= 0)
        {
            if(shmdt(semafors)==-1){}
            return ECLWRONGVAL;
        } // overeni intervalu zadanych hodnot
        else if ((semafors->serfSleep < 0) ||
                   (semafors->hackerSleep < 0) ||
                   (semafors->boatSleep < 0) ||
                   (semafors->serfSleep > 5000) ||
                   (semafors->hackerSleep > 5000) ||
                   (semafors->boatSleep > 5000))
        {
            if(shmdt(semafors)==-1){}
            return ECLWRONGVAL;
        }
        else
        {
            if(shmdt(semafors)==-1){return ECLWRONGVAL;}
            return EOK;
        }
    }
    else
    {
        if(shmdt(semafors)==-1){}
        return ECLWRONGCNT;
    }
}

/** Otevreni souboru */
void fileopen(){
    if((semafors->streamOut=fopen("rivercrossing.out","w"))==NULL)
    {
        perror("FILE OPENING");
        semafors->streamOut=stdout;
    }
}

/** Alokovani pameti */
int allocMem(){
    // alokovani sdilene pameti pro procNum (citac procesu)
    if((shm_id = shmget(IPC_PRIVATE, sizeof(synch), 0666 | IPC_CREAT)) < 0)
    { return ESHM; }

    if((semafors = shmat(shm_id, NULL, 0)) == NULL)
    { return ESHM; }

    return EOK;
}

/** Proces Serfa */
void SerfProc(int i) {

    int lastguy = 0; //posledni clen uzavirajici skupinu (kapitan)

    synch* semafors;
    if((semafors = shmat(shm_id, NULL, 0)) == NULL)
    {
        perror("SHMAT");
        exit(2);
    }

    //startovani
    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: serf: %d started\n", (semafors->pocet)++, i);
        fflush(semafors->streamOut);
    sem_post(semafors->checkready);

    //cekani na waiting
    sem_wait(semafors->waiting);

    (semafors->molo_Serfs)++;

    //skupeni se na molu
    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: serf: %d waiting for boarding: %d: %d\n", (semafors->pocet)++, i, semafors->molo_Hackers, semafors->molo_Serfs);
        fflush(semafors->streamOut);
    sem_post(semafors->checkready);

    //overeni zda jsou splneny podminky skupiny
    if ((semafors->molo_Hackers >= 2) && (semafors->molo_Serfs >= 2))
    {
        lastguy = 1;
        semafors->molo_Serfs -= 2;
        semafors->molo_Hackers -= 2;

        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Hacker);
        sem_post(semafors->boarding_Hacker);
    }
    else if (semafors->molo_Serfs == 4)
    {
        lastguy = 1;
        semafors->molo_Serfs -= 4;

        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Serf);
    }

    if (lastguy == 0)
    {
        sem_post(semafors->waiting);
    }

    sem_wait(semafors->boarding_Serf);

    //nalodeni se
    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: serf: %d boarding: %d: %d\n", (semafors->pocet)++, i, semafors->molo_Hackers, semafors->molo_Serfs);
        fflush(semafors->streamOut);
        (semafors->dec)--;
    sem_post(semafors->checkready);

    if (semafors->dec == 0)
    {
        semafors->dec = 4;
        sem_post(semafors->member);
        sem_post(semafors->member);
        sem_post(semafors->member);
        sem_post(semafors->member);
    }

    sem_wait(semafors->member);

    //podminky pro kapitana
    if (lastguy)
    {
        sem_wait(semafors->checkready);
            fprintf(semafors->streamOut, "%d: serf: %d captain\n", (semafors->pocet)++, i);
            fflush(semafors->streamOut);
        sem_post(semafors->checkready);

        sem_wait(semafors->boating);

        if (semafors->boatSleep != 0)
        {
            usleep(rand()%(semafors->boatSleep)*1000);
        }
        sem_post(semafors->landing);
        sem_post(semafors->landing);
        sem_post(semafors->landing);
        sem_post(semafors->landing);
    }
    else
    {
        sem_wait(semafors->checkready);
            fprintf(semafors->streamOut, "%d: serf: %d member\n", (semafors->pocet)++, i);
            fflush(semafors->streamOut);
            (semafors->dec)--;
        sem_post(semafors->checkready);

        if (semafors->dec == 1)
        {
            semafors->dec = 4;
            sem_post(semafors->boating);
        }
    }

    //vylodovani se
    sem_wait(semafors->landing);

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: serf: %d landing: %d: %d\n", (semafors->pocet)++, i, semafors->molo_Hackers, semafors->molo_Serfs);
        fflush(semafors->streamOut);
        (semafors->dec)--;
        (semafors->end)++;
    sem_post(semafors->checkready);

    if (semafors->dec == 0)
    {
        semafors->dec = 4;
        sem_post(semafors->waiting);
    }

    //finish pro vsechny cleny cekajici na ukonceni
    if (semafors->end == semafors->members * 2)
    {
        sem_post(semafors->finish);
    }

    sem_wait(semafors->finish);

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: serf: %d finished\n", (semafors->pocet)++, i);
        fflush(semafors->streamOut);
    sem_post(semafors->checkready);

    sem_post(semafors->finish);

    if(shmdt(semafors)==-1){}
}

/** Proces Hackera */
//stejny postup jako pri Serfovi
void HackerProc(int i) {

    int lastguy = 0;

    synch* semafors;
    if((semafors = shmat(shm_id, NULL, 0)) == NULL)
    {
        perror("SHMAT");
        exit(2);
    }

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: hacker: %d started\n", (semafors->pocet)++, i);
        fflush(semafors->streamOut);
    sem_post(semafors->checkready);

    sem_wait(semafors->waiting);

    (semafors->molo_Hackers)++;

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: hacker: %d waiting for boarding: %d: %d\n", (semafors->pocet)++, i, semafors->molo_Hackers, semafors->molo_Serfs);
        fflush(semafors->streamOut);
    sem_post(semafors->checkready);


    if ((semafors->molo_Hackers >= 2) && (semafors->molo_Serfs >= 2))
    {
        lastguy = 1;
        semafors->molo_Serfs -= 2;
        semafors->molo_Hackers -= 2;

        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Serf);
        sem_post(semafors->boarding_Hacker);
        sem_post(semafors->boarding_Hacker);
    }
    else if (semafors->molo_Hackers == 4)
    {
        lastguy = 1;
        semafors->molo_Hackers -= 4;

        sem_post(semafors->boarding_Hacker);
        sem_post(semafors->boarding_Hacker);
        sem_post(semafors->boarding_Hacker);
        sem_post(semafors->boarding_Hacker);
    }

    if (lastguy == 0)
    {
        sem_post(semafors->waiting);
    }

    sem_wait(semafors->boarding_Hacker);

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: hacker: %d boarding: %d: %d\n", (semafors->pocet)++, i, semafors->molo_Hackers, semafors->molo_Serfs);
        fflush(semafors->streamOut);
        (semafors->dec)--;
    sem_post(semafors->checkready);

    if (semafors->dec == 0)
    {
        semafors->dec = 4;
        sem_post(semafors->member);
        sem_post(semafors->member);
        sem_post(semafors->member);
        sem_post(semafors->member);
    }

    sem_wait(semafors->member);



    if (lastguy)
    {
        sem_wait(semafors->checkready);
            fprintf(semafors->streamOut, "%d: hacker: %d captain\n", (semafors->pocet)++, i);
            fflush(semafors->streamOut);
        sem_post(semafors->checkready);

        sem_wait(semafors->boating);

        if (semafors->boatSleep != 0)
        {
            usleep(rand()%(semafors->boatSleep)*1000);
        }
        sem_post(semafors->landing);
        sem_post(semafors->landing);
        sem_post(semafors->landing);
        sem_post(semafors->landing);
    }
    else
    {
        sem_wait(semafors->checkready);
            fprintf(semafors->streamOut, "%d: hacker: %d member\n", (semafors->pocet)++, i);
            fflush(semafors->streamOut);
            (semafors->dec)--;
        sem_post(semafors->checkready);

        if (semafors->dec == 1)
        {
            semafors->dec = 4;
            sem_post(semafors->boating);
        }
    }


    sem_wait(semafors->landing);

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: hacker: %d landing: %d: %d\n", (semafors->pocet)++, i, semafors->molo_Hackers, semafors->molo_Serfs);
        fflush(semafors->streamOut);
        (semafors->dec)--;
        (semafors->end)++;
    sem_post(semafors->checkready);

    if (semafors->dec == 0)
    {
        semafors->dec = 4;
        sem_post(semafors->waiting);
    }

    if (semafors->end == semafors->members * 2)
    {
        sem_post(semafors->finish);
    }

    sem_wait(semafors->finish);

    sem_wait(semafors->checkready);
        fprintf(semafors->streamOut, "%d: hacker: %d finished\n", (semafors->pocet)++, i);
        fflush(semafors->streamOut);
    sem_post(semafors->checkready);

    sem_post(semafors->finish);

    if(shmdt(semafors)==-1){}
}

/** Zabije proces */
void killProces() {
    // ukonceni
    clearSources();
    kill(getpid(), SIGTERM);
    exit(2);
}


