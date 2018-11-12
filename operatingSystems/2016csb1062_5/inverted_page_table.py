import math
from queue import Queue
import random
import sys

class inverted_page_table:
	def __init__(self,virtual_address_size, page_size, ram_size):
		self.offset_size = math.ceil(math.log(2,page_size))+10;
		self.ref_size = virtual_address_size - self.	offset_size-1;
		self.number_of_pages = int(ram_size / page_size * (2**10));
		self.page_table = [page_entry() for i in range(self.number_of_pages)]
		self.fifo = Queue()
		self.page_fault = False

	def FIFO(self,page_number,pid):
		temp = fifo.get()
		self.page_table[temp].page_number = page_number
		self.page_table[temp].pid = pid
		self.page_table[temp].frequency = 1
		fifo.add(temp)
		return temp

	def LFU(self,page_number,pid):
		ind = 0
		mini = self.page_table[0].frequency
		for i in range(self.number_of_pages):
			if(mini>self.page_table[i].frequency):
				mini = self.page_table[i].frequency
				ind = i
		self.page_table[ind].frequency = 1
		self.page_table[ind].page_number = page_number
		self.page_table[ind].pid = pid
		return ind

	def MFU(self,page_number,pid):
		ind = 0
		maxi = self.page_table[0].frequency
		for i in range(self.number_of_pages):
			if(maxi>self.page_table[i].frequency):
				maxi = self.page_table[i].frequency
				ind = i
		self.page_table[ind].frequency = 1
		self.page_table[ind].page_number = page_number
		self.page_table[ind].pid = pid
		return ind

	def page_service(self,page_number,pid):
		for i in range(self.number_of_pages):
			if self.page_table[i].page_number == -1:
				self.page_table[i].page_number = page_number
				self.page_table[i].pid = pid
				self.page_table[i].frequency = 1; 
				return i
		return -1;

	def translate(self,page_addr,pid,page_rep_algo):
		self.page_fault = False
		for i in range(self.number_of_pages):
			if self.page_table[i].page_number== page_addr :
				if self.page_table[i].pid == pid:
					self.page_table[i].frequency+=1
					return i
		self.page_fault = True
		ind = self.page_service(page_addr,pid)
		if ind == -1:
			return page_rep_algo(page_addr,pid)
		return ind;

class page_entry:
	def __init__(self):
		self.page_number = -1
		self.pid = -1
		self.frequency = -1


def simulate(virtual_address_size, page_size, ram_size, page_rep_algo, ref_addresses):
	page_table = inverted_page_table(virtual_address_size,page_size,ram_size)
	page_fault = 0;
	if(page_rep_algo == 'fifo'):
		page_rep_algo_func = page_table.FIFO;
	elif(page_rep_algo == 'lfu'):
		page_rep_algo_func = page_table.LFU;
	else:
		page_rep_algo_func = page_table.MFU;
	for ref_address in ref_addresses:
		binstring = (('{:0'+str(virtual_address_size)+'b}').format(ref_address))
		page_addr = int(binstring[:page_table.ref_size],2)
		offset = int(binstring[page_table.ref_size:],2)
		print("--------------------------------------------------------")
		print("Page number : "+str(page_addr)+ " offset : "+str(offset))
		phy_addr = page_table.translate(page_addr,0,page_table.FIFO);
		print("Frame number : "+str(phy_addr)+ " offset : "+str(offset))
		print("Page Fault : "+ str(page_table.page_fault))
		print("--------------------------------------------------------")
		if(page_table.page_fault):
			page_fault+=1;

	return (page_fault/len(ref_addresses))*100;



if __name__=="__main__":
	virtual_address_size = int(sys.argv[1]);
	page_size =int(sys.argv[2])
	ram_size = int(sys.argv[3])
	page_rep_algo = str(sys.argv[4])
	ref_addresses = []
	for i in range(10000):
		ref_addresses+=[random.randint(0,2**virtual_address_size)];
	print("page_fault percentage : "+str(simulate(virtual_address_size, page_size, ram_size, page_rep_algo, ref_addresses))+"%")

