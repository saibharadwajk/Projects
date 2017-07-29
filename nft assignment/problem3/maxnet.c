#include<stdio.h>

void multiply(int a[][102],int x[],float y[])
{
	FILE* fp;

	int i,j,k,count;
	fp=fopen("check.txt","w");
	for(i=0;i<100;i++)
	{
		if(a[2][i]==1)
		fprintf(fp,"%c ",'1');
		else
		fprintf(fp,"%c ",'.');
		if((i+1)%10==0)
		{
			fprintf(fp,"\n");
		}
	}
	fprintf(fp,"\n");
	for(i=0;i<100;i++)
	{
		if(x[i]==1)
		fprintf(fp,"%c ",'1');
		else
		fprintf(fp,"%c ",'.');
		if((i+1)%10==0)
		{
			fprintf(fp,"\n");
		}
	}
	fprintf(fp,"\n");
	fclose(fp);
	for(i=0;i<3;i++)
	{
		y[i]=10;
		for(j=0;j<100;j++)
		{
			y[i]=y[i]+a[i][j]*x[j];
		}
		y[i]=y[i]/2.0;
	}
}
void maxnet(float y[])
{

	float a[3][3];
	float temp[3];
	int valid[3]={1,1,1};
	int i,j;
	printf("------------\n");
	for(i=0;i<3;i++)
	{
		printf("%f\n",y[i]);
	}
	printf("-------------\n");
	float E=0.3;
	int count=0;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(i==j)
			{
				a[i][j]=1;
			}
			else
			{
				a[i][j]=0-E;
			}
		}
	}
	while(count<2)
	{
		for(i=0;i<3;i++)
		{
			temp[i]=0;
			for(j=0;j<3;j++)
			{
				temp[i]=temp[i]+a[i][j]*y[j];
			}
			if(temp[i]<=0)
			{
				temp[i]=0;
				if(valid[i]==1){
                    count++;
                    valid[i]=0;
				}
			}
		}
		for(i=0;i<3;i++)
		{
			y[i]=temp[i];
			//printf("%f\t",y[i]);
		}
		//printf("count=%d\n",count);
	//	getch();

	}
	for(i=0;i<3;i++){
        printf("%f\t",y[i]);
	}
	printf("\n");
}

int main()
{
	FILE* fp;
	fp=fopen("data.txt","r");
	char s[200];
	int out[3]={1,2,9};
	int i,j,k,count;
	int a[3][102];
	float y[3];
	int x[102];
	for(i=0;i<3;i++)
	{
		count=0;
		for(j=0;j<10;j++)
		{
			fscanf(fp,"%s",s);
			for(k=0;k<10;k++)
			{
				if(s[k]=='.')
				{
					a[i][count]=-1;
				}
				else
				{
					a[i][count]=1;
				}
				count++;
			}
		}
		fscanf(fp,"%s",s);
	}
	printf("scanning from data file is completed\n");
	fclose(fp);
	fp=fopen("input.txt","r");
	k=0;
	for(i=0;i<10;i++)
	{
		fscanf(fp,"%s",s);
		for(j=0;j<10;j++)
		{
			if(s[j]=='.')
			{
				x[k]=-1;
			}
			else
			{
				x[k]=1;
			}
			k++;
		}
	}
	printf("scanning form input file is completed\n");
	multiply(a,x,y);
	printf("multiplications is completed\n");
	maxnet(y);
	printf("maxnet is completed\n");
	for(i=0;i<3;i++)
	{
		if(y[i]!=0)
		{
			printf("THE OUTPUT IS %d",out[i]);
			break;
		}
	}
	fclose(fp);
	return 0;
}
