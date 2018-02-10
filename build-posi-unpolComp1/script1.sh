 #!/bin/bash
  
 for i in `seq 1 10`; do
     ./tangle2
     mv Tangle2.root sim0002000_$i.root 
 done
