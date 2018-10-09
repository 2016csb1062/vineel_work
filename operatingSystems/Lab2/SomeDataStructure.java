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
	public SomeDataStructure(int n)
	{
		series = new HashMap<Long,Long>(n);
		hashSet = new LinkedHashSet<Long>(n);
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

	void removeKey(Long key)
	{
		series.remove(key);
	}

}
