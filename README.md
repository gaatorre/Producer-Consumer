# Producer-Consumer
This is my attempt at creating a solution to the producer-consumer problem. For those who have never heard of the problem, it is a mulithreading synchronization problem. The problem describes two processes, the producer and the consumer, who share a common, fixed-size buffer used as a queue. The producer's job is to generate data, put it into the buffer, and start again. At the same time, the consumer is consuming the data (i.e., removing it from the buffer), one piece at a time. The problem is to make sure that the producer won't try to add data into the buffer if it's full and that the consumer won't try to remove data from an empty buffer. (Wikipedia).

In my case, all I went off of was the problem statement shown above. I wanted to see if I could solve the problem by myself and if my solution was remotely similar to how others might approach it.

To simplify the problem, I replaced the queue with an integer. The producer increments the number and the consumer decrements it.
