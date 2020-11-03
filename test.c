#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

typedef struct Student{
    char name[50];
    float gpa;
    int toeic;
    int tichLuy;
    int loan;
    char mail[50];
}SinhVien;

SinhVien sv = {"Bui Xuan Dong", 2.53, 490, 101, 0, "saunguxuan@gmail.com"};

void* sendMail(void *arg)
{	
    pthread_mutex_lock(&lock); 
    printf("ID thread: %ld\n", pthread_self());
    char contentMail[]="Thong tin sinh vien da duoc cap nhat:\n";
    char _IDThread[100];
    char _name[100];
    char _gpa[100];
    char _toeic[100];
    char _tichLuy[100];
    char _loan[100];
    sprintf(_IDThread,"ID thread: %ld\n",pthread_self());
    sprintf(_name,"Ho va ten: %s\n", sv.name);
    sprintf(_gpa,"GPA: %0.2f\n", sv.gpa);
    sprintf(_toeic,"Diem Toeic: %d\n", sv.toeic);
    sprintf(_tichLuy,"Tin chi da tich luy: %d\n", sv.tichLuy);
    sprintf(_loan,"No hoc phi: %d", sv.loan);
    
    char cmd[100];  // to hold the command.
    char tempFile[100];     // name of tempfile.
    
    strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

    FILE *fp = fopen(tempFile,"w"); // open it for writing.
    fprintf(fp,"%s\n",contentMail);        // write body to it.
    fprintf(fp,"%s\n",_IDThread); 
    fprintf(fp,"%s\n",_name); 
    fprintf(fp,"%s\n",_gpa); 
    fprintf(fp,"%s\n",_toeic); 
    fprintf(fp,"%s\n",_tichLuy); 
    fprintf(fp,"%s\n",_loan);
    fclose(fp);
    sprintf(cmd,"sendmail %s < %s", sv.mail,tempFile); // prepare command.
    system(cmd);     // execute it.

    printf("\nSuccessfully send mail\n");
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(0xFFFFFFFF);i++);

    printf("\n Job %d finished\n", counter);

    pthread_mutex_unlock(&lock);

    return NULL;

}
void* fixInfo(void *arg)
{
    pthread_mutex_lock(&lock);
    sv.toeic = 700;
    sv.loan = 1000000;
    sv.tichLuy = 122;
    printf("ID thread: %ld\n", pthread_self());
    printf("Ho va ten: %s\n", sv.name);
    printf("GPA: %0.2f\n", sv.gpa);
    printf("Diem Toeic: %d\n", sv.toeic);
    printf("Tin chi da tich luy: %d\n", sv.tichLuy);
    printf("No hoc phi: %d\n", sv.loan);
    printf("\nSuccessfully updated\n");
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(0xFFFFFFFF);i++);

    printf("\n Job %d finished\n", counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{
    int i = 0;
    int err, err2;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

  //  while(i < 2)
  //  {
       err = pthread_create(&(tid[0]), NULL, &fixInfo, NULL);
       if (err != 0)
           printf("\ncan't create thread :[%s]", strerror(err));
     //   i++;
 //   }
	err2 = pthread_create(&(tid[1]), NULL, &sendMail, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err2));

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}	
