# CBF-HT
Name lookup scheme for FIB in NDN

CBF-HT is a two-stage name lookup scheme combining counting bloom filter (CBF) and hash table (HT). Firstly, the longest name prefix matching result is obtained by using binary search in CBF, then linear backtracking is carried out in HT to get the final correct lookup result. CBF-HT can achieve high name lookup and update speed, and significantly reduce memory consumption. In addition, by modifying the binary search algorithm, we propose a memory-optimized binary search (MOBS) algorithm to further optimize the memory consumption.
