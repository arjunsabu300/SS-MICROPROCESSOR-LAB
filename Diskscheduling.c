#include <stdio.h>
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

void sstf()
{
    int i,j,min_index,current=initial,seek=0,min_distance;
    int visited[n];
    for(i=0;i<n;i++)
    {
        visited[i] = 0;
    }
    for(i=0;i<n;i++)
    {
        min_distance=1000000;
        for(j=0;j<n;j++)
        {
            if(visited[j]==0 && abs(current - r[j]) < min_distance)
            {
                min_distance=abs(current - r[j]);
                min_index=j;
            }
        }
        visited[min_index]=1;
        seek+=min_distance;
        printf("%d->",current);
        current=r[min_index];
    }
    printf("%d\n",current);

    printf("SSTF Total head movement(seek time) : %d\n\n\n\n", seek);
}

void scan()
{
    int i,j,temp_v,current=initial,seek=0,index;
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
    printf("%d\n",current);
    printf("SCAN Total head movement(seek time) : %d\n\n\n",seek);
}

void cscan()
{
    int i,j,temp_v,current=initial,seek=0,index;
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
    for(i=index;i<n;i++)
    {
        seek+=abs(temp[i]-current);
        printf("%d->",current);
        current=temp[i];
    }
    printf("%d->",current);
    seek+=abs(size-1-current);
    current=size-1;
    printf("%d->",current);
    seek+=current;
    current=0;
    for(i=0;i<index;i++)
    {
        seek+=abs(temp[i]-current);
        printf("%d->",current);
        current=temp[i];
    }
    printf("%d\n",current);
    printf("CSCAN Total head movement(seek time) : %d\n\n\n",seek);
}

void look()
{
    int i,j,temp_v,current=initial,seek=0,index;
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
    for(i=index;i<n;i++)
    {
        seek+=abs(temp[i]-current);
        printf("%d->",current);
        current=temp[i];
    }
    //printf("%d->",current);
    for(i=index-1;i>=0;i--)
    {
        seek+=abs(current-temp[i]);
        printf("%d->",current);
        current=temp[i];
    }
    printf("%d\n",current);
    printf("LOOK Total head movement(seek time) : %d\n\n\n",seek);
}

void clook()
{
    int i,j,temp_v,current=initial,seek=0,index;
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
    for(i=index;i<n;i++)
    {
        seek+=abs(temp[i]-current);
        printf("%d->",current);
        current=temp[i];
    }
    printf("%d->",current);
    seek+=current-temp[0];
    current=temp[0];
    for(i=1;i<index;i++)
    {
        seek+=abs(current-temp[i]);
        printf("%d->",current);
        current=temp[i];
    }
    printf("%d\n",current);
    printf("CLOOK Total head movement(seek time) : %d\n\n\n",seek);
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
    fcfs();
    scan();
    cscan();
    look();
    clook();
    sstf();
}






