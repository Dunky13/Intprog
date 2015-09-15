import java.util.concurrent.atomic.AtomicInteger;

public class syn2
{

	public void display(String s)
	{
		System.out.print(s);
		try
		{
			Thread.sleep((long)0.1);
		}
		catch (InterruptedException e)
		{
		}

	}

	private void start()
	{
		String parent = "ab";
		String child = "cd\n";
		AtomicInteger parSem = new AtomicInteger(1);
		AtomicInteger childSem = new AtomicInteger(0);
		new ThreadClass(this, parent, parSem, childSem).start();
		new ThreadClass(this, child, childSem, parSem).start();
	}

	public static void main(String[] argv)
	{
		new syn2().start();
	}

	class ThreadClass extends Thread
	{
		private syn2 synVar;
		private String stringToPrint;
		private AtomicInteger sem1;
		private AtomicInteger sem2;

		ThreadClass(syn2 synVar, String s, AtomicInteger sem1, AtomicInteger sem2)
		{
			this.synVar = synVar;
			this.stringToPrint = s;
			this.sem1 = sem1;
			this.sem2 = sem2;
		}

		private void threadWait()
		{
			synchronized (this.sem1)
			{
				try
				{
					while (this.sem1.get() == 0)
						this.sem1.wait();
				}
				catch (InterruptedException e)
				{
				}
				this.sem1.set(0);
			}
		}

		private void threadSignal()
		{
			synchronized (this.sem2)
			{
				this.sem2.set(1);
				this.sem2.notifyAll();
			}
		}

		public void run()
		{
			for (int i = 0; i < 10; i++)
			{
				threadWait();
				synVar.display(stringToPrint);
				threadSignal();
			}
		}
	}
}
