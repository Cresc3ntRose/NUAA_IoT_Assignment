semaphore mutex = 1, omutex = 1, empty = n, full = 0;

void producer()
{
    while (true)
    {
        produce_an_item;
        wait(empty);
        buffer[in] = item;
        wait(mutex);
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
        wait(omutex);
        out = (out + 1) % n;
        signal(omutex);
        signal(empty);
        consume_the_item;
    }
}