import java.util.*;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.io.*;
import dev.RealDevice;
import dev.Device;
import dev.UnrealDevice;


class uniqueNumberGenerater
{
	static volatile SomeDataStructure uniqueNumGenerated;
	static volatile  Long baseIndex;
	static volatile Long i;
	static int endLoop ;
	static Device dev;
	static long n;
	static volatile boolean done;
	static Lock lock;
	public static class MyThread extends Thread 
	{
		public void run()
		{
			while (!done) 
			{
				//------------Synchronize i and baseIndex
				int locali ;
				Long localBaseIndex ;
				Long localii;
				synchronized(this)
				{
					i+=1;
					localii =  i;
					if(i % endLoop == 0)
					{
						baseIndex+=1;
					}
					locali = (int)(i%endLoop);
					localBaseIndex = baseIndex;
				}

				//-------------GetBase
				Long base = uniqueNumGenerated.takeOldestGeneratedNumber(localBaseIndex);
				while(base==null)
				{
				 	base = uniqueNumGenerated.takeOldestGeneratedNumber(localBaseIndex);
				 	if (uniqueNumGenerated.length() >= n) 
					{
						done = true;
						break;
					}
				}

				//-------------Get The NextNumber
				Long nxtNumber = 0l;
				boolean deviceWorked = false;
				while(!deviceWorked)
				{
					try
					{
						nxtNumber = dev.f(base, locali);
						deviceWorked = true;
						if(nxtNumber == null)
						{
							deviceWorked = false;
						}
					}
					catch(InterruptedException e)
					{
						deviceWorked = false;
					}
				}

				uniqueNumGenerated.recordGeneratedNumber(localii+1,nxtNumber);
				if (uniqueNumGenerated.length() >= n) 
				{
					done = true;
					break;
				}
			}
		}
		
	}


	public static void main(String[] args) 
	{
		n = Long.parseLong(args[0]);
		Integer nOfThreads = Integer.parseInt(args[1]);
		String deviceType = args[2];
		Long bootDelay ;
		Long computeDelay;
		
		if(args.length>3)
			computeDelay = Long.parseLong(args[3]);
		else
			computeDelay = 3l;
		if(args.length>4)
			bootDelay = Long.parseLong(args[4]);
		else
			bootDelay = 250l;

		//-------------- Initializing global data
		uniqueNumGenerated = new SomeDataStructure();
		uniqueNumGenerated.recordGeneratedNumber(0l,2l);
		baseIndex = -1l;
		i=-1l;
		endLoop = Device.MULTIPLIERS.length;
		done = false;
		lock = new ReentrantLock();
		//---//Device Creation
		if(deviceType.equals("real"))
		{
			RealDevice.DeviceConfig config =new RealDevice.DeviceConfig( computeDelay, bootDelay, 1000);
			dev = new RealDevice(config);
			deviceType = "Real";
		}
		else
		{

			dev = new UnrealDevice(0);
			deviceType = "UnReal";
		}
		//----------------

		long start = System.currentTimeMillis();
		//------------------Threads
		MyThread threads[] = new MyThread[nOfThreads];

		//----------------For Loop of Threads
		for(int i=0;i<nOfThreads;i++)
		{
			threads[i] = new MyThread();
			threads[i].start();
		}

		//-----------------Wait for threads
		for(int i=0;i<nOfThreads;i++)
		{
			try
			{

				threads[i].join();
			}
			catch(InterruptedException e)
			{
				
			}
		}
		Iterator<Long> iter = uniqueNumGenerated.hashSet.iterator();
		for(Long i=0l;i<(n-1);i++)
		{
			iter.next();
		}

		Long result = iter.next();
		long end = System.currentTimeMillis();

		System.out.println("----------------------------------------");
		System.out.println("RESULTS SUMMARY");
		System.out.println("----------------------------------------");
		System.out.println("Target count (n)............: "+n);
		System.out.println("Number of threads...........: "+nOfThreads);
		System.out.println("Used device.................: "+deviceType);
		System.out.println("Time taken..................: "+(end-start) +" millisec");
		System.out.println("Resulting number............: "+result);
		System.out.println("Device invoked (approx).....: "+dev.age());

	}
}