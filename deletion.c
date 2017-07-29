#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#define MAX_KEYS 2
#define MAX_RECS 3
#define MIN_RECS 2
#define MIN_KEYS 1

//*******************************************************************************************************************************

typedef struct keynode_tag
{
    int active_keys;
    int key[MAX_KEYS+1];
    union ptr_tag
    {
        struct keynode_tag *ptr[MAX_KEYS+2];
        struct datanode_tag *dptr[MAX_RECS+2];
    } ptr_type;
    int u_type;
    struct keynode_tag  *parent_ptr;
} keynode_type;

//***************************************************************************************************************************

typedef struct book_record_tag
{
    int lib_ACC_num;
    char b_name[20];
    char a_name[20];
    int ISBN_num;
    char pub_name[20];
    float b_price;
} book_record_type;

//***************************************************************************************************************************

typedef struct datanode_tag
{
    int active_rec;
    keynode_type *parent_ptr;
    book_record_type records[MAX_RECS+1];
    struct datanode_tag *next,*prev;

} datanode_type;

//***************************************************************************************************************************

typedef enum {FAILURE,SUCCESS} status_code;

typedef struct database_tag
{
    keynode_type *root;
} database;

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
void delete1(database *,int);
void correct(keynode_type *,int);
//                                               Main Function
//**********************************************************************************************************************

main()
{
    int i=0,height,flag=0,option,num_rec,lib_num;
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
            printf("\n ENTER ELEMENT'S LIBRARY ACCESS NUMBER : ");
            scanf("%d",&lib_num);
            delete1(dptr,lib_num);
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
            else
            {
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
    if(dptr->root==NULL)
    {
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
                for(j=0,i=dcurr->active_rec; i<active_rec; i++,j++)
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
                if(dcurr->active_rec > MAX_RECS)
                {
                    printf("\tI entered activerec>max");
                    dcurr1 = make_datanode();
                    dcurr1->parent_ptr=dcurr->parent_ptr;
                    active_rec = dcurr->active_rec;
                    for(j=0,i=(MAX_RECS+1)/2; i<active_rec; i++,j++)
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
                            for(i=curr->active_keys; i>pos+1; i--)
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
                            for(j=0,i=mid+1; i<curr->active_keys; i++,j++)
                            {
                                ptr->key[j]=curr->key[i];
                                ptr->active_keys +=1;
                            }
                            // inserting new pointer
                            for(i=curr->active_keys; i>pos+1; i--)
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
                            for(j=0,i=mid+1; i<=curr->active_keys; i++,j++)
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
                                        for(i=curr->active_keys; i>pos; i--)
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
                                        for(j=0,i=mid+1; i<curr->active_keys; i++,j++)
                                        {
                                            ptr1->key[j]=curr->key[i];
                                            ptr1->active_keys +=1;
                                        }
                                        // inserting new pointer
                                        for(i=curr->active_keys; i>pos; i--)
                                        {
                                            curr->ptr_type.dptr[i]=curr->ptr_type.dptr[i-1];
                                        }
                                        curr->ptr_type.ptr[i]=ptr;
                                        for(j=0,i=mid+1; i<=curr->active_keys; i++,j++)
                                        {
                                            ptr1->ptr_type.ptr[j]=curr->ptr_type.ptr[i];
                                            ptr1->ptr_type.ptr[j]->parent_ptr=ptr1;
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
                    else
                    {
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
        for(i=0; i<=MAX_KEYS; i++)
        {
            ptr->ptr_type.dptr[i]=NULL;
        }
    }
    else
    {
        for(i=0; i<=MAX_KEYS; i++)
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
    else
    {
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
    int count=0,found=0,done=0;
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

void delete1(database *dptr,int key)
{
    keynode_type *kptr,*pptr;
    datanode_type*ptr,*sib;
    book_record_type rec;
    int i,pos,rec_pos,done=0,flag=0,found=0,flag2=0;
    if(dptr->root==NULL)
    {
        printf("\nDATABASE IS EMPTY");
    }
    else
    {
        kptr=dptr->root;
        while(kptr->u_type!=1)
        {
            pos = lsearch(kptr->key,kptr->active_keys,key);
            kptr = kptr->ptr_type.ptr[pos];
        }
        pos=lsearch(kptr->key,kptr->active_keys,key);
        ptr=kptr->ptr_type.dptr[pos];
        i=0;
        if(ptr!=NULL)
        {
            while((i<ptr->active_rec)&&found!=1)
            {
                if(ptr->records[i].lib_ACC_num==key) // finding the record in the datanode
                {
                    rec_pos=i;
                    found=1;
                }
                i++;
            }
            if(found!=1)
            {
                printf("\nTHE REQUIRED RECORD IS NOT THERE...");
            }
            else
            {
                done=0;
                printf("\ni have found the record at %d",rec_pos);
                // delete the record
                for(i=rec_pos; i<ptr->active_rec-1; i++)
                {
                    ptr->records[i]=ptr->records[i+1];
                }
                ptr->active_rec -=1;
                printf("\n i have deleted the record");
                if(ptr->active_rec>=MIN_RECS)
                {
                    printf("\nthis record has enough keys so i am done");
                    done=1;
                }
                else
                {
                    sib=ptr->prev;
                    if(sib!=NULL)// if left sibling is not null
                    {
                        if(ptr->parent_ptr==sib->parent_ptr)
                        {
                            printf("\n i am checking with left sibling it's there and ");
                            if(sib->active_rec>MIN_RECS)// if left sibling is having > min recs
                            {
                                printf("has %d elem and has last element %d",sib->active_rec,sib->records[sib->active_rec-1]);
                                rec=sib->records[sib->active_rec-1];
                                sib->active_rec -=1;
                                for(i=ptr->active_rec; i>0; i--)
                                {
                                    ptr->records[i]=ptr->records[i-1];
                                }
                                ptr->records[0]=rec;
                                printf("\n it donated %d",rec.lib_ACC_num);
                                ptr->active_rec+=1;// borrowed and inserted a record
                                printf(" and key in parent is changed from %d to",ptr->parent_ptr->key[pos-1]);
                                ptr->parent_ptr->key[pos-1]=sib->records[sib->active_rec-1].lib_ACC_num;
                                printf(" %d",ptr->parent_ptr->key[pos-1]);
                                done=1;
                            }
                        }
                        if(done!=1) // this implies either it does not have a left sibling or left sibling is having=min recs
                        {
                            if(ptr->next!=NULL)// checking with the right sibling
                            {
                                sib=ptr->next;
                                if(sib->parent_ptr==ptr->parent_ptr) // it is the right sibling
                                {
                                    printf("\n i am checking with right sib bcoz left one is not there or it has =min recs");
                                    flag=1;
                                    if(sib->active_rec>MIN_RECS)// if sibling can donate a record
                                    {
                                        printf("\n right one can donate and donates");
                                        rec=sib->records[0];
                                        printf(" %d",rec.lib_ACC_num);
                                        for(i=0; i<sib->active_rec-1; i++) // made corresponding arrangement in sibling
                                        {
                                            sib->records[i]=sib->records[i+1];
                                        }
                                        sib->active_rec -=1;
                                        ptr->records[ptr->active_rec]=rec; //inserted record in ptr
                                        ptr->active_rec +=1;
                                        printf(" and key in parent is changed from %d to",ptr->parent_ptr->key[pos]);
                                        ptr->parent_ptr->key[pos]=ptr->records[ptr->active_rec-1].lib_ACC_num;
                                        printf(" %d ",ptr->parent_ptr->key[pos]);
                                        done=1;
                                    }
                                    else // it has the right sibling but it has =min recs and left sib exist or doesnot exists(=min recs)
                                    {
                                        printf("\nright one is having =min recs");
                                        sib=ptr->next;
                                        for(i=0; i<sib->active_rec; i++)
                                        {
                                            ptr->records[ptr->active_rec]=sib->records[i];
                                            ptr->active_rec +=1;
                                        }
                                        if(sib->next!=NULL)
                                        {
                                            sib->next->prev=ptr;
                                        }
                                        ptr->next=sib->next;
                                        free(sib);
                                        // at this point i have merged with right node and deleted it now make corresponding changes in parent
                                        pptr=ptr->parent_ptr;
                                        for(i=pos; i<pptr->active_keys-1; i++)
                                        {
                                            pptr->key[i]=pptr->key[i+1];
                                        }

                                        for(i=pos+1; i<pptr->active_keys; i++)
                                        {
                                            pptr->ptr_type.dptr[i]=pptr->ptr_type.dptr[i+1];
                                        }
                                        pptr->active_keys -=1;
                                        printf(" \ni have merged it with right sibling ");
                                        if(pptr->active_keys>=MIN_KEYS) //if after making adj in parent if it is not deficient
                                        {
                                            printf("\n parent is ok after deleting a key ");
                                            done=1;
                                        }
                                        else // if it is deficient
                                        {
                                            printf("\n parent is deficient ");
                                            flag2=1;
                                        }
                                    }
                                }
                            }
                            if(flag!=1) // this means that there is no right sibling but left sibling is having =min keys
                            {
                                printf("\n it has no right sib hence merging with left one");
                                sib=ptr->prev;
                                if(ptr->next!=NULL)
                                {
                                    ptr->next->prev=sib;
                                }
                                sib->next=ptr->next;
                                for(i=0; i<ptr->active_rec; i++) // merging two nodes
                                {
                                    sib->records[sib->active_rec]=ptr->records[i];
                                    sib->active_rec +=1;
                                }
                                pptr=ptr->parent_ptr;
                                free(ptr); // upto this point i merged two nodes and freed one node
                                for(i=pos-1; i<pptr->active_keys-1; i++)
                                {
                                    pptr->key[i]=pptr->key[i+1];
                                }
                                for(i=pos; i<pptr->active_keys; i++)
                                {
                                    pptr->ptr_type.dptr[i]=pptr->ptr_type.dptr[i+1];
                                }
                                pptr->active_keys -=1;
                                if(pptr->active_keys>=MIN_KEYS) // if parent has greater than = min keys
                                {
                                    done=1;
                                }
                                else
                                {
                                    flag2=1; // indicating that parent is not having enough keys after adjustment
                                }
                            }
                        }
                    }
                    else if(ptr->next!=NULL) // if left sib is null and right one is not null
                    {
                        sib=ptr->next;
                        printf("\n left sib is null");
                        if(sib->active_rec>MIN_RECS)// if sibling can donate a record
                        {
                            printf("\n right one can donate ");
                            rec=sib->records[0];
                            printf("it is donating %d",rec.lib_ACC_num);
                            for(i=0; i<sib->active_rec-1; i++) // made corresponding arrangement in sibling
                            {
                                sib->records[i]=sib->records[i+1];
                            }
                            sib->active_rec -=1;
                            ptr->records[ptr->active_rec]=rec; //inserted record in ptr
                            ptr->active_rec +=1;
                            printf(" and key in parent is changed from %d to",ptr->parent_ptr->key[pos]);
                            ptr->parent_ptr->key[pos]=ptr->records[ptr->active_rec-1].lib_ACC_num;
                            printf(" %d ",ptr->parent_ptr->key[pos]);
                            done=1;
                        }
                        else // it has the right sibling but it has =min recs
                        {
                            printf("right can not donate hence merging with right one");
                            sib=ptr->next;
                            for(i=0; i<sib->active_rec; i++)
                            {
                                ptr->records[ptr->active_rec]=sib->records[i];
                                ptr->active_rec +=1;
                            }
                            if(sib->next!=NULL)
                            {
                                sib->next->prev=ptr;
                            }
                            ptr->next=sib->next;
                            free(sib);
                            // at this point i have merged with right node and deleted it now make corresponding changes in parent
                            pptr=ptr->parent_ptr;
                            for(i=pos; i<pptr->active_keys-1; i++) // adjusting parent keys and pointers
                            {
                                pptr->key[i]=pptr->key[i+1];
                            }

                            for(i=pos+1; i<pptr->active_keys; i++)
                            {
                                pptr->ptr_type.dptr[i]=pptr->ptr_type.dptr[i+1];
                            }
                            pptr->active_keys -=1;
                            if(pptr->active_keys>=MIN_KEYS) //if after making adj in parent if it is deficient
                            {
                                done=1;
                            }
                            else // if it is deficient
                            {
                                flag2=1;
                            }
                        }

                    }
                    else // if both siblings are null
                    {
                        printf("both sib are null");
                        if(ptr->active_rec>0)
                        {
                            done=1;
                        }
                        else
                        {
                            printf("entire tree is deleted");
                            free(ptr);
                            free(kptr);
                            dptr->root=NULL;
                            done=1;
                        }
                    }
                    // if parent is not adjusted
                    if(done!=1) // it tells us that record is deleted
                    {
                        printf("parent is not adjusted");
                        if(flag2!=0) // making corresponding changes in the parent
                        {
                            correct(pptr,1);
                        }
                    }
                }
            }
        }
        else // if the element is pointing to a null pointer => no such element exists
        {
            printf("\nREQUIRED RECORD IS NOT PRESENT IN THE DATABASE ");
        }
    }
}


void correct(keynode_type *ptr1,int first)
{
    keynode_type *sib,*pptr;
    int pos,done=0,i=0,adjust=0;
    int left_d,right_d,left_merge,right_merge,no_keys;
    left_d=right_d=left_merge=right_merge=0;
    if(ptr1->parent_ptr!=NULL)
    {
        pptr=ptr1->parent_ptr;
        while(i<=pptr->active_keys && done!=1) // finding out the position
        {
            if(pptr->ptr_type.ptr[i]==ptr1)
            {
                pos=i;
                done=1;
            }
            i++;
        }
        if(pos>0&&pos<pptr->active_keys) // it means both left and right siblings are present
        {
            sib=pptr->ptr_type.ptr[pos-1];
            if(sib->active_keys>MIN_KEYS) // if left sibling can donate
            {
                left_d=1;
                adjust=1;
            }
            else // if left sibling is not having enough keys to donate
            {
                sib=pptr->ptr_type.ptr[pos+1];
                if(sib->active_keys>MIN_KEYS) // if right sibling can donate
                {
                    right_d=1;
                }
                else // both left and right are deficient
                {
                    right_merge=1;
                }
            }
        }
        else if(pos==0) // this means that it has only right sibling
        {
            sib=pptr->ptr_type.ptr[pos+1];
            if(sib->active_keys>MIN_KEYS) // if right one can donate
            {
                right_d=1;
                adjust=1;
            }
            else // if right one can't donate and there is no left sibling
            {
                right_merge=1;
            }

        }
        else // this means it has only leftsibling
        {
            sib=pptr->ptr_type.ptr[pos-1];
            if(sib->active_keys>MIN_KEYS) // if left sibling can donate
            {
                left_d=1;
                adjust=1;
            }
            else // if left one can't donate and there is no right sibling
            {
                left_merge=1;
            }
        }
        if(left_d==1) // if it is left donate
        {
            sib=pptr->ptr_type.ptr[pos-1];
            for(i=ptr1->active_keys; i>0; i--) // making space for inserting key in the ptr
            {
                ptr1->key[i]=ptr1->key[i-1];
            }
            ptr1->key[0]=pptr->key[pos-1]; // inserting key from parent
            ptr1->active_keys +=1;
            for(i=ptr1->active_keys; i>0; i--) // making space for inserting ptr in the ptr
            {
                if(first==1)
                {
                    ptr1->ptr_type.dptr[i]=ptr1->ptr_type.dptr[i-1];
                }
                else
                {
                    ptr1->ptr_type.ptr[i]=ptr1->ptr_type.ptr[i-1];
                }
            }
            if(first==1)
            {
                ptr1->ptr_type.dptr[i]=sib->ptr_type.dptr[sib->active_keys];
                ptr1->ptr_type.dptr[i]->parent_ptr=ptr1;

            }
            else
            {
                ptr1->ptr_type.ptr[i]=sib->ptr_type.ptr[sib->active_keys];
                ptr1->ptr_type.ptr[i]->parent_ptr=ptr1;
            }
            pptr->key[pos-1]=sib->key[sib->active_keys-1];
            sib->active_keys -=1; // replaced the key in parent with sibling key and deleted it from sibling
        }
        if(right_d==1) // if right one can donate
        {
            sib=pptr->ptr_type.ptr[pos+1];
            ptr1->key[ptr1->active_keys]=pptr->key[pos];
            ptr1->active_keys +=1; // inserting from the parent
            if(first==1) // transfering one pointer
            {
                ptr1->ptr_type.dptr[ptr1->active_keys]=sib->ptr_type.dptr[0];
                ptr1->ptr_type.dptr[ptr1->active_keys]->parent_ptr=ptr1;
            }
            else
            {
                ptr1->ptr_type.ptr[ptr1->active_keys]=sib->ptr_type.ptr[0];
                ptr1->ptr_type.ptr[ptr1->active_keys]->parent_ptr=ptr1;
            }
            pptr->key[pos] = sib->key[0];

            for(i=0; i<sib->active_keys-1; i++) // deleting one key from sibling
            {
                sib->key[i]=sib->key[i+1];
            }
            for(i=0; i<sib->active_keys; i++) // deleting one pointer from sibling
            {
                if(first==1)
                {
                    sib->ptr_type.dptr[i]=sib->ptr_type.dptr[i+1];
                }
                else
                {
                    sib->ptr_type.ptr[i]=sib->ptr_type.ptr[i+1];
                }
            }
            sib->active_keys -=1;
        }
        if(left_merge==1) // if we need to merge with left one
        {
            sib=pptr->ptr_type.ptr[pos-1];
            sib->key[sib->active_keys]=pptr->key[pos-1]; // added the corresponding parent key
            sib->active_keys +=1;
            no_keys=sib->active_keys;
            for(i=0; i<ptr1->active_keys; i++) // merging with left sibling by transfering all keys
            {
                sib->key[sib->active_keys]=ptr1->key[i];
                sib->active_keys +=1;
            }
            for(i=0; i<=ptr1->active_keys; i++) // merging the pointers from the ptr to the sibling
            {
                if(first==1)
                {
                    sib->ptr_type.dptr[no_keys]=ptr1->ptr_type.dptr[i];
                    sib->ptr_type.dptr[no_keys]->parent_ptr=sib;
                }
                else
                {
                    sib->ptr_type.ptr[no_keys]=ptr1->ptr_type.ptr[i];
                    sib->ptr_type.ptr[no_keys]->parent_ptr=sib;
                }
                no_keys++;
            }
            free(ptr1); // freed that pointer
            for(i=pos-1; i<pptr->active_keys-1; i++) // removing one key from parent
            {
                pptr->key[i]=pptr->key[i+1];
            }
            for(i=0; i<pptr->active_keys; i++) // transfering
            {
                pptr->ptr_type.ptr[i]=pptr->ptr_type.ptr[i+1];
            }
            pptr->active_keys -=1;
            if(pptr->active_keys>=MIN_KEYS)
            {
                adjust=1;
            }
            else
            {
                correct(pptr,0);
            }
        }
        if(right_merge==1) // if we need to merge with right one
        {
            sib=pptr->ptr_type.ptr[pos+1];
            ptr1->key[ptr1->active_keys]=pptr->key[pos]; // added the corresponding parent key
            ptr1->active_keys +=1;
            no_keys=ptr1->active_keys;
            for(i=0; i<sib->active_keys; i++) // merging with left sibling by transfering all keys
            {
                ptr1->key[ptr1->active_keys]=sib->key[i];
                ptr1->active_keys +=1;
            }
            for(i=0; i<=sib->active_keys; i++) // merging the pointers from the ptr to the sibling
            {
                if(first==1)
                {
                    ptr1->ptr_type.dptr[no_keys]=sib->ptr_type.dptr[i];
                    ptr1->ptr_type.dptr[no_keys]->parent_ptr=ptr1;
                }
                else
                {
                    ptr1->ptr_type.ptr[no_keys]=sib->ptr_type.ptr[i];
                    ptr1->ptr_type.ptr[no_keys]->parent_ptr=ptr1;
                }
                no_keys++;
            }
            free(sib); // freed that pointer
            for(i=pos-1; i<pptr->active_keys-1; i++) // removing one key from parent
            {
                pptr->key[i]=pptr->key[i+1];
            }
            for(i=0; i<pptr->active_keys; i++) // transfering
            {
                pptr->ptr_type.ptr[i]=pptr->ptr_type.ptr[i+1];
            }
            pptr->active_keys -=1;
            if(pptr->active_keys>=MIN_KEYS)
            {
                adjust=1;
            }
            else
            {
                correct(pptr,0);
            }
        }

    }
    else // if parent ptr is null
    {
        if(first==1) // if root is at first level
        {

        }
        /*else
        {
            if(ptr1)
            {

            }
        }
        */
    }
}

