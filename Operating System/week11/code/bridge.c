#include <thread.h>
#include <thread-sync.h>

#define NORTH 0
#define SOUTH 1
#define MAX_ON_BRIDGE 3

mutex_t mutex;
cond_t north_cond, south_cond;
sem_t bridge_sem;

int current_direction = NORTH; // 桥的方向
int num_on_bridge = 0;         // 桥上的行人数量
int waiting_north = 0;         // 等待向北过桥的人数
int waiting_south = 0;         // 等待向南过桥的人数

void cross_bridge() // 模拟过桥时间
{
    usleep(1000000);
}

void init_bridge()
{
    mutex_init(&mutex);
    pthread_cond_init(&north_cond, NULL);
    pthread_cond_init(&south_cond, NULL);
    SEM_INIT(&bridge_sem, MAX_ON_BRIDGE);
}

void north_to_south(int id)
{
    mutex_lock(&mutex);

    waiting_north ++ ;
    while (current_direction == SOUTH
           || num_on_bridge >= MAX_ON_BRIDGE)
    {
        printf("Person %d from north wants to cross to south but has to wait\n", id);
        cond_wait(&north_cond, &mutex);
    }
    waiting_north -- ;

    P(&bridge_sem);
    num_on_bridge ++ ;
    current_direction = NORTH;

    printf("Person %d is crossing from north to south\n", id);
    mutex_unlock(&mutex);

    cross_bridge();

    mutex_lock(&mutex);
    num_on_bridge -- ;
    if (num_on_bridge == 0)
        if (waiting_south > 0)
        {
            current_direction = SOUTH;
            cond_broadcast(&south_cond);
        } 
        else
            cond_broadcast(&north_cond);
    V(&bridge_sem);
    mutex_unlock(&mutex);
}

void south_to_north(int id)
{
    mutex_lock(&mutex);

    waiting_south ++ ;
    while (current_direction == NORTH
           || num_on_bridge >= MAX_ON_BRIDGE)
    {
        printf("Person %d from south wants to cross to north but has to wait\n", id);
        cond_wait(&south_cond, &mutex);
    }
    waiting_south -- ;

    P(&bridge_sem);
    num_on_bridge ++ ;
    current_direction = SOUTH;

    printf("Person %d is crossing from south to north\n", id);
    mutex_unlock(&mutex);

    cross_bridge();

    mutex_lock(&mutex);
    num_on_bridge -- ;
    if (num_on_bridge == 0)
        if (waiting_north > 0)
        {
            current_direction = NORTH;
            cond_broadcast(&north_cond);
        } else
            cond_broadcast(&south_cond);
    V(&bridge_sem);
    mutex_unlock(&mutex);
}

int main()
{
    init_bridge();

    for (int i = 0; i < 5; i ++ )
    {
        create(north_to_south);
        create(south_to_north);
    }

    return 0;
}
