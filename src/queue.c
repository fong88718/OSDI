#include "queue.h"

void queue_push(Queue *q, char c)
{
    if(queue_full(q))
        return;
    q->buf[q->rear] = c;
    q->rear = (q->rear + 1) % MAX_SIZE;
}
char queue_pop(Queue *q)
{
    if(queue_empty(q))
        return 0;
    char c = q->buf[q->front];
    q->front = (q->front + 1) % MAX_SIZE;
    return c;
}
int queue_empty(Queue *q)
{
    return q->front == q->rear;
}
int queue_full(Queue *q)
{
    return q->front == (q->rear + 1) % MAX_SIZE;
}
void queue_init(Queue *q)
{
    q->front = q->rear = 0;
}