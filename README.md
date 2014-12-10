analyze-libsvm-file
===================

Outputs some statistics about how many features and classes are present in the libsvm training/test data file.

Example output for the `heart_scale` file that is distributed with libsvm:
```
Classes: 
  1.0  44.444%  120
 -1.0  55.556%  150
Features:
    1  97.407%  263    7
    2 100.000%  270
    3 100.000%  270
    4 100.000%  270
    5 100.000%  270
    6 100.000%  270
    7  99.259%  268    2
    8 100.000%  270
    9 100.000%  270
   10  99.630%  269    1
   11  54.815%  148  122
   12 100.000%  270

  ALL  96.239% 3378  132
```

How to read the output:
-----------------------
Classes:
`1.0  44.444%  120` means, there are `120` samples (`44.444%` of all samples) in the class with the label `1.0`

Features: 
`1  97.407%  263    7` means, that `263` samples contain the feature and `7` samples don't contain it. 
So `97.407%` of the samples contain feature `1`. 
The last number is omitted, if all samples contain that feature.

The last line is a summary for all the features.
