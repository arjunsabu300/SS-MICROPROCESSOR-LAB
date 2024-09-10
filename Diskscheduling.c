#include <stdlib.h>

int n,r[25],temp[25],initial,size;

void fcfs()
{
    int i,current=initial,seek=0;
    for(i=0;i<n;i++)
    {
        printf("%d->",current);
        seek+=abs(r[i]-current);
        current=r[i];
    }
    printf("%d\n",current);
    printf("FCFS Total head movement(seek time) : %d\n\n\n",seek);
}

void scan()
{
    int i,j,temp_v,current=initial,seek=0,index,k;
    printf("1.Increasing Order\n2.Decreasing Order\n");
    printf("Enter your choice : ");
    scanf("%d",&k);
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(temp[j] > temp[j+1])
            {
                temp_v=temp[j];
                temp[j]=temp[j+1];
                temp[j+1]=temp_v;
            }
        }
    }
    for(i=0;i<n;i++)
    {
        if(current < temp[i])
        {
            index=i;
            break;
        }
    }
    if(k==1)
    {
       for(i=index;i<n;i++)
       {
           seek+=abs(temp[i]-current);
           printf("%d->",current);
           current=temp[i];
       }
       printf("%d->",current);
       seek+=abs(size-1-current);
       current=size-1;
       for(i=index-1;i>=0;i--)
       {
           seek+=abs(current-temp[i]);
           printf("%d->",current);
           current=temp[i];
       }
       printf("%d",current);
    }
    else if(k==2)
    {
       for(i=index-1;i>=0;i--)
       {
          seek+=abs(current-temp[i]);
          printf("%d->",current);
          current=temp[i];
       }
       printf("%d->",current);
       seek+=abs(current);
       current=0;
       for(i=index;i<n;i++)
       {
          seek+=abs(temp[i]-current);
          printf("%d->",current);
          current=temp[i];
       }
       printf("%d",current);
    }
    printf("\n");
    printf("SCAN Total head movement(seek time) : %d\n\n\n",seek);
}

void cscan()
{
    int i,j,temp_v,current=initial,seek=0,index,k;
    printf("CSCAN TRAVERSAL\n");
    printf("1.Ascending Order\n2.Descending Order\n");
    printf("Enter your choice : ");
    scanf("%d",&k);
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(temp[j] > temp[j+1])
            {
                temp_v=temp[j];
                temp[j]=temp[j+1];
                temp[j+1]=temp_v;
            }
        }
    }
    for(i=0;i<n;i++)
    {
        if(current < temp[i])
        {
            index=i;
            break;
        }
    }
    if(k==1)
    {
       for(i=index;i<n;i++)
       {
           seek+=abs(temp[i]-current);
           printf("%d->",current);
           current=temp[i];
       }
       printf("%d->",current);
       seek+=abs(size-1-current);
       current=size-1;
    //printf("%d->",current);
       seek+=current;
       current=0;
       for(i=0;i<index;i++)
       {
           seek+=abs(temp[i]-current);
           printf("%d->",current);
           current=temp[i];
       }
    }
    else if(k==2)
    {
       for(i=index-1;i>=0;i--)
       {
          seek+=abs(current-temp[i]);
          printf("%d->",current);
          current=temp[i];
       }
       printf("%d->",current);
       seek+=abs(current);
       current=0;
       printf("%d->",current);
       seek+=abs(size-1-current);
       current=size-1;
       for(i=n-1;i>=index;i--)
       {
          seek+=abs(current-temp[i]);
          printf("%d->",current);
          current=temp[i];
       }
    }
    printf("%d\n",current);
    printf("CSCAN Total head movement(seek time) : %d\n\n\n",seek);
}

int main()
{
    int i;
    printf("How many requests ? : ");
    scanf("%d",&n);
    printf("Enter the request sequence : ");
    for(i=0;i<n;i++)
    {
        scanf("%d",&r[i]);
    }
    for(i=0;i<n;i++)
    {
        temp[i]=r[i];
    }
    printf("Enter the total disk size : ");
    scanf("%d",&size);
    printf("Enter the initial head position : ");
    scanf("%d",&initial);
    int c=1;
    while(c)
    {
       int a;
       printf("1.FCFS\n2.SCAN\n3.CSCAN\n4.Exit\n");
       printf("Enter your choice : ");
       scanf("%d",&a);
       switch(a)
       {
          case 1 : fcfs();
                   break;
          case 2 : scan();
                   break;
          case 3 : cscan();
                   break;
          case 4 : c=0;
       }
    }
}
