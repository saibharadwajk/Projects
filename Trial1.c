#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#define MAX_KEYS 2
#define MAX_RECS 3
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

typedef struct book_record_tag{
    int lib_ACC_num;
    char b_name[20];
    char a_name[20];
    int ISBN_num;
    char pub_name[20];
    float b_price;
}book_record_type;

typedef struct datanode_tag{
    int active_rec;
    keynode_type *parent_ptr;
    book_record_type records[MAX_RECS+1];
    struct datanode_tag *next,*prev;

}datanode_type;

typedef enum{FAILURE,SUCCESS} status_code;

typedef struct database_tag
{
    keynode_type *root;
}database;


// Function Declarations

void insert_data(database *dptr,book_record_type record);
keynode_type *make_keynode(int u_type);
datanode_type *make_datanode();
status_code insert(database *dptr);
void insert_rec(datanode_type *ptr,book_record_type record);
void insert_key(keynode_type *ptr,int key);
int lsearch(int a[],int n,int k);

main()
{
    int i=0;
    status_code sc;
    database *dptr;
    database d;
    dptr=&d;
    dptr->root=NULL;
    while(i!=1)
    {
        printf("\nEnter data... ");
        sc=insert(dptr);
        printf("\npress 1 to exit ");
        scanf("%d",&i);
    }
    getch();
}

status_code insert(database *dptr)
{
    book_record_type record;
    printf("\nEnter Library Access Number : ");
    scanf("%d",&record.lib_ACC_num);
    printf("\nEnter Book Name : ");
    scanf("%s",record.b_name);
    insert_data(dptr,record);
}

void insert_data(database *dptr,book_record_type record)
{
    keynode_type *ptr,*curr,*ptr1;
    datanode_type *dcurr,*dcurr1;
    int current,active_rec,pos,i,j,done=0,key,mid;
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

        if(dcurr==NULL)
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
                for(j=0,i=dcurr->active_rec;i<active_rec;i++,j++){
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
            insert_rec(dcurr,record);
            // if it crosses the limit

            if(dcurr->active_rec > MAX_RECS)
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
                    insert_key(curr,current);
                    if(curr->active_keys<=MAX_KEYS)
                    {
                        for(i=curr->active_keys;i>pos+1;i--)
                        {

                            curr->ptr_type.dptr[i]=curr->ptr_type.dptr[i-1];
                        }
                        curr->ptr_type.dptr[i]=dcurr1;
                        dcurr1->parent_ptr=curr;
                    }
                    else
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
                                curr=curr->parent_ptr;
                                insert_key(curr,current);
                                pos = lsearch(curr->key,curr->active_keys,ptr->key[0]);
                                if(curr->active_keys<=MAX_KEYS)
                                {
                                    for(i=curr->active_keys;i>pos;i--)
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
                                    for(j=0,i=mid+1;i<=curr->active_keys;i++,j++)
                                    {
                                        ptr1->ptr_type.ptr[j]=curr->ptr_type.ptr[i];
                                        ptr1->ptr_type.ptr[j]->parent_ptr=ptr;
                                    }
                                    curr->active_keys=mid;
                                    ptr=ptr1;
                                }
                            }
                            else //if curr==NULL
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
                else{
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

datanode_type *make_datanode()
{

    datanode_type *ptr;
    ptr=(datanode_type *)malloc(sizeof(datanode_type));
    ptr->active_rec=0;
    ptr->parent_ptr=NULL;
    ptr->next=NULL;
    ptr->prev=NULL;
}

void insert_key(keynode_type *ptr,int key)
{
    int i;
    i=ptr->active_keys;
    printf("\nI entered insert key");
    while(key<ptr->key[i-1]&&i>0)
    {
       ptr->key[i]=ptr->key[i-1];
       i--;
    }
    ptr->key[i]=key;
    ptr->active_keys+=1;
}
// need to think about update
void insert_rec(datanode_type *ptr,book_record_type record)
{
    int key,i;
    key = record.lib_ACC_num;
    i=ptr->active_rec;
    printf("number of active records is %d",i);
     printf("\nI entered insert rec");
     printf("\nI am going to check %d with %d",key,ptr->records[i-1].lib_ACC_num);
    while((key < ptr->records[i-1].lib_ACC_num) && i>0 )
    {
        printf("\n I enterd while ");
        ptr->records[i]=ptr->records[i-1];
        i--;
    }
    ptr->records[i]=record;
    ptr->active_rec+=1;
    printf("I inserted record at pos %d",i);
}

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
