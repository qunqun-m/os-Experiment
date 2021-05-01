
#include<stdio.h>
#include<math.h>
#include <iostream>
#define Buffersize 8
using namespace std;
int data=1; //规定数据从1开始
int Consumewait=1;  //消费者等待数目
struct Buffer{
    int space[Buffersize];   //缓冲区大小
    int writePointer;  //缓冲区写指针
    int readPointer;   //缓冲区读指针
}buffer;

struct PCQueue{
int queue[20];  //等待队列大小
int tail;  //等待队列尾指针
int head;  //等待队列头指针
}ProducerQueue,ConsumerQueue;
//生产者等待队列,消费者等待队列

struct DataQueue{
int queue[20];
int tail;  //生产队列尾指针m
int head;  //生产队列头指针n
}ProducerDataQueue,ConsumerDataQueue;
//生产数据队列，消费数据队列

//p1 产生数据
void ProducerData()
{
    printf("生产者生产数据队列:");
    ProducerDataQueue.queue[ProducerDataQueue.tail]=data;
    //将生产的第一个数据存入queue[tail]位置
    ProducerDataQueue.tail++;
    data++;
    int j=ProducerDataQueue.head;
    int a=ProducerDataQueue.tail;
    for(;j<a;j++)
        {
            printf("%d ",ProducerDataQueue.queue[j]);
            //将生产数据队列输出
        }
    cout<<endl;
}

//p2 产生数据送入缓冲区
void ProducerIntoBuffer()
{
    /*
     如果生产数据不为空
        如果缓冲区已经满了，生产者把生产数据送入等待队列（说明消费者队列是没有消费者等待要消费的）
        如果缓冲区没有满，直接把数据送到buffer里面
            考虑是否存在消费者。
            如果消费者等待队列存在消费者，那直接消费掉数据
            如果消费者等待队列不存在消费者，则直接将数据写入缓冲区
     如果生产数据为空
        那就没有数据可以供生产者推入buffer
     */
    if(ProducerDataQueue.queue[ProducerDataQueue.head]>0)
    {
        if((buffer.writePointer==buffer.readPointer)&&(buffer.space[buffer.readPointer]!=0))//缓冲区已满，需要将生产者生产的数据存入等待队列
    {
        printf("缓冲区已满，生产者等待队列:");
        ProducerQueue.queue[ProducerQueue.tail]=ProducerDataQueue.queue[ProducerDataQueue.head];   //将生产数据队列的首数据取出，送入等待队列
        ProducerDataQueue.head++;
        ProducerQueue.tail++;
        int j=ProducerQueue.head;
        int a=ProducerQueue.tail;
        for(;j<a;j++)
            {
                printf("%d ",ProducerQueue.queue[j]);    //依次输出生产者等待队列中的数据
            }
        cout<<endl;
        printf("生产者生产数据队列:");
        int j1=ProducerDataQueue.head;
        int a1=ProducerDataQueue.tail;
        for(;j1<a1;j1++)
        {
            printf("%d ",ProducerDataQueue.queue[j1]);    //将生产数据队列输出
        }
        cout<<endl;
    }
    else //缓冲区未满
    {
        if(ConsumerQueue.queue[ConsumerQueue.head]>0)//消费者等待队列不空
            {
                ConsumerQueue.head++;//消费者等待队列头指针右移
                printf("%d数据已经被生产并且被消费\n",ProducerDataQueue.queue[ProducerDataQueue.head]);
                ProducerDataQueue.head++;
                printf("缓冲区为空，消费者等待队列:");
                int j=ConsumerQueue.head;
                int a=ConsumerQueue.tail;
                for(;j<a;j++)
                {
                    printf("C%d ",ConsumerQueue.queue[j]);    //依次输出消费者等待队列中等待进程
                }
                cout<<endl;
                printf("生产者生产数据队列:");
                int j1=ProducerDataQueue.head;
                int a1=ProducerDataQueue.tail;
                for(;j1<a1;j1++)
                {
                    printf("%d ",ProducerDataQueue.queue[j1]);    //将生产数据队列输出
                }
                cout<<endl;
            }
        else//消费者等待队列为空，无消费者进程等待
            {
                buffer.space[buffer.writePointer]=ProducerDataQueue.queue[ProducerDataQueue.head];//将数据写入缓冲区
                ProducerDataQueue.head++;    //生产数据队列头指针右移
                buffer.writePointer++; //缓冲区写指针加1
                if(buffer.writePointer==Buffersize)//写指针写到缓冲区末尾，则回到0位置
                    {
                        buffer.writePointer=0;
                    }
                if(ProducerDataQueue.queue[ProducerDataQueue.head]>0)
                {
                    printf("生产者生产数据队列:");
                    int j=ProducerDataQueue.head;
                    int a=ProducerDataQueue.tail;
                    for(;j<a;j++)
                    {
                        printf("%d ",ProducerDataQueue.queue[j]);    //将生产数据队列输出
                    }
                    cout<<endl;
                }
                else
                    printf("生产者未生产数据\n");
            }
    }
    }
    else
    {
        printf("生产者未生产数据\n");
    }
}

//c1 将数据从缓冲区取出
void BufferIntoConsumer()
{
    /*
     如果缓冲区为空，没有数据可以被消费，则消费者进入等待队列
     如果缓冲区不为空，消费者消费数据，把数据先取出到消费者数据队列
        如果生产者队列不为空，那缓冲区的数据一旦进入了消费者数据队列，生产者数据等待队列的数据马上直接进入了缓冲区
     */
    
    if((buffer.writePointer==buffer.readPointer)&&(buffer.space[buffer.writePointer]==0))//缓冲区为空，无数据可以消费
        {
        printf("缓冲区为空，消费者等待队列:");
        ConsumerQueue.queue[ConsumerQueue.tail]=Consumewait; //消费者进程进入等待队列
        ConsumerQueue.tail++;
        Consumewait++;  //消费者等待进程数+1
        int j=ConsumerQueue.head;
        int a=ConsumerQueue.tail;
        for(;j<a;j++)
        {
            printf("C%d ",ConsumerQueue.queue[j]);
        }
        cout<<endl;
    }
    else//缓冲区不为空
        {
            ConsumerDataQueue.queue[ConsumerDataQueue.tail]=buffer.space[buffer.readPointer];
            //将数据取出到消费队列
            ConsumerDataQueue.tail++;
            buffer.space[buffer.readPointer]=0;
            buffer.readPointer++;
            if(buffer.readPointer==Buffersize)//读指针读到缓冲区末尾，则回到0位置
            {
                buffer.readPointer=0;
            }
            printf("消费者消费数据");
            int j=ConsumerDataQueue.head;
            int a=ConsumerDataQueue.tail;
            for(;j<a;j++)
            {
                printf("%d ",ConsumerDataQueue.queue[j]);    //将消费数据队列输出
            }
            cout<<endl;

            if(ProducerQueue.queue[ProducerQueue.head]>0)//生产者等待队列不为空
            {
                buffer.space[buffer.writePointer]=ProducerQueue.queue[ProducerQueue.head];
                //唤醒生产者等待队列首数据
                ProducerQueue.head++;   //等待队列头指针右移
                buffer.writePointer++; //缓冲区写指针右移
                if(buffer.writePointer==Buffersize)//写指针写到缓冲区末尾，则回到0位置
                    {
                        buffer.writePointer=0;
                    }
                printf("缓冲区已满，生产者等待队列:");
                int j=ProducerQueue.head;
                int a=ProducerQueue.tail;
                for(;j<a;j++)
                {
                    printf("%d ",ProducerQueue.queue[j]);    //将生产数据队列输出
                }
                cout<<endl;
            }
    }
}

//c2 将数据送入消费者
void ConsumerData()
{
    /*
     从消费者数据队列中取出来数据的函数
     如果消费者数据队列不为空才可以执行，直接取出即可
     */
    if(ConsumerDataQueue.queue[ConsumerDataQueue.head]>0)  //消费队列不为空
    {
        printf("消费者消费数据");
        ConsumerDataQueue.head++;
        int j=ConsumerDataQueue.head;
        int a=ConsumerDataQueue.tail;
        for(;j<a;j++)
        {
            printf("%d ",ConsumerDataQueue.queue[j]);    //将消费数据队列输出
        }
    cout<<endl;
    }
    else
        printf("没有数据可以消费\n");

}
void printQueue()
{
    int a;
    for(a=0;a<Buffersize;a++)
    {
        printf(" %d ",buffer.space[a]);
    }
    cout<<endl;
    printf("写指针位置:%d     读指针位置:%d\n",buffer.writePointer,buffer.readPointer);
    if((buffer.writePointer==buffer.readPointer)&&(buffer.space[buffer.readPointer]!=0))//缓冲区已满，需要将数据队列中的进程存入等待队列
    {
        int j=ProducerQueue.tail;
        if(ProducerQueue.queue[j]!=0)    //等待队列不为空，说明进程进入等待状态
            printf("生产者进程:等待  ");
        else
            printf("生产者进程:就绪  ");
    }
    else
    {
        printf("生产者进程:就绪  ");
    }
    if((buffer.writePointer==buffer.readPointer)&&(buffer.space[buffer.writePointer]==0))//缓冲区为空，无进程可以消费
    {
        int j=ConsumerQueue.tail;
        if(ConsumerQueue.queue[j]!=0)    //生产者等待队列不为空，说明进程进入等待状态
            printf("消费者进程:等待\n");
        else
            printf("消费者进程:就绪\n");
    }
    else
    {
        printf("消费者进程:就绪\n");
    }
}
int main()
{
    cout<<"按下p1产生数据 | 按下c1将数据从缓冲区取出 | "<<endl;
    cout<<"按下p2将数据送入缓冲区 | 按下c2将数据送入消费者 | 按下“e”退出程序"<<endl;
   while(1)
    {
        char input[2];
        cin>>input;
        if(input[0]=='p'&&input[1]=='1')//按下“p1”产生数据
        {
            ProducerData();
        }
        else if(input[0]=='p'&&input[1]=='2')//按下“p2”将数据送入缓冲区
        {
            ProducerIntoBuffer();
        }
        else if(input[0]=='c'&&input[1]=='1')//按下“c1”将数据从缓冲区取出
        {
            BufferIntoConsumer();
        }
        else if(input[0]=='c'&&input[1]=='2')//按下“c2”将数据送入消费者
        {
            ConsumerData();
        }
        else if(input[0]=='e')
        {
            exit(0);
        }
        else
            printf("输入错误，请重新输入");
        printQueue();
    }
    return 0;
}

