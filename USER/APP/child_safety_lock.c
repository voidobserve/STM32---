#include "child_safety_lock.h"
#include <stdbool.h>
#include "usart2.h"

volatile bool lock = false; // ��ͯ����false--�رգ�true--����

void LockUp(void)
{
    lock = true;
}

void UnLock(void)
{
    lock = false;
}

void LockUpdate(void)
{
    if (true == lock)
    {
        LockUp();
    }
    else
    {
        UnLock();
    }
}
