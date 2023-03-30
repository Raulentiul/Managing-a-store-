#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product { ///defining a product
    int code;
    char name[50];
    float price;
    int quantity;
    char recDate[11];
    char expDate[11];
} Product;

typedef struct Node {  ///self referential structure
    Product val;
    struct Node *next;
} Node;

typedef struct List { ///defining a list
    Node *head;
    int size;
} List;

Product createProduct(int code, char *name, float price, int quantity, char *recDate, char* expDate){  ///creating struct products
    Product product;
    product.code = code;
    strcpy(product.name, name);
    product.price = price;
    product.quantity = quantity;
    strcpy(product.recDate, recDate);
    strcpy(product.expDate, expDate);
    return product;
}

void appendSorted(List *l, Product p) { ///put the products in an arranged list
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->val = p; ///assign the product to the new node

    if (l->head == NULL || l->head->val.price >= p.price) { ///if the list is empty or the new product has the smallest price
        newNode->next = l->head; ///set the new node as the head of the list
        l->head = newNode;
    } else {
        Node *curr = l->head;
        while (curr->next != NULL && curr->next->val.price < p.price) { ///find the position of the new node
            curr = curr->next;
        }
        newNode->next = curr->next; ///insert the new node after the current node
        curr->next = newNode;
    }
    l->size++; ///increase the size of the list
}

Node* findByCode(List l, int code) { ///function that finds by code a product
    Node *current = l.head; ///the current node is the head

    while (current != NULL) {
        if (current->val.code == code) { ///if the current code is the code we are searching for, return it
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void deleteByCode(List *l, int code) { ///a function that deletes a product by its code
    Node *curr = l->head;
    Node *prev = NULL;

    while (curr != NULL) {
        if (curr->val.code == code) {
            if (prev == NULL) { ///if the node to delete is the head, set the next node as the head
                l->head = curr->next;
            } else {
                prev->next = curr->next; ///remove the current node from the list
            }
            free(curr); ///free the memory allocated for the current node
            l->size--; ///decrease the size of the list
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void deleteExpired(List *l, char *date) { ///a function that deletes all expired products
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year); ///parsing the date

    Node *curr = l->head;
    Node *prev = NULL;

    while (curr != NULL) {
        int expDay, expMonth, expYear;
        sscanf(curr->val.expDate, "%d/%d/%d", &expDay, &expMonth, &expYear); ///parsing the expiration date

        if (expYear < year || (expYear == year && expMonth < month) || (expYear == year && expMonth == month && expDay < day)) { ///compare the expiration date with the current date
            if (prev == NULL) {
                l->head = curr->next;
                free(curr);
                curr = l->head;
            } else {
                prev->next = curr->next; ///remove the current node from the list
                free(curr);
                curr = prev->next;
            }
            l->size--; ///decrease the size of the list
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}


void printValid(List l, char *date) { ///function that prints all products that have not expired
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year); ///we are parsing the date string

    Node *curr = l.head;
    while (curr != NULL) {
        int expDay, expMonth, expYear;
        sscanf(curr->val.expDate, "%d/%d/%d", &expDay, &expMonth, &expYear); ///we are parsing the expiration date

        if (expYear > year || (expYear == year && expMonth > month) || (expYear == year && expMonth == month && expDay >= day)) { ///compare the expiration date with the given date
            printf("%d ", curr->val.code);
            printf("%s ", curr->val.name);
            printf("%.1f ", curr->val.price);
            printf("%d ", curr->val.quantity);
            printf("%s ", curr->val.recDate);
            printf("%s", curr->val.expDate);
            printf("\n");
        }
        curr = curr->next;
    }
}

void printExpired(List l, char *date) { ///a function that prints all products that have expired (similarly to the last function)
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year);

    Node *curr = l.head;
    while (curr != NULL) {
        int expDay, expMonth, expYear;
        sscanf(curr->val.expDate, "%d/%d/%d", &expDay, &expMonth, &expYear);

        if (expYear < year || (expYear == year && expMonth < month) || (expYear == year && expMonth == month && expDay < day)) { ///compare the expiration date with the given date
            printf("%d ", curr->val.code);
            printf("%s ", curr->val.name);
            printf("%.1f ", curr->val.price);
            printf("%d ", curr->val.quantity);
            printf("%s ", curr->val.recDate);
            printf("%s", curr->val.expDate);
            printf("\n");
        }
        curr = curr->next;
    }

}

void print(List l) { ///function that prints the products
    Node *curr = l.head; ///curr means current
    while (curr != NULL) {
        printf("%d ", curr->val.code);
        printf("%s ", curr->val.name);
        printf("%.1f ", curr->val.price);
        printf("%d ", curr->val.quantity);
        printf("%s ", curr->val.recDate);
        printf("%s", curr->val.expDate);
        printf("\n");
        curr = curr->next;
    }
}

int main(){
    int n, code, quantity;
    float price;
    char name[50], recDate[50], expDate[50];
    scanf("%d", &n);

    List l;
    l.head = NULL; ///creating the head
    while(n){ ///introducing the products
        scanf("%d", &code);
        scanf("%s", name);
        scanf("%f", &price);
        scanf("%d", &quantity);
        scanf("%s", recDate);
        scanf("%s", expDate);
        appendSorted(&l, createProduct(code, name, price, quantity, recDate, expDate)); ///create products and sort them by their price
        n--;
    }

    printf("test 1\n");  ///first test: print all the products
    print(l);

    printf("test 2\n"); ///second test: change the name of a selected product
    Node *p = findByCode(l, 102); ///a function that looks for a product by its code
    strcpy(p->val.name, "Poiana"); ///changing the name of the product
    print(l);

    printf("test 3\n"); ///third test: print all products that have not expired yet
    printValid(l, "20/03/2020");

    printf("test 4\n"); ///fourth test: print all products that have expired
    printExpired(l, "20/03/2020");

    printf("test 5\n"); ///fifth test: delete a product by its code
    deleteByCode(&l, 104);
    print(l);

    printf("test 6\n"); ///sixth test: delete all expired products
    deleteExpired(&l, "20/03/2020");
    print(l);

    return 0;
}

