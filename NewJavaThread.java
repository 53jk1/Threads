public class NewJavaThread extends Thread {

    private int data;
    private int retval;

    public void run() {
        System.out.println("I was run in a new thread with " + Integer.toHexString(data) + " as data.");
        retval = 0xC0DE;
    }

    public int getReturnValue() {
        return retval;
    }

    public static void main(String args[]) {
        //Create a new thread
        NewJavaThread th = new NewJavaThread();
        th.data = 0x12345678;
        th.start();

        //Waiting for the end of a new thread.
        while (true) {
            try {
                th.join();
                break;
            } catch (InterruptedException e) {
                //The wait has been interrupted.
                //In this case, this should never happen
                //(the second thread does nothing to break the first thread),
                //but Java compilers may require that the interrupt be caught.
            }
        }
        System.out.println("Second thread returned: " + Integer.toHexString(th.getReturnValue()));

    }

}
