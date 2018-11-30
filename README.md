# MQLC
C++ implementation of "On Multi-Query Local Community Detection"

First, please compile source code files in the folder "source_code" (We tested the code under Cygwin). For example, 

g++ ./source_code/*.cpp -o MRW

The output executable file is MRW.exe (under Windows platform).

You can either directly execute MRW.exe (commond in ./MRW) and follow instructions to run a demo or run MRW with command. For example:

./MRW -i ./graph/com-amazon.ungraph.txt -o ./ -s ./graph/demo_seeds.txt -a 0.1 -b 0.6 -k 5 -g 0.3

Parameters:

-i InputFileNameEdge
-s InputFileNameSeeds
-o OutputFolder: default: current folder
-a alpha
-b beta
-g gamma
-k K

For the input files:

1. InputFileNameEdge: edge file

MRW supports both weighted and unweighted graphs.

For weighted graph, the input format for an edge is "source_node	target_node	weight" (delimiter is "\t"). For example:

source_node	target_node	weight
1	88160	1.3
22	118052	2.1
35	161555	7.5
14	244916	0.5
25	346495	3.8

For unweighted graph, the input format for an edge is "source_node	target_node" (delimiter is "\t"). For example:

source_node	target_node
1	88160
22	118052
35	161555
14	244916
25	346495

2. InputFileNameSeeds: query nodes set

Please list the query nodes in a row delimited with "\t" or in a column. For example:

Row: 
201574	42687	161938	201570

Column:
201574
42687
161938
201570

Note that in the input files, lines starting with "#" are considered as comments.

For output file: MRW will output detected communities for each query node in a line. Note that, if some query nodes are merged together, i.e., MRW considers them coming from the same community, it will show, for example, the following results:
--------------------------------------------------------------------------------
# Detected Community for Query node 1: 201574
201574	216724	215112	252984	201572	201570	161938	277248	487203	512040	42687	523685	

# Detected Community for Query node 2: 42687	MERGES with Query node 1: 201574
201574	216724	215112	252984	201572	201570	161938	277248	487203	512040	42687	523685	

# Detected Community for Query node 3: 161938	MERGES with Query node 1: 201574
201574	216724	215112	252984	201572	201570	161938	277248	487203	512040	42687	523685	

# Detected Community for Query node 4: 201570	MERGES with Query node 1: 201574
201574	216724	215112	252984	201572	201570	161938	277248	487203	512040	42687	523685	
--------------------------------------------------------------------------------

Query node 2, 3 and 4 are merged to Query node 1, which means these four nodes are considered to be from the same detected community.
Reference
@inproceedings{bian2018on,
  title={On Multi-Query Local Community Detection},
  author={Bian, Yuchen and Yan, Yaowei and Cheng, Wei and Wang, Wei, and Luo, Dongheng, and Zhang, Xiang},
  booktitle={Data Mining (ICDM), 2018 IEEE International Conference on},
  year={2018},
  organization={IEEE}
}
