import sys
import tkinter as tk
from tkinterdnd2 import DND_FILES, TkinterDnD

# Parser class
##################################################
class sub_text:
	def __init__(self):
		self.old_files = []
		self.new_files = []
		self.old_data = []
		self.new_data = []
		self.sub_result = []
		self.delimiter = ''
		self.delimiter_usage = False
	
	def add_old_files(self, given_list):
		self.old_files = given_list
		
	def add_new_files(self, given_list):
		self.new_files = given_list
		
	def parse_new_data(self):
		for filepath in self.new_files:
			with open(filepath, 'r') as file:
				self.new_data += file.read().splitlines()
		
	def parse_old_data(self):
		for filepath in self.old_files:
			with open(filepath, 'r') as file:
				self.old_data += file.read().splitlines()
				
	def sub_text(self):
		for item in self.new_data:
			if self.delimiter_usage:
				item_disnamed = item.split(self.delimiter)[0]
				if not any(search_item.startswith(item_disnamed) for search_item in self.old_data):
					if not any(search_item.startswith(item_disnamed) for search_item in self.sub_result):
						self.sub_result.append(item)
			else:
				if not item in self.old_data:
					if not item in self.sub_result:
						self.sub_result.append(item)
					
	def set_delimiter(self, symbol):
		self.delimiter = symbol
		
	def set_delimiter_usage(self, value):
		self.delimiter_usage = value
				
	def get_result(self):
		return self.sub_result
##################################################

# GUI callbacks
##################################################
def add_to_old_gui_list(event):
	items = []
	items += event.data.split()
	for item in items:
		old_files_list.insert(tk.END, item)
		old_files.append(item)
		
def add_to_new_gui_list(event):
	items = []
	items += event.data.split()
	for item in items:
		new_files_list.insert(tk.END, item)
		new_files.append(item)
	
def parse_button_pressed():
	st = sub_text()
	
	# No GUI switch, unfortunately
	#st.set_delimiter_usage(True)
	#st.set_delimiter('#')
	
	st.add_old_files(old_files)
	st.add_new_files(new_files)
	st.parse_old_data()
	st.parse_new_data()
	st.sub_text()
	
	window_result = TkinterDnD.Tk()
	result_text = tk.Text(window_result)
	for item in st.get_result():
		result_text.insert(tk.END, item + '\n')
	
	result_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)
	scroll = tk.Scrollbar(window_result, command=result_text.yview)
	scroll.pack(side=tk.LEFT, fill=tk.Y)
	result_text.config(yscrollcommand=scroll.set)
	
def clean_button_pressed():
	old_files.clear()
	new_files.clear()
	new_files_list.delete(0, tk.END)
	old_files_list.delete(0, tk.END)
##################################################

# Main function
##################################################
if __name__ == "__main__":
	window = TkinterDnD.Tk()
	window.resizable(False,False)
	window.eval('tk::PlaceWindow . center')
	
	old_files = []
	new_files = []
	
	f_new = tk.LabelFrame(text = "Drag and drop NEW files here")
	new_files_list = tk.Listbox(f_new,width = 50, height = 5)
	new_files_list.pack(padx=10, pady=10)
	f_new.pack(padx=10, pady=10)
	
	f_old = tk.LabelFrame(text = "Drag and drop OLD files here")	
	old_files_list = tk.Listbox(f_old, width = 50, height = 5)
	old_files_list.pack(padx=10, pady=10)
	f_old.pack(padx=10, pady=10)
	
	f_bot = tk.Frame()
	button_clean = tk.Button(
		f_bot,
		text = "Clean",
		width = 15,
		height = 2,
		command = clean_button_pressed
	)
	button_process = tk.Button(
		f_bot,
		text = "Parse",
		width = 15,
		height = 2,
		command = parse_button_pressed
	)
	button_clean.pack(side=tk.LEFT, padx=10, pady=10)
	button_process.pack(side=tk.RIGHT, padx=10, pady=10)
	f_bot.pack()
	

	old_files_list.drop_target_register(DND_FILES)
	old_files_list.dnd_bind('<<Drop>>', add_to_old_gui_list)
	
	new_files_list.drop_target_register(DND_FILES)
	new_files_list.dnd_bind('<<Drop>>', add_to_new_gui_list)
	
	window.mainloop()
##################################################
	
