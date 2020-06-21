#include <stdio.h>
#include <windows.h>

#define UNUSED(a) (void)a
HANDLE ev_thread_ready;

DWORD WINAPI InfiniteLoop(LPVOID unused) {
    UNUSED(unused);
    puts("[2] Second thread ready! Entering infinite loop.");

    //Let's let the first thread know that this thread has started.
    SetEvent(ev_thread_ready);

    //An infinite loop.
    #ifdef __GNUC__
        __asm(
              ".intel_syntax noprefix\n"
              "mov eax, 0x12345678\n"
              "1:\n"
              " cmp eax, 0x12345678\n"
              " je 1b\n"
              ".att_syntax\n");
    #endif // __GNUC__
        // The above code corresponds to the following pseudocode:
        //  eax = 0x12345678;
        //  while(eax == 0x12345678) {  }

        //Version for Microsoft Visual C ++.
    #ifdef _MSC_VER
        __asm {
            mov eax, 0x12345678
            infloop:
                cmp eax, 0x12345678
                je infloop
        }
    #endif // _MSC_VER

        puts("[2] The infinite loop has ended!");
        return 0;
}

int main(void) {
    ev_thread_ready = CreateEvent(NULL, FALSE, FALSE, NULL);

    //Disable stdout caching - in this program we want to transfer data to the standard console output stream as soon as possible.
    setvbuf(stdout, NULL, _IONBF, 0);

    puts("[1] Creating a new thread.");
    HANDLE h = CreateThread(NULL, 0, InfiniteLoop, NULL, 0, NULL);

    //Let's wait until the second thread is ready (it will let us know via the ev_thread_ready event);
    WaitForSingleObject(ev_thread_ready, INFINITE);
    CloseHandle(ev_thread_ready);
    puts("[1] Second thread said it's ready. Suspending it...");
    bool retry = true;
    do {
        SuspendThread(h);

        CONTEXT ctx;
        ctx.ContextFlags = CONTEXT_FULL; //Download the entire context.

        //Thread is not paused yet [7], so the GetThreadContext function may fail the first time.
        while (!GetThreadContext(h, &ctx)) {
            Sleep(0);
        }

        //If the EAX registry value is not 0x12345678, we have paused the thread too soon. Otherwise, you can change the EAX value.
        if (ctx.Eax == 0x12345678) {
            ctx.Eax = 0xDEADC0DE; //Any other value.
            SetThreadContext(h, &ctx);
            retry = false;
            puts("[1] Changex EAX to 0xDEADC0DE!"); //or any other value user want
        } else {
            puts("[1] Suspended thread to early. Retrying.");
        }

        ResumeThread(h);
    } while(retry);

    //Let's wait for the second thread to finish.
    WaitForSingleObject(h, INFINITE);
    CloseHandle(h);

    puts("[1] The end.");
    return 0;
}
