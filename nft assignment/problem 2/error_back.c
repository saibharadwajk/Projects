#include<stdio.h>
#include<math.h>
#define opN 3
#define hiddenN 25
#define inputN 100
#define lambda 0.24

float N=0.56;
float input[inputN];
float H_output[hiddenN];
float output[opN];
float desired[opN];
float error[opN];
float wm1[hiddenN][inputN];
float wm2[opN][hiddenN];
float Hidden_vj[hiddenN];
float output_vj[opN];
float delta_hidden[hiddenN];
float delta_output[opN];
// activation function

void activation_Func(float a[],float b[],int n)
{
    int i=0;
    for(i=0; i<n; i++)
    {
        a[i]=(1/(1+expf(-1*lambda*b[i])));
    }
}

// derivative of activation function

void derivative_activation_func(float a[],int n)
{
    int i=0;
    float val,p;
    for(i=0; i<n; i++)
    {
        val=expf(-1*lambda*a[i]);
        p=(1+val)*(1+val);
        a[i]=(lambda*val)/p;
    }
}

// first multiplication

void firstMul(float wm1[][100],float input[],float Hidden_vj[])
{
    int i,j,k;
    for(k=0;k<hiddenN;k++)
    {
        Hidden_vj[k]=0.0;
        for(i=0; i<inputN; i++)
        {
            Hidden_vj[k]+=input[i]*wm1[k][i];
        }
    }
}

// second multiplication

void secondMul(float wm2[][25],float H_output[],float output_vj[])
{
    int i,j,k;
    for(k=0; k<opN; k++)
    {
        output_vj[k]=0;
        for(i=0; i<hiddenN; i++)
        {
            output_vj[k]+=H_output[i]*wm2[k][i];
        }
    }
}

void printWm(float a[][inputN],float b[][hiddenN])
{
    int i=0,j=0;
    printf("\n------------------\n");
    for(i=0; i<opN; i++)
    {
        for(j=0; j<hiddenN; j++)
        {
            printf("%f ",b[i][j]);
        }
        printf("\n");
    }
    printf("\n------------------\n");
    getch();
    printf("\n------------------\n");
    for(i=0; i<hiddenN; i++)
    {
        for(j=0; j<inputN; j++)
        {
            printf("%f ",a[i][j]);
        }
        printf("\n");
    }
    printf("\n------------------\n");
    getch();
}

int main()
{
    int i,j,flag,pos,count,t=10000;
    FILE *fp,*fp2;
    float sum=0,max;
    int res[3]= {1,2,9};
    char s[20];

    // Initialize

    for(i=0; i<hiddenN; i++)
    {
        for(j=0; j<inputN; j++)
        {
            wm1[i][j]=(((float)rand())/10000.0)/10000.0;
        }
    }
    for(i=0; i<opN; i++)
    {
        for(j=0; j<hiddenN; j++)
        {
            wm2[i][j]=(((float)rand())/10000.0)/10000.0;
        }
    }
    printf("\n--------------------------------------------------------\n");
    printf("\n\n \t\t\tTRAINING \n\n");
    printf("\n--------------------------------------------------------\n\n");

    //---------------Training-------------------------
    while(t--)
    {
        fp=fopen("training2.txt","r");
        if(fp!=NULL)
        {
            flag=1;
            while(flag==1)
            {
                count=0;
                for(i=0; i<10; i++)
                {
                    if(fscanf(fp,"%s",s)!=1)
                    {
                        flag=0;
                        break;

                    }
                    for(j=0; j<10; j++)
                    {

                        if(s[j]=='1')
                        {
                            input[count]=1;
                        }
                        else
                        {
                            input[count]=-1;
                        }
                        count++;

                    }
                }
                if(flag==1)
                {

                    fscanf(fp,"%s",s);
                    //    printf("the expected output is :\n");

                    for(i=0; i<opN; i++)
                    {
                        desired[i]=s[i]-'0';
                    }

                    firstMul(wm1,input,Hidden_vj);
                    activation_Func(H_output,Hidden_vj,hiddenN);

                    secondMul(wm2,H_output,output_vj);
                    activation_Func(output,output_vj,opN);

                    // calculate error
                    for(i=0; i<opN; i++)
                    {
                        error[i]=desired[i]-output[i];
                    }

                    /** Modify Output Weights**/

                    derivative_activation_func(output_vj,opN);

                    for(i=0; i<opN; i++)
                    {

                        delta_output[i]=error[i]*output_vj[i];

                        for(j=0; j<hiddenN; j++)
                        {

                            wm2[i][j]+=N*delta_output[i]*H_output[j];
                        }
                    }

                    /** Modify Hidden Weights **/

                    derivative_activation_func(Hidden_vj,hiddenN);

                    for(i=0; i<hiddenN; i++)
                    {
                        sum=0;
                        for(j=0; j<opN; j++)
                        {
                            sum+=delta_output[j]*wm2[j][i];
                        }
                        delta_hidden[i]=Hidden_vj[i]*sum;
                    }

                    for(i=0; i<hiddenN; i++)
                    {

                        for(j=0; j<inputN; j++)
                        {
                            wm1[i][j]+=N*delta_hidden[i]*input[j];
                        }
                    }

                }
            }

        }
        else
        {
            printf("no input file\n");
        }
        fclose(fp);
    }
    /** Testing Part **/
    printf("\n\t\t\tTESTING STARTS\t\t\n\n");
    printf("\n--------------------------------------------------------\n\n");

    getch();
    fp2=fopen("testing.txt","r");
    if(fp2!=NULL)
    {
        flag=1;
        while(flag==1)
        {
            count=0;
            for(i=0; i<10; i++)
            {
                if(fscanf(fp,"%s",s)!=1)
                {
                    flag=0;
                    break;

                }
                for(j=0; j<10; j++)
                {

                    if(s[j]=='1')
                    {
                        input[count]=1;
                    }
                    else
                    {
                        input[count]=-1;
                    }
                    count++;
                    printf("%c",s[j]);
                }
                printf("\n");
            }
            getch();
            printf("\n--------------------------------------------------------\n");

            firstMul(wm1,input,Hidden_vj);
            activation_Func(H_output,Hidden_vj,hiddenN);

            secondMul(wm2,H_output,output_vj);
            activation_Func(output,output_vj,opN);

            // calculate error
            printf("\noutput  :   \n\n");
            max=output[0];
            pos=0;
            for(i=0; i<opN; i++)
            {
                printf("%f\t",output[i]);
                if(max<output[i])
                {
                    max=output[i];
                    pos=i;
                }
            }
            printf("\n\n Given input pattern  is %d \n\n",res[pos]);
            printf("\n-----------------------------------\n\n\n");
        }

    }
    else
    {
        printf("no input file\n");
    }
    fclose(fp);

    return 0;
}
