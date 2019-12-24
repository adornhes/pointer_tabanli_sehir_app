#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct komsular
{
    int plaka_kodu2;
    struct komsular *next_komsu;
    char ksehir_adi[20];

} ;
typedef struct komsular *knode;   //komsular structini isaret eden pointer veri tipimiz
struct sehirler
{
    int komsu_say;
    int plaka_kodu;
    char sehir_adi[20];
    char bolge[2];
    struct sehirler *next_sehir;
    knode ilk_komsu;
};
typedef struct sehirler *node;    // sehirler structini isaret eden pointer veri tipimiz

node ilk=NULL;
node son=NULL;
node iter=NULL;
node iter2=NULL;
node gecici=NULL;
knode kson=NULL;
knode kgecici=NULL;
knode kiter=NULL;

node createNode()       //sehirler structi icin bellekte yer tahsis eden kod parcasi...
{
    node temp;
    temp = (node)malloc(sizeof(struct sehirler));
    temp->next_sehir=NULL;
    return temp;
}
knode createKNode()       // komsu sehirler structi icin bellekte yer tahsis eden kod parcasi...
{
    knode temp;
    temp=(knode)malloc(sizeof(struct komsular));
    temp->next_komsu=NULL;
    return temp;
}
int fileread1(FILE *p)
{
    char trash[100];
    char buffer[150];
    p=fopen("sehirler.txt","r");
    char sub[20];
    char sub2[3];
    int sub3;
    int linecheck=0;
    while(1)
    {
        if(ilk!=NULL)
        {
            gecici=createNode();
            if(gecici==NULL)
            {
                printf("\nbellek tahsis edilemedi...");
                break;
            }
            fgets(buffer,sizeof(buffer),p);
            if(feof(p))
            {
                return 0;
            }
            sscanf(buffer,"%d,%[^,],%[^,]",&sub3,&sub,&sub2);
            // printf("\nplaka:%d  ad:%s  bolge:%s\n",sub3,sub,sub2);
            strcpy(gecici->sehir_adi,sub);
            strcpy(gecici->bolge,sub2);
            gecici->plaka_kodu=sub3;
            sub[2]="";
            sub3=0;
            sub2[20]="";
            buffer[150]="";   //trashleri temizledik..
            son->next_sehir=gecici;
            son=gecici;
            son->ilk_komsu=NULL;
            linecheck++;
        }
        if(ilk==NULL)
        {
            ilk=createNode();
            fgets(buffer,sizeof(buffer),p);
            if(feof(p))
            {
                return 0;
            }
            sscanf(buffer,"%d,%[^,],%[^,]",&sub3,&sub,&sub2);
            //  printf("plaka:%d  ad:%s  bolge:%s\n",sub3,sub,sub2);
            strcpy(ilk->sehir_adi,sub);
            strcpy(ilk->bolge,sub2);
            ilk->plaka_kodu=sub3;
            sub[2]="";
            sub2[20]="";
            sub3=0;
            buffer[150]="";
            ilk->next_sehir=NULL;
            ilk->ilk_komsu=NULL;
            son=ilk;
            linecheck++;
        }
    }
    fclose(p);
}
int fileread2(FILE *p)
{
    int trash,ksayac=0;
    char trash1[15],trash2[2],buffer[150],subbuffer[150];
    p=fopen("sehirler.txt","r");
    iter=ilk;
    char *delimiter;
    int temp; // sort icin gerekli degisken.Dongu icinde tekrar tanimlanmasina gerek yoktur..
    while(1)
    {
        fgets(buffer,150,p);      //dosyadan bir satir okuduk...
        if(feof(p))
        {
            return 0;
        }
        sscanf(buffer,"%d,%[^,],%[^,],%[^\n]",&trash,&trash1,&trash2,&subbuffer);// satiri veri yapimiz icin uygun formata getirdik.
        trash2[2]="";                                                             //subbuffer bizim icin gerekli veriyi gecici olarak saklar...
        trash1[15]="";
        buffer[150]="";  //trashleri temizledik
        delimiter=strtok(subbuffer,",");
        while(delimiter!=NULL)
        {
            iter2=ilk;                     //komsu sehirlerin plakalarini bulmak icin her komsu sehir eklendiginde ana dugumun ilk sehrine donen pointer...
            if(iter->ilk_komsu==NULL)
            {
                iter->ilk_komsu=createKNode();
                if(iter->ilk_komsu==NULL)              //ilk_komsu icin node olusturduktan sonra ikinci NULL mu kontrolunu yaptik..
                {
                    printf("\n ilk kgecici icin bellek tahsis edilemedi...\n");
                    break;
                }
                strcpy(iter->ilk_komsu->ksehir_adi,delimiter);
                kson=iter->ilk_komsu;
                kson->next_komsu=NULL;
                while(1)                                                 //komsu sehilerin plakalarini bulan dongu..
                {
                    if(strcmp(delimiter,iter2->sehir_adi)==0)    //komsu sehir dugumlerini ana dugumler ile karsilastiran if-else..
                    {
                        kson->plaka_kodu2=iter2->plaka_kodu;    //match yakalanirsa ana dugumde ki plaka kodu komsu ilgili komsu node a tasinir..
                        break;
                    }
                    else
                    {
                        iter2=iter2->next_sehir;                 //sehir isimleri math olmassa ana dugum ilerler..
                    }
                }
            }
            else
            {
                kgecici=createKNode();
                if(kgecici==NULL)
                {
                    printf("\nkgecici ye bellek tahsis edilemedi...\n");
                    break;
                }
                strcpy(kgecici->ksehir_adi,delimiter);
                kson->next_komsu=kgecici;
                kson=kgecici;
                kson->next_komsu=NULL;
                kgecici=NULL;
                while(1)
                {
                    if(strcmp(delimiter,iter2->sehir_adi)==0 )     //komsu sehir dugumlerini ana dugumler ile karsilastiran if-else..
                    {
                        kson->plaka_kodu2=iter2->plaka_kodu;    //match yakalanirsa ana dugumde ki plaka kodu komsu ilgili komsu node a tasinir..
                        break;
                    }
                    else
                    {
                        iter2=iter2->next_sehir;                 //sehir isimleri math olmassa ana dugum ilerler..
                    }
                }
            }
            delimiter=strtok(NULL,",");
            ksayac++;
        }
        iter->komsu_say=ksayac;
        ksayac=0;
        knode knode1= iter->ilk_komsu;     //sort icin gerekli yardimci degiskenler olusturuluyor...
        knode knode2=NULL;
        for(knode1; knode1!=NULL; knode1=knode1->next_komsu)         //bubble sort ile komsularin plaka kodlari siralandi...
        {
            for(knode2=knode1->next_komsu; knode2!=NULL; knode2=knode2->next_komsu)
            {
                if(knode1->plaka_kodu2>knode2->plaka_kodu2)
                {
                    temp=knode1->plaka_kodu2;
                    knode1->plaka_kodu2=knode2->plaka_kodu2;
                    knode2->plaka_kodu2=temp;
                }
            }
        }
        iter=iter->next_sehir;
    }
    fclose(p);
}
node bul1(char *param,int check)           //sehir adinin kullanicidan alip listede arama yapar..
{
    if(check==1)
    {
        int newparam=atoi(param);
        iter=NULL;
        iter=ilk;
        while(iter!=NULL)
        {
            if(iter->plaka_kodu==newparam)
            {
                return iter;
            }
            else
            {
                iter=iter->next_sehir;
            }
            if(iter==NULL)
            {
                return NULL;
            }
        }
    }
    else
    {
        iter=NULL;
        iter=ilk;
        while(iter!=NULL)
        {
            if(strcmp(iter->sehir_adi,param)==0)
            {
                return iter;
            }
            else
            {
                iter=iter->next_sehir;
            }
            if(iter==NULL)
            {
                return NULL;
            }
        }
    }
    return NULL;
}
int yeni_sehir_ekle()
{
    char *yeni_sehir_ad=NULL;
    yeni_sehir_ad=(char*)malloc(sizeof(char)*20);
    char yeni_bolge[2];
    int yeni_plaka;
    printf("\nEklenecek sehirin adini giriniz: ");
    fgetchar();
    scanf("%s",yeni_sehir_ad);
    printf("Eklenecek sehirin bolge kisaltmasini giriniz:");
    scanf("%s",yeni_bolge);
    printf("Eklenecek sehirin plaka kodunu:");
    scanf("%d",&yeni_plaka);
    iter=ilk;
    while(iter!=NULL)
    {
        if(strcmp(iter->sehir_adi,yeni_sehir_ad)==0)
        {
            printf("\nEklenmek istenen sehir ile ayni ada sahip baska bir sehir vardir.Menuye donuldu.");
            return 0;
        }
        iter=iter->next_sehir;
    }
    iter=ilk;
    node temp=NULL;
    node yardimci=NULL,yardimci2=NULL;
    int a=0,secim,b;
    printf("\nNereye eklensin(1-3)\nBasa eklemek icin 1 yazip enter tusuna basiniz\nSona eklemek icin 2 yazip enter tusuna basiniz:\nPlaka koduna gore ekleme yapmak icin 3 yazip enter a basiniz:");
    scanf("%*c%d",&secim);
    switch(secim)
    {
    case 1:
        temp=createNode();
        temp->plaka_kodu=yeni_plaka;
        strcpy(temp->sehir_adi,yeni_sehir_ad);
        strcpy(temp->bolge,yeni_bolge);
        temp->next_sehir=ilk;
        ilk=temp;
        ilk->ilk_komsu=NULL;
        temp=NULL;
        yardimci2=ilk->next_sehir;
        while(yardimci2!=NULL)
        {
            yardimci2->plaka_kodu++;
            yardimci2=yardimci2->next_sehir;
        }
        iter=ilk;
        while(iter!=NULL)
        {
            kiter=iter->ilk_komsu;
            while(kiter!=NULL)
            {
                kiter->plaka_kodu2++;
                kiter=kiter->next_komsu;
            }
            iter=iter->next_sehir;
        }
        printf("\n----case 1 calsiti---\n");
        return 0;
    case 2:
        temp=createNode();
        strcpy(temp->bolge,yeni_bolge);
        strcpy(temp->sehir_adi,yeni_sehir_ad);
        temp->plaka_kodu=yeni_plaka;
        son->next_sehir=temp;
        son=temp;
        son->next_sehir=NULL;
        son->ilk_komsu=NULL;
        printf("\n-----case 2 calisti-----\n");
        return 0;
    case 3:
        while(iter!=NULL)
        {
            yardimci2=NULL;
            yardimci=NULL;
            if(iter->plaka_kodu==yeni_plaka-1)
            {
                printf("-------------");
                yardimci=iter->next_sehir;  //veri kaybini onlemek icin eklenecek node'un next ini yedekliyoruz.
                iter->next_sehir=NULL;
                temp=createNode();
                strcpy(temp->bolge,yeni_bolge);
                temp->plaka_kodu=yeni_plaka;
                strcpy(temp->sehir_adi,yeni_sehir_ad);
                iter->next_sehir=temp;         //node eklendi
                temp->next_sehir=yardimci;  //yedeklenen node eklenen node nextine baglandi veri kaybini onledik.
                temp->ilk_komsu=NULL;
                yardimci2=iter->next_sehir->next_sehir;
                b=yardimci2->plaka_kodu;
                while(yardimci2!=NULL)
                {
                    yardimci2->plaka_kodu++;
                    yardimci2=yardimci2->next_sehir;
                }
                break;
            }
            iter=iter->next_sehir;
        }
        iter=ilk;
        while(iter!=NULL)
        {
            kiter=iter->ilk_komsu;
            while(kiter!=NULL)
            {
                if(kiter->plaka_kodu2>=b)
                {
                    kiter->plaka_kodu2++;
                }
                kiter=kiter->next_komsu;
            }

            iter=iter->next_sehir;
        }
        return 0;
    }
    temp=NULL;
}
int yeni_komsu_ekle()
{
    iter=NULL;
    iter=ilk;
    kiter=NULL;
    char location[20];
    char add[20];
    int a,b;                    //eklenecek komsunun plaka kodu  girilmesi halinde b ,eklenecek komsunun plaka kodudur.
    //komsu eklenmesi istenilen sehirin plaka kodu girilmesi halinde a,sehirin plaka kodudur.
    printf("Komsu eklemek istediginiz sehirin adini yazip enter a basin:");
    scanf("%s",location);
    printf("Eklemek istediginiz komsu sehirin adini giriniz:");
    scanf("%s",add);
    iter=NULL;
    iter=ilk;
    while(iter!=NULL)
    {
        if(strcmp(iter->sehir_adi,add)==0)
        {
            goto label3;
        }
        iter=iter->next_sehir;
    }
    printf("\nKomsu eklenemedi.(eklenmek istenen komsu bilgisi mevcut degil)");
    return 0;
label3:
    iter=NULL;
    iter=ilk;
    while(iter!=NULL)
    {
        if(strcmp(iter->sehir_adi,add)==0)
        {
            b=iter->plaka_kodu;
        }
        iter=iter->next_sehir;
    }
    iter=ilk;
    while(iter!=NULL)
    {
        if(strcmp(iter->sehir_adi,location)==0)
        {
            kiter=iter->ilk_komsu;
            if(kiter==NULL)
            {
                knode temp=createKNode();
                iter->ilk_komsu=temp;
                temp->plaka_kodu2=b;
                iter->komsu_say++;
                temp->next_komsu=NULL;
                return 0;
            }
            if(kiter->plaka_kodu2==b)
            {
                printf("\n\nKomsu ekleme basarisiz.(Eklemek istediginiz komsu zaten vardir)");
                return 0;
            }
            if(kiter->plaka_kodu2>b)
            {
                knode temp=createKNode();
                if(temp==NULL)
                {
                    return 0;
                }
                temp->next_komsu=NULL;
                knode yardimci=NULL;
                temp->next_komsu=iter->ilk_komsu;
                iter->ilk_komsu=temp;
                temp->plaka_kodu2=b;
                iter->komsu_say++;
                return 1;
            }
            if(kiter->next_komsu==NULL)
            {
                knode yardimci;
                knode temp=createKNode();
                yardimci=kiter->next_komsu;
                kiter->next_komsu=temp;
                temp->plaka_kodu2=b;
                temp->next_komsu=yardimci;
                yardimci=NULL;
                iter->komsu_say++;
                return 1;
            }
            while(kiter!=NULL)
            {
                if(kiter->next_komsu->plaka_kodu2==b)
                {
                    printf("\n\nKomsu ekleme basarisiz.(Eklemek istenilen komsu zaten mevcuttur menuye donuluyor...)");
                    return 0;
                }
                if(kiter->next_komsu->plaka_kodu2>b)
                {
                    knode temp=createKNode();
                    if(temp==NULL)
                    {
                        exit(0);
                    }
                    knode yardimci;
                    yardimci=kiter->next_komsu;
                    kiter->next_komsu=temp;
                    temp->plaka_kodu2=b;
                    temp->next_komsu=yardimci;
                    yardimci=NULL;
                    printf("\nKomsu ekleme basarili menuye donuluyor...");
                    iter->komsu_say++;
                    return 1;
                }
                if(kiter->next_komsu->plaka_kodu2<b)
                {
                    kiter=kiter->next_komsu;
                }
                if(kiter->next_komsu==NULL)
                {
                    knode temp=createKNode();
                    if(temp==NULL)
                    {
                        exit(0);
                    }
                    knode yardimci=NULL;
                    yardimci=kiter->next_komsu;
                    kiter->next_komsu=temp;
                    temp->plaka_kodu2=b;
                    temp->next_komsu=yardimci;
                    yardimci=NULL;
                    printf("\nKomsu ekleme basarili menuye donuluyor...");
                    iter->komsu_say++;
                    return 1;
                }
            }
        }
        if(iter->next_sehir==NULL)
        {
            printf("\nKomsu ekleme basarisiz.(Ekleme yapilmak istenen sehir mevcut degildir)");
            return 0;
        }
        iter=iter->next_sehir;
    }
}
int sil()
{
    char location[20],komsu[20];
    int secim,a;
    printf("\nSehir silmek icin 1,komsu silmek icin 2 yazip enter a basin.");
    scanf("%d",&secim);
    switch(secim)
    {
    case 1:
        printf("Silinecek sehirin adini yazip enter a basin.");
        scanf("%*c%s",&location);
        iter=ilk;
        while(iter!=NULL)
        {
            if(strcmp(iter->sehir_adi,location)==0)
            {

                goto label2;
            }
            iter=iter->next_sehir;
        }
        printf("\nSilinmek istenen sehir mevcut degildir.Menuye donuluyor..");
        return 0;
label2:
        iter=ilk;
        iter2=NULL;
        while(iter!=NULL)
        {
            if(strcmp(ilk->sehir_adi,location)==0)
            {
                node yardimci;
                yardimci=ilk->next_sehir;
                ilk=NULL;
                ilk=yardimci;
                iter2=ilk;
                while(iter2!=NULL)
                {
                    iter2->plaka_kodu--;
                    kiter=iter2->ilk_komsu;
                    while(kiter!=NULL)
                    {
                        kiter->plaka_kodu2--;
                        if(kiter->plaka_kodu2==0)
                        {
                            knode yardimci2;
                            yardimci2=iter2->ilk_komsu->next_komsu;
                            iter2->ilk_komsu=NULL;
                            iter2->ilk_komsu=yardimci2;
                            iter2->komsu_say--;
                        }
                        kiter=kiter->next_komsu;
                    }
                    iter2=iter2->next_sehir;
                }
                return 0;
            }
            if(strcmp(iter->next_sehir->sehir_adi,location)==0)
            {
                node yardimci;
                iter->next_sehir->ilk_komsu=NULL;
                yardimci=iter->next_sehir->next_sehir;
                iter->next_sehir=NULL;
                iter->next_sehir=yardimci;
                a=iter->plaka_kodu;
                iter2=ilk;
                knode yardimci1;
                knode yardimci2;
                while(iter2!=NULL)
                {
                    if(iter2->plaka_kodu>a)
                    {
                        iter2->plaka_kodu--;
                    }
                    kiter=iter2->ilk_komsu;
                    while(1)
                    {
                        if(kiter->plaka_kodu2>a)
                        {
                            kiter->plaka_kodu2--;
                        }
                        kiter=kiter->next_komsu;
                        if(kiter==NULL)
                        {
                            break;
                        }
                    }
                    kiter=iter2->ilk_komsu;
                    while(1)
                    {
                        if(iter2->ilk_komsu->plaka_kodu2==a)
                        {
                            yardimci=iter2->ilk_komsu->next_komsu;
                            iter2->ilk_komsu=NULL;
                            iter2->ilk_komsu=yardimci;
                            iter2->komsu_say--;
                            break;
                        }
                        if(iter2->ilk_komsu->next_komsu==NULL)
                        {
                            break;
                        }
                        if(kiter->next_komsu!=NULL)
                        {
                            if(kiter->next_komsu->plaka_kodu2==a)
                            {
                                if(kiter->next_komsu->next_komsu!=NULL)
                                {
                                    yardimci1=kiter;
                                    yardimci2=kiter->next_komsu->next_komsu;
                                    yardimci1->next_komsu=NULL;
                                    yardimci1->next_komsu=yardimci2;
                                    iter2->komsu_say--;
                                    break;
                                }
                                else
                                {
                                    kiter->next_komsu=NULL;
                                    iter2->komsu_say--;
                                    break;
                                }
                            }
                        }
                        kiter=kiter->next_komsu;
                        if(kiter->next_komsu==NULL)
                        {
                            break;
                        }
                    }
                    iter2=iter2->next_sehir;
                }
                return 0;
            }
            if(strcmp(ilk->sehir_adi,location)==0)
            {
                node yardimci;
                yardimci=ilk->next_sehir;
                ilk=NULL;
                ilk=yardimci;
                iter2=ilk;
                while(iter2!=NULL)
                {
                    iter2->plaka_kodu--;
                    kiter=iter2->ilk_komsu;
                    while(kiter!=NULL)
                    {
                        kiter->plaka_kodu2--;
                        if(kiter->plaka_kodu2==0)
                        {
                            knode yardimci2;
                            yardimci2=iter2->ilk_komsu->next_komsu;
                            iter2->ilk_komsu=NULL;
                            iter2->ilk_komsu=yardimci2;
                            iter2->komsu_say--;
                        }
                        kiter=kiter->next_komsu;
                    }
                    iter2=iter2->next_sehir;
                }
                return 0;
            }
            iter=iter->next_sehir;
        }
        return 0;
    case 2:
        location[20]="";
        int komsu_plaka=0;
        printf("Komsunu silmek istediginiz sehirin adini giriniz:");
        scanf("%*c%s",location);
        printf("Silmek istediginiz komsu sehirin adini giriniz:");
        scanf("%*c%s",komsu);
        iter=ilk;
        int check=0;
        while(iter!=NULL)
        {
            if(strcmp(iter->sehir_adi,location)==0)
            {
                check++;
            }
            iter=iter->next_sehir;
        }
        if(check==0)
        {
            printf("\nKomsu silme basarisiz.(Komsusu silinmesi istenen sehir mevcut degildir)\n\n");
            return 0;
        }
        iter=ilk;
        while(iter!=NULL)
        {
            if(strcmp(iter->sehir_adi,komsu)==0)
            {
                komsu_plaka=iter->plaka_kodu;
            }
            iter=iter->next_sehir;
        }
        iter=ilk;
        while(iter!=NULL)
        {
            if(strcmp(iter->sehir_adi,location)==0)
            {
                kiter=iter->ilk_komsu;
                while(kiter!=NULL)
                {
                    if(iter->ilk_komsu->plaka_kodu2==komsu_plaka)
                    {
                        knode yardimci2;
                        yardimci2=iter->ilk_komsu->next_komsu;
                        iter->ilk_komsu=NULL;
                        iter->ilk_komsu=yardimci2;
                        iter->komsu_say--;
                        return 0;
                    }
                    if(kiter->next_komsu!=NULL)
                    {
                        if(kiter->next_komsu->plaka_kodu2==komsu_plaka)
                        {
                            if(kiter->next_komsu->next_komsu!=NULL)
                            {
                                knode yardimci1=kiter;
                                knode yardimci2=kiter->next_komsu->next_komsu;
                                yardimci1->next_komsu=NULL;
                                yardimci1->next_komsu=yardimci2;
                                iter->komsu_say--;
                                return 0;
                            }
                            else
                            {
                                kiter->next_komsu=NULL;
                                iter->komsu_say--;
                                return 0;
                            }
                        }
                    }
                    kiter=kiter->next_komsu;
                }
            }
            iter=iter->next_sehir;
        }
        printf("\nKomsu silme basarisiz.(Komsu mevcut degildir)\n\n");
        return 0;
    }
}
void bolgelistele(char *bolge)
{
    iter=NULL;
    iter=ilk;
    int a=1;
    while(iter!=NULL)
    {
        if(strcmp(iter->bolge,bolge)==0)
        {
            printf("--------%d.sehir-------",a);
            printf("\nAd:%s",iter->sehir_adi);
            printf("\nPlaka kodu:%d",iter->plaka_kodu);
            printf("\nKomsu sayisi:%d\n",iter->komsu_say);
            a++;
            iter=iter->next_sehir;
        }
        else
        {
            iter=iter->next_sehir;
        }
    }
}
void komsulistele1(int *a)
{
    printf("\n\n");
    iter=NULL;
    iter=ilk;
    int b=1;
    while(iter!=NULL)
    {
        if(iter->komsu_say>a)
        {
            printf("--------%d.sehir-------",b);
            printf("\nAd:%s",iter->sehir_adi);
            printf("\nPlaka kodu:%d",iter->plaka_kodu);
            printf("\nKomsu sayisi:%d\n",iter->komsu_say);
            b++;
            iter=iter->next_sehir;
        }
        else
        {
            iter=iter->next_sehir;
        }
    }
}
void komsulistele2(int a)
{
    printf("\n\n");
    iter=NULL;
    iter=ilk;
    int b=1;
    while(iter!=NULL)
    {
        if(iter->komsu_say<a)
        {
            printf("\n--------%d.sehir-------",b);
            printf("\nAd:%s",iter->sehir_adi);
            printf("\nPlaka kodu:%d",iter->plaka_kodu);
            printf("\nKomsu sayisi:%d\n",iter->komsu_say);
            b++;
            iter=iter->next_sehir;
        }
        else
        {
            iter=iter->next_sehir;
        }
    }
}
void komsulistele3(int a)
{
    printf("\n\n");
    iter=NULL;
    iter=ilk;
    int b=1;
    while(iter!=NULL)
    {
        if(iter->komsu_say==a)
        {
            printf("--------%d.sehir-------",b);
            printf("\nAd:%s",iter->sehir_adi);
            printf("\nPlaka kodu:%d",iter->plaka_kodu);
            printf("\nKomsu sayisi:%d\n",iter->komsu_say);
            b++;
            iter=iter->next_sehir;
        }
        else
        {
            iter=iter->next_sehir;
        }
    }
}
void modeli_yazdir(FILE *p)
{
    p=fopen("output.txt","w");
    iter=ilk;
    kiter=NULL;
    while(iter!=NULL)
    {
        fprintf(p,"%d %s %s",iter->plaka_kodu,iter->sehir_adi,iter->bolge);
        kiter=iter->ilk_komsu;
        while(kiter!=NULL)
        {
            fprintf(p,"--->%d",kiter->plaka_kodu2);
            kiter=kiter->next_komsu;
        }
        iter=iter->next_sehir;
        fprintf(p,"\n");
    }
    printf("\nModelin son hali output.txt ye yazildi menuye donuluyor...\n\n");
    fclose(p);
}
int bonus()
{
    char buffer[150],subbuffer[20];
    int plaka[10],i=0;



    int alt_limit,ust_limit,length;
    printf("Komsu sayisi alt limiti yazip enter a basin:");
    scanf("%*c%d",&alt_limit);
    printf("Komsu sayisi ust limiti yazip enter a basin:");
    scanf("%d",&ust_limit);
    printf("\nSinirlayici komsu sehir adlarini giriniz(ad,ad,ad,.. formatinda):");
    getchar();
    gets(buffer);
    char *delimiter1;
    iter=ilk;
    iter2=ilk;
    delimiter1=strtok(buffer,",");
    int mevcut_mu=0;
    while(delimiter1!=NULL)
    {
        while(iter2!=NULL)
        {
            if(strcmp(delimiter1,iter2->sehir_adi)==0)
            {
                mevcut_mu=mevcut_mu+1;
                break;
            }
            iter2=iter2->next_sehir;
        }
        if(mevcut_mu==0)
        {
            printf("\nSinirlayici komsulardan biri liste yapisinda mevcut degildir degildir(%s).Menuye donuluyor..",delimiter1);
            return 0;
        }
        iter=ilk;
        while(iter!=NULL)
        {
            if( strcmp(iter->sehir_adi,delimiter1)==0)
            {
                plaka[i]=iter->plaka_kodu;
                i++;
            }
            iter=iter->next_sehir;
        }

        delimiter1=strtok(NULL,",");
        iter2=ilk;
        mevcut_mu=0;
    }
    int k,l,tutucu,n; //sort icin gerekli degiskenler
    n=i;
    for(k=0; k<n; k++)    //kullanicidan kisitlama icin verilen komsularin plakalarini siraladik(bubble sort)

    {
        for(l=0; l<n-1-k; l++)
        {
            if(plaka[l]>plaka[l+1])
            {
                tutucu=plaka[l];
                plaka[l]=plaka[l+1];
                plaka[l+1]=tutucu;
            }
        }
    }
    int komsu_sehir_say=i;
    int kontrol=0;        //bir sonraki kod parcasinda butun komsularin bulunup bulunmadiðini anlamamiza yardimci olacak sayi..
    int j=1,q=0;        //j ; sonuclari yazdirirken gorsellik acisindan yardimci degisken..
    i=0;
    iter=ilk;
    while(iter!=NULL)
    {
        kontrol=0;           //ana dugumde her ilerledigimizde girilen komsularin tamaminin bulunmasini denetleyen degisken sifirlanir..
        i=0;                 //ana dugumde her ilerledigimizde plaka dizimizde basa donuyoruz.
        if(iter->komsu_say>alt_limit&&iter->komsu_say<ust_limit)
        {

            kiter=iter->ilk_komsu;
            while(kiter!=NULL)
            {
                if(kiter->plaka_kodu2==plaka[i])
                {
                    kontrol++;       //Kullanicidan alinan komsu bulundu ve kontrol sayimiz 1 azaldi , kod sonunda kontrol 0 olursa girilen butun komsular bulunmustur..
                    if(kontrol==komsu_sehir_say)
                    {
                        printf("\n------%d.sehir------\n%s\nPlaka kodu:%d\nKomsu sayisi:%d",j,iter->sehir_adi,iter->plaka_kodu,iter->komsu_say);
                        j++;
                        kontrol=0;
                        i=0;
                        break;
                    }
                    i++;
                }
                kiter=kiter->next_komsu;
            }
        }
        iter=iter->next_sehir;
    }
    return 0;
}
int main()
{
    FILE *p;
    fileread1(p);
    fileread2(p);
    char aranan[20];      //Hata onlemek icin while disinda tanimlanan yardimci degiskenler
    int check=0;
    node yardimci=NULL;
    knode yardimci2=NULL;
    char bolge[2];
    int u;
    int i;
    while(1)
    {
label:
        printf("1.Listede ara\n");
        printf("2.Bolge adina gore listele\n");
        printf("3.Komsu sayisina gore listele\n");
        printf("4.Yeni sehir veya komsuluk ekle\n");
        printf("5.Mevcut sehir veya komsulugu sil\n");
        printf("6.Mevcut modeli output.txt dosyasina yaz\n");
        printf("7.Mevcut modeli konsola yaz\n");
        printf("8.Ayrintili listeleme\n");
        printf("9.Konsolu temizle\n");
        printf("10.Cikis\n");
        printf("Secim yapiniz(1-10):");
        scanf("%d",&i);
        switch(i)
        {
        case 1:
            printf("aramak istediginiz sehrin adini veya plaka kodunu (Sehir adi icin sadece ilk harfin buyuk olmasina dikkat edin): ");
            scanf("%s",&aranan);
            if(isdigit(aranan[0]))
            {
                check++;
                if(bul1(&aranan,check)==NULL)
                {
                    printf("\naranan sehir yoktur.Yeni sehir eklemek ister misin sahip ?(Y/N)");
                    char a;
                    scanf("%*c%c",&a);
                    if(a=='Y')
                    {
                        yeni_sehir_ekle();
                        fgetchar();
                        check=0;
                        break;
                    }
                    else
                    {
                        printf("menuye donuluyor...");
                        fgetchar();
                        check=0;
                        break;
                    }
                }
                else
                {
                    yardimci=bul1(&aranan,check);
                    yardimci2=yardimci->ilk_komsu;
                    printf("\naratilan sehrin adi:%s \nbulundugu bolge: %s",yardimci->sehir_adi,yardimci->bolge);
                    printf("\naratilan sehrin komsulari: ");
                    while(yardimci2!=NULL)
                    {
                        printf("%d ",yardimci2->plaka_kodu2);
                        yardimci2=yardimci2->next_komsu;
                    }
                    check=0;
                    break;
                }
            }
            else
            {
                if(bul1(&aranan,check)==NULL)
                {
                    printf("\naranan sehir yoktur.Yeni sehir eklemek ister misin ?(Y/N)");
                    char a;
                    scanf("%*c%c",&a);
                    if(a=='Y')
                    {
                        printf("\nelse\n");
                        yeni_sehir_ekle();
                        check=0;
                    }
                    else
                    {
                        printf("menuye donuluyor...");
                        fgetchar();
                        check=0;
                        break;
                    }
                }
                else
                {
                    yardimci=NULL;
                    yardimci2=NULL;
                    yardimci=bul1(&aranan,check);
                    yardimci2=yardimci->ilk_komsu;
                    printf("\naratilan sehrin plakasi:%d \nbulundugu bolge: %s",yardimci->plaka_kodu,yardimci->bolge);
                    printf("\naratilan sehrin komsulari: ");
                    while(yardimci2!=NULL)
                    {
                        printf("%d ",yardimci2->plaka_kodu2);
                        yardimci2=yardimci2->next_komsu;
                    }
                    check=0;
                }
            }
            break;
        case 2:
            printf("\nListelemek istediginiz bolgenin kisaltmasini giriniz:");
            scanf("%s",bolge);
            bolgelistele(&bolge);
            break;
        case 3:
            printf("\nListelemek istediginiz komsu sayisini giriniz:");
            scanf("%d",&u);
            int y;
            printf("\nListeleme modu seciniz(1-3)\n1:komsu sayisi %d den buyuk sehirler\n2:komsu sayisi %d den kucuk sehirler\n3:komsu sayisi %d ye esit sehirler",u,u,u);
            scanf("%*c%d",&y);
            printf("\n");
            switch(y)
            {
            case 1:
                printf("komsu sayisi %d den buyuk sehirler:",u);
                komsulistele1(u);
                printf("\n\n");
                goto label;
            case 2:
                printf("komsu sayisi %d den kucuk sehirler:",u);
                komsulistele2(u);
                printf("\n\n");
                goto label;
            case 3:
                printf("komsu sayisi %d ye esit sehirler:",u);
                komsulistele3(u);
                printf("\n\n");
                goto label;
            }
        case 4:
            printf("Sehir eklemek icin 1,komsuluk eklemek icin 2 yazip enter a basin:");
            int a;
            scanf("%*c%d",&a);
            if(a==1)
                yeni_sehir_ekle();
            else
                yeni_komsu_ekle();
            break;

        case 5:
            sil();
            break;
        case 6:
            modeli_yazdir(&p);
            break;
        case 7:
            iter=ilk;
            while(iter!=NULL)
            {
                kiter=iter->ilk_komsu;
                printf("\n%d %s %s komsu sayisi:%d      ",iter->plaka_kodu,iter->sehir_adi,iter->bolge,iter->komsu_say);
                while(kiter!=NULL)
                {
                    printf(" %d ",kiter->plaka_kodu2);
                    kiter=kiter->next_komsu;
                }
                iter=iter->next_sehir;
            }
            break;
        case 8:
            bonus();
            break;
        case 9:
            system("cls");
            goto label;
        case 10:
            printf("\n\n-------CIKIS YAPILDI------\n\n");
            exit(0);
        }
        printf("\n\n\n------------\n\n\n");
    }
    return 0;
}









