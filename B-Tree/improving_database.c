#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#define MAX_KEYS 2
#define MAX_RECS 3

//*******************************************************************************************************************************

typedef struct keynode_tag{
    int active_keys;
    int key[MAX_KEYS+1];
    union ptr_tag{
        struct keynode_tag *ptr[MAX_KEYS+2];
        struct datanode_tag *dptr[MAX_RECS+2];
    }ptr_type;
    int u_type;
    struct keynode_tag  *parent_ptr;
}keynode_type;

//***************************************************************************************************************************

typedef struct book_record_tag{
    int lib_ACC_num;
    char b_name[20];
    char a_name[20];
    int ISBN_num;
    char pub_name[20];
    float b_price;
}book_record_type;

//***************************************************************************************************************************

typedef struct datanode_tag{
    int active_rec;
    keynode_type *parent_ptr;
    book_record_type records[MAX_RECS+1];
    struct datanode_tag *next,*prev;

}datanode_type;

//***************************************************************************************************************************

typedef enum{FAILURE,SUCCESS} status_code;

typedef struct database_tag
{
    keynode_type *root;
}database;

//*************************************************************************************************************************

//                                          Function Declarations
//**********************************************************************************************************************

void insert_data(database *dptr,book_record_type record);
keynode_type *make_keynode(int u_type);
datanode_type *make_datanode();
status_code insert(database *dptr);
int insert_rec(datanode_type *ptr,book_record_type record);
void insert_key(keynode_type *ptr,int key);
int lsearch(int a[],int n,int k);
void print_database(database *);
int Height(database *);
int getNumRecords(database *);
//                                               Main Function
//**********************************************************************************************************************

main()
{
    int i=0,height,flag=0,option,num_rec;
    status_code sc;
    database *dptr;
    database d;
    dptr=&d;
    dptr->root=NULL;
    printf("\t\tWELCOME TO THE DATABASE OF LIBRARY RECORDS");
    printf("\nDATABASE MENU:\n");
    printf("1).INSERT A RECORD \n2).DELETE A RECORD\n4).GET NUMBER OF RECORDS\n");
    printf("5).HEIGHT OF THE TREE\n6).RANGE SEARCH\n7).PRINT DATABASE\n8).EXIT");
    printf("\nCHOOSE AN OPTION : ");
    scanf("%d",&option);

    while(flag!=1)
    {
        switch(option)
        {
            case 1:
                system("cls");
                printf("\nENTER DATA... ");
                sc=insert(dptr);
                printf("\n\nPress Enter key to continue...");
                getch();
                system("cls");
                break;

            case 2:

                    break;

            case 3:
                system("cls");

                  break;

            case 4:
                system("cls");
                num_rec=getNumRecords(dptr);
                if(num_rec!=0)
                {
                    printf("\nNUMBER OF ACTIVE RECORDS ARE %d ",num_rec);
                }
                else
                {
                    printf("\nOOPS!! DATABASE IS EMPTY.... ");
                }
                printf("\n\nPress Enter key to continue...");
                getch();
                break;

             case 5:
                  height=Height(dptr);
                  if(height==0)
                  {
                      printf("\nOOPS!! DATABASE IS EMPTY.... ");
                  }
                  else{
                      printf("\nHEIGHT IS %d ",height);
                  }
                  break;
             case 6:
                 system("cls");
                    break;

             case 7:
                    system("cls");
                    print_database(dptr);
                    break;
             case 8:
                    flag=1;
                    break;
            default:
                printf("\nCHOOSE A VALID OPTION");
                break;
        }
        if(flag!=1)
        {
            printf("\n\n\t\tWELCOME TO THE MAIN MAENU");
            printf("\n1).INSERT A RECORD \n2).DELETE A RECORD\n4).GET NUMBER OF RECORDS\n");
            printf("5).HEIGHT OF THE TREE\n6).RANGE SEARCH\n7).PRINT DATABASE\n8).EXIT");
            printf("\n\nCHOOSE AN OPTION : ");
            scanf("%d",&option);
        }
    }
    getch();
}

//                                              Insert function
//****************************************************************************************************************************

status_code insert(database *dptr)
{
    book_record_type record;
    printf("\nEnter Library Access Number : ");
    scanf("%d",&record.lib_ACC_num);
    printf("\nEnter Book Name : ");
    scanf("%s",record.b_name);
    insert_data(dptr,record);
}

//                                                 Insert Data Function
//***************************************************************************************************************************

void insert_data(database *dptr,book_record_type record)
{
    keynode_type *ptr,*curr,*ptr1;
    datanode_type *dcurr,*dcurr1;
    int current,active_rec,pos,i,j,done=0,key,mid,status;
    static int first=0;
    // if tree is empty
    if(dptr->root==NULL){
        ptr= make_keynode(1);
        ptr->active_keys = 1;
        ptr->key[0]=record.lib_ACC_num;
        ptr->ptr_type.dptr[0]=make_datanode();
        ptr->ptr_type.dptr[0]->records[0]=record;
        ptr->ptr_type.dptr[0]->active_rec=1;
        ptr->ptr_type.dptr[0]->parent_ptr=ptr;
        ptr->ptr_type.dptr[0]->next=NULL;
        ptr->ptr_type.dptr[0]->prev=NULL;
        dptr->root = ptr;
    }
    // if tree has atleast a single node
    else
    {
        curr = dptr->root;
        printf("my root address - %u and my u_type is %d",curr,curr->u_type);
        key = record.lib_ACC_num;
        while(curr->u_type!=1)
        {
            pos = lsearch(curr->key,curr->active_keys,key);
            curr = curr->ptr_type.ptr[pos];
        }
        pos = lsearch(curr->key,curr->active_keys,key);
        dcurr = curr->ptr_type.dptr[pos];
        // if a node has less than minimum number of records

        if(dcurr==NULL) // if at the start the position to be inserted is null
        {
            printf("\nI am in dcurr =NULL");
            dcurr=curr->ptr_type.dptr[pos-1];
            if(dcurr->active_rec < MAX_RECS)
            {
                dcurr->records[dcurr->active_rec]=record;
                dcurr->active_rec+=1;
                 current = dcurr->records[dcurr->active_rec -1].lib_ACC_num;
                curr->key[pos-1]=current;
                printf("\n now root is %d",curr->key[pos-1]);
                 printf("\tactive records %d ",dcurr->active_rec);
            }
            else
            {
                active_rec = dcurr->active_rec;
                dcurr->active_rec = (active_rec+1)/2;
                dcurr1=make_datanode();
                for(j=0,i=dcurr->active_rec;i<active_rec;i++,j++)
                {
                    dcurr1->records[j]=dcurr->records[i];
                    dcurr1->active_rec +=1;
                }
                dcurr1->parent_ptr=dcurr->parent_ptr;
                dcurr->next=dcurr1;
                dcurr1->prev=dcurr;
                dcurr1->next=NULL;
                dcurr1->records[j]=record;
                dcurr1->active_rec +=1;
                current = dcurr->records[dcurr->active_rec -1].lib_ACC_num;
                curr->key[pos-1]=current;
                first=1;
                curr->ptr_type.dptr[pos]=dcurr1;
                printf("\n now root is %d ",curr->key[pos-1]);
                printf("\tactive records %d ",dcurr1->active_rec);
            }
        }
        // else
        else //dcurr!=NULL
        {
           status= insert_rec(dcurr,record);
            // if it crosses the limit
            if(status==1)
            {
                printf("\nrecord is updated ");
            }
            else
            {
                if(dcurr->active_rec > MAX_RECS) // if the datanode is full
                {
                    printf("\tI entered activerec>max");
                    dcurr1 = make_datanode();
                    dcurr1->parent_ptr=dcurr->parent_ptr;
                    active_rec = dcurr->active_rec;
                    for(j=0,i=(MAX_RECS+1)/2;i<active_rec;i++,j++)
                    {
                        dcurr1->records[j]=dcurr->records[i];
                        dcurr1->active_rec+=1;
                    }
                    dcurr1->prev=dcurr;
                    dcurr1->next=dcurr->next;
                    dcurr->next=dcurr1;
                    if(dcurr1->next!=NULL)
                    {
                        (dcurr1->next)->prev= dcurr1;
                    }
                    printf("\t I have copied all data to the new node");
                    dcurr->active_rec=j;
                    printf("\nnow dcurr is having %d ",j);
                    current = dcurr->records[dcurr->active_rec-1].lib_ACC_num;
                    curr=dcurr->parent_ptr;
                    if(first!=0)
                    {
                        printf("\nnow key from %u is inserted to %u",dcurr,curr);
                        insert_key(curr,current);
                        if(curr->active_keys<=MAX_KEYS) // if parent is having satisfying max rec cond after inserting
                        {
                            for(i=curr->active_keys;i>pos+1;i--)
                            {

                                curr->ptr_type.dptr[i]=curr->ptr_type.dptr[i-1];
                            }
                            curr->ptr_type.dptr[i]=dcurr1;
                            dcurr1->parent_ptr=curr;
                        }
                        else // if it is of excess keys
                        {
                            ptr=make_keynode(1);
                            mid=(curr->active_keys)/2;
                            current = curr->key[mid];
                            // copying keys to new node
                            for(j=0,i=mid+1;i<curr->active_keys;i++,j++)
                            {
                                ptr->key[j]=curr->key[i];
                                ptr->active_keys +=1;
                            }
                            // inserting new pointer
                            for(i=curr->active_keys;i>pos+1;i--)
                            {
                                curr->ptr_type.dptr[i]=curr->ptr_type.dptr[i-1];
                            }
                            curr->ptr_type.dptr[i]=dcurr1;
                            /*if(i<=mid)
                            {
                                dcurr1->parent_ptr=curr;
                            }
                            else
                            {
                                dcurr->parent_ptr = ptr;
                            }*/
                            // copying pointers to new node
                            for(j=0,i=mid+1;i<=curr->active_keys;i++,j++)
                            {
                                ptr->ptr_type.dptr[j]=curr->ptr_type.dptr[i];
                                ptr->ptr_type.dptr[j]->parent_ptr=ptr;
                            }
                            curr->active_keys=mid;
                            ptr->parent_ptr=curr->parent_ptr;
                            done=0;
                            while(done!=1)
                            {

                                if(curr->parent_ptr!=NULL)
                                {
                                    printf("\nnow key from %u is inserted to",curr);
                                    curr=curr->parent_ptr;
                                    printf(" %u",curr);
                                    insert_key(curr,current);
                                    pos = lsearch(curr->key,curr->active_keys,ptr->key[0]);
                                    if(curr->active_keys<=MAX_KEYS) // if it satisfies max keys cond after inserting
                                    {
                                        for(i=curr->active_keys;i>pos;i--) // inserting new ptr in the parent
                                        {

                                            curr->ptr_type.ptr[i]=curr->ptr_type.ptr[i-1];
                                        }
                                        curr->ptr_type.ptr[i]=ptr;
                                        ptr->parent_ptr=curr;
                                        done=1;
                                    }
                                    else
                                    {
                                        ptr1=make_keynode(0);
                                        mid=(curr->active_keys)/2;
                                        ptr1->parent_ptr=curr->parent_ptr;
                                        current = curr->key[mid];
                                        // copying keys to new node
                                        for(j=0,i=mid+1;i<curr->active_keys;i++,j++)
                                        {
                                            ptr1->key[j]=curr->key[i];
                                            ptr1->active_keys +=1;
                                        }
                                        // inserting new pointer
                                        for(i=curr->active_keys;i>pos;i--)
                                        {
                                            curr->ptr_type.dptr[i]=curr->ptr_type.dptr[i-1];
                                        }
                                        curr->ptr_type.ptr[i]=ptr;
                                        for(j=0,i=mid+1;i<=curr->active_keys;i++,j++)// copying pointers to new node
                                        {
                                            ptr1->ptr_type.ptr[j]=curr->ptr_type.ptr[i];
                                            ptr1->ptr_type.ptr[j]->parent_ptr=ptr1;
                                        }
                                        curr->active_keys=mid;
                                        ptr=ptr1;
                                    }
                                }
                                else //if curr->parent_ptr==NULL
                                {
                                    ptr1=make_keynode(0);
                                    ptr1->active_keys=1;
                                    ptr1->parent_ptr=NULL;
                                    ptr1->key[0]=current;
                                    ptr1->ptr_type.ptr[0]=curr;
                                    ptr1->ptr_type.ptr[1]=ptr;
                                    curr->parent_ptr=ptr1;
                                    ptr->parent_ptr=ptr1;
                                    done=1;
                                    dptr->root=ptr1;
                                    printf("\nnow root is %u and value is %d ",ptr1,ptr1->key[0]);
                                }
                            }

                        }
                    }
                    else{ // if it is the first datanode spliting
                        curr->key[0]=dcurr->records[dcurr->active_rec-1].lib_ACC_num;
                        curr->ptr_type.dptr[1]=dcurr1;
                        dcurr1->parent_ptr=curr;
                        printf("\n now the root is %d ",curr->key[0]);
                        first =1;
                    }
                }
            }
        }
    }
}

//                                              Make KeyNode Function
//***************************************************************************************************************************

keynode_type *make_keynode(int u_type)
{
    keynode_type *ptr;
    int i;
    ptr = (keynode_type *)malloc(sizeof(keynode_type));
    ptr->u_type=u_type;
    ptr->active_keys=0;
    if(u_type==1)
    {
        for(i=0;i<=MAX_KEYS;i++)
        {
            ptr->ptr_type.dptr[i]=NULL;
        }
    }
    else{
        for(i=0;i<=MAX_KEYS;i++)
        {
            ptr->ptr_type.ptr[i]=NULL;
        }
    }
    ptr->parent_ptr=NULL;
}

//                                              Make DataNode Function
//*************************************************************************************************************************

datanode_type *make_datanode()
{

    datanode_type *ptr;
    ptr=(datanode_type *)malloc(sizeof(datanode_type));
    ptr->active_rec=0;
    ptr->parent_ptr=NULL;
    ptr->next=NULL;
    ptr->prev=NULL;
}

//                                              Insert Key Function
//******************************************************************************************************************************

void insert_key(keynode_type *ptr,int key)
{
    int i,j;
    i=ptr->active_keys;
    printf("\nI entered insert key %d and checking with (",key);
    for(j=0;j<ptr->active_keys;j++)
    {
        printf("  %d",ptr->key[j]);
    }
    printf(" )");
    while(key<ptr->key[i-1]&&i>0)
    {
        printf("   %d",ptr->key[i-1]);
       ptr->key[i]=ptr->key[i-1];
       i--;
    }
    printf(" and ins at %d",i);
    ptr->key[i]=key;
    ptr->active_keys+=1;
}
//                                                  Insert Record Function
//*****************************************************************************************************************************

int insert_rec(datanode_type *ptr,book_record_type record)
{
    int key,i;
    key = record.lib_ACC_num;
    i=ptr->active_rec;
    printf("number of active records is %d",i);
     printf("\nI entered insert rec");
     printf("\nI am going to check %d with %d",key,ptr->records[i-1].lib_ACC_num);
    while((key <= ptr->records[i-1].lib_ACC_num) && i>0 )
    {
        printf("\n I enterd while ");
        if(key!=ptr->records[i-1].lib_ACC_num)
        {
            ptr->records[i]=ptr->records[i-1];
        }
        i--;
    }
    if(key==ptr->records[i].lib_ACC_num)
    {
        ptr->records[i]=record;
        return 1;
    }
    else{
        ptr->records[i]=record;
        ptr->active_rec+=1;
        printf("I inserted record at pos %d",i);
        return 0;
    }
}

//                                              Linear Search Function
//***************************************************************************************************************************

int lsearch(int a[],int n,int k)
{
    int i=0,count=0;
    while((i<n)&&(a[i]<k))
    {
        count++;
        i++;
    }
    printf("I am lsearch returning %d",count);
    return count;

}

//                                              Print DataBase Function
//=============================================================================================================================

void print_database(database *dptr)
{
    keynode_type *kptr;
    datanode_type *ptr;
    int i;
    if(dptr->root==NULL)
    {
        printf("\n******************************DATABASE IS EMPTY*******************************");
    }
    else
    {
        kptr=dptr->root;
        while(kptr->u_type!=1)
        {
            kptr=kptr->ptr_type.ptr[0];
        }
        ptr=kptr->ptr_type.dptr[0];
        printf("\n***********************************************************************************");
        while(ptr!=NULL)
        {
            i=0;
            while(i<ptr->active_rec)
            {
                printf("\nACCESS NUMBER : %d",ptr->records[i].lib_ACC_num);
                printf("\nBOOK NAME : %s ",ptr->records[i].b_name);
                printf("\n********************************************************************************");
                i++;
            }
            ptr=ptr->next;
        }
    }
}

int Height(database *dptr)
{
    keynode_type *ptr;
    int count=0;
    if(dptr->root!=NULL)
    {
        ptr=dptr->root;
        while(ptr->u_type!=1)
        {
            ptr=ptr->ptr_type.ptr[0];
            count++;
        }
        count++;
    }
    return count;
}


int getNumRecords(database* dptr)
{
    keynode_type *kptr;
    datanode_type *ptr;
    int count=0;
    if(dptr->root!=NULL)
    {
        kptr=dptr->root;
        while(kptr->u_type!=1)
        {
            kptr=kptr->ptr_type.ptr[0];
        }
        ptr = kptr->ptr_type.dptr[0];
        count=0;
        while(ptr!=NULL)
        {
            count=count+ptr->active_rec;
            ptr=ptr->next;
        }
    }
    return count;

}
