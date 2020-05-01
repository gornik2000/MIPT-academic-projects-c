#==============================================================================
CC=g++
C=gcc
#==============================================================================
compiler: 	compiler.cpp								\
			./nikita_backend_frontend/ng_tree.cpp 		\
			./nikita_backend_frontend/tree_com.cpp		\
			./leonid_backend_frontend/frontEnd.c 		\
			./leonid_backend_frontend/backEndRealCPU.c 	\
			./leonid_backend_frontend/backEndSoftCPU.c
	$(CC)	 compiler.cpp -o Compiler.exe
	$(CC)	./nikita_backend_frontend/ng_tree.cpp		-o ng_tree.exe
	$(CC)	./nikita_backend_frontend/tree_com.cpp		-o tree_com.exe
	$(C)	./leonid_backend_frontend/frontEnd.c		-o cb_tree.exe
	$(C)	./leonid_backend_frontend/backEndRealCPU.c	-o tree_asm.exe
	$(C)	./leonid_backend_frontend/backEndSoftCPU.c	-o tree_s.exe
#==============================================================================
clean:
	rm -rf *.o *.gch
#==============================================================================