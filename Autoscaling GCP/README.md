# Graph Coloring Problem with autoscaling

The Graph coloring problem (GCP) is one of the NP-complete problems. It takes a graph as an input and outputs a color for each node such that no nodes sharing a common edge have the same color. The aim of this project is not to find the mimimum coloring (aka chromatic number) but to provide an **approximate coloring** for the input graph in linear number of rounds of message passing (among the workers in the distributed setup).

# Architecture

The EC2 instaces provided by the aws service are used as **workers**. EC2 is the Amazon Web Service you use to create and run virtual machines in the cloud (these virtual machines are called 'instances').
There is **master** controller that takes the user input and dsitributes the work among the workers, gather partial output from each woker, combines it and outputs the final result. If enough instances are not available, the master spawns new instances. It also starts any sleeping instance to meet the required number of instances.

# Algorithm

1. Get the graph.
2. Divide the graph into multiple forests.
>Provide an orientation for the graph (make it a "directed" graph).
>Assign an id to each node. The direction of the edge is from the node with the higher id to the lower.
>Label every outgoing vertex from 1 upto the degree for that node.
>Edges with same label form a forest.
3. Distribute forests among different workers **(1 forest = 1 worker)**.
4. Each worker colors the forest (which is 2-colorable) and sends the coloring to the master.
5. Gather all the different coloring and assemble them.
>Concatenate the different colors obtained for each vertex from each forest.
>The coloring obtained after concatenation is a valid graph coloring.

# How to setup 'worker'

1. Login into your ec2-instance (via ssh using the key-pair generated during instance launch).
2. Download the folder "worker" onto it.
3. cd worker && make (install gcc and automake if this step fails)
4. If there was no problem, an executable by the name 'worker' will be created in the present directory.
5. Config the ec2-instance to start the worker at boot
>https://superuser.com/questions/685471/how-can-i-run-a-command-after-boot

Note: While setting up master, you will require an ami_id. Log in into your aws management console and create an image id from the worker we just setup so that you won't have to repeat the above steps for all the workers that will be dynamically created.
	
# How to setup 'master'

1. Make sure you have aws sdk for c++ installed on the master and CMAKE_PREFIX_PATH points to it.
>export CMAKE_PREFIX_PATH=/path/to/sdk_build_dir if not already present in .bashrc
2. Dowload the folder "master" and "User Interface" onto it.
3. cd master
>Make changes to src/mode2.cpp 
>>Line 75: Replace the 3 string arguments with your aws_access_key_id, aws_secret_access_key, region 

>Make changes to src/ec2.cpp 
>>Line 144: Replace the ami_id with the image id of your choice 

>>Line 148: Replace the instance type (t2_micro default) 

>>Line 151: Replace the key name with your key pair name 

>>Line 152: Replace the security group id with your own security group id 

4. mkdir build && cd build
5. cmake ../../master
6. If there was no problem, an executable by the name 'master' will be created in your build directory.
	
Note: The program uses 2 port numbers: 35000 and 45000. Make sure that your security policy allows incoming and outgoing TCP traffic on these ports.

# Input and Output

Method 1: Manually provide input and interpret output. Input to the worker is a string with the following semantics:
>`#nodes <degree node 0> <id of the neighbors>  ... <degree node n> <id of the neighbors>`

> Example: For a simple graph like : (0) ---> (1) ---> (2), the input will be 3 1 1 1 2 0.

Method 2: Use the "User Interface" provided
>Open index.html.

>Draw a node by clicking anywhere on the screen.

>Draw edges by pressing the mouse down at source and lifting it up at destination.

>Once the complete graph is done, press Enter.

>Copy the displayed string and feed it as input to master.

>Copy the last string of output from master and paste it inside the file name 'output'.

>Now browse 'output' from index.html and open it.

>Press Space to see the graph coloring.

Note: When you run master, it will ask you to enter the mode. Enter '0' for local testing (when worker is installed on the same system as master) or '1' to use the aws services.
