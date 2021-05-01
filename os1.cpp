
//七状态代码

#include <iostream>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#define MAXNEI 3
#define MAXWAI 10
using namespace std;
struct PCB
{
    //int time;//分配的时间片
    //int state;//进程状态
    int name;//第几个进程
};
queue<PCB*>Running;
queue<PCB*>Ready;
queue<PCB*>Blocked;
queue<PCB*>Blocked_Suspend;
queue<PCB*>Ready_Suspend;
int num = 1;

void initPCB()
{
    /*
    需要根据内存是否为满来考虑进入ready还是ready suspend进程
    */
    int sizeOfNei = Running.size() + Ready.size() + Blocked.size();
    int sizeOfWai = sizeOfNei + Ready_Suspend.size() + Blocked_Suspend.size();
    if (sizeOfNei >= MAXNEI)
    {
        cout << "内存已满" << endl;
        if (sizeOfWai >= MAXNEI + MAXWAI)
        {
            cout << "外存已满，不能添加" << endl;
        }
        else
        {
            cout << "新建的PCB存入外存" << endl;
            struct PCB* pcb = (struct PCB*)malloc(sizeof(struct PCB));
            pcb->name = num++;
            //printf("\ndebug:%d\n",num);
            Ready_Suspend.push(pcb);
        }
    }
    else
    {
        cout << "新建的PCB存入内存" << endl;
        struct PCB* pcb = (struct PCB*)malloc(sizeof(struct PCB));
        pcb->name = num++;
        //printf("\ndebug:%d\n",num);
        if (Running.empty() == true)
        {
            Running.push(pcb);
        }
        else
        {
            Ready.push(pcb);
        }
    }
}
void printQueue()
{
    printf("Running Queue:");
    if (!Running.empty())
    {
        for (int i = 0; i < Running.size(); i++)
        {
            struct PCB* pcb = Running.front();
            Running.pop();
            printf(" %d", pcb->name);
            Running.push(pcb);
        }
    }
    printf("\nReady Queue:");
    if (!Ready.empty())
    {
        for (int i = 0; i < Ready.size(); i++)
        {
            struct PCB* pcb = Ready.front();
            Ready.pop();
            printf(" %d", pcb->name);
            Ready.push(pcb);
        }
    }
    printf("\nBlocked Queue:");
    if (!Blocked.empty())
    {
        for (int i = 0; i < Blocked.size(); i++)
        {
            struct PCB* pcb = Blocked.front();
            Blocked.pop();
            printf(" %d", pcb->name);
            Blocked.push(pcb);
        }
    }
    printf("\nReady_Suspend Queue:");
    if (!Ready_Suspend.empty())
    {
        for (int i = 0; i < Ready_Suspend.size(); i++)
        {
            struct PCB* pcb = Ready_Suspend.front();
            Ready_Suspend.pop();
            printf(" %d", pcb->name);
            Ready_Suspend.push(pcb);
        }
    }
    printf("\nBlocked_Suspend Queue:");
    if (!Blocked_Suspend.empty())
    {
        for (int i = 0; i < Blocked_Suspend.size(); i++)
        {
            struct PCB* pcb = Blocked_Suspend.front();
            Blocked_Suspend.pop();
            printf(" %d", pcb->name);
            Blocked_Suspend.push(pcb);
        }
    }
    printf("\n");
}
void timeOut()
{
    if (Running.empty())
    {
        cout << "当前没有进程在运行！" << endl;
        return;
    }
    struct PCB* pcb = Running.front();
    Running.pop();
    if (Ready.empty())
    {
        Running.push(pcb);
        return;
    }
    Ready.push(pcb);
    pcb = Ready.front();
    Ready.pop();
    Running.push(pcb);
    return;
}
void SuspendtoReady() {
    //考虑如果内存满了，则把阻塞队列的内容塞到阻塞挂起那里
    //如果内存没满，则把就绪挂起的内容塞到就绪队列那里
    int sizeOfNei = Running.size() + Ready.size() + Blocked.size();
   
    if (Ready_Suspend.size() > 0) {
        if(sizeOfNei>=MAXNEI)
        {
            struct PCB* pcb = Blocked.front();
            Blocked.pop();
            Blocked_Suspend.push(pcb);
        
        }
        struct PCB* pcb = Ready_Suspend.front();
        Ready_Suspend.pop();
        Ready.push(pcb);
    }
    else {
        cout << "就绪挂起队列为空，不能激活。" << endl;
    }
    if(Running.size()==0)
    {
        struct PCB* pcb=Ready.front();
        Ready.pop();
        Running.push(pcb);
    }
   
}
void disPatch()
{
    /*
     当Ready为空时，Running为空时，不存在dispath
     当Ready为空，Running不为空时，dispatch调度的结果不变
     以上两条不写了

     当Ready不为空，Running为空时，可以自动dispatch，无需手动dispatch
     当Ready不为空，Running不为空时，dispatch做的工作是把Running的放在Ready队末，把Ready队首给Running
     */
    if (!Ready.empty())
    {
        if (!Running.empty())
        {
            struct PCB* pcb = Running.front();
            Running.pop();
            Ready.push(pcb);
            pcb = Ready.front();
            Ready.pop();
            Running.push(pcb);
            return;
        }
        else
        {
            struct PCB* pcb = Ready.front();
            Ready.pop();
            Running.push(pcb);
            return;
        }
    }
    else {
        /*if (Running.empty())
        {
            cout << "不能进行dispatch,当前没有进程在运行！" << endl;
            return;
        }*/
        cout << "当前没有就绪进程" << endl;
        SuspendtoReady();
    }
}
void waitEvent()
{
    if (!Running.empty())
    {
        struct PCB* pcb = Running.front();
        Running.pop();
        Blocked.push(pcb);
    }
    else {
        cout << "当前没有进程在运行！" << endl;
    }
    int sizeOfNei = Running.size() + Ready.size() + Blocked.size();
    //如果Ready不为空，先把Ready调过去，则只是内存中变化；如果Ready空，判断内存够不够大，如果够大就调外存的挂起进程，否则不调
    if (!Ready.empty())
    {
        struct PCB* pcb = Ready.front();
        Ready.pop();
        Running.push(pcb);
    }
    else {
        //ready是空的
        //判断ready挂起队列是否为空
        //不为空直接让它激活并运行
        if(sizeOfNei<MAXNEI)
        {
            if (Ready_Suspend.size() > 0) {
                struct PCB* pcb = Ready_Suspend.front();
                Ready_Suspend.pop();
                Running.push(pcb);
            }
        }
    }
   
    return;
}

void occurEvent()
{
    if (!Blocked.empty())
    {
        struct PCB* pcb = Blocked.front();
        Blocked.pop();
        Ready.push(pcb);
    }
    else {
        cout << "阻塞队列没有进程在等待事件发生！" << endl;
    }
    if (Running.empty())
    {
        disPatch();
        //当CPU没有进程时，事件发生，Blocked->Ready->Running
    }
    return;
}

void releaseEvent()
{
    if (Running.empty())
    {
        cout << "当前没有进程在运行！" << endl;
        return;
    }
    if (!Running.empty())
    {
        Running.pop();

        if (Ready.size() > 0) {
            struct PCB* pcb = Ready.front();
            Ready.pop();
            Running.push(pcb);
        }
        else {
            if (Ready_Suspend.size() > 0) {
                struct PCB* pcb = Ready_Suspend.front();
                Ready_Suspend.pop();
                Running.push(pcb);
            }
            else {
                cout << "无可运行进程。" << endl;
            }
        }
        //disPatch();
        return;
    }
}

void ReadytoSuspend() {
    if (Ready.size() > 0) {
        //就绪队列中有进程
        struct PCB* pcb = Ready.front();
        Ready.pop();
        Ready_Suspend.push(pcb);
    }
    else {
        cout << "就绪队列为空，不能挂起" << endl;
    }
}



void BlocktoSuspend() {
    if (Blocked.size() > 0) {
        struct PCB* pcb = Blocked.front();
        Blocked.pop();
        Blocked_Suspend.push(pcb);
    }
    else {
        cout << "阻塞队列为空，不能挂起。" << endl;
    }
}

void SuspendtoBlock() {
    //阻塞事件激活
    //先判断内存满没满，如果满了就挂起低优先级进程，如果没满就放入阻塞队列
    int sizeOfNei = Running.size() + Ready.size() + Blocked.size();
    if(sizeOfNei>=MAXNEI)
    {
        if(Blocked.size()>0)
        {
            struct PCB* pcb = Blocked.front();
            Blocked.pop();
            Blocked_Suspend.push(pcb);
            pcb = Blocked_Suspend.front();
            Blocked_Suspend.pop();
            Blocked.push(pcb);
            
        }
        else{
            cout<<"阻塞队列为空，且内存已满，无法激活。"<<endl;
        }
    }
    else{
        if (Blocked_Suspend.size() > 0) {
            struct PCB* pcb = Blocked_Suspend.front();
            Blocked_Suspend.pop();
            Blocked.push(pcb);
        }
        else {
            cout << "阻塞挂起队列为空，不能激活。" << endl;
        }
    }

}

void SuspendEventOcc() {
    
    if (Blocked_Suspend.size() > 0) {
        struct PCB* pcb = Blocked_Suspend.front();
        Blocked_Suspend.pop();
        Ready_Suspend.push(pcb);
    }
    else {
        cout << "阻塞挂起队列为空" << endl;
        return;
    }

    if (Ready.size() == 0) {
        if (Running.size() == 0) {
            struct PCB* pcb = Ready_Suspend.front();
            Ready_Suspend.pop();
           Running.push(pcb);
        }
    }
}

void RuntoSuspend() {
    if (Running.size() > 0) {
        struct PCB* pcb = Running.front();
        Running.pop();
        Ready_Suspend.push(pcb);
    }
    else {
        cout << "运行队列为空" << endl;
    }

    if (Ready.size() > 0) {
        struct PCB* pcb = Ready.front();
        Ready.pop();
        Running.push(pcb);
    }
    else {
        if (Ready_Suspend.size() > 0) {
            struct PCB* pcb = Ready_Suspend.front();
            Ready_Suspend.pop();
            Running.push(pcb);
        }
        else {
            cout << "无可运行进程" << endl;
        }
    }
}
int main()
{
    cout << "1.(new)新建一个进程" << endl;
    cout << "2.(timeout)时间片超时" << endl;
    cout << "3.(dispatch)重新分配" << endl;
    cout << "4.(event wait)事件等待" << endl;
    cout << "5.(event occurs 1)等待的事件发生" << endl;
    cout << "6.(event occurs 2)挂起事件等待的事件发生" << endl;
    cout << "7.(suspend)Ready事件挂起" << endl;
    cout << "8.(suspend)Block事件挂起" << endl;
    cout << "9.(activate)Readysuspend事件激活" << endl;
    cout << "10.(activate)Blocksuspend事件激活" << endl;
    cout << "11.(release)事件释放" << endl;
    int choice;
    while (1)
    {
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            initPCB();
            break;
        }
        case 2:
        {
            timeOut();
            break;
        }
        case 3:
        {
            disPatch();
            break;
        }
        case 4:
        {
            waitEvent();
            break;
        }
        case 5:
        {
            occurEvent();
            break;
        }
        case 6:
        {
            SuspendEventOcc();
            break;
        }
        case 7:
        {
            ReadytoSuspend();
            break;
        }
        case 8:
        {
            BlocktoSuspend();
            break;
        }
        case 9: {
            SuspendtoReady();
            break;
        }
        case 10: {
            SuspendtoBlock();
            break;
        }
        case 11: {
            releaseEvent();
            break;
        }
        default:
            break;
        }
        printQueue();

    }
    return 0;
}
