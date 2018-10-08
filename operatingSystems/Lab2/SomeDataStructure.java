import java.util.HashMap;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.io.*;


public class SomeDataStructure
{
	//series contains index and values map
	HashMap <Long,Long> series;

	//hashSet to get unique values added
	LinkedHashSet <Long> hashSet;

	//constructor
	public SomeDataStructure()
	{
		series = new HashMap<Long,Long>();
		hashSet = new LinkedHashSet<Long>();
	}

	//get the value of given index/key
	synchronized Long takeOldestGeneratedNumber(Long baseIndex)
	{
		return series.get(baseIndex);
	}

	//inserts into series and hashSet
	synchronized void recordGeneratedNumber(Long index,Long value)
	{
		series.put(index,value);
		int temp = hashSet.size();
		System.out.println(length());
		hashSet.add(value);
	}

	//returns length of hashSet
	synchronized Long length()
	{
		Long siz = (long)hashSet.size();
		return siz;  
	}

}
