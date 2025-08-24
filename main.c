#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 10
#define MAX_PRODUCTS 10
#define MAX_CART 10
#define MAX_ORDERS 50

typedef struct {
    char username[20];
    char password[20];
    int isAdmin; // 1 = admin, 0 = user
} User;

typedef struct {
    int id;
    char name[30];
    float price;
    int stock;
} Product;

typedef struct {
    int pid;
    int qty;
} CartItem;

typedef struct {
    char username[20];
    CartItem items[MAX_CART];
    int cart_count;
} Order;

User users[MAX_USERS] = {{"admin","admin123",1}};
int user_count = 1;

Product products[MAX_PRODUCTS];
int product_count = 0;

Order orders[MAX_ORDERS];
int order_count = 0;

// ---------- Functions ----------

void add_sample_products() {
    products[0] = (Product){1,"T-Shirt",499.0,10};
    products[1] = (Product){2,"Jeans",999.0,5};
    products[2] = (Product){3,"Mouse",299.0,15};
    product_count = 3;
}

int login(char *uname) {
    char u[20], p[20];
    printf("Username: "); scanf("%s", u);
    printf("Password: "); scanf("%s", p);
    for(int i=0;i<user_count;i++){
        if(strcmp(users[i].username,u)==0 && strcmp(users[i].password,p)==0){
            strcpy(uname,u);
            return users[i].isAdmin;
        }
    }
    return -1; // login failed
}

void register_user() {
    if(user_count >= MAX_USERS){ printf("User limit reached.\n"); return; }
    char u[20], p[20];
    printf("Enter username: "); scanf("%s", u);
    printf("Enter password: "); scanf("%s", p);
    for(int i=0;i<user_count;i++){
        if(strcmp(users[i].username,u)==0){ printf("Username exists.\n"); return; }
    }
    strcpy(users[user_count].username,u);
    strcpy(users[user_count].password,p);
    users[user_count].isAdmin = 0;
    user_count++;
    printf("Registration successful!\n");
}

void list_products() {
    printf("\nID\tName\tPrice\tStock\n");
    for(int i=0;i<product_count;i++)
        printf("%d\t%s\t%.2f\t%d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
}

void add_product() {
    if(product_count >= MAX_PRODUCTS){ printf("Product limit reached.\n"); return; }
    Product p;
    p.id = product_count+1;
    printf("Product name: "); scanf("%s", p.name);
    printf("Price: "); scanf("%f", &p.price);
    printf("Stock: "); scanf("%d", &p.stock);
    products[product_count++] = p;
    printf("Product added successfully.\n");
}

void update_product() {
    int id; printf("Enter Product ID to update: "); scanf("%d",&id);
    for(int i=0;i<product_count;i++){
        if(products[i].id == id){
            printf("New Name: "); scanf("%s", products[i].name);
            printf("New Price: "); scanf("%f", &products[i].price);
            printf("New Stock: "); scanf("%d", &products[i].stock);
            printf("Product updated.\n"); return;
        }
    }
    printf("Product not found.\n");
}

void delete_product() {
    int id; printf("Enter Product ID to delete: "); scanf("%d",&id);
    for(int i=0;i<product_count;i++){
        if(products[i].id == id){
            for(int j=i;j<product_count-1;j++)
                products[j]=products[j+1];
            product_count--;
            printf("Product deleted.\n"); return;
        }
    }
    printf("Product not found.\n");
}

void add_to_cart(char *uname) {
    int pid, qty;
    printf("Product ID: "); scanf("%d",&pid);
    printf("Quantity: "); scanf("%d",&qty);
    for(int i=0;i<product_count;i++){
        if(products[i].id==pid){
            if(products[i].stock>=qty){
                int found=0;
                for(int j=0;j<order_count;j++){
                    if(strcmp(orders[j].username,uname)==0){
                        orders[j].items[orders[j].cart_count++] = (CartItem){pid,qty};
                        found=1;
                        break;
                    }
                }
                if(!found){
                    strcpy(orders[order_count].username,uname);
                    orders[order_count].items[0]=(CartItem){pid,qty};
                    orders[order_count].cart_count=1;
                    order_count++;
                }
                products[i].stock -= qty;
                printf("Added to cart.\n");
            } else printf("Not enough stock.\n");
            return;
        }
    }
    printf("Product not found.\n");
}

void view_cart(char *uname){
    printf("\nCart for %s:\nID\tName\tQty\tPrice\tTotal\n",uname);
    for(int i=0;i<order_count;i++){
        if(strcmp(orders[i].username,uname)==0){
            float total=0;
            for(int j=0;j<orders[i].cart_count;j++){
                int pid = orders[i].items[j].pid;
                int qty = orders[i].items[j].qty;
                for(int k=0;k<product_count;k++){
                    if(products[k].id==pid){
                        float line = qty*products[k].price;
                        total+=line;
                        printf("%d\t%s\t%d\t%.2f\t%.2f\n", pid, products[k].name, qty, products[k].price, line);
                    }
                }
            }
            printf("Grand Total: %.2f\n", total);
            return;
        }
    }
    printf("Cart empty.\n");
}

// ---------- Main ----------

int main() {
    add_sample_products();
    int role;
    char uname[20];
    int choice;
    printf("1.Login\n2.Register\nChoice: "); scanf("%d",&choice);
    if(choice==1){
        role = login(uname);
        if(role==-1){ printf("Invalid credentials.\n"); return 0; }
    } else if(choice==2){
        register_user();
        role = login(uname);
        if(role==-1){ printf("Invalid credentials.\n"); return 0; }
    } else return 0;

    printf("Welcome %s!\n", uname);

    while(1){
        if(role==1){
            printf("\nAdmin Menu:\n1.List Products\n2.Add Product\n3.Update Product\n4.Delete Product\n0.Exit\nChoice: ");
            scanf("%d",&choice);
            if(choice==1) list_products();
            else if(choice==2) add_product();
            else if(choice==3) update_product();
            else if(choice==4) delete_product();
            else if(choice==0) break;
            else printf("Invalid choice.\n");
        } else {
            printf("\nUser Menu:\n1.List Products\n2.Add to Cart\n3.View Cart\n0.Exit\nChoice: ");
            scanf("%d",&choice);
            if(choice==1) list_products();
            else if(choice==2) add_to_cart(uname);
            else if(choice==3) view_cart(uname);
            else if(choice==0) break;
            else printf("Invalid choice.\n");
        }
    }
    printf("Thanks for shopping!\n");
    return 0;
}
