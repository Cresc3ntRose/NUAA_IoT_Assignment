#include <thread.h>
#include <thread-sync.h>

#define CAN_PRODUCE (fruit_in_dish == 0)
#define CAN_CONSUME (fruit_in_dish == 1)

typedef enum FruitType // 水果类型
{
    NONE, // 无
    ORANGE, // 橙子
    APPLE // 苹果
}FruitType;

int fruit_in_dish; // 盘中水果数量
FruitType fruit_type; // 盘中水果类型
mutex_t mutex, orange, apple; // 线程锁、橙子锁、苹果锁
sem_t fill, empty; // 信号量

void init()
{
    fruit_in_dish = 0;
    fruit_type = NONE;

    mutex_init(&mutex);
    mutex_init(&orange);
    mutex_init(&apple);

    SEM_INIT(&fill, 0);
    SEM_INIT(&empty, 1);
}

void father()
{
    while (1)
    {
        P(&empty);
        mutex_lock(&mutex);
        mutex_lock(&orange);
        
        printf("put orange\n");
        fflush(stdout);

        fruit_in_dish ++ ;
        fruit_type = ORANGE;

        mutex_unlock(&orange);
        mutex_unlock(&mutex);
        V(&fill);

        usleep(1000000);
    }
}

void mother()
{
    while (1)
    {
        P(&empty);
        mutex_lock(&mutex);
        mutex_lock(&apple);
        
        printf("put apple\n");
        fflush(stdout);

        fruit_in_dish ++ ;
        fruit_type = APPLE;
        
        mutex_unlock(&apple);
        mutex_unlock(&mutex);
        V(&fill);

        usleep(1000000);
    }
}

void daughter()
{
    while (1)
    {
        P(&fill);
        mutex_lock(&mutex);
        mutex_lock(&orange);

        while (fruit_type != ORANGE)
        {
            mutex_unlock(&orange);
            mutex_unlock(&mutex);
            V(&fill);
            P(&fill);
            mutex_lock(&mutex);
            mutex_lock(&orange);
        }

        printf("eat orange\n");
        fflush(stdout);

        fruit_in_dish -- ;
        fruit_type = NONE;

        mutex_unlock(&orange);
        mutex_unlock(&mutex);
        V(&empty);

        usleep(1000000);
    }
}

void son()
{
    while (1)
    {
        P(&fill);
        mutex_lock(&mutex);
        mutex_lock(&apple);

        while (fruit_type != APPLE)
        {
            mutex_unlock(&apple);
            mutex_unlock(&mutex);
            V(&fill);
            P(&fill);
            mutex_lock(&mutex);
            mutex_lock(&apple);
        }

        printf("eat apple\n");
        fflush(stdout);

        fruit_in_dish -- ;
        fruit_type = NONE;

        mutex_unlock(&apple);
        mutex_unlock(&mutex);
        V(&empty);

        usleep(1000000);
    }
}

int main()
{
    init();

    for (int i = 0; i < 1; i ++ )
    {
        create(father);
        create(daughter);
        create(mother);
        create(son);
    }
}
