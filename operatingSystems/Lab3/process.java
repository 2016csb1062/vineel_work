class ProcessObject
{
	String id;
	Integer [] allocated;
	Integer [] requesting;
	boolean terminated;

	ProcessObject(String processId ,Integer [] a , Integer [] r)
	{
		id = processId;
		allocated = a;
		requesting = r;
		terminated = false;
	}

	boolean checkTermination(Integer [] resourcesLeftOut  )
	{
		if(terminated)
		{
			return false;
		}
		for(int i=0;i<requesting.length;i++)
		{
			if(requesting[i]>resourcesLeftOut[i])
			{
				return false;
				
			}
		}
		terminated = true;
		for(int i=0;i<allocated.length;i++)
		{
			resourcesLeftOut[i] += allocated[i];
		}
		return true;
	}

	void printProcess()
	{
		System.out.println("ProcessId : "+id);
		System.out.println("allocated Resources :");
		for(int i=0;i<allocated.length;i++)
		{
			System.out.print(allocated[i]+" ");
		}
		System.out.println("");
		System.out.println("requesting Resources :");
		for(int i=0;i<requesting.length;i++)
		{
			System.out.print(requesting[i]+" ");
		}
		System.out.println("");
	}
}