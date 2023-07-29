#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>


#define NUMBER_OF_FOOD_ITEMS_PER_RESTO 2
#define MAX_NO_OF_FOOD_ORDERS 7
#define MAX_NO_OF_FOOD_ITEMS 20
#define NUMBER_OF_FOOD_LOCATIONS 9
#define NUMBER_OF_USERS 10
#define NUMBER_OF_AGENTS 10
#define NUMBER_OF_ORDERS 5

int global_order_no = 1;
int global_user_order = 1;


struct order_input{
    char user_id[20];
    char food_loc[20];
    char dish[20];
    int price;
};
struct dish_input
{
    char dish[20];
    int price;
};

struct Food_loc_input{
	int id;
	int height;
    char name[20];
    char address[20];
    int seats;
    char outdoor_seating[20];
    char wifi[20];
    char category[20];
    char cuisine[20];
    char area[20];
    
};

struct user_input{
    int user_id;
    char name[20];
    char phone_no[20];
    char address[100];
    int height;
};

struct agent_input{
    int agent_id;
    char name[20];
    char phone_no[20];
    char area[20];
    int height;
    int accumulated_commision;
    int on_order;
};


typedef struct date{
    int day;
    int month;
    int year;
}Date;

//typedef struct dish_tree{
//	int dish_no;
//    char dish[100];
//    int price;
//    int fav_dish;
//    Date first_order;
//    Date last_order;
//    int height;
//    struct dish_tree* left;
//    struct dish_tree* right;
//}Dish;


typedef struct dish{
    char dish[NUMBER_OF_FOOD_ITEMS_PER_RESTO][20];
    int price[NUMBER_OF_FOOD_ITEMS_PER_RESTO];
    int fav_dish[NUMBER_OF_FOOD_ITEMS_PER_RESTO];
    
}Dish;

typedef struct dish_ll{
    char dish[100];
    int price;
    int fav_dish;
    Date first_order;
    Date last_order;
    struct dish_ll* left;
    struct dish_ll* right;
}Dish_LL;

typedef struct food{
    Dish dish;
    Dish_LL dish_ll;
    
}Food;
//typedef struct food{
//    Dish dish;
//    Dish dish_ll;
//    
//}Food;

typedef struct Order{
    int order_no;
    char order[100];
    int agent_id;
    int user_id;
    char hotel[20];
    Date date;
    int price;
    int order_count;
    int height;
    struct Order* left;
    struct Order* right;
}Order;

typedef struct special_facilities{
    char outdoor_seating[10];//yes or no
    char wifi[10];
}Special_facilities;

typedef struct category_tree{
	int hotel_id;
    char name[20];
    char address[100];
    char category[20];
    char cuisine[20];
    char area[20];
    int total_seats;
    int fav_food_location;
    int total_orders;
    int height;
    Special_facilities special_facilities;
    Food menu;
    struct category_tree* left;
    struct category_tree* right;
}Category;


typedef struct delivery_agent{
    int agent_id;
    char name[20];
    char phone_no[20];
    char area[20];
    int accumulated_commision;
    int on_order;
    int height;
    struct delivery_agent* left;
    struct delivery_agent* right;
    //to keep account of all orders delivered by agent use ll
}Agent;

typedef struct user{
    int user_id;
    char name[20];
    char phone_no[20];
    char address[100];
    Dish* dish_ll;
    int height;
    struct user* left;
    struct user* right;
}User;


Category* newnode(int id,char* name, char* address, char* category, char* cuisine, char* area,
int total_seats,int fav_food_loc,int total_order,Special_facilities sp,Food menu)
{
	Category* root =(Category*)malloc(sizeof(Category));
	root->hotel_id=id;
	strcpy(root->name,name);
	strcpy(root->address,address);
	strcpy(root->category,category);
	strcpy(root->cuisine,cuisine);
	strcpy(root->area,area);
	root->total_seats=total_seats;
	root->fav_food_location=fav_food_loc;
	root->total_orders=total_order;
	root->special_facilities=sp;
	root->menu=menu;
	root->right=NULL;
	root->left=NULL;
	root->height=1;
	return(root);
}

int height1(Category *root)
{
	if(root==NULL)
	  return 0;
	return root->height;
}

int max(int a, int b)
{
	return (a>b)?a:b;
}

Category *rightRotate(Category *y)
{
	Category *x=y->left;
	Category *T2=x->right;
	x->right=y;
	y->left=T2;
	y->height=max(height1(y->left),height1(y->right))+1;
	x->height=max(height1(x->left),height1(x->right))+1;
	return x;
}

Category *leftRotate(Category *x)
{
	Category *y=x->right;
	Category *T2=y->left;
	
	y->left=x;
	x->right=T2;
	x->height=max(height1(x->left),height1(x->right))+1;
	y->height=max(height1(y->left),height1(y->right))+1;
	return y;
}

int getbalance1(Category *root)
{
	if(root==NULL)
	 return 0;
	return height1(root->left)-height1(root->right);
}

Category *insert(Category *root,int id,char* name, char* address, char* category, char* cuisine,
 char* area,int total_seats,int fav_food_loc,int total_order,Special_facilities sp,Food menu)
{
	if(root==NULL)
	{
	   return newnode(id,name,address,category,cuisine,area,total_seats,fav_food_loc,total_order,sp,menu);	
	 } 
	 
	if(id<root->hotel_id)
	{
		root->left=insert(root->left,id,name,address,category,cuisine,area,total_seats,fav_food_loc,total_order,sp,menu);
	}
	else if(id>root->hotel_id)
	{
		root->right=insert(root->right,id,name,address,category,cuisine,area,total_seats,fav_food_loc,total_order,sp,menu);
	}
	else
	{
		return root;
	}
	
	root->height=1+max(height1(root->left),height1(root->right));
	int balance=getbalance1(root);
	//ll case
	if(balance>1 && id<root->left->hotel_id)
	root=rightRotate(root);
	//rr case
	if(balance<-1 && id>root->right->hotel_id)
	root= leftRotate(root);
	//lr case
	if(balance>1 && id>root->left->hotel_id)
	{
		root->left=leftRotate(root->left);
		root=rightRotate(root);
	}
	//rl case
	if(balance<-1 && id<root->right->hotel_id)
	{
		root->right=rightRotate(root->right);
		root= leftRotate(root);	
	}
	return root;
	
}

void food_location_input(Category** food_loc)
{
	/* 1. allocate node */
    int count = 0,k=0;
    Category* first_food_loc = *food_loc;

     FILE *fp1,*fp2; // declare a file pointer
    char filename_1[] = "food_loc_input.txt"; // name of the input file
    char filename_2[] = "dish_input.txt";


    fp1 = fopen(filename_1, "r");
    fp2 = fopen(filename_2, "r");
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return ;
    }

    if (fp2 == NULL) {
        printf("Failed to open file %s\n", filename_2);
        return ;
    }

    // read the input values from the file
    struct Food_loc_input table[NUMBER_OF_FOOD_LOCATIONS];
    struct dish_input dish_inp[MAX_NO_OF_FOOD_ITEMS]; 
int i;
    // read the table data from the file
    for (i = 0; i < NUMBER_OF_FOOD_LOCATIONS; i++) {
        fscanf(fp1, "%d %s %s %d %s %s %s %s %s",table[i].id,table[i].name, table[i].address, &table[i].seats,table[i].outdoor_seating,table[i].wifi,table[i].category,table[i].cuisine,table[i].area);
    }

    for (i = 0; i < MAX_NO_OF_FOOD_ITEMS; i++) {
        fscanf(fp2, "%s %d ",dish_inp[i].dish,&dish_inp[i].price);
    }

    // for (int i = 0; i < NUMBER_OF_FOOD_LOCATIONS; i++) {
    //     printf( "%s %s %d %s %s %s %s %s", table[i].name, table[i].address, table[i].seats,table[i].outdoor_seating,table[i].wifi,table[i].category,table[i].cuisine,table[i].area);
    // }

    // for (int i = 0; i < NUMBER_OF_FOOD_ITEMS; i++) {
    //     printf( "%s %d ",dish_inp[i].dish,dish_inp[i].price);
    // }


    for( i =0 ; i< NUMBER_OF_FOOD_LOCATIONS; i++){


        Category* new_node
		= (Category*)malloc(sizeof(Category));

//        count++;
//	    //Category* last = first_food_loc; /* used in step 5*/
//	    /* 2. put in the data */
//        //  printf("Enter FOOD_LOCATION name : ");
//        // scanf("%s",new_node->name);
        strcpy(new_node->name,table[i].name);
        new_node->hotel_id=table[i].id;
        strcpy(new_node->address,table[i].address);
        new_node->total_seats = table[i].seats;
        strcpy(new_node->special_facilities.outdoor_seating,table[i].outdoor_seating);
        strcpy(new_node->special_facilities.wifi,table[i].wifi);
        strcpy(new_node->category,table[i].category);
        strcpy(new_node->cuisine,table[i].cuisine);
         strcpy(new_node->area,table[i].area);
        int j= 0;
        while(j<NUMBER_OF_FOOD_ITEMS_PER_RESTO){
            
            strcpy(new_node->menu.dish.dish[j],dish_inp[k].dish);
            new_node->menu.dish.price[j] = dish_inp[k].price;
         
            j++;
            k++;
            
        }
//        new_node->left=NULL;
//        new_node->right=NULL;
       insert(*new_node,new_node->hotel_id,new_node->address,new_node->category,new_node->cuisine,new_node->area,new_node->total_seats,
	   new_node->fav_food_location,new_node->total_orders,new_node->special_facilities,new_node->menu);
       
        
//       new_node->next = (*food_loc);
//        new_node->prev = NULL;

        /* 4. change prev of head User to new User */
//        if ((*food_loc) != NULL)
//            (*food_loc)->prev = new_node;
//
//        /* 5. move the head to point to the new User */
//        (*food_loc) = new_node;
    }

 

	 fclose(fp1);
     fclose(fp2);

	return;
}

int height2(User *root)
{
	if(root==NULL)
	  return 0;
	return root->height;
}

//int max(int a, int b)
//{
//	return (a>b)?a:b;
//}

User *rightRotate2(User *y)
{
	User *x=y->left;
	User *T2=x->right;
	x->right=y;
	y->left=T2;
	y->height=max(height2(y->left),height2(y->right))+1;
	x->height=max(height2(x->left),height2(x->right))+1;
	return x;
}

User *leftRotate2(User *x)
{
	User *y=x->right;
	User *T2=y->left;
	
	y->left=x;
	x->right=T2;
	x->height=max(height2(x->left),height2(x->right))+1;
	y->height=max(height2(y->left),height2(y->right))+1;
	return y;
}

int getbalance2(User *root)
{
	if(root==NULL)
	 return 0;
	return height2(root->left)-height2(root->right);
}

User *insert2(User *root, User *res)
{
	if(root==NULL)
	{
	   root=res;
	  return root;	
	 } 
	 
	if(res->user_id<root->user_id)
	{
		root->left=insert2(root->left,res);
	}
	else 
	{
		root->right=insert2(root->right,res);
	}
	
	root->height=1+max(height2(root->left),height2(root->right));
	int balance=getbalance2(root);
	//ll case
	if(balance>1 && res->user_id<root->left->user_id)
	root=rightRotate2(root);
	//rr case
	if(balance<-1 && res->user_id>root->right->user_id)
	root= leftRotate2(root);
	//lr case
	if(balance>1 && res->user_id>root->left->user_id)
	{
		root->left=leftRotate2(root->left);
		root=rightRotate2(root);
	}
	//rl case
	if(balance<-1 && res->user_id<root->right->user_id)
	{
		root->right=rightRotate2(root->right);
		root= leftRotate2(root);	
	}
	return root;
}

void user_input(User** first_user){


    FILE *fp1; // declare a file pointer
    char filename_1[] = "user_input.txt"; // name of the input file

    fp1 = fopen(filename_1, "r");
   
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return ;
    }


    // read the input values from the file
    struct user_input user_inp[NUMBER_OF_USERS];
    int i;
     // read the table data from the file
    for (i = 0; i < NUMBER_OF_USERS; i++) 
    {
         fscanf(fp1, "%s %s %s %s", user_inp[i].user_id,user_inp[i].name,user_inp[i].phone_no,user_inp[i].address);
    }


    for( i =0 ; i < NUMBER_OF_USERS; i++)
    {
        User* new_User
		= (User*)malloc(sizeof(User));

        /* 2. put in the data */
        //new_User->height=user_inp[i].height;
        new_User->user_id=user_inp[i].user_id;
        strcpy(new_User->name,user_inp[i].name);
        strcpy(new_User->phone_no,user_inp[i].phone_no);
        strcpy(new_User->address,user_inp[i].address);
        /* 3. Make next of new User as head and previous as NULL
        */
//        new_User->next = (*first_user);
//        new_User->prev = NULL;
//
//        /* 4. change prev of head User to new User */
//        if ((*first_user) != NULL)
//            (*first_user)->prev = new_User;
//
//        /* 5. move the head to point to the new User */
        //(*first_user) = new_User;
        *first_user=insert2(*first_user,new_User);

    }

    fclose(fp1);
    return;

}

int height3(Agent *root)
{
	if(root==NULL)
	  return 0;
	return root->height;
}

//int max(int a, int b)
//{
//	return (a>b)?a:b;
//}

Agent *rightRotate3(Agent *y)
{
	Agent *x=y->left;
	Agent *T2=x->right;
	x->right=y;
	y->left=T2;
	y->height=max(height3(y->left),height3(y->right))+1;
	x->height=max(height3(x->left),height3(x->right))+1;
	return x;
}

Agent *leftRotate3(Agent *x)
{
	Agent *y=x->right;
	Agent *T2=y->left;
	
	y->left=x;
	x->right=T2;
	x->height=max(height3(x->left),height3(x->right))+1;
	y->height=max(height3(y->left),height3(y->right))+1;
	return y;
}

int getbalance3(Agent *root)
{
	if(root==NULL)
	 return 0;
	return height3(root->left)-height3(root->right);
}

Agent *insert3(Agent *root, Agent *res)
{
	if(root==NULL)
	{
	   root=res;
	  return root;	
	 } 
	 
	if(res->agent_id<root->agent_id)
	{
		root->left=insert3(root->left,res);
	}
	else 
	{
		root->right=insert3(root->right,res);
	}
	
	root->height=1+max(height3(root->left),height3(root->right));
	int balance=getbalance3(root);
	//ll case
	if(balance>1 && res->agent_id<root->left->agent_id)
	root=rightRotate3(root);
	//rr case
	if(balance<-1 && res->agent_id,root->right->agent_id)
	root= leftRotate3(root);
	//lr case
	if(balance>1 && res->agent_id>root->left->agent_id)
	{
		root->left=leftRotate3(root->left);
		root=rightRotate3(root);
	}
	//rl case
	if(balance<-1 && res->agent_id<root->right->agent_id)
	{
		root->right=rightRotate3(root->right);
		root= leftRotate3(root);	
	}
	return root;
}
void agent_input(Agent** head_ref)
{
	/* 1. allocate Agent */

    FILE *fp1; // declare a file pointer
    char filename_1[] = "agent_input.txt"; // name of the input file



    fp1 = fopen(filename_1, "r");
   
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return ;
    }


    // read the input values from the file
    struct agent_input agent_inp[NUMBER_OF_AGENTS];
int i;
     // read the table data from the file
        for (i = 0; i < NUMBER_OF_AGENTS; i++) {
            fscanf(fp1, "%s %s %s %s %d %d", agent_inp[i].agent_id,agent_inp[i].name,agent_inp[i].phone_no,agent_inp[i].area,&agent_inp[i].accumulated_commision,&agent_inp[i].on_order);
        }
    for(i =0 ; i< NUMBER_OF_AGENTS;i++){

        Agent* new_Agent
		= (Agent*)malloc(sizeof(Agent));

       new_Agent->height=agent_inp[i].height;
       new_Agent->agent_id=agent_inp[i].agent_id;
       strcpy(new_Agent->name,agent_inp[i].name);
       strcpy(new_Agent->phone_no,agent_inp[i].phone_no);
       strcpy(new_Agent->area,agent_inp[i].area);
       new_Agent->accumulated_commision = agent_inp[i].accumulated_commision;
       new_Agent->on_order = agent_inp[i].on_order;


        *head_ref=insert3(*head_ref,new_Agent);
//        new_Agent->next = (*head_ref);
//        new_Agent->prev = NULL;
//
//        /* 4. change prev of head Agent to new Agent */
//        if ((*head_ref) != NULL)
//            (*head_ref)->prev = new_Agent;
//
//        /* 5. move the head to point to the new Agent */
//        (*head_ref) = new_Agent;

    }
	
}

void print_food_location_list(Category* node)
{  
    Category* last = node;
	int count = NUMBER_OF_FOOD_LOCATIONS;
	printf("\n Inorder traversal\n");
	while (count!=0) {
		print_food_location_list(node->left);
		printf("Hotel id : %d\n",node->hotel_id);
		
		printf("Name of FOOD_LOCATION : %s \n",node->name);
     
        // printf("Address of FOOD_LOCATION : %s\n",node->address);
            
        printf("Total no of seats : %d \n",node->total_seats);
            
        printf("Outdoor seating : %s \n",node->special_facilities.outdoor_seating);
            
        printf("wifi : %s \n",node->special_facilities.wifi);

        printf("category : %s\n ",node->category);

        printf("cusine : %s\n",node->cuisine);

        printf("area : %s\n",node->area);


        printf("--MENU-- \n");
        int i;
        for(i =0 ; i < NUMBER_OF_FOOD_ITEMS_PER_RESTO ; i++){
            printf("dish: ");
            printf("%s\n",node->menu.dish.dish[i]);  
            printf("price: ");
            printf("%d\n",node->menu.dish.price);  


        }
        printf("---------------------------------------\n");
        
		print_food_location_list(node->right);
        count--;
	}

    

}

void print_user_list(User* user)
{
	User* last;
	int count=NUMBER_OF_USERS;
	
	printf("\n Inorder Traversal  \n");
	while (count!=0) {
		print_user_list(user->left);
        printf("User id : %d \n",user->user_id);
		printf("Name of user : %s \n",user->name);
     
        printf("Address of user : %s\n",user->address);
            
        printf("Phone no : %s \n",user->phone_no);
            
       
        printf("---------------------------------------\n");

	    print_user_list(user->right);
	    count--;
	}

}

void print_agent_list(Agent* agent){
    Agent* last;
    int count=NUMBER_OF_AGENTS;
	printf("\n Inorder Traversal \n");
	while (count!=0){
		print_agent_list(agent->left);
        printf("Agent id : %d \n",agent->agent_id);
		printf("Name of Agent : %s \n",agent->name);
     
        printf("Phone no : %s\n",agent->phone_no);
            
        printf("accumulated commision : %d \n",agent->accumulated_commision);
            
        printf("Phone no : %s \n",agent->phone_no);
            
       
        printf("---------------------------------------\n");
        print_agent_list(agent->right);
        count--;
//		last = agent;
//		agent = agent->next;
	}
}

void search1(Category** head){

    char category[20],cuisine[20],area[20];
    printf("Enter category : ");
    scanf("%s",category);

    printf("Eating locations by cusine are: ");
    Category* category_ptr = *head;
    while(category_ptr!=NULL){
    	search1(&category_ptr->left);
        if(!strcmp(category_ptr->category,category)){
            printf("%s \n",category_ptr->name);
        }
        search1(&category_ptr->right);

        //category_ptr = category_ptr->next;
    }
}

void search2(Category** head)
{
	char cuisine[20];
	printf("Enter cusine : ");
    scanf("%s",cuisine);
    printf("Eating locations by cusine are: ");
    Category* category_ptr = *head;
    
     while(category_ptr!=NULL){
     	search2(&category_ptr->left);
        if(!strcmp(category_ptr->cuisine,cuisine)){
            printf("%s \n",category_ptr->name);
        }
        search2(&category_ptr->right);
    }
}

 void search3(Category** head)   
 {
 	char area[20];
 	 printf("Enter area : ");
    scanf("%s",area);
    printf("Eating locations by area are: ");
    Category* category_ptr = *head;
     while(category_ptr!=NULL){
     	search3(&category_ptr->left);
        if(!strcmp(category_ptr->area,area)){
            printf("%s \n",category_ptr->name);
        }
        search3(&category_ptr->right);

        //category_ptr = category_ptr->next;
    }

 }

int height4(Order *root)
{
	if(root==NULL)
	  return 0;
	return root->height;
}

//int max(int a, int b)
//{
//	return (a>b)?a:b;
//}

Order *rightRotate4(Order *y)
{
	Order *x=y->left;
    Order *T2=x->right;
	x->right=y;
	y->left=T2;
	y->height=max(height4(y->left),height4(y->right))+1;
	x->height=max(height4(x->left),height4(x->right))+1;
	return x;
}

Order *leftRotate4(Order *x)
{
	Order *y=x->right;
	Order *T2=y->left;
	
	y->left=x;
	x->right=T2;
	x->height=max(height4(x->left),height4(x->right))+1;
	y->height=max(height4(y->left),height4(y->right))+1;
	return y;
}

int getbalance4(Order *root)
{
	if(root==NULL)
	 return 0;
	return height4(root->left)-height4(root->right);
}

Order *insert4(Order *root, Order *res)
{
	if(root==NULL)
	{
	   root=res;
	  return root;	
	 } 
	 
	if(res->order_no<root->order_no)
	{
		root->left=insert4(root->left,res);
	}
	else 
	{
		root->right=insert4(root->right,res);
	}
	
	root->height=1+max(height4(root->left),height4(root->right));
	int balance=getbalance4(root);
	//ll case
	if(balance>1 && res->order_no<root->left->order_no)
	root=rightRotate4(root);
	//rr case
	if(balance<-1 && res->order_no,root->right->order_no)
	root= leftRotate4(root);
	//lr case
	if(balance>1 && res->order_no>root->left->agent_id)
	{
		root->left=leftRotate4(root->left);
		root=rightRotate4(root);
	}
	//rl case
	if(balance<-1 && res->order_no<root->right->order_no)
	{
		root->right=rightRotate4(root->right);
		root= leftRotate4(root);	
	}
	return root;
}

void add_order(Order** head_ref, char order[100],int price,Date date,int agent_id,int user_id,char hotel[20])
{
    // printf("check_point_1\n");
	/* 1. allocate order */
	Order* new_order
		= (Order*)malloc(sizeof(Order));

	// Order* last = *head_ref; /* used in step 5*/

	/* 2. put in the data */
	strcpy(new_order->order,order);
    new_order->agent_id=agent_id;
    new_order->user_id=user_id;
    strcpy(new_order->hotel,hotel);

    new_order->order_no = global_order_no;
    new_order->price = price;
    new_order->order_count=1;

    new_order->date.day = date.day;
    new_order->date.month = date.month;
    new_order->date.year = date.year;

    // printf("%s %d:%d:%d \n",new_order->order , new_order->date.day,new_order->date.month,new_order->date.year);
    *head_ref=insert4(*head_ref,new_order);
    

	/* 3. This new order is going to be the last order, so
	// 	make next of it as NULL*/
	// new_order->next = NULL;

	/* 4. If the Linked List is empty, then make the new
		order as head */
	// if (*head_ref == NULL) {
	// 	new_order->prev = NULL;
	// 	*head_ref = new_order;
	// 	return;
	// }

	// /* 5. Else traverse till the last order */
	// while (last->next != NULL)
	// 	last = last->next;

	// /* 6. Change the next of last order */
	// last->next = new_order;

	// /* 7. Make last order as previous of new order */
	// new_order->prev = last;
//        
//     new_order->next = (*head_ref);
//        new_order->prev = NULL;
//
//        /* 4. change prev of head Agent to new Agent */
//        if ((*head_ref) != NULL)
//            (*head_ref)->prev = new_order;
//
//        /* 5. move the head to point to the new Agent */
//        (*head_ref) = new_order;

    
// printf("check_point_2\n");
	return;
}

void place_order(Category **first_food_location,Agent **first_agent,User **first_user,Order **first_order,int user_id,char food_location[100],char order[100],int price)
{

    Category *food_location_ptr = *first_food_location;
    Agent *agent_ptr = *first_agent;
    User *user_ptr = *first_user;
    Order* first_order_ptr = *first_order;
    
    int found_user = 0;
    while(found_user !=1 ){
    
        if(user_ptr->user_id==user_id){
            // printf("User exists and its id : %s\n",user_ptr->user_id);
            found_user = 1; 
        } 
       else 
       user_ptr=user_ptr->left;
	   user_ptr = user_ptr->right;
    }

    if(found_user == 0){
        printf("return from user");
        return;
    }

     int i;
    int found_food_location = 0;
    while( found_food_location !=1)
    {  
        if(!strcmp(food_location_ptr->name,food_location))
        {   
            found_food_location = 1;
            //  printf("food location found and its name is : %s\n",food_location_ptr->name);
              for( i = 0 ; i < MAX_NO_OF_FOOD_ITEMS ; i++)
              {
                if(!strcmp(food_location_ptr->menu.dish.dish[i],order))
                {
                    // printf("Dish found and its name is %s \n",food_location_ptr->menu.dish.dish[i]);

                    Date date;
                    // printf("The day of order : ");
                    // scanf("%d",&date.day);
                    // printf("The month of order : ");
                    // scanf("%d",&date.month);
                    // printf("The year of order : ");
                    // scanf("%d",&date.year);
                     
                     date.day = 1;
                     date.month = 1;
                     date.year = 2022;

                     int found_agent = 0;
                    while(agent_ptr->on_order == 1){
                        agent_ptr = agent_ptr->left;
                        agent_ptr=agent_ptr->right;
                    }
                    agent_ptr->on_order = 1;

                    add_order(first_order,order,price,date,agent_ptr->agent_id,user_ptr->user_id,food_location_ptr->name);//appending order in global list
                    

                    // printf("Ordered Date : %d : %d : %d\n",user_ptr->order->date.day,user_ptr->order->date.month,user_ptr->order->date.year);
                    
    
                    // while(strcmp(user_ptr->dish_ll->dish,order) && user_ptr->dish_ll!=NULL){
                    //     user_ptr->dish_ll = user_ptr->dish_ll->next;
                    // }
                    
                    
                    // if(user_ptr->dish_ll==NULL){
                    //     add_dish_ll(&(user_ptr->dish_ll),order,price);
                    // }
                    // else
                    // {
                    //     printf("Dish already in global menu");
                    //     user_ptr->dish_ll->fav_dish++;
                    // }

                    // printf("check_point_dish_ll_1_ends\n");


                    //we will check for a free agent to deliver food
                   


                    
                    
                    
                } 
             }
        }
        else food_location_ptr = food_location_ptr->left;
        food_location_ptr = food_location_ptr->right;
	}

       


    // printf("Order to be delivered : %s\n",first_order_ptr->order);
    // printf("Ordered Date : %d : %d : %d\n",first_user_order_list_tra->date.day,first_user_order_list_tra->date.month,first_user_order_list_tra->date.year);
    printf("Order number  : %d\n",global_order_no);
    printf("Restaurant : %s\n",food_location_ptr->name);
    printf("User id  : %s\n",user_ptr->user_id );
    printf("Agent id : %s\n",agent_ptr->agent_id);
    printf("------------------------------\n");

    global_order_no++;

    //till here✅

                   
}

void print_global_order_list(Order** first_order){

    Order* first_order_ptr = *first_order;

    int found = 0;
     while(first_order_ptr!=NULL)
    {
        printf("Order no : %d\n",first_order_ptr->order_no);
        printf("Order : %s\n",first_order_ptr->order);
        printf("Order price : %d\n",first_order_ptr->price);
        printf("Order date : %d:%d:%d\n",first_order_ptr->date.day,first_order_ptr->date.month,first_order_ptr->date.year);
        printf("Order count : %d \n",first_order_ptr->order_count);
        printf(" %s\n",first_order_ptr->hotel);

        first_order_ptr = first_order_ptr->left;
        first_order_ptr=first_order_ptr->right;
        found = 1;
         printf("-----------------\n");
    }
   
    if(found == 0 ) printf("Order list is empty\n ");
    return;
}

Order *minValue(Order *node)
{
	Order *current =node;
	while(current->left!=NULL)
	{
		current=current->left;		
	}
	return current;
}

Order *deletenode(Order* root, Order* res)
{
	if(root==NULL)
	{
		root=res;
		return root;	
	}
	if(res->order_no<root->order_no)
	{
		root->left=deletenode(root->left,res);
	}
	else if(res->order_no>root->order_no)
	{
		root->right=deletenode(root->right,res);
	}
	else
	{
		if(root->left==NULL || root->right==NULL)
		{
			Order *temp=root->left ? root->left:root->right;
			if(temp==NULL)
			{
				temp=root;
				root=NULL;
			}
			else
			{
				*root=*temp;
				free(temp);
			}
			
		}
		else
		{
			Order *temp= minValue(root->right);
			root->order_no=temp->order_no;
			root->right=deletenode(root->right,res);
		}
	}
	if(root==NULL)
	{
		return root;
	}
	root->height=1+max(height4(root->left),height4(root->right));
	int balance=getbalance4(root);
	//left left case
	if(balance>1 && getbalance4(root->left)>=0)
	    return rightRotate4(root);
	//left right case
	if(balance>1 && getbalance4(root->left)<0)
	{
		root->left=leftRotate4(root->left);
		return rightRotate4(root);
	}
	//right right case
	if(balance<-1 && getbalance4(root->right)<=0)
	{
		return leftRotate4(root);
	}
	//right left case
	if(balance<-1 && getbalance4(root->right)>0)
	{
		root->right=rightRotate4(root->right);
		return leftRotate4(root);
	}
	return root;
}




void delivery(Order** first_order,Agent** first_agent, char order[100],int order_no,int price)
{
    //Delete a node in a LL
    Order *first_order_ptr = *first_order;
    Agent *first_agent_ptr = *first_agent;
	/* base case */
	if (first_order_ptr == NULL)
		return;

	/* Change next only if node to be deleted is NOT the last node */
    int found_order = 0;
    while(first_order_ptr!=NULL && found_order==0)
    {
        if(first_order_ptr->order_no == order_no && !strcmp(first_order_ptr->order,order)) found_order=1;
        else{
            first_order_ptr = first_order_ptr->left;
            first_order_ptr = first_order_ptr->right;
            // printf("Inside first_order_ptr\n");
        }
    }
    // printf("TO be Free Agent : %s \n",first_order_ptr->agent_id);

    int found_agent = 0;
    while(found_agent == 0 && first_agent_ptr != NULL)
    {
        if(first_agent_ptr->agent_id==first_order_ptr->agent_id) found_agent = 1;
        else
        {
            first_agent_ptr = first_agent_ptr->left;
            first_agent_ptr = first_agent_ptr->right;
            // printf("Inside second_agent_ptr\n");
        } 
    }
    
    if(first_agent_ptr==NULL) printf("Delivery boy not present\n");
    else{
        first_agent_ptr->on_order = 0;
        printf("Accumulated commmision %d ",first_agent_ptr->accumulated_commision);
        first_agent_ptr->accumulated_commision += price/10;
        printf("Final Accumulated commmision %d ",first_agent_ptr->accumulated_commision);
        printf("Free Agent : %s \n",first_agent_ptr->agent_id);
    }
    *first_order=deletenode(*first_order,first_order_ptr);

    //1st node
//    if(first_order_ptr->next == NULL && first_order_ptr->prev == NULL){
//        *first_order = NULL;
//    }
//    else if(first_order_ptr->next == NULL){
//        //last node
//        first_order_ptr->prev->next = NULL;
//    }
//    else{
//        //general case
//        first_order_ptr->prev->next = first_order_ptr->next;
//        first_order_ptr->next->prev = first_order_ptr->prev;
//    }
//    
//
//	/* Finally, free the memory occupied by del*/
//	free(first_order_ptr);
	//return;
}
void cancel_order(Order** first_order,Agent** first_agent, char order[100],int order_no,int price)
{
    //Delete a node in a LL
    Order *first_order_ptr = *first_order;
    Agent *first_agent_ptr = *first_agent;
	/* base case */
	if (first_order_ptr == NULL)
		return;

	/* Change next only if node to be deleted is NOT the last node */
    int found_order = 0;
    while(first_order_ptr!=NULL && found_order==0)
    {
        if(first_order_ptr->order_no == order_no && !strcmp(first_order_ptr->order,order)) found_order=1;
        else{
            first_order_ptr = first_order_ptr->left;
            first_order_ptr = first_order_ptr->right;
            // printf("Inside first_order_ptr\n");
        }
    }
    // printf("TO be Free Agent : %s \n",first_order_ptr->agent_id);

    int found_agent = 0;
    while(found_agent == 0 && first_agent_ptr != NULL)
    {
        if(first_agent_ptr->agent_id==first_order_ptr->agent_id) found_agent = 1;
        else
        {
            first_agent_ptr = first_agent_ptr->left;
            first_agent_ptr = first_agent_ptr->right;
            // printf("Inside second_agent_ptr\n");
        } 
    }
    
    if(first_agent_ptr==NULL) printf("Delivery boy not present\n");
    else{
        first_agent_ptr->on_order = 0;
        // printf("Accumulated commmision %d ",first_agent_ptr->accumulated_commision);
        // first_agent_ptr->accumulated_commision += price/10;
        // printf("Final Accumulated commmision %d ",first_agent_ptr->accumulated_commision);
        printf("Free Agent : %s \n",first_agent_ptr->agent_id);
    }

     *first_order=deletenode(*first_order,first_order_ptr);
    //1st node
//    if(first_order_ptr->next == NULL && first_order_ptr->prev == NULL){
//        *first_order = NULL;
//    }
//    else if(first_order_ptr->next == NULL){
//        //last node
//        first_order_ptr->prev->next = NULL;
//    }
//    else{
//        //general case
//        first_order_ptr->prev->next = first_order_ptr->next;
//        first_order_ptr->next->prev = first_order_ptr->prev;
//    }
    

	/* Finally, free the memory occupied by del*/
//	free(first_order_ptr);
//	return;
}

void pair_order_agent(Order** first_order){
    Order* first_order_ptr = *first_order;

    while(first_order_ptr!=NULL)
    {

        printf("%s -> %s\n",first_order_ptr->order,first_order_ptr->agent_id);
        first_order_ptr = first_order_ptr->left;
        first_order_ptr = first_order_ptr->right;

    }

    return;

}

int diff_of_days(Date date_of_order,Date date_today)
{
    Date diff;
    if(date_today.day < date_of_order.day){
        diff.day = date_today.day+30 - date_of_order.day;
        date_today.month = date_today.month - 1;
    }
    else diff.day = date_today.day - date_of_order.day;

    if(date_today.month < date_of_order.month){
        diff.month = date_today.month+12 - date_of_order.month;
        date_today.year = date_today.year - 1;
    }
    else diff.month = date_today.month - date_of_order.month;

    diff.year = date_today.year - date_of_order.year;

    return (diff.day + diff.month*30 + diff.year*365);

}

void fav_food_of_user(Order** first_order,Order** first_user_order,char user_id[20])
{

    Date date;
    date.day = 1;
    date.month = 4;
    date.year = 2022;
    Order* first_order_ptr = *first_order;
    Order* first_user_order_ptr = *first_user_order; 
    int max_count1,max_count2,max_count3;
    max_count1=max_count2=max_count3=0;
    
    while(first_order_ptr != NULL )
    {
            
        // if(first_user_order_ptr==NULL){
                //printf("checkpoint 1\n");
                
                if(first_order_ptr->user_id==user_id)
                {
                    //printf("checkpoint 2\n");
                    Order* first_user_order_ptr_2 = *first_user_order;
                    
                   int count=0;
                    int found_order = 0;
                    while(first_user_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                        //printf("checkpoint 3\n");
                        if(!strcmp(first_user_order_ptr_2->order,first_order_ptr->order) && diff_of_days(first_order_ptr->date,date)<=90 ) {
                            first_user_order_ptr_2->order_count++;
                        count=first_user_order_ptr_2->order_count;
                            found_order =1;
                            //printf("checkpoint 4\n");
                            if(count>max_count1)
                            {
                            	max_count1=count;
							}
                        }
                         first_user_order_ptr_2 = first_user_order_ptr_2->left;
                         first_user_order_ptr_2 = first_user_order_ptr_2->right;
                    }
                    
                     while(first_user_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                        //printf("checkpoint 3\n");
                        if(!strcmp(first_user_order_ptr_2->order,first_order_ptr->order) && diff_of_days(first_order_ptr->date,date)<=90 ) {
                            first_user_order_ptr_2->order_count++;
                            count=first_user_order_ptr_2->order_count;
                            found_order =1;
                            //printf("checkpoint 4\n");
                            if(count>max_count2 && count<max_count1)
                            {
                            	max_count2=count;
							}
                        }
                         first_user_order_ptr_2 = first_user_order_ptr_2->left;
                         first_user_order_ptr_2 = first_user_order_ptr_2->right;
                    }
                    
                     while(first_user_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                        //printf("checkpoint 3\n");
                        if(!strcmp(first_user_order_ptr_2->order,first_order_ptr->order) && diff_of_days(first_order_ptr->date,date)<=90 ) {
                            first_user_order_ptr_2->order_count++;
                            count=first_user_order_ptr_2->order_count;
                            found_order =1;
                           // printf("checkpoint 4\n");
                            if(count>max_count3 && count<max_count2)
                            {
                            	max_count3=count;
							}
                        }
                         first_user_order_ptr_2 = first_user_order_ptr_2->left;
                         first_user_order_ptr_2 = first_user_order_ptr_2->right;
                    }
                       

                    
//                        if(found_order==0  && diff_of_days(first_order_ptr->date,date)<=90 ){
////                        add_order(first_user_order,first_order_ptr->order,first_order_ptr->price,first_order_ptr->date,first_order_ptr->agent_id,first_order_ptr->user_id,first_order_ptr->hotel);
////                        printf("checkpoint 5\n");
//                         
//                    }
                    
//                    printf("checkpoint 6\n");
              }
              
                first_order_ptr = first_order_ptr->left;
                first_order_ptr = first_order_ptr->right;
                //printf("checkpoint 7\n");
}
    while(first_order_ptr!=NULL)
    {
    	if(first_order_ptr->order_count==max_count1)
    	{
    		printf("%s\n",first_order_ptr->order);
		}
		if(first_order_ptr->order_count==max_count2)
    	{
    		printf("%s\n",first_order_ptr->order);
		}
		if(first_order_ptr->order_count==max_count3)
    	{
    		printf("%s\n",first_order_ptr->order);
		}
		first_order_ptr=first_order_ptr->left;
		first_order_ptr=first_order_ptr->right;
		
	}

}

void find_fav_restaurant(int N,Order** first_order,Order** first_hotel_order )
{
    Date date;
    date.day = 1;
    date.month = 4;
    date.year = 2022;
    Order* first_order_ptr = *first_order;
    Order* first_hotel_order_ptr = *first_hotel_order; 
    int max_count1,max_count2,max_count3;
    max_count1=max_count2=max_count3=0;
    int count=0;
    while(first_order_ptr != NULL )
    {
            
        // if(first_user_order_ptr==NULL){
                //printf("checkpoint 1\n");
                
                // if(!strcmp(first_order_ptr->user_id,user_id))
                // {
                    //printf("checkpoint 2\n");
                    Order* first_hotel_order_ptr_2 = *first_hotel_order;
                    
                   
                    int found_order = 0;
                    while(first_hotel_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                       // printf("checkpoint 3\n");
                        if(!strcmp(first_hotel_order_ptr_2->hotel,first_order_ptr->hotel) && diff_of_days(first_order_ptr->date,date)<=N ) {
                            first_hotel_order_ptr_2->order_count++;
                            count=first_hotel_order_ptr_2->order_count;
                            found_order =1;
                             if(count>max_count1)
                            {
                            	max_count1=count;
							}
                            //printf("checkpoint 4\n");
                            
                        }
                         first_hotel_order_ptr_2 = first_hotel_order_ptr_2->left;
                         first_hotel_order_ptr_2 = first_hotel_order_ptr_2->right;
                    }
                     while(first_hotel_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                       // printf("checkpoint 3\n");
                        if(!strcmp(first_hotel_order_ptr_2->hotel,first_order_ptr->hotel) && diff_of_days(first_order_ptr->date,date)<=N ) {
                            first_hotel_order_ptr_2->order_count++;
                            count=first_hotel_order_ptr_2->order_count;
                            found_order =1;
                             if(count>max_count2 && count<max_count1)
                            {
                            	max_count2=count;
							}
                            //printf("checkpoint 4\n");
                            
                        }
                         first_hotel_order_ptr_2 = first_hotel_order_ptr_2->left;
                         first_hotel_order_ptr_2 = first_hotel_order_ptr_2->right;
                    }
                     while(first_hotel_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                       // printf("checkpoint 3\n");
                        if(!strcmp(first_hotel_order_ptr_2->hotel,first_order_ptr->hotel) && diff_of_days(first_order_ptr->date,date)<=N ) {
                            first_hotel_order_ptr_2->order_count++;
                            count=first_hotel_order_ptr_2->order_count;
                            found_order =1;
                             if(count>max_count3 && count<max_count2)
                            {
                            	max_count3=count;
							}
                            //printf("checkpoint 4\n");
                            
                        }
                         first_hotel_order_ptr_2 = first_hotel_order_ptr_2->left;
                         first_hotel_order_ptr_2 = first_hotel_order_ptr_2->right;
                    }
                     
//                        if(found_order==0  && diff_of_days(first_order_ptr->date,date)<=N ){
//                        add_order(first_hotel_order,first_order_ptr->order,first_order_ptr->price,first_order_ptr->date,first_order_ptr->agent_id,first_order_ptr->user_id,first_order_ptr->hotel);
//                        printf("checkpoint 5\n");
//                    
//                    }
//                    
//                    printf("checkpoint 6\n");
            //   }
              
                first_order_ptr = first_order_ptr->left;
                 first_order_ptr = first_order_ptr->right;
             }
                //printf("checkpoint 7\n");
   while(first_hotel_order_ptr!=NULL)
   {
   	if(first_hotel_order_ptr->order_count==max_count1)
   	{
   		printf("%s\n",first_hotel_order_ptr->hotel);
	}
	if(first_hotel_order_ptr->order_count==max_count2)
   	{
   		printf("%s\n",first_hotel_order_ptr->hotel);
	}
	if(first_hotel_order_ptr->order_count==max_count3)
   	{
   		printf("%s\n",first_hotel_order_ptr->hotel);
	}
	
	first_hotel_order_ptr=first_hotel_order_ptr->left;
	first_hotel_order_ptr=first_hotel_order_ptr->right;
	
   }

}

void find_fav_foods(int N,Order** first_order,Order** first_food_order){

    Date date;
    date.day = 1;
    date.month = 4;
    date.year = 2022;
    Order* first_order_ptr = *first_order;
    Order* first_hotel_order_ptr = *first_food_order; 
    int count,max_count1,max_count2,max_count3;
    count=max_count1=max_count2=max_count3=0;
    
    while(first_order_ptr != NULL )
    {
            
        // if(first_user_order_ptr==NULL){
                //printf("checkpoint 1\n");
                
                // if(!strcmp(first_order_ptr->user_id,user_id))
                // {
                   // printf("checkpoint 2\n");
                    Order* first_food_order_ptr_2 = *first_food_order;
                    
                   
                    int found_order = 0;
                    while(first_food_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                        //printf("checkpoint 3\n");
                        if(!strcmp(first_food_order_ptr_2->order,first_order_ptr->order) && diff_of_days(first_order_ptr->date,date)<=N ) {
                            first_food_order_ptr_2->order_count++;
                            count=first_food_order_ptr_2->order_count;
                            found_order =1;
                            if(count>max_count1)
                            {
                            	max_count1=count;
							}
                            //printf("checkpoint 4\n");
                        }
                         first_food_order_ptr_2 = first_food_order_ptr_2->left;
                         first_food_order_ptr_2 = first_food_order_ptr_2->right;
                    }
                       
                    while(first_food_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                        //printf("checkpoint 3\n");
                        if(!strcmp(first_food_order_ptr_2->order,first_order_ptr->order) && diff_of_days(first_order_ptr->date,date)<=N ) {
                            first_food_order_ptr_2->order_count++;
                            count=first_food_order_ptr_2->order_count;
                            found_order =1;
                            if(count>max_count2 && count<max_count1)
                            {
                            	max_count2=count;
							}
                            //printf("checkpoint 4\n");
                        }
                         first_food_order_ptr_2 = first_food_order_ptr_2->left;
                         first_food_order_ptr_2 = first_food_order_ptr_2->right;
                    }
                       
                       while(first_food_order_ptr_2!=NULL && found_order==0 ){

                        //  first_user_order_ptr_2->order_count = 1;
                        found_order = 0;
                        //printf("checkpoint 3\n");
                        if(!strcmp(first_food_order_ptr_2->order,first_order_ptr->order) && diff_of_days(first_order_ptr->date,date)<=N ) {
                            first_food_order_ptr_2->order_count++;
                            count=first_food_order_ptr_2->order_count;
                            found_order =1;
                            if(count>max_count3 && count<max_count2)
                            {
                            	max_count3=count;
							}
                            //printf("checkpoint 4\n");
                        }
                         first_food_order_ptr_2 = first_food_order_ptr_2->left;
                         first_food_order_ptr_2 = first_food_order_ptr_2->right;
                    }
                       

                    
//                        if(found_order==0  && diff_of_days(first_order_ptr->date,date)<=N ){
//                        add_order(first_food_order,first_order_ptr->order,first_order_ptr->price,first_order_ptr->date,first_order_ptr->agent_id,first_order_ptr->user_id,first_order_ptr->hotel);
//                        printf("checkpoint 5\n");
                    
                    
                    
                    //printf("checkpoint 6\n");
            //   }
              
                first_order_ptr = first_order_ptr->left;
                 first_order_ptr = first_order_ptr->right;
                //printf("checkpoint 7\n");
     
    }
    
   while(first_hotel_order_ptr!=NULL)
   {
   	if(first_hotel_order_ptr->order_count==max_count1)
   	{
   		printf("%s\n",first_hotel_order_ptr->order);
	}
	if(first_hotel_order_ptr->order_count==max_count2)
   	{
   		printf("%s\n",first_hotel_order_ptr->order);
	}
	if(first_hotel_order_ptr->order_count==max_count2)
   	{
   		printf("%s\n",first_hotel_order_ptr->order);
	}
	first_hotel_order_ptr=first_hotel_order_ptr->left;
	first_hotel_order_ptr=first_hotel_order_ptr->right;
   }

    


}



void print_area_wise_agents(Agent** first_agent,char area[100]){
    Agent* first_agent_ptr = *first_agent;

    if(*first_agent==NULL) return;
    while(first_agent_ptr!=NULL)
    {
        if(!strcmp(first_agent_ptr->area,area)) printf("%s -> %s\n",first_agent_ptr->agent_id,first_agent_ptr->name);

        first_agent_ptr = first_agent_ptr->left;
        first_agent_ptr = first_agent_ptr->right;
        
    }

    return;


}

void print_restaurant_details(Category** first_food_location,char hotel[20],char address[20]){

    Category* first_food_location_ptr = *first_food_location;

    int found_hotel = 0;
    while(first_food_location_ptr != NULL)
    {
        if(!strcmp(first_food_location_ptr->name,hotel) && !strcmp(first_food_location_ptr->address,address)){
            printf("%s -> %s\n",first_food_location_ptr->name,first_food_location_ptr->address);
            found_hotel = 1;
        } 
        
        first_food_location_ptr = first_food_location_ptr->left;
        first_food_location_ptr = first_food_location_ptr->right;
        
    }

    if(found_hotel==0) printf("Hotel not found\n");
    return;

}


void Range_search(int userid1,int userid2,User** firstuser,Order** firstorder)
{
   User* first_user_ptr= *firstuser;
   Order* next_order_ptr;
   Order* first_order_ptr= *firstorder;
   while(first_order_ptr!=NULL)
   {
   	if(first_order_ptr->user_id==userid1)
   	  next_order_ptr=first_order_ptr;
   	while(next_order_ptr->user_id!=userid2)
   	{
   		printf("Order no : %d\n",next_order_ptr->order_no);
        printf("Order : %s\n",next_order_ptr->order);
        printf("Order price : %d\n",next_order_ptr->price);
        printf("Order date : %d:%d:%d\n",next_order_ptr->date.day,next_order_ptr->date.month,next_order_ptr->date.year);
        printf("Order count : %d \n",next_order_ptr->order_count);
        printf(" %s\n",next_order_ptr->hotel);

        next_order_ptr = next_order_ptr->left;
        next_order_ptr=next_order_ptr->right;
       // found = 1;
         printf("-----------------\n");
   		
	   }
   	 
   }
  
}


int main()
{
	Category* first_food_location = NULL;
    Agent* first_agent = NULL;
    User* first_user=NULL;
    Dish* dish_ll = NULL;
   Order* first_order=NULL;

	// Insert 6. So linked list becomes 6->NULL

    food_location_input(&first_food_location);
    
    
    user_input(&first_user);
    agent_input(&first_agent);
    
	printf("Created Resto is: ");
	//print_user_list(first_user);
     print_food_location_list(first_food_location);
    print_user_list(first_user);
    

    User *user_ptr = first_user;
   while(user_ptr!=NULL){
   	user_ptr=user_ptr->left;
       user_ptr->dish_ll = NULL;
       user_ptr=user_ptr->right;
        //user_ptr = user_ptr->next;
        
   }


    FILE *fp1; // declare a file pointer
    char filename_1[] = "orders.txt"; // name of the input file

    fp1 = fopen(filename_1, "r");
   
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return 0;
    }


    // read the input values from the file
    struct order_input order_inp[NUMBER_OF_ORDERS];
    int count =  NUMBER_OF_ORDERS;
      
//
	int i;
    for( i =0 ; i < NUMBER_OF_ORDERS ;i++)
    {

        // char user_id[20];
        // char food_loc[20];
        // char dish[20];
        // int price;

        // printf("Enter user id :");
        // scanf("%s",user_id);

        // printf("Enter food location : ");
        // scanf("%s",food_loc);

        // printf("Enter dish : ");
        // scanf("%s",dish);

        // printf("Enter price : ");
        // scanf("%d",&price);

        fscanf(fp1," %s %d %s %d ",order_inp[i].user_id,order_inp[i].food_loc,order_inp[i].dish,&order_inp[i].price);  

   }

    for(i =0 ; i < NUMBER_OF_ORDERS ; i++){

      place_order( &first_food_location,&first_agent,&first_user,&first_order,order_inp[i].user_id,order_inp[i].food_loc,order_inp[i].dish,order_inp[i].price);

    }

     print_global_order_list(&first_order);
   
     Order* ptr = first_order;
        while(ptr!=NULL){
           printf("ORDER NO : %d\n",ptr->order_no);
           ptr = ptr->left;
           ptr=ptr->right;
       }
    search1(&first_food_location);
    search2(&first_food_location);
    search3(&first_food_location);
//     pair_order_agent(&first_order);
//     delivery(&first_order,&first_agent,"Fried_Rice",2,120);
//     cancel_order(&first_order,&first_agent,"Fried_Rice",2,120);
//     print_global_order_list(&first_order);
//     print_agent_list(first_agent);
//     pair_order_agent(&first_order);

//     fav_food_of_user(&first_order,&first_user,8);
//     fav_food_of_user(&first_order,&first_user,5);
//     //first_user_new_order = mergeSort(first_user_new_order);
//     //print_global_order_list(&first_user_new_order);
//
    // find_fav_restaurant(100,&first_order,&first_food_location);
//    // first_hotel_new_order = mergeSort(first_hotel_new_order);
//    // print_global_order_list(&first_hotel_new_order);
//
 //    find_fav_foods(100,&first_order,&first_food_location);
    // first_food_new_order = mergeSort(first_food_new_order);
    // print_global_order_list(&first_food_new_order);

     char area[100]="North";
     print_area_wise_agents(&first_agent,area);

    char hotel[20]="Hotel_B";
    char address[20]="Address_B";
    print_restaurant_details(&first_food_location,hotel,address);
    
  Range_search(5,8,&first_user,&first_order);
}
  
    



		
	     
