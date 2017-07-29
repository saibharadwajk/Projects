#include<stdio.h>
int main()
{
	FILE *fp,*op;
	fp=fopen("testing.txt","r");
	//op=fopen("training.txt","w");
	
	int count=0;
	char s[20];
	while(fgets(s,1000,fp)!=NULL)
	{
		//if(s[0]!='B')
		//{
		//	fprintf(op,"%s",s);
	//	}
		count++;
		//if(count==0)
		//break;
	}
	/*while(fgets(s,100,fp)!=NULL)
	{
		fprintf(op,"%s",s);
	}*/
	printf("the count is %d\n",count);
	fclose(fp);
	//fclose(op);
	return 0;
}
