#include<stdio.h>
#include<conio.h>
typedef struct node_type
{
	int result;
	int  inp[5];
}node;

node no[501];
char s[15];

void assign(int i,char*s)
{
	if(s[0]=='R')
	{
		no[i].result=1;
	}
	else
	{
		no[i].result=-1;
	}
	
	no[i].inp[0]=s[2]-'0';
	no[i].inp[1]=s[4]-'0';
	no[i].inp[2]=s[6]-'0';
	no[i].inp[3]=s[8]-'0';
	no[i].inp[4]=1;
}


int main()
{
	FILE *fp,*op;
	fp=fopen("training.txt","r");
	int count,flag=0,i,k,max_count;
	i=0;
	while(fgets(s,1000,fp)!=NULL)
	{
		assign(i,s);
		i++;
	}
	fclose(fp);
	printf("reading form file is completed\n");
	
	
	int input[5];
	int output;
	int wm[5]={0,0,0,0,0};
	int n=1,value;
	int temp_count=0;
	max_count=-1;
	printf("processing........\n");
	count=0;
	i=0;
	while(count<450)
	{
		if(temp_count>1000)
		{
			printf("more than 1000 loops are completed but still it didnt converge\n");
			printf("here max count is %d\n",max_count);
			getch();
			temp_count=0;
		}
		printf("input vector is :\n");
		for(k=0;k<5;k++)
		{
			input[k]=no[i].inp[k];
			printf("%d ",input[k]);
		}
		
		printf("\n");
		getch();
		value=0;
		printf("the weight matrix is :\n");
		for(k=0;k<5;k++)
		{
			printf("%d ",wm[k]);
			value=value+input[k]*wm[k];
		}
		printf("\n");
		getch();
		printf("the value after multiplying is %d\n",value);
		getch();
		if(value>0)
		{
			value=1;
		}
		else
		{
			value=-1;
		}
		printf("the result expected is %d\n",no[i].result);
		getch();
		
		if(no[i].result==value)
		{
			
			count++;
		}
		else
		{
			count=0;
			if(value==-1)
			{
				for(k=0;k<5;k++)
				{
					wm[k]=wm[k]+input[k];
				}
			}
			else
			{
				for(k=0;k<5;k++)
				{
					wm[k]=wm[k]-input[k];
				}
			}
		}
		
		
		
		if(count>max_count)
		{
			max_count=count;
		}
		i++;
		if(i>=450)
		{
			
			i=i%450;
			temp_count++;
		}
	}
	printf("the converging weight vector is :");
	for(k=0;k<5;k++)
	{
		printf("%d ",wm[k]);
	}
	printf("\n");
	return 0;
}
