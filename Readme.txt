Build
- you can make 'runchain' file by typing just 'make'.
- If you want to rebuild, build 'make' after 'make clean' to delete previous 'runchain' and .o files.

Run
- We need a comPanyNode and some Nodes to run.
- I suggest to use multiple terminals to identify network and individual result.

- First, run the company node by './runchain 0 "runningtime" '  e.x) './runchain 0 200' or './runchain 0'
	 you can apply the running time in seconds for the system, default running time is 180 seconds if you don't input. Suggested running time is at least 100 seconds for all nodes to interact fully. 
	 P.S) When I ran the program, I applied 180 seconds for 10 nodes, 300 seconds for 20 nodes and 450 seconds for 30 nodes. And then they succeed to interact fully. 

- Second, run the each node by './runchain 1 "available port Number" ' e.x)'./runchain 1 5000'
	 The program is using IP address "127.0.0.1". So choose the available port Number, except for 1012 and 1013 which are used for company node. After, waiting time for initialization, 6 seconds, you can see the interacting message. The request occurs every 5 seconds.

- Result
	After running, you can see the hash string for individual node. And you can see block hash determined by 3 difficulty and block's participants hash string in each block. Finally, you can see the winner's hash string and check whether wins or not itself. If the node is winner, the condition for the winning prize is printed out. 

- Bug
	-If you run again the program right after finishing, there is some bind error which print out already using address. Then just wait for a while, maybe 10 seconds or more. You can run again.

