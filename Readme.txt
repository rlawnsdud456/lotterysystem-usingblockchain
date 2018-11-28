Build
- you can make 'runchain' file by typing just 'make'.
- If you want to rebuild, build 'make' after 'make clean' to delete previous 'runchain', .o files and log folder .

Run
- We need a comPanyNode and some Nodes to run.
- I suggest to use multiple terminals to identify network and individual result.

- First, run the company node by './runchain 0 "runningtime" '  e.x) './runchain 0 200' or './runchain 0'
	 you can apply the running time in seconds for your running, default running time is 120 seconds if you don't input. Suggested running time is at least 100 seconds for all nodes to interact fully. 
	 P.S) When I ran the program, I applied 180 seconds for 10 nodes, 300 seconds for 20 nodes and 450 seconds for 30 nodes. And then they succeed to interact fully. 

- Second, run the each node by './runchain 1 "available port Number" ' e.x)'./runchain 1 5000'
	 The program is using IP address "127.0.0.1". So choose the available port Number, except for 11012 and 11013 which are used for company node. After, waiting time for initialization, you can see the interacting message. The request occurs every 5 seconds.

- You can run in demo with 10 Nodes in 120 seconds by 'make demo' or './runchain 2' after 'make'. The node's portnumber is from 5000 to 5009. After successfully running the program, the result of the each node is saved to log file like 'log0.txt' in log folder. Command 'make' create the log folder automatically. And 'make clean' automatically deletes the log folder and log files. 

- Result
	After running, the result of the each Node is saved in 'log5000.txt' file in log folder.You can see the hash string for individual node. And you can see block hash determined by 3 difficulty and block's participants hash string in each block. Finally, you can see the winner's hash string and check whether wins or not itself. If the node is winner, the condition for the winning prize is printed out. 

- Bug
	-If you run again the program right after finishing, there is some bind error which print out already using address. Then just wait for a while, maybe 10 seconds or more. You can run again.
	-If you run the demo, the program fork 10 processes. The parent process which is the companysiedeNode binds to Network after fork. So if it fails to bind, it checks and turns off the main after waiting for the all of child processes to exit. As I mentioned at previous, you can run it after waiting some seconds. 
