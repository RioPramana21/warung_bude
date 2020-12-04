#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX_INDEX_CUST 25

/*
KENDALA:
1. Untuk print bude's menu tidak rata tengah karena tidak sempat, jadi saya hanya berfokus pada fungsionalitasnya saja
2. Filenya tidak dipisah dalam header files karena pada saat dipisah agak kebingungan dengan cara menempatkan global pointersnya
karena ada beberapa function controllers.h yang error pada saat dipisah (semua structnya saya coba masukin ke models.h dan diinclude)
*/

struct Dish {
    char name[255];
    int price, qty;
    Dish *prev, *next;
}*headDish = NULL, *tailDish = NULL, *currDish = NULL, *newDish = NULL, *removedDish = NULL;

struct Order {
    char name[255];
    int qty, price;
    Order *prev, *next;
}*newOrder = NULL, *currOrder = NULL, *delOrder = NULL;

struct Customer {
    char name[255];
    Order *headOrder, *tailOrder;
    Customer *prev, *next;    
}*headCustomer[26], *tailCustomer[26], *currCustomer = NULL, *tempCustomer = NULL, *newCust = NULL;

void mainMenu();
int parseInputMenu(char* s);
Dish* createNewDish(char* name, int price, int qty);
void pushDish(char* name, int price, int qty);
bool isUniqueDish(char* name);
bool validateDishName(char* name);
void addDish();
void printAllDish();
bool validateRemovedDishName(char* name);
Dish* searchDish(char* name);
void popHeadDish();
void popTailDish();
void popDish(char* name);
void removeDish();
bool validateCustomerName(char* name);
unsigned long int DJB2(char* name);
Customer* createCustomer(char* name);
void insertCustomer(char* name);
void addCustomer();
Customer* findCustomer(char* name);
void searchCustomer();
void printAllCustomer();
void viewWarteg();
Order* createNewOrder(char* name, int price, int qty);
void pushOrder(Customer* c, char* name, int price, int qty);
Order* popOrder(Customer* c);
void clearOrder(Customer* c);
bool processOrder(char* input, char* name, int qty);
void order();
void deleteCustomer(char* name);
void payment();
void exitWarteg();

const char* detectOS() {
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}

//////////////////////// MAIN ///////////////////////////
int main() {
    mainMenu();

    return 0;
}
//////////////////////// MAIN ///////////////////////////

void displayTime(){
    time_t current_time = time(NULL);
    struct tm *tm = localtime(&current_time);
    printf("%s\n", asctime(tm));
    //asctime menerima pointer yang menunjuk ke struct tm kemudian timenya diubah menjadi bentuk string
}

void mainMenu(){
    char input[255] = {};
    int menu = 0;
    do{
        system("cls||clear");
        printf("%s\n", detectOS());
        displayTime();
        puts("1. Add Dish");
        puts("2. Remove Dish");
        puts("3. Add Customer");
        puts("4. Search Customer");
        puts("5. View Warteg");
        puts("6. Order");
        puts("7. Payment");
        puts("8. Exit Warteg");
        printf(">> ");
        scanf("%[^\n]", input);
        getchar();
        menu = parseInputMenu(input);
        switch(menu){
            case 1:
                addDish();
                break;
            case 2:
                removeDish();
                break;
            case 3:
                addCustomer();
                break;
            case 4:
                searchCustomer();
                break;
            case 5:
                viewWarteg();
                break;
            case 6:
                order();
                break;
            case 7:
                payment();
                break;
            case 8:
                exitWarteg();
                break;
        }
    }while(menu != 8); //selama tidak exit atau input invalid akan mengulang terus
}

int parseInputMenu(char* s){
    if (strlen(s) != 1) return 0; //kalau lengthnya bukan 1 sudah pasti input menunya invalid
    if (*s >= 0 || *s <= 9) return (*s)-'0'; //kalau lengthnya 1 return nilai int nya
    return 0;
}

Dish* createNewDish(char* name, int price, int qty){
	newDish = (Dish*)malloc(sizeof(Dish));
    strcpy(newDish->name, name);
	newDish->price = price;
    newDish->qty = qty;
	newDish->next = NULL;
	newDish->prev = NULL;
	return newDish;
}

void pushDish(char* name, int price, int qty){
    newDish = createNewDish(name,price,qty);
    if (tailDish == NULL){
        headDish = tailDish = newDish;
    }
    else{
        newDish->prev = tailDish;
        tailDish->next = newDish;
        tailDish = newDish;
    }
}

bool isUniqueDish(char* name){
    if (!headDish) return true;
    currDish = headDish;
    while(currDish){
        if (!(strcmp(currDish->name,name))) return false;
        currDish = currDish->next;
    }
    return true;
}

bool validateDishName(char* name){
    if (!isUniqueDish(name)) return false; //kalau namanya unik berarti sudah pasti dish yang dicari tidak ada
    int len = strlen(name);
    for (int i = 0; i < len; ++i){
        if (*(name+i) != ' ' && (*(name+i) < 'a' || *(name+i) > 'z')) return false; //validasi lowercase
    }
    return true;
}

void addDish(){
    char name[255] = {};
    int price = 0, qty = 0;
    system("cls||clear");
    do{
        printf("Insert the name of the dish [Lowercase letters]: ");
        scanf("%[^\n]", name);
        getchar();
    }while(!(validateDishName(name)));
    do{
        printf("Insert the price of the dish [1000..50000]: ");
        scanf("%d", &price);
        getchar();
    }while(price < 1000 || price > 50000);
    do{
        printf("Insert the quantity of the dish [1..999]: ");
        scanf("%d", &qty);
        getchar();
    }while(qty < 1 || qty > 999);
    pushDish(name,price,qty);
    puts("The dish has been added!");
    printf("Press enter to continue...");
    getchar();
}

void printAllDish(){
    puts("               Bude's Menu");
    puts("==============================================");
    puts("No.       Name                Quantity    Price");
	currDish = headDish;
    int nomor = 1;
	while(currDish){
		printf("%d.  %-29s%00d   Rp%d\n", nomor, currDish->name, currDish->qty, currDish->price);
		currDish = currDish->next;
        nomor++;
	}
    puts("==============================================");
}

bool validateRemovedDishName(char* name){
    int len = strlen(name);
    for (int i = 0; i < len; ++i){
        if (*(name+i) != ' ' && (*(name+i) < 'a' || *(name+i) > 'z')) return false;
    }
    return true;
}

Dish* searchDish(char* name){
    if (!headDish) return NULL;
    currDish = headDish;
    while(currDish){
        if (!(strcmp(currDish->name,name))) return currDish;
        currDish = currDish->next;
    }
    return NULL;
}

void popHeadDish(){
    if (!headDish) return;
    currDish = headDish;
    if (headDish == tailDish){
        headDish = tailDish = NULL;
    }
    else{
        headDish = headDish->next;
        headDish->prev = NULL;
    }
}

void popTailDish(){
    if (!headDish) return;
    currDish = tailDish;
    if (headDish == tailDish){
        headDish = tailDish = NULL;
    }
    else{
        tailDish = tailDish->prev;
        tailDish->next = NULL;
    }
}

void popDish(char* name){
	if (!strcmp(headDish->name,name)){
		popHeadDish();
	}
	else if (!strcmp(tailDish->name,name)){
		popTailDish();
	}
	else{
		currDish = headDish;
		while(currDish){
			if (!strcmp(currDish->name,name)){
				break;
			}
			currDish = currDish->next;
		}
        currDish->prev->next = currDish->next;
        currDish->next->prev = currDish->prev;
	}
}

void removeDish(){
    system("cls||clear");
    if (!headDish){
        puts("There is no dish, please add more dish to the menu");
        printf("Press enter to continue...");
        getchar();
        return;
    }
    printAllDish();
    char name[255] = {};
    do{
        printf("Insert dish's name to be deleted: ");
        scanf("%[^\n]", name);
        getchar();
        removedDish = searchDish(name); //cari dishnya
    }while(!validateRemovedDishName(name) || !removedDish); //validasi input + dish tidak ketemu
    popDish(name); //kalau ketemu langsung remove
    puts("The dish has been removed!");
    printf("Press enter to continue...");
    getchar();
}

bool validateCustomerName(char* name){
    int len = strlen(name);
    for (int i = 0; i < len; ++i){
        if (*(name+i) == ' ' || (*(name+i) >= '0' && *(name+i) <= '9')){ //validasi no space dan only alphabet
            return false;
        }
    }
    return true;
}

unsigned long int DJB2(char* name){
    unsigned long int hash = 5381;
    int c;
    while(c = *name++){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % (MAX_INDEX_CUST+1); //di modulo 26 karena menggunakan index 0-25
}

Customer* createCustomer(char* name){
    tempCustomer = (Customer*)malloc(sizeof(Customer));
	strcpy(tempCustomer->name, name);
    tempCustomer->headOrder = tempCustomer->tailOrder = NULL;
	tempCustomer->next = tempCustomer->prev = NULL;
	return tempCustomer;
}

void insertCustomer(char* name){
	int index = DJB2(name);
	newCust = createCustomer(name);
	if(!headCustomer[index]){
		headCustomer[index] = tailCustomer[index] = newCust;
	}
	else{
		tailCustomer[index]->next = newCust;
		newCust->prev = tailCustomer[index];
		tailCustomer[index] = newCust;
	}
}

void addCustomer(){
    system("cls||clear");
    char name[255] = {};
    do{
        printf("Insert the customer's name [Without space]: ");
        scanf("%[^\n]", name);
        getchar();
    }while(!validateCustomerName(name));
    insertCustomer(name);
    puts("Customer has been added!");
    printf("Press enter to continue...");
    getchar();
}

Customer* findCustomer(char* name){
    int index = DJB2(name);
    if (!headCustomer[index]){
        return NULL;
    }
    currCustomer = headCustomer[index];
    while(currCustomer){
        if(strcmp(currCustomer->name, name) == 0){
            return currCustomer;
            break;
        }
        currCustomer = currCustomer->next;
    }
    return NULL;
}

void searchCustomer(){
    system("cls||clear");
    char name[255] = {};
    do{
        printf("Insert the customer's name to be searched: ");
        scanf("%[^\n]", name);
        getchar();
    } while(!validateCustomerName(name));
    tempCustomer = findCustomer(name);
    if (!tempCustomer){
        printf("%s is not present.\n", name);
    }
    else{
        printf("Name : %s\n", tempCustomer->name);
        printf("Index: %d\n", DJB2(tempCustomer->name));
    }
    printf("Press enter to continue...");
    getchar();
}

void printAllCustomer(){
    bool flag = false;
	for (int i = 0; i < 26; ++i){
		if(headCustomer[i]){
            if (!flag){
                puts("Customer's List");
            }
            flag = true;
			currCustomer = headCustomer[i];
			printf("%d. %s", i, currCustomer->name);
            currCustomer = currCustomer->next;
			while(currCustomer){
				printf(" -> %s", currCustomer->name);
				currCustomer = currCustomer->next;
			}
		    printf("\n");
		}
	}
    if (!flag){ //kalau ga ada customer print error message
        puts("There is no customer");
    }
}

void viewWarteg(){
    system("cls||clear");
    printAllCustomer();
    printf("Press enter to continue...");
    getchar();
}

Order* createNewOrder(char* name, int price, int qty){
	newOrder = (Order*)malloc(sizeof(Order*));
	newOrder->price = price;
    newOrder->qty = qty;
    strcpy(newOrder->name,name);
	newOrder->next = NULL;
	newOrder->prev = NULL;
	return newOrder;
}

void pushOrder(Customer* c, char* name, int price, int qty){
    newOrder = createNewOrder(name,price,qty);
    if (c->tailOrder == NULL){
        c->headOrder = c->tailOrder = newOrder;
    }
    else{
        newOrder->prev = c->tailOrder;
        c->tailOrder->next = newOrder;
        c->tailOrder = newOrder;
    }
}

Order* popOrder(Customer* c){
	if (!c->headOrder) return NULL;
    delOrder = c->headOrder;
    if (c->headOrder == c->tailOrder){
        c->headOrder = c->tailOrder = NULL;
    }
    else{
        c->headOrder = c->headOrder->next;
        c->headOrder->prev = NULL;
    }
    return delOrder;
}

void clearOrder(Customer* c){
	if (!c->headOrder) return;
    currOrder = c->headOrder;
    while(currOrder){
        delOrder = popOrder(c);
        currOrder = currOrder->next;
        free(delOrder);
    }
}

bool processOrder(char* input, char* name, int qty){
    int len = strlen(input), multiplier = 10;
    bool flagX = false; //kalau true artinya udah ketemu 'x'
    for (int i = len-1; i >= 0; --i){ //loop dari akhir
        if (!flagX){
            if (*(input+i) < '0' || *(input+i) > '9'){ //kalau charnya bukan angka
                if (*(input+i) == 'x' && *(input+i-1) == ' '){ //kalau charnya itu 'x' dan char sebelumnya itu ' '
                    flagX = true;
                    for (int k = i+1; k < len; ++k){ //mulai scan angkanya lagi dari index i+1 untuk qty
                        qty = qty*multiplier + (*(input+k) - '0');
                    }
                    i--; //buat skip ' '
                    *(name+i) = '\0'; //buat taruh null terminator di namenya
                }
                else return false;
            }
        }
        else if (flagX){ //kalau sudah true flagX nya tinggal scan satu per satu char nya ke name dari belakang
            *(name+i) = *(input+i);
        }
    }
    currDish = searchDish(name); //cari dishnya
    if (!currDish) return false; //kalau tidak ada berarti input invalid
    else if (currDish->qty < qty || qty <= 0) return false; //validasi jumlah qty
    else{
        currDish->qty -= qty;
        if (currDish->qty == 0) popDish(currDish->name);
        pushOrder(tempCustomer,name,currDish->price,qty);
        return true;
    }
}

void order(){
    system("cls||clear");
    if (!headDish){
        puts("There is no dish available on the menu, please add more");
    }
    else{
        char name[255] = {}, choice[255] = {}, dishName[255] = {};
        int total = 0, qty = 0;
        do{
            printf("Insert the customer's name: ");
            scanf("%[^\n]", name);
            getchar();
            tempCustomer = findCustomer(name);
        }while(!tempCustomer);
        do{
            printf("Insert the amount of dish: ");
            scanf("%d", &total);
            getchar();
        }while(total < 1);
        for (int i = 1; i <= total; ++i){
            qty = 0;
            printf("[%d] Insert the dish's name and quantity: ", i);
            scanf("%[^\n]", choice);
            getchar();
            if(!processOrder(choice,name,qty)) i--; //kalau ada yang invalid, i-- untuk mengulang input
        }
        puts("Order success!");
    }
    printf("Press enter to continue...");
    getchar();
}

void deleteCustomer(char* name){
	int index = DJB2(name);
    //popheadCustomer
    if(strcmp(headCustomer[index]->name, name) == 0){
        //kondisi cuma ada 1 data
        if(headCustomer[index] == tailCustomer[index]){
            free(headCustomer[index]);
            headCustomer[index] = tailCustomer[index] = NULL;
        }
        else{
            tempCustomer = headCustomer[index];
            headCustomer[index] = headCustomer[index]->next;
            free(tempCustomer);
            headCustomer[index]->prev = NULL;
        }
    }
}

void payment(){
    system("cls||clear");
    int index = 0;
    do{
        printf("Insert the customer's index: ");
        scanf("%d", &index);
        getchar();
    }while(index < 0 || index > 25);
    if(!headCustomer[index]){
        printf("There is no customer with index %d\n", index);
    }
    else if (!(headCustomer[index]->headOrder)){
        puts("This customer has no order");
    }
    else{
        int nomor = 1, totalHarga = 0;
        printf("%s\n", headCustomer[index]->name);
        currOrder = headCustomer[index]->headOrder;
        while(currOrder){
            printf("[%d] %s x%d\n", nomor, currOrder->name, currOrder->qty);
            nomor++;
            totalHarga += currOrder->qty*currOrder->price;
            currOrder = currOrder->next;
        }
        printf("Total: Rp%d\n", totalHarga);
        clearOrder(headCustomer[index]); //pop semua order milik customernya
        deleteCustomer(headCustomer[index]->name); //baru pop customernya
    }
    printf("Press enter to continue...");
    getchar();
}

void exitWarteg(){
    puts("Please expand your terminal to full screen!");
    printf("Press enter to continue...");
    getchar();
    system("cls||clear");
    char s[255] = {};
    FILE *fp = fopen("splash-screen.txt", "r");
    while(!feof(fp)){
        fscanf(fp, "%[^\n]\n", s);
        printf("%s\n", s);
    }
    fclose(fp);
}