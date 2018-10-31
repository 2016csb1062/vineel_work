import java.io.*;
import java.util.*;




class DeadLockDetect
{
	//contains all the instances of the process
	static ArrayList <ProcessObject> processList;
	//contains number of each resouce left out
	static Integer [] resourcesLeftOut;

	//read and update the processList and resorcesLeft
	public static void read(String fileFormat,String filePath)
	{
		Scanner scanner=null;
		processList = new ArrayList <ProcessObject>();
		try
		{
			scanner = new Scanner(new File(filePath));
        }
        catch(Exception e)
        {
        	System.out.println("Error at line 17 : " + e);
        }
        String line;
        line=scanner.nextLine();
        String delimeter;

        while(scanner.hasNextLine())
        {
        	line = scanner.nextLine();

        	String [] cols =null;
            int extraChars = 1;
            if(fileFormat .equals("csv"))
            {
                cols = line.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)", -1);
                extraChars = 2;
            }
            else
            {
                cols = line.split("\t");	
            }
            //Extract Process Id
        	String processId = cols[0];
        	
        	//Extract allocated Resources
        	String [] allocatedChars = cols[1].substring(extraChars,cols[1].length()-extraChars).split(",");
        	Integer [] allocatedResouces = new Integer [allocatedChars.length];
        	for (int i=0;i<allocatedChars.length;i++)
        	{
        		allocatedResouces[i] = Integer.valueOf(allocatedChars[i]);
        	}

        	//Extract requested Resources 
        	String [] requestedChars = cols[2].substring(extraChars,cols[1].length()-extraChars).split(",");
        	Integer [] requestedResources = new Integer [requestedChars.length];
        	for(int i=0;i<requestedChars.length;i++)
        	{
        		requestedResources[i] = Integer.valueOf(requestedChars[i]);
        	}

        	//Extract remaining resources
        	if(cols.length>3)
        	{
        		String [] resourcesLeftOutChars = cols[3].substring(extraChars,cols[1].length()-extraChars).split(",");
        		resourcesLeftOut = new Integer[resourcesLeftOutChars.length];
        		for(int i=0;i<resourcesLeftOutChars.length;i++)
        		{
        			resourcesLeftOut[i] = Integer.valueOf(resourcesLeftOutChars[i]);
        		}
               
        	}

        	//create process and add to processLIst
        	ProcessObject temp = new ProcessObject(processId,allocatedResouces,requestedResources);
        	processList.add(temp);
		}
    }

    //Detect the deadLock from the global values processList and resourcesLeftOut 
    public static void detectDeadLock()
    {
    	int n = processList.size();
    	boolean deadLockDetected = false;


    	boolean processTerminated[] = new boolean[n];
    	for(int i = 0;i<n;i++)
    	{
    		processTerminated[0] = false;
    	}
    	int numOfProcessTerminated = 0;

    	while(true)
    	{
    		int temp = numOfProcessTerminated;
    		for(int i=0;i<n;i++)
    		{
    			if(processList.get(i).checkTermination(resourcesLeftOut))
    			{
    				processTerminated[i] = true;
    				numOfProcessTerminated+=1;
    				
    			}
    		}

    		if(temp == numOfProcessTerminated)
    		{
    			deadLockDetected = true;
    			break;
    		}

    		if(numOfProcessTerminated == n)
    		{
    			break;
    		}
    	}

    	if(deadLockDetected)
    	{
    		System.out.println("DeadLock Detected !");
    		System.out.println("The processes in the DeadLock are :- ");
            HashSet<Integer> resourcesInLock = new HashSet<Integer>();
    		for(int i=0;i<n;i++)
    		{
    			if(!processList.get(i).terminated)
    			{
                    System.out.print("Process : ");
    				System.out.print(processList.get(i).id);
                    System.out.print("\t Requested Resources That are not available :- ");       
                    for(int j=0;j<processList.get(i).requesting.length;j++)
                    {
                        if(processList.get(i).requesting[j]>resourcesLeftOut[j])
                        {
                            System.out.print("R"+j+":"+(processList.get(i).requesting[j]-resourcesLeftOut[j])+" ");
                        }
                    }
                    System.out.println("");
    			}
    		}

    	}

    	else
    	{
    		System.out.println("DeadLock not detected!");
    	}
    }


	public static void main(String[] argv)
	{
		if(argv.length !=2)
		{
			System.out.println("arguments format of programme : ");
			System.out.println("java DeadLockDetect fileFormat(csv/tsv) file");
			return;
		}
		read(argv[0],argv[1]);
		detectDeadLock();
	}

}
