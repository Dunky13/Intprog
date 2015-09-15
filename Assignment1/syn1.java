public class syn1
{
	synchronized public void display(String s)
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
		String parent = "Hello world\n";
		String child = "Bonjour monde\n";

		new ThreadClass(this, parent).start();
		new ThreadClass(this, child).start();
	}

	public static void main(String[] argv)
	{
		new syn1().start();
	}

	class ThreadClass extends Thread
	{
		private syn1 synclass;
		private String stringToPrint;

		ThreadClass(syn1 syn1, String s)
		{
			this.synclass = syn1;
			this.stringToPrint = s;
		}

		public void run()
		{
			for (int i = 0; i < 10; i++)
			{
				synclass.display(stringToPrint);
			}
		}
	}
}