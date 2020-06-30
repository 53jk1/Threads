import java.util.concurrent.Executors;
import java.util.concurrent.ExectuorService;
import java.util.concurrent.Future;

class PoolTest {
    public static void main(String args[]) {
        //Creating a new thread pool.
        ExecutorService pool = Executors.newFixedThreadPool(4);

        //80 pool task orders. Only 4 tasks will be active at a time.
        //The submit method returns an object implementing the Future interface, which you can ask if the task has already been completed. Alternatively, you can use the simpler method execute.

        Future[] tasks = new Future[80];
        for (int i=0; i < 80; i++){
            tasks[i] = pool.submit(new MyThread(i));
        }

        //Close acceptance of new tasks in the pool and delete the pool after all tasks have been completed.
        pool.shutdown();

        //Ask each task in turn until all of them are over.

        while(!pool.isTerminated()) {

            //Check and view the status of all tasks.
            String s ="";
            for (int i = 0; i < 80; i++){
                s += tasks[i].isDone() ? "D" : ".";
            }

            System.out.println("All done!");
        }
    }

    class MyThread implements Runnable{
        private int data;
        MyThread(int data) {
            this.data = data;
        }

        public void run() {
            try {
                //Wait for some time before completing the task.
                Thread.sleep(25 * (this.data % 7));
            } catch (InterruptedException e) {
                //In this program no thread interrupts another, so it will never execute.
            }
        }
    }
}