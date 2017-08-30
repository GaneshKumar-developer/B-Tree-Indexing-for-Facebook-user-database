#include<iostream>
#include<fstream>
#include<cstring>

using namespace std;
class user
{
    public:
    char name[20];
    char email[20];
    char gender[2];
    void read_data();
    void print_data();

};

class node
{
public:
    int id[4];
    user user_obj[4];

    node * next[4];
    node * parent;
    int size;
    node();

};



class file
{
   public:
  fstream fptr;
  file()
  {
    fptr.open("Btree.txt",ios::out|ios::app);;
  }
  };


void user::print_data()
{

cout<<"Name:"<<name<<endl;
cout<<"Email:"<<email<<endl;
cout<<"Gender:"<<gender<<endl;
}


node :: node()
{
    for(int i = 0; i < 4; i++)
        next[i] = NULL;
    parent = NULL;
    size = 0;
}


class btree
{
    public:
    node * root;
    node* findLeaf(int key,int &level);
    void updateKey(node *p,node *c,int newKey,user u);
    void search(int key);
    void insert(int key);
    void insertIntoNode(node *n,int key,node *address,user u);
    void promote(node *n,int key,node *address,user u);
    node* split(node *n);
    void traverse(node *ptr);
    btree();
};

//Constructor
btree :: btree()
{
    root = NULL;
}


void user::read_data()
{

cout<<"Enter the Name of the user:\n";
cin>>name;
cout<<"Enter the Email of the user:\n";
cin>>email;
cout<<"Enter the gender of the user ,Male-M ,Female-F :\n";
cin>>gender;
}

int main()
{
    btree b;
    int choice = 1,key;

    while(choice <=2)
    {
        cout<<"\n1.Insert id key\n";
        cout<<"2.Search id key\n";

        cin>>choice;
        switch(choice)
        {
        case 1:
            cout<<"\n\nEnter the ID of the user,to be inserted\n";
            cin>>key;
            b.insert(key);
            break;
        case 2:
            cout<<"\n\nEnter the ID of the user,to be searched\n";
            cin>>key;
            b.search(key);
            break;
            }
    }
    return 0;
}


void btree :: insert(int key)
{
    file file_obj;
    user u;
    if( root == NULL)
    {   root = new node;
        root->id[root->size] = key;
        u.read_data();
        root->user_obj[root->size]=u;
        file_obj.fptr<<key<<"|"<<u.name<<"|"<<u.gender<<"|"<<u.email<<"\n";
        root->size++;
        return;
    }
    int level;
    node *leaf = findLeaf(key,level);
    int i;
    for(i = 0; i < leaf->size; i++)
        if(leaf -> id[i] == key)
        {
            cout<<"The ID to be inserted already exists"<<endl;
            return;
        }


        u.read_data();
        file_obj.fptr<<key<<"|"<<u.name<<"|"<<u.gender<<"|"<<u.email<<"\n";
    promote(leaf,key,NULL,u);
    cout<<"---------------\n";

    traverse(root);
    cout<<"----------------\n";
}

node* btree :: findLeaf(int key,int &level)
{
    node *ptr = root;
    node *prevptr = NULL;
    level = 0;
    int i;
    while(ptr)
    {
        i = 0;
        level++;
        while(i < ptr -> size-1 && key > ptr -> id[i])
            i++;
        prevptr = ptr;
        ptr = ptr -> next[i];
    }
    return prevptr;
}


void btree :: promote(node *n,int key,node *address,user u)
{
    if( n == NULL)
        return;
    if(n -> size < 4)
    {
        insertIntoNode(n,key,address,u);
        return;
    }
    if( n == root)
    {
        root = new node;
        n->parent = root;
    }
    node *newptr = split(n);
    node *t;
    if(key < n->id[0])
        t = newptr;
    else
        t = n;
    insertIntoNode(t,key,address,u);
    promote(n->parent,n->id[n->size-1],n,n->user_obj[n->size-1]);
    promote(newptr->parent,newptr->id[newptr->size-1],newptr,newptr->user_obj[newptr->size-1]);
}

void btree :: insertIntoNode(node *n,int key,node *address,user u)
{
    int i;

    if( n == NULL)
        return;
    //Finding duplicate entry
    for(i = 0; i < n->size; i++)
        if(n->id[i] == key)
            return;

    i = n->size-1;

    while(i >= 0 && n -> id[i] > key)
    {
        n->id[i+1] = n->id[i];
        n->user_obj[i+1]=n->user_obj[i];
        n->next[i+1] = n->next[i];
        i--;
    }
    i++;
    n->id[i] = key;
    n->user_obj[i]=u;
    n->next[i] = address;
    n->size++;
    if( i == n->size-1)
        updateKey(n->parent,n,key,u);
}


void btree :: updateKey(node *parent,node *child,int newkey,user u)
{
    if( parent == NULL)
        return;
    if(parent->size == 0)
        return;
    int oldkey = child->id[child->size-2];
    for(int i = 0; i < parent->size; i++)
        if(parent->id[i] == oldkey)
        {   parent->id[i] = newkey;
            parent->user_obj[i]=u;
            parent->next[i] = child;
        }

}


node* btree :: split(node *n)
{
    int midpoint = (n -> size+1)/2;
    int newsize = n->size - midpoint;

    node *newptr = new node;
    node *child;

   newptr->parent = n -> parent;

    int i;

    for(i = 0; i < midpoint; i++)
    {
        newptr->id[i] = n->id[i];
        newptr ->next[i] = n->next[i];
        newptr->user_obj[i]=n->user_obj[i];
        n->id[i] = n->id[i+midpoint];
        n->next[i] = n->next[i+midpoint];
        n->user_obj[i]=n->user_obj[i+midpoint];
    }
    n->size = midpoint;
    newptr -> size = newsize;
    for( i = 0; i < n->size; i++)
    {
        child = n->next[i];
        if(child!= NULL)
            child -> parent = n;
    }
    for( i = 0; i < newptr -> size; i++)
    {
        child = newptr -> next[i];
        if(child!= NULL)
            child -> parent = newptr;
    }
    return newptr;
}

void btree :: traverse(node *ptr)
{
    if(ptr == NULL)
    {

        return;
    }


    for(int i = 0; i < ptr->size; i++)
    {
        cout<<ptr->id[i];
    }

    cout<<endl;

    for(int j = 0; j < ptr->size; j++)
        traverse(ptr->next[j]);


}


void btree :: search(int key)
{
    if(root == NULL)
    {
        cout<<"The tree Does not exist"<<endl;
        return;
    }
    int level;
    node *leaf = findLeaf(key,level);
    int flag = 0;
    for(int i = 0; i < leaf ->size; i++)
        if(leaf->id[i] == key)
        {
            flag = 1;
            cout<<"The ID:"<<key<<" Exists in the B-Tree at the level"<<level<<endl;
            cout<<"The info of the user:\n";
            leaf->user_obj[i].print_data();

        }
    if(!flag)
        cout<<"The Key Searched for was not found"<<endl;
}
