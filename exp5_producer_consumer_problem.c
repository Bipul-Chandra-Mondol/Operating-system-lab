#include <stdio.h>
#include <stdlib.h>

// Initializing the mutex variable with the value 1.
int mutex = 1;

// Initializing the full variable with the value 0.
int full = 0;

// empty variable will store the number of empty slots in the buffer.
int empty = 10, data = 0;

// A function that will resemble producers' production of data
void producer()
{
    // decrementing the value of mutex
    --mutex;

    // Increase the number of full slots
    ++full;

    // decrementing the number of slots available
    --empty;

    // incrementing data which means that the data is produced.
    data++;

    printf("\nProducer produces item number: %d\n", data);

    // incrementing the value of mutex
    ++mutex;
}

// A function that will resemble the consumer's consumption of data
void consumer()
{
    // decrementing the value of mutex
    --mutex;

    // Decrease the number of full slots
    --full;

    // incrementing the number of slots available
    ++empty;

    printf("\nConsumer consumes item number: %d.\n", data);

    // since data is consumed, let us decrease the value of data
    data--;

    // incrementing the value of mutex
    ++mutex;
}

int main()
{
    int n, i;
    printf("\n1. Enter 1 for Producer"
           "\n2. Enter 2 for Consumer"
           "\n3. Enter 3 to Exit");

    for (i = 1; i > 0; i++)
    {
        printf("\nEnter your choice: ");
        scanf("%d", &n);

        // using switch case as there can be multiple types of choice.
        switch (n)
        {
        case 1:
            /*
            If the value of mutex is 1 and the buffer is not full, then we can produce the data
            */
            if ((mutex == 1) && (empty != 0))
            {
                producer();
            }

            // else, the buffer must be full.
            else
            {
                printf("The Buffer is full. New data cannot be produced!");
            }
            break;

        case 2:
            /*
            If the value of mutex is 1 and the buffer is not empty, then we can consume the data
            */
            if ((mutex == 1) && (full != 0))
            {
                consumer();
            }

            // else, the buffer must be empty.
            else
            {
                printf("The Buffer is empty! New data cannot be consumed!");
            }
            break;

        // Exit Condition
        case 3:
            exit(0);
            break;
        }
    }
}



// source: https://www.scaler.com/topics/producer-consumer-problem-in-c/

//input output demo:
/* 
1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 1
Producer produces item number: 1

1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 2
Consumer consumes item number: 1.

1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 1
Producer produces item number: 2

1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 1
Producer produces item number: 3

1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 2
Consumer consumes item number: 3.

1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 2
Consumer consumes item number: 2.

1. Enter 1 for Producer
2. Enter 2 for Consumer
3. Enter 3 to Exit
Enter your choice: 2
The Buffer is empty! New data cannot be consumed!
Enter your choice: 3
*/