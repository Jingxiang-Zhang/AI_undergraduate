#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include "math.h"
#include "time.h"
#include <unistd.h>
#include <bits/stdc++.h>
#define CITY_NUM 30     //�����������б����0~CITY_NUM-1
#define POPSIZE 100        //��Ⱥ������
#define MAXVALUE 10000000   //·�����ֵ����
#define N 100000//��Ҫ����ʵ����õ�·��ֵ����
unsigned seed=(unsigned)time(0);
double Hash[CITY_NUM+1];
typedef struct CityPosition
{
    double x;
    double y;
}CityPosition;

CityPosition CityPos[100]={
    {11003.611100,42102.500000},{11108.611100,42373.888900},{11133.333300,42885.833300},
	{11155.833300,42712.500000},{11183.333300,42933.333300},{11297.500000,42853.333300},
	{11310.277800,42929.444400},{11416.666700,42983.333300},{11423.888900,43000.277800},
	{11438.333300,42057.222200},{11461.111100,43252.777800},{11485.555600,43187.222200},
	{11503.055600,42855.277800},{11511.388900,42106.388900},{11522.222200,42841.944400},
	{11569.444400,43136.666700},{11583.333300,43150.000000},{11595.000000,43148.055600},
	{11600.000000,43150.000000},{11690.555600,42686.666700},{11715.833300,41836.111100},
	{11751.111100,42814.444400},{11770.277800,42651.944400},{11785.277800,42884.444400},
	{11822.777800,42673.611100},{11846.944400,42660.555600},{11963.055600,43290.555600},
	{11973.055600,43026.111100},{12058.333300,42195.555600},{12149.444400,42477.500000},
	{12286.944400,43355.555600},{12300.000000,42433.333300},{12355.833300,43156.388900},
	
	{11005.611100,42104.500000},{11110.611100,42375.888900},{11135.333300,42887.833300},
	{11157.833300,42714.500000},{11185.333300,42935.333300},{11299.500000,42855.333300},
	{11312.277800,42911.444400},{11418.666700,42985.333300},{11427.888900,43005.277800},
	{11440.333300,42011.222200},{11465.111100,43257.777800},{11488.555600,43184.222200},
	{11505.055600,42857.277800},{11515.388900,42108.388900},{11521.222200,42842.944400},
	{11511.444400,43138.666700},{11588.333300,43155.000000},{11591.000000,43145.055600},
	{11602.000000,43152.000000},{11694.555600,42687.666700},{11719.833300,41838.111100},
	{11753.111100,42816.444400},{11775.277800,42658.944400},{11784.277800,42884.444400},
	{11824.777800,42675.611100},{11849.944400,42667.555600},{11962.055600,43298.555600},
	{11975.055600,43028.111100},{12062.333300,42195.555600},{12140.444400,42472.500000},
	{12288.944400,43377.555600},{12302.000000,42432.333300},{12351.833300,43154.388900},
	
	{11053.611100,42152.500000},{11158.611100,42343.888900},{11123.333300,42845.833300},
	{11115.833300,42752.500000},{11183.333300,42943.333300},{11197.500000,42843.333300},
	{11320.277800,42989.444400},{11446.666700,42983.333300},{11223.888900,43040.277800},
	{11408.333300,42057.222200},{11411.111100,43272.777800},{11485.555600,43187.222200},
	{11513.055600,42815.277800},{11551.388900,42146.388900},{11552.222200,42871.944400},
	{11542.444400,43176.666700},{11583.333300,43110.000000},{11585.000000,43188.055600},
	{11640.000000,43180.000000},{11640.555600,42666.666700},{11745.833300,41896.111100},
	{11781.111100,42844.444400},{11711.277800,42681.944400},{11715.277800,42864.444400},
	{11812.777800,42643.611100},{11856.944400,42880.555600},{11935.055600,43280.555600},
	{11903.055600,43016.111100},{12018.333300,42155.555600},{12159.444400,42487.500000},
	{12206.944400,43325.555600},{12340.000000,42233.333300},{12385.833300,43146.388900},
	{12386.666700,43355.722200}
	
};

double CityDistance[CITY_NUM][CITY_NUM];//���о���ʵ�

typedef struct{
    int colony[POPSIZE][CITY_NUM+1];//������Ⱥ,Ĭ�ϳ������б��Ϊ0������б�ŵ����һ�����л�Ӧ��Ϊ0
    double fitness[POPSIZE];// ÿ���������Ӧֵ����1/Distance[POPSIZE]
    double Distance[POPSIZE];//ÿ���������·��
    int BestRooting[CITY_NUM+1];//���ų���·������
    double BestFitness;//����·����Ӧֵ
    double BestValue;//����·������
    int BestNum;
}TSP,*PTSP;

/*������о���ʵ�CityDistance[i][j]*/
void CalculatDist()
{
    int i,j;
    double temp1,temp2;
    for(i=0;i<CITY_NUM;i++){
        for(j=0;j<=CITY_NUM;j++){//���һ�����л�Ӧ�÷��ص������ڵ�
            temp1=CityPos[j].x-CityPos[i].x;
            temp2=CityPos[j].y-CityPos[i].y;
            CityDistance[i][j]=sqrt(temp1*temp1+temp2*temp2);
        }
    }
}
/*���鸴��*/
void copy(int a[],int b[])
{
    int i=0;
    for(i=0;i<CITY_NUM+1;i++)
    {
        a[i]=b[i];
    }
}

/*������������ɵĽڵ��Ƿ��ڵ�ǰȺ���У�0�Žڵ���Ĭ�ϳ����ڵ����ֹ�ڵ�*/
bool check(TSP &city,int pop,int num,int k)
{
    int i;
    for(i=0;i<=num;i++){
        if(k==city.colony[pop][i])
            return true;//�����ɽڵ�������Ѿ����ɵ�·����
    }
    return false;//�����ɽڵ�û�д������Ѿ����ɵ�·����
}

/****************��Ⱥ��ʼ������Ϊcity.colony[i][j]��ֵ****************/
void InitColony(TSP &city)
{
    int i,j,r;
    for(i=0;i<POPSIZE;i++){
        city.colony[i][0]=0;
        city.colony[i][CITY_NUM]=0;
        city.BestValue=MAXVALUE;
        city.BestFitness=0;//��ӦֵԽ��Խ��
    }

    for(i=0;i<POPSIZE;i++)
    {
        for(j=1;j<CITY_NUM;j++)
        {
            r=rand()%(CITY_NUM-1)+1;//����1��CITY_NUM-1֮��������
            while(check(city,i,j,r))//�������������ţ���Ϊcity.colony[i][j]��ֵ
            {
                r=rand()%(CITY_NUM-1)+1;
            }

            city.colony[i][j]=r;
        }
    }
}

/*������Ӧֵ,����Ӧ���������������ѡ����*/
void CalFitness(TSP &city)
{
    int i,j;
    int start,end;
    int Best=0;
    for(i=0;i<POPSIZE;i++){//��ÿ���������·������Ӧֵ
        city.Distance[i]=0;
        for(j=1;j<=CITY_NUM;j++){
            start=city.colony[i][j-1];end=city.colony[i][j];
            city.Distance[i]=city.Distance[i]+CityDistance[start][end];//city.Distance[i]ÿ���������·��
        }
        city.fitness[i]=N/city.Distance[i];
        if(city.fitness[i]>city.fitness[Best])//ѡ��������Ӧֵ����ѡ�����и����е����·��
            Best=i;
    }
    copy(city.BestRooting,city.colony[Best]);//�����Ÿ��忽����city.BestRooting
    city.BestFitness=city.fitness[Best];
    city.BestValue=city.Distance[Best];
    city.BestNum=Best;
}


/****************ѡ�����ӣ����̶ķ�****************/
void Select(TSP &city)
{
	/*
	//��װ���Ա������� 
    int TempColony[POPSIZE][CITY_NUM+1];
    int i,j,t;
    double s;
    double GaiLv[POPSIZE];
    double SelectP[POPSIZE+1];
    double avg;
    double sum=0;
    for(i=0;i<POPSIZE;i++)
    {
        sum+=city.fitness[i];
    }
    for(i=0;i<POPSIZE;i++)
    {
        GaiLv[i]=city.fitness[i]/sum;
    }
    SelectP[0]=0;
    for(i=0;i<POPSIZE;i++)
    {
        SelectP[i+1]=SelectP[i]+GaiLv[i]*RAND_MAX;
    }
    memcpy(TempColony[0],city.colony[city.BestNum],sizeof(TempColony[0]));//void *memcpy(void *dest, const void *src, size_t n)��Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����n���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ����
    for(t=1;t<POPSIZE;t++)
    {
        double ran = rand() % RAND_MAX + 1;
        s= (double) ran / 100.0;
        for(i=1;i<POPSIZE;i++)
        {
            if(SelectP[i]>=s)
                break;
        }
        memcpy(TempColony[t],city.colony[i-1],sizeof(TempColony[t]));
    }
    for(i=0;i<POPSIZE;i++)
    {
        memcpy(city.colony[i],TempColony[i],sizeof(TempColony[i]));
    }
    */

	//��װƽ���������� 
    int TempColony[POPSIZE][CITY_NUM+1];
    int i,j,t;
    double s;
    double GaiLv[POPSIZE];
    double SelectP[POPSIZE+1];
    double avg;
    double sum=0;
    for(i=0;i<POPSIZE;i++)
    {
        sum+=city.fitness[i]*city.fitness[i];
    }
    for(i=0;i<POPSIZE;i++)
    {
        GaiLv[i]=city.fitness[i]*city.fitness[i]/sum;
    }
    SelectP[0]=0;
    for(i=0;i<POPSIZE;i++)
    {
        SelectP[i+1]=SelectP[i]+GaiLv[i]*RAND_MAX;
    }
    memcpy(TempColony[0],city.colony[city.BestNum],sizeof(TempColony[0]));//void *memcpy(void *dest, const void *src, size_t n)��Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����n���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ����
    for(t=1;t<POPSIZE;t++)
    {
        double ran = rand() % RAND_MAX + 1;
        s= (double) ran / 100.0;
        for(i=1;i<POPSIZE;i++)
        {
            if(SelectP[i]>=s)
                break;
        }
        memcpy(TempColony[t],city.colony[i-1],sizeof(TempColony[t]));
    }
    for(i=0;i<POPSIZE;i++)
    {
        memcpy(city.colony[i],TempColony[i],sizeof(TempColony[i]));
    }
    
}

/****************���棺ͷβ���䣬�м����˳�򽻲�****************/
void Cross(TSP &city,double pc)//���������pc
{
    int i,j,t,l;
    int a,b,ca,cb;
    int Temp1[CITY_NUM+1],Temp2[CITY_NUM+1];
    for(i=0;i<POPSIZE;i++)
    {
        double s=((double)(rand()%RAND_MAX))/RAND_MAX;
        if(s<pc)
        {
            cb=rand()%POPSIZE;
            ca=cb;
            if(ca==city.BestNum||cb==city.BestNum)//�������������ֱ�ӽ����´�ѭ��
                continue;

            l=rand()%(CITY_NUM/2)+1;  //1-19
            a=rand()%(CITY_NUM-l)+1; //1-37

            memset(Hash,0,sizeof(Hash));//void *memset(void *s, int ch, size_t n);��s�е�ǰλ�ú����n���ֽ� �� ch �滻������ s ��
            Temp1[0]=Temp1[CITY_NUM]=0;
            for(j=1;j<=l;j++)//����˳�������ѡ������ͨ��Hash���Ϊ1
            {
                Temp1[j]=city.colony[cb][a+j-1]; //a+L=2~38 20~38
                Hash[Temp1[j]]=1;
            }
            for(t=1;t<CITY_NUM;t++)
            {
                if(Hash[city.colony[ca][t]]==0)
                {
                    Temp1[j++]=city.colony[ca][t];
                    Hash[city.colony[ca][t]]=1;
                }
            }
            memcpy(city.colony[ca],Temp1,sizeof(Temp1));
        }
    }

}

/****************����****************/
double GetFittness(int a[CITY_NUM+1])
{
    int i,start,end;
    double Distance=0;
    for(i=0;i<CITY_NUM;i++)
    {
        start=a[i];   end=a[i+1];
        Distance+=CityDistance[start][end];
    }
    return N/Distance;
}
/*�Ի�����*/
void Mutation(TSP &city,double pm)//���������pm
{
	
	//����֮��Ի�
    int i,k,m;
    int Temp[CITY_NUM+1];
    for(k=0;k<POPSIZE;k++)
    {
        double s=((double)(rand()%RAND_MAX))/RAND_MAX;//�����������0~1��
        i=rand()%POPSIZE;//�������0~POPSIZE֮�����
        if(s<pm&&i!=city.BestNum)//i!=city.BestNum������֤���ŵĸ��岻����
        {
            int a,b,t;
            a=(rand()%(CITY_NUM-1))+1;
            b=(rand()%(CITY_NUM-1))+1;
            copy(Temp,city.colony[i]);
            if(a>b)//��֤��b>=a
            {
                t=a;
                a=b;
                b=t;
            }
            for(m=a;m<(a+b)/2;m++)
            {
                t=Temp[m];
                Temp[m]=Temp[a+b-m];
                Temp[a+b-m]=t;
            }

            if(GetFittness(Temp)<GetFittness(city.colony[i]))
            {
                a=(rand()%(CITY_NUM-1))+1;
                b=(rand()%(CITY_NUM-1))+1;
                //copy(Temp,city.colony[i]);
                memcpy(Temp,city.colony[i],sizeof(Temp));
                if(a>b)
                {
                    t=a;
                    a=b;
                    b=t;
                }
                for(m=a;m<(a+b)/2;m++)
                {
                    t=Temp[m];
                    Temp[m]=Temp[a+b-m];
                    Temp[a+b-m]=t;
                }

                    if(GetFittness(Temp)<GetFittness(city.colony[i]))
                    {
                        a=(rand()%(CITY_NUM-1))+1;
                        b=(rand()%(CITY_NUM-1))+1;
                        //copy(Temp,city.colony[i]);
                        memcpy(Temp,city.colony[i],sizeof(Temp));
                        if(a>b)
                        {
                            t=a;
                            a=b;
                            b=t;
                        }
                        for(m=a;m<(a+b)/2;m++)
                        {
                            t=Temp[m];
                            Temp[m]=Temp[a+b-m];
                            Temp[a+b-m]=t;
                        }
                    }

            }
            memcpy(city.colony[i],Temp,sizeof(Temp));
        }
    }
    
    /*
    //�����㻥��
    int i,k,m;
    int Temp[CITY_NUM+1];
    for(k=0;k<POPSIZE;k++)
    {
        double s=((double)(rand()%RAND_MAX))/RAND_MAX;//�����������0~1��
        i=rand()%POPSIZE;//�������0~POPSIZE֮�����
        if(s<pm&&i!=city.BestNum)//i!=city.BestNum������֤���ŵĸ��岻����
        {
            int a,b,t;
            a=(rand()%(CITY_NUM-1))+1;
            b=(rand()%(CITY_NUM-1))+1;
            copy(Temp,city.colony[i]);
            if(a>b)//��֤��b>=a
            {
                t=a;
                a=b;
                b=t;
            }

            t=Temp[a];
            Temp[a]=Temp[b];
            Temp[b]=t;
            
            if(GetFittness(Temp)<GetFittness(city.colony[i]))
            {
                a=(rand()%(CITY_NUM-1))+1;
                b=(rand()%(CITY_NUM-1))+1;
                //copy(Temp,city.colony[i]);
                memcpy(Temp,city.colony[i],sizeof(Temp));
                if(a>b)
                {
                    t=a;
                    a=b;
                    b=t;
                }
             	t=Temp[a];
            	Temp[a]=Temp[b];
            	Temp[b]=t;

                    if(GetFittness(Temp)<GetFittness(city.colony[i]))
                    {
                        a=(rand()%(CITY_NUM-1))+1;
                        b=(rand()%(CITY_NUM-1))+1;
                        //copy(Temp,city.colony[i]);
                        memcpy(Temp,city.colony[i],sizeof(Temp));
                        if(a>b)
                        {
                            t=a;
                            a=b;
                            b=t;
                        }
                        t=Temp[a];
            			Temp[a]=Temp[b];
            			Temp[b]=t;
                    }

            	}
            memcpy(city.colony[i],Temp,sizeof(Temp));
        }
    }
    */
}

void OutPut(TSP &city)
{
    int i,j;
    printf("���·��Ϊ:\n");
    for(i=0;i<=CITY_NUM;i++)
    printf("%5d",city.BestRooting[i]);
    printf("\n���·��ֵΪ��%f\n",(city.BestValue));
        printf("\n�����Ӧ��ֵΪ��%f\n",(city.BestFitness));

}


int main()
{

        TSP city;
        double pcross,pmutation;//������ʺͱ������
        int MaxEpoc;//����������
        int i;
        srand(seed);
        MaxEpoc=20000;
        pcross=0.85; pmutation=0.15;
        CalculatDist();//������о���ʵ�
        InitColony(city);//���ɳ�ʼ��Ⱥ
        CalFitness(city);//������Ӧֵ,����Ӧ���������������ѡ����

clock_t start,end;
start=clock();	
        for(i=0;i<MaxEpoc;i++)
        {

            Select(city);//ѡ��(����)�����̶ķ�
            Cross(city,pcross);//����
            Mutation(city,pmutation);//����
            CalFitness(city);//������Ӧֵ
        }
end=clock();
	double endtime=(double)(end-start)/CLOCKS_PER_SEC;
	cout<<"Total time:"<<endtime*1000<<"ms"<<endl;	//msΪ��λ

       OutPut(city);//���
    return 0;
}

