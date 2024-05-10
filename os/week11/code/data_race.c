semaphore mutex = 1, empty = n, full = 0;

void producer()
{
    while (true)
    {
        produce_an_item;
        wait(empty);
        wait(mutex);
        buffer[in] = item;
        in = (in + 1) % n;
        signal(mutex);
        signal(full);
    }
}

void consumer()
{
    while (true)
    {
        wait(full);
        item = buffer[out];
        out = (out + 1) % n;
        signal(empty);
        consume_the_item;
    }
}