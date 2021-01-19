@ECHO OFF 																		 
ECHO = = = Demonstrating how to determine code cache size  = = = 						 
ECHO Utility for the \"Program optimization technique\"  book by Kris Kaspersky	 
ECHO N NOPs                        ...CLOCK...                                  
ECHO -------------------------------------------------------------------------  
	code.cache.size.2.exe 				 
	DEL code.cache.size.2.exe 			 
	code.cache.size.4.exe 				 
	DEL code.cache.size.4.exe 			 
	code.cache.size.8.exe 				 
	DEL code.cache.size.8.exe 			 
	code.cache.size.16.exe 				 
	DEL code.cache.size.16.exe 			 
	code.cache.size.32.exe 				 
	DEL code.cache.size.32.exe 			 
	code.cache.size.64.exe 				 
	DEL code.cache.size.64.exe 			 
	code.cache.size.128.exe 				 
	DEL code.cache.size.128.exe 			 
	code.cache.size.256.exe 				 
	DEL code.cache.size.256.exe 			 
	code.cache.size.512.exe 				 
	DEL code.cache.size.512.exe 			 
