#include<stdio.h>
int main()
{
	FILE *fp;
	fp=fopen("testing.txt","r");

	int input[3],i,j,result,expected,wm[3]={1,1,0},count,wrongs;
	char s[20];
	count=1;
	wrongs=0;
	while(fgets(s,1000,fp)!=NULL)
	{
		//printf("entering\n");
		if(s[0]=='R')
		{
			expected=1;
		}
		else
		{
			expected=-1;
		}

		input[0]=s[2]-'0';
		input[0]=input[0]*(s[4]-'0');
		input[1]=s[6]-'0';
		input[1]=input[1]*(s[8]-'0');
		input[2]=1;
		result=0;
		for(j=0;j<3;j++)
		{
			result=result+input[j]*wm[j];
		}
		if(result>0)
		{
			result=1;
		}
		else
		{
			result=-1;
		}
		if(result!=expected)
		{
			//printf("your expected reslut is not correct for item %d\n",count);
			printf("s is %s\n",s);
			for(j=0;j<3;j++)
			{
				printf("%d ",input[j]);
			}
			printf("\n");
			printf("expected result is %d\n",expected);
			printf("the result is %d\n",result);
			//getch();
			wrongs++;
		}
		count++;
	}
	printf("wrongs=%d\n",wrongs);
	fclose(fp);
	return 0;
}
