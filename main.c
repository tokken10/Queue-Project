#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define LINES        12

int currentTime;

//Customer Struct
typedef struct customer {
    char customerName[9];
    int lineNumber;
    int time;
    int numberItems;
} customer;

//NODE
typedef struct node {

    customer* data;
    struct node *next;

} node;


// QUEUE Struct
typedef struct queue {

    node *front;
    node *back;
    int size;

} queue;


queue queue_list[LINES];



void initialize(queue *q)
{
    q->front = q->back = NULL;
    q->size = 0;
  //  q->back = NULL;

}


int isEmpty(queue *q)
{
    return (q->front == NULL);
}



void enqueue(queue *q, customer* data)
{
 //temp node
    node* temp = (node*)malloc(sizeof(struct node));
    temp->data = data;
    temp->next = NULL;


    if (q->back == NULL) {
        q->front = q->back = temp;
        return;
    }

    q->back->next = temp;
    q->back = temp;
}


customer* dequeue(queue *q)
{

    customer* data;


    if (q->front == NULL){
        data = NULL;
        return;
    }

    data = q->front->data;


    node* temp = q->front;

    q->front = q->front->next;

    // If front becomes NULL, then change back also as NULL
    if (q->front == NULL)
        q->back = NULL;

    free(temp);
    return data;
}


customer* peek(queue *q)
{
    node *temp;
    customer* data;

    temp = q->front;
    if (temp != NULL)
        data = temp->data;
    else
        data = NULL;

    return data;
}


customer * seekCustomer(void)
{
    customer *currentCustomer;
    customer *bestCustomer;
    int pass;
    int queue_number;
    queue *q;
    int mintime;
    int flag;



    for (pass = 1;  pass <= 2;  ++pass) {

        bestCustomer = NULL;
        mintime = INT_MAX;
        flag = 0;

        // loop through the queue and find customer
        for (queue_number = 0;  queue_number < LINES;  ++queue_number) {

            q = &queue_list[queue_number];


            currentCustomer = peek(q);
            if (currentCustomer == NULL)
                continue;

           flag = 1;

            // minimum time
           if (currentCustomer->time < mintime)
                mintime = currentCustomer->time;

            if (currentCustomer->time > currentTime)
                continue;

            if (bestCustomer == NULL) {
                bestCustomer = currentCustomer;
                continue;
            }

            if (currentCustomer->numberItems < bestCustomer->numberItems) {
                bestCustomer = currentCustomer;
                continue;
            }
        }


        if (! flag)
            break;


        // dequeue and free node
        if (bestCustomer != NULL) {

            q = &queue_list[bestCustomer->lineNumber - 1];
            dequeue(q);
            break;

        }

        currentTime = mintime;
    }

    return bestCustomer;
}


void custdo(customer *customer)
{
    int totalTime;


    if (currentTime < customer->time)
        currentTime = customer->time;


    totalTime = 0;
    totalTime += 30;
    totalTime += customer->numberItems * 5;


    currentTime += totalTime;

    printf("%s from line %d checks out at time %d.\n",
        customer->customerName,customer->lineNumber,currentTime);

    free(customer);
}


void testcase(FILE *file)
{
    queue *q;
    customer *customer;


    for (int queue_number = 0;  queue_number < LINES;  ++queue_number) {

        q = &queue_list[queue_number];
        initialize(q);
    }


    int numCustomers;
    fscanf(file," %d", &numCustomers);

    // read in all customer data
    for (int i = 0;  i < numCustomers;  ++i) {

        customer = malloc(sizeof(*customer));

        fscanf(file," %d", &customer->time);
        fscanf(file," %d", &customer->lineNumber);
        fscanf(file," %s", customer->customerName);
        fscanf(file," %d", &customer->numberItems);


        q = &queue_list[customer->lineNumber - 1];
        enqueue(q,customer);
    }


    currentTime =0;

    while (1) {

        customer = seekCustomer();


        if (customer == NULL)
            break;

        custdo(customer);
    }
}

int main(int argc, char **argv)
{
    char *newFile;
    FILE *file;

    --argc;
    ++argv;

    // open the input file
    if (argc > 0)
        newFile = *argv;

    else
        newFile = "assignment2input.txt";
    file = fopen(newFile,"r");

    if (file == NULL) {
        perror(newFile);
        exit(1);
    }

    // test cases
    int testCases;
    fscanf(file," %d", &testCases);

    // process
    for (;  testCases > 0;  --testCases)
        testcase(file);

    fclose(file);

    return 0;
}
